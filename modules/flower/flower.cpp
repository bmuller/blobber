#include "flower.h"

Flower::Flower() : ModInterface("Flower"), clear(false) {} 

void Flower::init(Camarea &area, ProjectionWindow &pw) {
  string s, red, green, blue;

  // register some callbacks for Gtk events on the ProjectionWindow
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Flower::key));
  pw.signal_expose_event().connect_notify(sigc::mem_fun(*this, &Flower::expose));
  
  // read image files 
  image = new Glib::RefPtr<Gdk::Pixbuf>[10];
  for(int i = 0; i < 10; i++) {
    int_to_string(i+1, s);
    image[i] = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/flower/flower" + s + ".png");    
  }

  // register poi
  register_poi(area, 1);
}

Flower::~Flower() {}

void Flower::update(Camarea &area, ProjectionWindow &pw) {
  COORD t;

  // get Gdk::Window
  Glib::RefPtr<Gdk::Window> window = pw.get_window();
  if(!window) { return; }

  // get the poi
  BVector<PIXEL> poi;
  get_poi(area, poi);
   
  // plant flower seed
  if ( poi.size() != 0 ) {
    flowers.push_back(Entity(poi[0].coord));
    pw.translate_coordinates(poi[0].coord,t);
    window->draw_rectangle(pw.get_style()->get_white_gc(),true,t.x,t.y,5,5);
  }

  // if 'r' pressed, clear pw
  if (clear) {
    pw.clear();
    clear = false;
  }  


  // paint flowers
  for ( BVector<Entity>::iterator it = flowers.begin(); it < flowers.end(); it++ ) { 
    pw.translate_coordinates( COORD((*it).loc.x,(*it).loc.y), t);
    window->draw_pixbuf(pw.get_style()->get_black_gc(), 
                        image[it->state], 0, 0, t.x - 60, t.y - 70, 
                        image[0]->get_width(), 
                        image[0]->get_height(), 
                        Gdk::RGB_DITHER_NONE, 0, 0);
    if( (it->state++) == 9 ) { it = flowers.erase(it); }   
  }
}


void Flower::key(GdkEventKey * event){
  switch(event->keyval) {
    case GDK_r     : clear = true; break;
    case GDK_R     : clear = true; break;
    default        : break;
  }
}

void Flower::expose(GdkEventExpose* event) {}

extern "C" {
  ModInterface *get_module() { return new Flower(); };
};

