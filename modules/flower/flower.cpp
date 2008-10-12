#include "flower.h"

Flower::Flower() : ModInterface("Flower") {} 

void Flower::init(Camarea &area, ProjectionWindow &pw) {
  string s, red, green, blue;

  // register some callbacks for Gtk events on the ProjectionWindow
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Flower::key));
  pw.signal_expose_event().connect_notify(sigc::mem_fun(*this, &Flower::expose));
  
  // read image files 
  image = new Glib::RefPtr<Gdk::Pixbuf>[9];
  for(int i = 0; i < 9; i++) {
    int_to_string(i+1, s);
    image[i] = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/flower/flower" + s + ".png");    
  }

  // set the scale factor between the camarea and pw
  pw.get_window()->get_size(win_width, win_height);
  scale[0] = (double) win_width / (double) area.width;
  scale[1] = (double) win_height / (double) area.height;
  resize();

  // register poi
  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");
  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range, 1);
}

Flower::~Flower() {}

void Flower::update(Camarea &area, ProjectionWindow &pw) {
  int dest_x, dest_y, dest_w, dest_h;

  // get the Gdk::Window, recompute the scale
  Glib::RefPtr<Gdk::Window> window = pw.get_window();
  if(!window) { return; }
  scale[0] = (double) win_width / (double) area.width;
  scale[1] = (double) win_height / (double) area.height;

  // get the poi
  vector<PIXEL> poi;
  get_poi(area, poi);
   
  if ( poi.size() != 0 ) {
    flowers.push_back(Entity(poi[0].coord));
  }
  
  // start with black opaque background
  //scr->fill(0x000000FF);  

  // paint flowers
  for ( vector<Entity>::iterator it = flowers.begin(); it < flowers.end(); it++ ) { 
    dest_x = floor( scale[0]*(it->loc.x+10) );
    dest_y = floor( scale[1]*(it->loc.y+20) );
    dest_w = ceil(scale[0]*image[0]->get_width());
    dest_h = ceil(scale[1]*image[0]->get_height());
    image[it->state]->composite(scr, dest_x, dest_y, dest_w, dest_h, dest_x, dest_y, scale[0], scale[1], Gdk::INTERP_BILINEAR, 255);
    if( (it->state++) == 8 ) { it = flowers.erase(it); }   
  }

  // paint the local buffer to the pw
  //window->draw_pixbuf(pw.get_style()->get_black_gc(), scr, 0, 0, 0, 0, win_width, win_height, Gdk::RGB_DITHER_NONE, 0, 0);
  scr->render_to_drawable(window, pw.get_style()->get_black_gc(), 0, 0, 0, 0, win_width, win_height, Gdk::RGB_DITHER_NONE, 0, 0);
  //scr->render_to_drawable_alpha(window, 0, 0, 0, 0, win_width, win_height, Gdk::PIXBUF_ALPHA_BILEVEL, 255, Gdk::RGB_DITHER_NONE, 0, 0);
}


void Flower::resize() {
  // resize the local buffer
  scr = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,true,8,win_width,win_height);
  scr->fill(0x000000FF);
}

void Flower::key(GdkEventKey * event){
  switch(event->keyval) {
    case GDK_Pause : break;
    case GDK_r     : scr->fill(0x000000FF); break;
    case GDK_R     : scr->fill(0x000000FF); break;
    default        : break;
  }
}

void Flower::expose(GdkEventExpose* event) {
  win_width = event->area.width;
  win_height = event->area.height;
  resize(); 
}

extern "C" {
  ModInterface *get_module() { return new Flower(); };
};

