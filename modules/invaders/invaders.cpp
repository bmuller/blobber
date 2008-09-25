#include "invaders.h"


Invaders::Invaders() : ModInterface("Invaders") { }

void Invaders::init(Camarea &area, ProjectionWindow &pw) {
  timer = clock();
  string alien_number, alien_rows, red, green, blue;
  int int_alien_number, int_alien_rows;

  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");
  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range, 1);

  // initialize aliens, laser
  config_get_set("alien_number", alien_number, "30");
  config_get_set("alien_rows", alien_rows, "4");
  int_alien_number = string_to_int(alien_number);
  int_alien_rows = string_to_int(alien_rows);
  if(int_alien_number % int_alien_rows != 0) { int_alien_rows++; }
  for(int i= int_alien_number; i > 0; i--) {
    //aliens.push_back(COORD(1,1));
  }
  laser = COORD(area.width / 2, area.bounds.bottom);
 
}

void Invaders::update(Camarea &area, ProjectionWindow &pw) {
  
  vector<PIXEL> poi;
  get_poi(area, poi);

  // clear the ProjectionWindow
  pw.clear();

  // redraw the commander
  if(poi.size() != 0) { laser.copy(poi[0].coord); }
  draw_laser(area, pw, laser);

  /* -compute new locations of beams, aliens
     -determine if 
      1) commander missile has hit an alien
      2) alien missile has hit commander
      3) aliens have reached commander base
    -draw new locations of commander, beams, and aliens
    -there are points of interest
    -go ahead and iterate through them and play
  */
}

void Invaders::draw_laser(Camarea &area, ProjectionWindow &pw,  COORD location) {
  pw.draw_circle(COORD(location.x, area.height), 5, BLUE);
}

void Invaders::draw_aliens(ProjectionWindow &pw, vector<COORD> &locations) {}
void Invaders::draw_beams(ProjectionWindow &pw, vector<COORD> &locations) {}
void Invaders::draw_explosions(ProjectionWindow &pw, vector<COORD> &locations) {}


extern "C" {
  ModInterface *get_module() { return new Invaders(); };
};

