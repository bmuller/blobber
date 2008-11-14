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
};

Duckhunt::~Duckhunt() {
  if(duck != NULL) 
    delete duck;
};

void Duckhunt::projection_window_exposed(ProjectionWindow &pw) {
  duck->scale(duck_proportion, pw);
  duck->paint(pw);

  string filelocation;
  get_resource_path(filelocation, "background.png");
  pw.set_background_image(filelocation);
};

void Duckhunt::init(Camarea &area, ProjectionWindow &pw) {
  direction = 1;
  DIMENSION drawing_area;
  pw.get_drawing_area_dimensions(drawing_area);

  COORD center((drawing_area.width / 2), (drawing_area.height / 2));

  string filelocation;
  get_resource_path(filelocation, "duck.png");
  duck = new Sprite(filelocation, center, COLOR(51, 204, 255));
  duck->paint(pw);

  // get the proprtion we are to the projection window and save it
  duck->dimensions.get_proportion(duck_proportion, pw.dimensions);

  // use default CRANGE criteria
  register_poi(area, 2);

  BOUNDS sb(0, drawing_area.height-120, 110, drawing_area.width-40); 
  sky.copy(sb);
};


void Duckhunt::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);
 

  BOUNDS b;
  duck->get_bounds(b);
  if(!sky.contains(b))
    direction = -direction;

  //string location;
  //duck->center.to_s(location);
  //cout << "location: " << location << endl;

  int x = (direction * 10) + duck->center.x;
  duck->move(COORD(x, duck->center.y), pw);

  //int y = (direction * 10) + duck->center.y;
  //duck->move(COORD(duck->center.x, y), pw);
};


extern "C" {
  ModInterface *get_module() { 
    return new Duckhunt(); 
  };
};
