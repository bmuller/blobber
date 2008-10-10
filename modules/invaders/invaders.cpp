#include "invaders.h"

Invaders::Invaders() : crosshairs(7,COORD(0,0),WHITE), ModInterface("Invaders") { }

void Invaders::init(Camarea &area, ProjectionWindow &pw) {

  // register some callbacks for Gtk events on the ProjectionWindow
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Invaders::test));
  //pw.signal_exposed_event().connect_notify(sigc::mem_fun(*this, &Invaders::resize));
  
  // create a Gdk::Pixbuf that can be rendered to the ProjectionWindow
  pw.get_window()->get_size(win_width, win_height);
  image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,false,8,win_width,win_height);

  // register poi
  string red, green, blue;
  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");
  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range, 1);

  // start the timer
  timer = clock();

  // initialize aliens

  /*
  aliens = (Alien *) malloc(10 * sizeof(Alien));
  for (Alien * it = aliens; it - aliens < 10; it++) { init_alien(it, area); }
  */
}

Invaders::~Invaders() {
  //delete [] aliens;
}

void Invaders::update(Camarea &area, ProjectionWindow &pw) {
  clock_t split_timer = clock();
  float elapsed_time = ( (float) split_timer - timer) / CLOCKS_PER_SEC;
  timer = split_timer;

  vector<PIXEL> poi;
  get_poi(area, poi);

  Glib::RefPtr<Gdk::Window> window = pw.get_window();
  if(!window) { return; }
  
  //window->get_size(win_width_new, win_height_new);

  image->render_to_drawable(window, pw.get_style()->get_black_gc(),
    0, 0, 0, 0, win_width, win_height, 
    Gdk::RGB_DITHER_NONE, 0, 0);
   
  if ( poi.size() != 0 ) {
    crosshairs.move(poi[0].coord, pw);
  }

  /*
  for ( Alien * it = aliens; it - aliens < 10; it++) {  
    draw_alien(pw, cr, it, true);
    if ( in_bounds(it, crosshairs.get_center() ) ) { 
      explosions.push_back( crosshairs.get_center() );
      init_alien(it, area);
      //score++;
    }
    else if ( it->center.y > area.bounds.bottom ) {init_alien(it, area); }
    else { it->center.y += 0.05 * it->speed; }
    draw_alien(pw, cr, it); 
  }
  */
}

void Invaders::test(GdkEventKey * event){
  string s;
  int_to_string(image->get_bits_per_sample(), s);
  debug("bits per sample: " + s); 
}

extern "C" {
  ModInterface *get_module() { return new Invaders(); };
};

