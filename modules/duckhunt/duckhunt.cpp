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

#include "duckhunt.h"

using namespace std;
using namespace blobber;

Duckhunt::Duckhunt() : ModInterface("duckhunt", "like the old nintendo game") { 
  duck = NULL;
  deadduck = NULL;
  xhairs = NULL;
};

Duckhunt::~Duckhunt() {
  if(duck != NULL) 
    delete duck;
  if(deadduck != NULL)
    delete deadduck;
  if(xhairs != NULL)
    delete xhairs;
};

void Duckhunt::projection_window_exposed(ProjectionWindow &pw) {
  duck->scale(duck_proportion, pw);
  duck->paint(pw);

  deadduck->scale(deadduck_proportion, pw);

  string filelocation;
  get_resource_path(filelocation, "background.png");
  pw.set_background_image(filelocation);
};

void Duckhunt::init(Camarea &area, ProjectionWindow &pw) {
  score = show_dead_count = 0;

  string sspeed;
  config_get_set("speed", sspeed, "15.0");
  speed = string_to_double(sspeed);

  // start duck in middle of screen
  DIMENSION drawing_area;
  pw.get_drawing_area_dimensions(drawing_area);
  COORD center((drawing_area.width / 2), (drawing_area.height / 2));

  string filelocation;
  get_resource_path(filelocation, "duck.png");
  duck = new Sprite(filelocation, center, COLOR(51, 204, 255));
  duck->paint(pw);

  // make deadduck but don't paint
  get_resource_path(filelocation, "deadduck.png");
  deadduck = new Sprite(filelocation, center, COLOR(51, 204, 255));

  // get the proprtion we are to the projection window and save it
  duck->dimensions.get_proportion(duck_proportion, pw.dimensions);
  deadduck->dimensions.get_proportion(deadduck_proportion, pw.dimensions);

  xhairs = new MovableCrosshairs(10, COORD(0,0), RED, COLOR(51, 204, 255));

  // use default CRANGE criteria
  register_poi(area, 2);

  // set up sky area and pick a random coordinate
  BOUNDS sb(0, drawing_area.height-120, 110, drawing_area.width-40); 
  sky.copy(sb);
  sky.random_coordinate(destination);
};


void Duckhunt::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);

  if(poi.size() > 0)
    xhairs->move(poi[0].coord, pw);

  // see if duck just got shot
  BOUNDS b;
  duck->get_bounds(b);
  if(poi.size() > 0 && b.contains(poi[0].coord)) { //duck->in_bounds(poi[0].coord)) {
    duck->clear(pw);
    deadduck->center.copy(duck->center);
    deadduck->paint(pw);
    show_dead_count = 1;
    score++;
  }

  if(show_dead_count > 0 && show_dead_count < 10) {
    show_dead_count++;
    return;
  }

  if(show_dead_count > 0) {
    show_dead_count = 0;
    deadduck->clear(pw);
    sky.random_coordinate(destination);
    duck->move(destination, pw);
  }  

  // if duck has arrived at random location, pick a new one
  if(destination.distance_from(duck->center) < ((speed / 2) + 1))
    sky.random_coordinate(destination);

  duck->move(speed, destination, pw);
};


extern "C" {
  ModInterface *get_module() { 
    return new Duckhunt(); 
  };
};
