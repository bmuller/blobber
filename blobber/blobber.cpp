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

  class Blopper {
  private:
    CameraWindow win;
    ProjectionWindow proj;
    BOUNDS visible_bounds;
    vector<ModInterface*> mods;
    bool aligned;
    Configuration *config;
  public:
    Blopper(string device) : win(device), proj(win.area.width, win.area.height), aligned(false) {
      if(win.area.hascam) {
	proj.set_transient_for(win);
	proj.show();
      }
      Glib::signal_idle().connect(sigc::mem_fun(*this, &Blopper::on_idle));
      
      // until we do an alignment, just set the projection area and camarea bounds to be the 
      // full visible area of camera
      BOUNDS b(0, win.area.height, 0, win.area.width);
      win.area.set_bounds(b);
      proj.set_bounds(b);

      config = new Configuration();
    };
    
    ~Blopper() {
      debug("Freeing all modules...");
      for(unsigned int i=0; i<mods.size(); i++) {
	debug("Freeing " + mods[i]->name + "...");
	delete mods[i];
      }
      delete config;
    };

    void add_mod(ModInterface * mi) {
      debug("Module " + mi->name + " loaded...");
      mi->set_config(config);
      mi->init(win.area, proj, config);
      mods.push_back(mi);
    };
    
    bool on_idle() {
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

    void run() {
      Glib::set_prgname("blobber");
      Glib::set_application_name("blobber");
      Gtk::Main::run(win);
      debug("Finished running");
    };

    void align() {
      proj.show_alignment_graphics();
      
      BOUNDS b;
      COORD coord;
      b.top = win.area.height;
      b.left = win.area.width;
      b.right = b.bottom = 0;
      unsigned char * data = (unsigned char *) win.area.frame->data;
      for(int index=0; index < (win.area.height*win.area.width); index++) {
	// looking for blue
	if(data[index*4] > 205 && data[index*4+1] < 190 && data[index*4+2] < 150) {
	  coord.x = index % win.area.width;
	  coord.y = index / win.area.width;
	  b.update(coord);
	}
      }

      if(b.right == b.bottom && b.left == win.area.width) {
	b.top = b.left = 0;
	b.bottom = win.area.height;
	b.right = win.area.width;
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

};

int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  blobber::Blopper b("/dev/video0");

  b.add_mod(blobber::ModInterface::load_module("lasertag"));
  b.add_mod(blobber::ModInterface::load_module("projectionoptions"));

  b.run();
  return 0;
}

