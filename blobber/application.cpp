/*
    Blobber is webcam based blob tracking software
    Copyright (C) 2008 Charleston, SC Linux Users Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "blobber.h"

namespace blobber {
  using namespace std;

  Application * Application::_inst = 0;

  void Application::load_modules() {
    if(!config->is_set("mods_enabled")) {
      vector<string> mods;
      mods.push_back("lasertag");
      mods.push_back("projectionoptions");
      config->set("mods_enabled", mods);
    }
    vector<string> mods_enabled;
    config->get("mods_enabled", mods_enabled);
    for(unsigned int i=0; i<mods_enabled.size(); i++)
      add_mod(ModInterface::load_module(mods_enabled[i]));
  };

  void Application::reload_config() {      
    // handle modules
    for(unsigned int i=0; i<mods.size(); i++) {
      delete mods[i];
    }
    mods.clear();
    if(config->is_set("mods_enabled")) {
      vector<string> mods_enabled;
      config->get("mods_enabled", mods_enabled);
      // add modules that should be enabled
      for(unsigned int i=0; i<mods_enabled.size(); i++)
	add_mod(ModInterface::load_module(mods_enabled[i]));
    }

    // set device in camera window - FIXME set_device throws an unhandled exception?
    string dev;
    Configuration::get_config()->get("device", dev, DEFAULT_DEVICE);
//    win.area.set_device(dev);
  };

  Application::Application() : win(), proj(win.area.dimensions), aligned(false) {
    config = Configuration::get_config();   

    if(win.area.hascam) {
      proj.set_transient_for(win);
      proj.show();
    }
    Glib::signal_idle().connect(sigc::mem_fun(*this, &Application::on_idle));
    //Glib::signal_timeout().connect(sigc::mem_fun(*this, &Application::on_idle), 50);      

    // until we do an alignment, just set the projection area and camarea bounds to be the 
    // full visible area of camera
    BOUNDS b(0, win.area.dimensions.height, 0, win.area.dimensions.width);
    win.area.set_bounds(b);
    proj.set_bounds(b);
    
    load_modules();
  };
    
  Application::~Application() {
    debug("Freeing all modules...");
    for(unsigned int i=0; i<mods.size(); i++) {
      debug("Freeing " + mods[i]->name + "...");
      delete mods[i];
    }
    proj.finish();
    // config should be last thing deleted
    Configuration::delete_config();
  };
  
  Application * Application::get_app() {
    if(_inst == 0)
      _inst = new Application();
    return _inst;
  };

  void Application::delete_app() {
    if(_inst != 0) {
      delete _inst;
      _inst = 0;
    }
  };

  bool Application::module_loaded(ModInterface *mi) {
    for(unsigned int i=0; i<mods.size(); i++)
      if(mods[i] == mi)
	return true;
    return false;
  };

  void Application::add_mod(ModInterface * mi) {
    debug("Module " + mi->name + " loading...");
    if(module_loaded(mi)) {
      debug("loading canceled - module already loaded");
    } else {
      mi->set_config(config);
      mi->init(win.area, proj);
      mods.push_back(mi);
    }
  };
    
  bool Application::on_idle() {
    win.area.update_frame();

    if(win.area.hascam && win.area.manual_align) {
      debug("Manually setting camera's bounds");
      aligned = true;
      proj.set_bounds(win.area.bounds);
      visible_bounds.copy(win.area.bounds);
      win.area.manual_align = false; // so we don't keep doing this over and over
    } else if(win.area.hascam && proj.need_alignment) {
      align();
    } 

    if(win.area.hascam && aligned) {
      win.area.draw_bounds(visible_bounds);
    }

    // if an expose event occured in project window alert modules
    if(win.area.hascam && proj.i_exposed_myself) {
      for(unsigned int i=0; i<mods.size(); i++) {
	mods[i]->projection_window_exposed(proj);
      }
      proj.i_exposed_myself = false;
    }

    // if there is a camera
    if(win.area.hascam) {
      for(unsigned int i=0; i<mods.size(); i++) {
	mods[i]->update(win.area, proj);
      }
    }

    win.area.update_screen();
    return true;
  };

  void Application::run() {
    Glib::set_prgname("blobber");
    Glib::set_application_name("blobber");
    Gtk::Main::run(win);
    debug("Finished running");
  };

  void Application::align() {
    proj.show_alignment_graphics();
      
    BOUNDS b;
    COORD coord;
    b.top = win.area.dimensions.height;
    b.left = win.area.dimensions.width;
    b.right = b.bottom = 0;
    unsigned char * data = (unsigned char *) win.area.frame->data;
    for(int index=0; index < (win.area.dimensions.height*win.area.dimensions.width); index++) {
      // looking for blue
      if(data[index*4] > 205 && data[index*4+1] < 190 && data[index*4+2] < 150) {
	coord.x = index % win.area.dimensions.width;
	coord.y = index / win.area.dimensions.width;
	b.update(coord);
      }
    }
    
    if(b.right == b.bottom && b.left == win.area.dimensions.width) {
      b.top = b.left = 0;
      b.bottom = win.area.dimensions.height;
      b.right = win.area.dimensions.width;
    } else {
      aligned = true;
      win.area.set_bounds(b);
      proj.set_bounds(b);
      visible_bounds.copy(b);
      proj.hide_alignment_graphics();
      debug("Found camera bounds!  Go get a beer.");
    }
  };

};

