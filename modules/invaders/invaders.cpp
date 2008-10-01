#include "invaders.h"

Invaders::Invaders() : crosshairs(7,COORD(0,0),WHITE), ModInterface("Invaders") { }

void Invaders::init(Camarea &area, ProjectionWindow &pw) {
  // begin experimental 
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Invaders::test));
  // end experimental
  alien_timer = clock();
  update_timer = alien_timer;
  string red, green, blue;

  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");
  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range, 1);

  aliens = (Alien *) malloc(10 * sizeof(Alien));
  for (Alien * it = aliens; it - aliens < 10; it++) { init_alien(it, area); }
}

Invaders::~Invaders() {
  delete [] aliens;
}

void Invaders::update(Camarea &area, ProjectionWindow &pw) {
  volatile_update(area, pw);
}

void volatile Invaders::volatile_update(Camarea &area, ProjectionWindow &pw) {
  //clock_t split_timer = clock();
  //float alien_time = ( (float) split_timer - alien_timer ) / CLOCKS_PER_SEC;
  //float update_time = ( (float) split_timer - update_timer) / CLOCKS_PER_SEC;
  //update_timer = split_timer;

  vector<PIXEL> poi;
  get_poi(area, poi);

  Cairo::RefPtr<Cairo::Context> cr;
  Glib::RefPtr<Gdk::Window> window = pw.get_window();
  if(!window) { return; }
  cr = window->create_cairo_context();

  //cr->translate(-pw.vprojbounds.left, -pw.vprojbounds.top);
  //cr->scale( 1 / ( (double) pw.vprojbounds.width() * pw.width ), 1 / ( (double) pw.vprojbounds.height() * pw.height ) );

  if ( poi.size() != 0 ) {
    crosshairs.move(poi[0].coord, pw);
  }
  
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
}

void Invaders::draw_alien(ProjectionWindow &pw, Cairo::RefPtr<Cairo::Context> cr, Alien * a, bool erase) {
  COORD t;
  pw.translate_coordinates(a->center, t);
  cr->move_to(t.x+15,t.y);
  cr->line_to(t.x+5,t.y-10);
  cr->line_to(t.x+10,t.y-15);
  cr->arc(t.x+13,t.y-18,SQRT_18,0.75 * M_PI,2.75 * M_PI);
  cr->line_to(t.x+5,t.y-10);
  cr->line_to(t.x-5,t.y-10);
  cr->line_to(t.x-5,t.y-15);
  cr->arc(t.x-5,t.y-20,3,0.5 * M_PI,2.5 * M_PI);
  cr->line_to(t.x-5,t.y-10);
  cr->line_to(t.x-15,t.y);
  cr->arc_negative(t.x,t.y,15,M_PI,0);
  if(erase) {
    cr->set_line_width(3);
    cr->set_source_rgb(0, 0, 0); 
  }
  else {
    cr->set_line_width(1);  
    cr->set_source_rgb(a->color.cairo_red(), a->color.cairo_green(), a->color.cairo_blue()); 
  }  
  cr->stroke();
}

   
void Invaders::draw_explosions(ProjectionWindow &pw) {}
void Invaders::test(GdkEventKey * event){ debug("Invaders::test called!"); }

extern "C" {
  ModInterface *get_module() { return new Invaders(); };
};

