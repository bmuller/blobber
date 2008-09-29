#include "invaders.h"

Invaders::Invaders() : ModInterface("Invaders") { }

void Invaders::init(Camarea &area, ProjectionWindow &pw) {
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
  //clock_t split_timer = clock();
  //float alien_time = ( (float) split_timer - alien_timer ) / CLOCKS_PER_SEC;
  //float update_time = ( (float) split_timer - update_timer) / CLOCKS_PER_SEC;
  //update_timer = split_timer;

  vector<PIXEL> poi;
  get_poi(area, poi);

  // clear the pw
  Cairo::RefPtr<Cairo::Context> cr;
  Glib::RefPtr<Gdk::Window> window = pw.get_window();
  if(!window) { return; }
  cr = window->create_cairo_context();
  
  for ( Alien * it = aliens; it - aliens < 10; it++) {
    draw_alien(pw, cr, it, true);
    if ( poi.size() != 0 && in_bounds(it, poi[0].coord) ) { 
      explosions.push_back( poi[0].coord );
      init_alien(it, area);
      //score++;
    }
    else if ( it->center.y > area.bounds.bottom ) {init_alien(it, area); }
    else { it->center.y += 0.05 * it->speed;}
    draw_alien(pw, cr, it);
  }
}

void Invaders::draw_alien(ProjectionWindow &pw, Cairo::RefPtr<Cairo::Context> cr, Alien * a, bool erase) {
  COORD translated;
  pw.translate_coordinates(a->center, translated);
  cr->arc(translated.x, translated.y, 5, 0.0, 2.0 * M_PI);
  cr->set_line_width(1);
  if(erase) {
    cr->set_line_width(3);
    cr->set_source_rgb(0, 0, 0); 
  }
  else { 
    cr->set_source_rgb(a->color.cairo_red(), a->color.cairo_green(), a->color.cairo_blue()); 
    cr->set_line_width(1);  
  }
  cr->stroke();
}   
void Invaders::draw_explosions(ProjectionWindow &pw) {}

extern "C" {
  ModInterface *get_module() { return new Invaders(); };
};

