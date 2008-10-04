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

#include "movable_shapes.h"

using namespace std;
using namespace blobber;

MovableShapes::MovableShapes() : ModInterface("MoveableShapes", "Move shapes around"), xhairs(9,COORD(0,0),WHITE) { 
  missing_point_count = 0;
};

MovableShapes::~MovableShapes() {
  for(unsigned int i=0; i<shapes.size(); i++)
    delete shapes[i];
};

void MovableShapes::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  string red, green, blue;
  // get config values - and set them to defaults if they are not set already
  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");

  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range, 1);

  COORD middle(area.width/2, area.height/2);
  MovableSquare *square = new MovableSquare(20, middle, BLUE);
  shapes.push_back(square);

  middle.x += 20;
  middle.y += 20;
  MovableCircle *circle = new MovableCircle(30, middle, RED);
  shapes.push_back(circle);
};

void MovableShapes::projection_window_exposed(ProjectionWindow &pw) {
  for(unsigned int i=0; i<shapes.size(); i++) 
    shapes[i]->paint(pw);
  xhairs.paint(pw);
};

void MovableShapes::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);

  // no points means nothing is selected, return
  if(poi.size() == 0) {
    // if we just went blank, get rid of old crosshairs
    // we should repaint object if it was just selected, or paint
    // old crosshairs black if nothing was selected
    if(missing_point_count == 0) {
      bool found = false;
      for(unsigned int i=0; (i<shapes.size() && !found); i++) {
	if(shapes[i]->selected) {
	  shapes[i]->paint(pw);
	  found = true;
	}
      }
      if(!found)
	xhairs.clear(pw);
    }
    missing_point_count++;
    return;
  }

  // clear selected attr if we've missed the laser for 3 or more iterations;
  if(missing_point_count > 3) {
    for(unsigned int i=0; i<shapes.size(); i++) 
      shapes[i]->selected = false;
    xhairs.move(poi[0].coord, pw);
  } else {
    xhairs.move(poi[0].coord, pw, false);
  }

  missing_point_count = 0;

  // for each point
  bool isselected = false;
  for(unsigned int i=0; i<shapes.size(); i++) {
    // if previously selected, move, then update all the other shapes 
    // (in case we were occluding one and need to redraw)
    if(shapes[i]->selected) {  
      shapes[i]->move(poi[0].coord, pw);
      for(unsigned int j=0; j<shapes.size(); j++)
	shapes[j]->paint(pw);
      // make sure selected ends up in front
      shapes[i]->paint(pw);
      isselected = true;
      break;
    } 
  }    

  // could be a new selection - but only one can be selected
  if(!isselected) {
    for(unsigned int i=0; i<shapes.size(); i++) {
      // only one new one can be selected
      if((shapes[i]->selected = shapes[i]->in_bounds(poi[0].coord)))
	break;
    }
  }
};


extern "C" {
  ModInterface *get_module() { 
    return new MovableShapes(); 
  };
};
