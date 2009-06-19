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

#include "camarea.h"
#include "exception.h"

#include <string>
#include <iostream>

namespace blobber {

  using namespace std;

  Camarea::Camarea() : device(), hascam(true), manual_align(false), fg(NULL), frame(NULL), mouse_clicked(false) {
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    string dev;    
    config = Configuration::get_config();
    config->get("device", dev, DEFAULT_DEVICE);
    set_device(dev);

    reload_default_criteria();
  };

  Camarea::~Camarea() {
    if(hascam) {
      delete fg;
      delete frame; 
    }
  };

  void Camarea::reload_default_criteria() {
    // get default criteria values - and set them to defaults if they are not set already 
    COLOR vdefault(60, 0, 0);
    COLOR lower;
    config->get_set_color(lower, vdefault, "default");

    // get window size
    string window_size;
    config->get_set("default_criteria_window", window_size, DEFAULT_CRITERIA_WINDOW_SIZE);
    default_criteria_window = string_to_int(window_size);

    // Our range for the default criterea will include a range +-default_criteria_window from the default color
    CRANGE range(lower, default_criteria_window);
    default_criteria.copy(range);

    string colors;
    lower.to_string(colors);
    debug("Setting default criteria to look for " + colors + " += " + window_size);
  };

  void Camarea::set_brightness(int brightness) {
    if(!hascam) return;
    fg->set_brightness(brightness);
  };

  void Camarea::set_contrast(int contrast) {
    if(!hascam) return;
    fg->set_contrast(contrast);
  };

  void Camarea::set_saturation(int saturation){
    if(!hascam) return;
    fg->set_saturation(saturation);
  }

  void Camarea::set_exposure(int exposure, bool isAuto) {
    if (!hascam) return;
    fg->set_exposure(exposure, isAuto);
  }
  
  void Camarea::set_device(string _device) { 
    hascam = false;
    device = _device;
    if(fg) {
      delete fg;
      fg = NULL;
    };
    if(frame) {
      delete frame;
      frame = NULL;
    };
    try {
      fg = FrameGrabberFactory::create(device);
      frame = fg->makeFrame();
      dimensions.width = frame->width;
      dimensions.height = frame->height;
      hascam = true;
    } catch(NoSuchVideoDeviceException &e) {
      debug("No video device found");
      noCam = Cairo::ImageSurface::create_from_png(Glib::build_filename(string(DATAROOTDIR), "nocam.png"));
      hascam = false;
      dimensions.width = 352;
      dimensions.height = 288;
    } catch(exception &e) {
      // not sure if the above catch is necessary?
      debug(e.what());
      noCam = Cairo::ImageSurface::create_from_png(Glib::build_filename(string(DATAROOTDIR), "nocam.png"));
      hascam = false;
      dimensions.width = 352;
      dimensions.height = 288;
    }
  };
  

  void Camarea::set_bounds(BOUNDS &b) {
    bounds.copy(b);
  };

  bool Camarea::on_motion_notify_event(GdkEventMotion* event) {  
    COORD c((int) event->x, (int) event->y);
    if(mouse_clicked && hascam && c.distance_from(mouse_click) > 2) {
      mouse_drawing.from_coords(mouse_click, c);
      // make sure we don't exceed our dimensions (which can happen
      // if you just keep moving the mouse outside of the window
      mouse_drawing.normalize(dimensions);
    }
  };

  bool Camarea::on_button_press_event(GdkEventButton* event) { 
#ifdef DEBUG
    if(hascam) {
      unsigned char * data = (unsigned char *) frame->data;
      int index = (dimensions.width * (int) event->y) + (int) event->x;
      cout << " R:" << (int) data[index*4+2] << " G:" << (int) data[index*4+1] << " B:" << (int) data[index*4] << endl;
    }
    cout.flush();
#endif
    if(hascam) {
      mouse_clicked = true;
      mouse_click.update((int) event->x, (int) event->y);
      mouse_click.normalize(dimensions);
      // next line removes any previous box that might exist and prevents
      // the application from drawing it
      mouse_drawing.from_coords(mouse_click, mouse_click);
    }
  };

  bool Camarea::on_button_release_event(GdkEventButton* event) { 
    mouse_clicked = false;
    COORD c((int) event->x, (int) event->y);

    // make sure we don't exceed our dimensions (which can happen
    // if you just keep moving the mouse outside of the window
    c.normalize(dimensions);

    // event->button is 1 if left mouse button, 3 is right mouse button
    if(hascam && c.distance_from(mouse_click) > 2 && event->button == 1) {
      bounds.from_coords(c, mouse_click);
      manual_align = true;
      config->set_bounds(bounds, "camarea_bounds");
    } else if(hascam && c.distance_from(mouse_click) > 2 && event->button == 3) {
      BOUNDS blob_bounds;
      blob_bounds.from_coords(c, mouse_click);
      COLOR blob;
      if(find_the_blob(blob, blob_bounds, (unsigned char *) frame->data)) {
	config->set_color(blob, "default");
	reload_default_criteria();
      } else debug("couldn't find a blob in the coordinates given");
    }
  };

  // Find the brightest color in the bounds of the data
  bool Camarea::find_the_blob(COLOR &blob, BOUNDS &bounds, unsigned char *data) {
    blob.set(0, 0, 0);
    // there are +1 and -1 modifications to the bounds so that we are looking "inside"
    // of the drawn rectangle and don't include the pixels of the rectangle (which are 
    // BLUE).
    for(int x=bounds.left+1; x<bounds.right-1; x++) {
      for(int y=bounds.top+1; y<bounds.bottom-1; y++) {
	int index = x+(y*dimensions.width);
	COLOR pixelcolor((int) data[index*4+2], (int) data[index*4+1], (int) data[index*4]);
	if(pixelcolor.brighter_than(blob))
	  blob.copy(pixelcolor);
      }
    }
  };

  void Camarea::update_frame() {
    if(!hascam)	
      return;

    fg->grabFrame(frame);
    unsigned char * data = (unsigned char *) frame->data; 

    poi.clear();

    // for each module and criteria update the poi
    std::map<string, vector<CRANGE> >::iterator iter;
    for(iter = poi_criteria.begin(); iter != poi_criteria.end(); iter++)  
      for(unsigned int i=0; i<iter->second.size(); i++)
	update_poi(data, iter->first, iter->second[i]);
    
    // update the modules that just went w/ the default criteria - this will be
    // a single pass
    update_default_poi(data);
  };

  // update the modules that just went w/ the default criteria - this will be
  // a single pass
  void Camarea::update_default_poi(unsigned char *data) {
    int mods_left_to_hit_max = default_criteria_mods.size();

    // iterate through the pixel data for the given criteria
    for(int x=bounds.left; x<bounds.right; x++) {
      for(int y=bounds.top; y<bounds.bottom; y++) {
	int index = x+(y*dimensions.width);
	COLOR pixelcolor((int) data[index*4+2], (int) data[index*4+1], (int) data[index*4]);
	// if pixel matches default criteria
	if(default_criteria.contains(pixelcolor)) {
	  PIXEL p(pixelcolor, COORD(x, y));
	  // alert all the modules that should know
	  for(unsigned int i=0; i<default_criteria_mods.size(); i++) {
	    string modname = default_criteria_mods[i];
	    if(poi[modname].size() < maxPoints[modname]) {
	      poi[modname].push_back(p);
	      if(poi[modname].size() == maxPoints[modname])
		mods_left_to_hit_max--;
	    }
	  }
	}
	if(mods_left_to_hit_max == 0)
	  break;
      }
    }    
  };

  void Camarea::update_poi(unsigned char *data, string modname, CRANGE &criteria) {
    // iterate through the pixel data for the given criteria
    for(int x=bounds.left; x<bounds.right; x++) {
      for(int y=bounds.top; y<bounds.bottom; y++) {
	int index = x+(y*dimensions.width);
	COLOR pixelcolor((int) data[index*4+2], (int) data[index*4+1], (int) data[index*4]);
	if(criteria.contains(pixelcolor)) {
	  PIXEL p(pixelcolor, COORD(x, y));
	  poi[modname].push_back(p);
	}
	// following will never be true if -1 passed in as maxpoints for this criteria
	if(poi[modname].size() == maxPoints[modname])
	  break;
      }
    }    
  };

  void Camarea::update_screen() {
    Glib::RefPtr<Gdk::Window> window = get_window();
    if(window) {
      Cairo::RefPtr<Cairo::Context> context = get_window()->create_cairo_context();
      if(hascam) {
	surface = Cairo::ImageSurface::create((unsigned char *) frame->data, Cairo::FORMAT_RGB24, frame->width, frame->height, frame->bytesperline); 
      } else {
	surface = noCam;
      }
      context->set_source(surface, 0.0, 0.0);
      context->rectangle (0.0, 0.0, dimensions.width, dimensions.height);
      context->clip();
      context->paint(); 
    }
  };

  
  void Camarea::draw_bounds(BOUNDS &b, COLOR c) {
    unsigned char * data = (unsigned char *) frame->data;

    // These will place a border on the camera window to show what it "sees"                                                                                          
    for(int index=(b.top*dimensions.width+b.left); index<(b.top*dimensions.width+b.right); index++) {
      data[index*4] = c.blue;
      data[index*4+1] = c.green;
      data[index*4+2] = c.red;
    }

    for(int index=(b.bottom*dimensions.width+b.left); index<(b.bottom*dimensions.width+b.right); index++) {
      data[index*4] = c.blue;
      data[index*4+1] = c.green;
      data[index*4+2] = c.red;
    }
    
    for(int index=(b.top*dimensions.width+b.left); index<(b.bottom*dimensions.width+b.left); index+=dimensions.width) {
      data[index*4] = c.blue;
      data[index*4+1] = c.green;
      data[index*4+2] = c.red;
    }
    
    for(int index=(b.top*dimensions.width+b.right); index<(b.bottom*dimensions.width+b.right); index+=dimensions.width) {
      data[index*4] = c.blue;
      data[index*4+1] = c.green;
      data[index*4+2] = c.red;
    }
  };

  void Camarea::register_poi_criteria(string modname, CRANGE range, int maxPoi) {
    poi_criteria[modname].push_back(range);
    maxPoints[modname] = maxPoi;
  };

  void Camarea::register_poi(string modname, int maxPoi) {
    default_criteria_mods.push_back(modname);
    maxPoints[modname] = maxPoi;
  };

  void Camarea::get_poi(string modname, vector<PIXEL> &modpoi) {
    modpoi.assign(poi[modname].begin(), poi[modname].end());
  };

  Cairo::RefPtr< Cairo::ImageSurface > Camarea::getSurface() {
    return surface;
  };
};
