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

#include "multi_colored_tag.h"

using namespace std;
using namespace blobber;

MultiColoredTag::MultiColoredTag() : ModInterface("MultiColoredTag"), missing_counter(0) { 
  lastpoint.x = 0; 
  lastpoint.y = 0; 
};


void MultiColoredTag::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  register_poi_criteria(area, CRANGE(COLOR(60, 0, 0)), 100); // 100 is max number of poi returned
};


void MultiColoredTag::update(Camarea &area, ProjectionWindow &pw) {
  PIXEL * poi;
  int poi_n;
  get_poi(area, poi, poi_n);
  
  if(poi_n == 0)
    missing_counter++;
  else {
    if(lastpoint.x!=0 && lastpoint.y!=0 && missing_counter < 2) {
      if((poi[0].color.blue - poi[0].color.green) >= 10)
	pw.draw_line(lastpoint, poi[0].coord, BLUE, 1.0);
      else if((poi[0].color.green - poi[0].color.blue) >= 10)    
	pw.draw_line(lastpoint, poi[0].coord, GREEN, 1.0);
    }
    lastpoint.copy(poi[0].coord);
  }

  if(poi_n != 0)
    missing_counter = 0;
};

extern "C" {
  ModInterface *get_module() { return new MultiColoredTag(); };
};
