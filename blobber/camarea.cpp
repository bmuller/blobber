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

  Camarea::Camarea(string _device) : device(_device), hascam(true), mouse_clicked(false), manual_align(false) {
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    try {
      fg = FrameGrabberFactory::create(device);
      frame = fg->makeFrame();
      width = frame->width;
      height = frame->height;
    } catch(NoSuchVideoDeviceException &e) {
      debug("No video device found");
      noCam = Cairo::ImageSurface::create_from_png(string(DATAROOTDIR) + string("/nocam.png"));
      hascam = false;
      width = 352;
      height = 288;    
    }
  };

  Camarea::~Camarea() {
    if(hascam) {
      delete fg;
      delete frame; 
      for (std::map<string, PIXEL*>::iterator it = poi.begin(); it != poi.end(); it++) {
        delete (*it).second;
      }
    }
  };

  void Camarea::set_device(string _device) { 
    device = _device;
    delete fg;
    delete frame;
    fg = FrameGrabberFactory::create(device);
    frame = fg->makeFrame();
    width = frame->width;
    height = frame->height;
  };
  
  void Camarea::set_bounds(BOUNDS &b) {
    bounds.copy(b);
  };

  bool Camarea::on_motion_notify_event (GdkEventMotion* event) { 
    /* doesn't actually work - not sure it matters
       if(mouse_clicked && hascam) {
       BOUNDS b;
       COORD c((int) event->x, (int) event->y);
       b.from_coords(c, mouse_click);
       draw_bounds(b);
       };
    */
  };

bool Camarea::on_button_press_event(GdkEventButton* event) { 
#ifdef DEBUG
  cout << "camarea mouse press (" << event->x << ", " << event->y << ")";
  if(hascam) {
    unsigned char * data = (unsigned char *) frame->data;
    int index = (width * (int) event->y) + (int) event->x;
    cout << " R:" << (int) data[index*4+2] << " G:" << (int) data[index*4+1] << " B:" << (int) data[index*4] << endl;
  } else {
    cout << endl;
  }
  cout.flush();
#endif
  if(hascam) {
    mouse_clicked = true;
    mouse_click.x = (int) event->x;
    mouse_click.y = (int) event->y;
  }
};

bool Camarea::on_button_release_event(GdkEventButton* event) { 
#ifdef DEBUG
  cout << "camarea mouse release (" << event->x << ", " << event->y << ")" << endl; 
  cout.flush();
#endif
  COORD c((int) event->x, (int) event->y);
  if(hascam && c.distance_from(mouse_click) > 2) {
    bounds.from_coords(c, mouse_click);
    manual_align = true;
  }
  mouse_clicked = false;
};

void Camarea::update_frame() {
  if(!hascam)
    return;

  fg->grabFrame(frame);
  unsigned char * data = (unsigned char *) frame->data; 

  // set points of interests for modules
  for(int x=bounds.left; x<bounds.right; x++) {
    for(int y=bounds.top; y<bounds.bottom; y++) {
      int index = x+(y*width);
      COLOR pixelcolor((int) data[index*4+2], (int) data[index*4+1], (int) data[index*4]);
      update_poi(pixelcolor, COORD(x, y));
    }
  }

};

void Camarea::update_poi(COLOR &color, COORD coord) {
  std::map<string, vector<CRANGE> >::iterator iter;

  // for each module
  for(iter = poi_criteria.begin(); iter != poi_criteria.end(); iter++ ) {
    string modname = iter->first;
    vector<CRANGE> criteria = iter->second;
    poi_n[modname] = 0;
	
    // for each criteria each module has set
    for(unsigned int i=0; i<criteria.size(); i++) {
      if(criteria[i].contains(color)) {
        if( poi_n[modname] == poi_max[modname] ) { break; }
	PIXEL p(color, coord);
	poi[modname][ poi_n[modname]++ ] = p;
      }
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
    context->rectangle (0.0, 0.0, width, height);
    context->clip();
    context->paint(); 
  }
};


void Camarea::draw_bounds(BOUNDS &b) {
  unsigned char * data = (unsigned char *) frame->data;

  // These will place a border on the camera window to show what it "sees"                                                                                          
  for(int index=(b.top*width+b.left); index<(b.top*width+b.right); index++) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }

  for(int index=(b.bottom*width+b.left); index<(b.bottom*width+b.right); index++) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }

  for(int index=(b.top*width+b.left); index<(b.bottom*width+b.left); index+=width) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }

  for(int index=(b.top*width+b.right); index<(b.bottom*width+b.right); index+=width) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }
};

void Camarea::register_poi_criteria(string modname, CRANGE range, int max) {
  poi_criteria[modname].push_back(range);
  poi_max[modname] = max;
  poi[modname] = (PIXEL *) malloc(max*sizeof(PIXEL));
};

void Camarea::get_poi(string modname, PIXEL* modpoi, int& n) {
  modpoi = poi[modname];
  n = poi_n[modname];
};

};
