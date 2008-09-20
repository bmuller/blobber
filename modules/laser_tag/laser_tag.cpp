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

#include "laser_tag.h"

using namespace std;
using namespace blobber;

LaserTag::LaserTag() : ModInterface("LaserTag"), missing_counter(0) { 
  lastpoint.x = 0; 
  lastpoint.y = 0; 
};

void LaserTag::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  string red, green, blue;
  // get config values - and set them to defaults if they are not set already
  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");

  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range, 50); // 50 is max number of poi returned
};


void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  PIXEL * poi;
  int poi_n;
  get_poi(area, poi, poi_n);
  
  if(poi_n == 0)
    missing_counter++;
  else {
    //points.push_back(poi[0]);
    if(lastpoint.x!=0 && lastpoint.y!=0 && missing_counter < 2)
      pw.draw_line(lastpoint, poi[0].coord, pw.colors[pw.preferred_color], 5.0);    
    lastpoint.copy(poi[0].coord);
  }

  /* Trying to smooth line out
  if(points.size() == 3) {
    points.push_back(points[points.size() - 1]);
    pw.draw_curve(points, pw.colors[pw.preferred_color]);
    COORD last = points[points.size() - 1];
    points.clear();
    points.push_back(last);
  };
  */
  if(poi_n != 0)
    missing_counter = 0;
};


extern "C" {
  ModInterface *get_module() { 
    return new LaserTag(); 
  };
};
