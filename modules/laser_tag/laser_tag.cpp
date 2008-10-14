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

LaserTag::LaserTag() : ModInterface("LaserTag", "Basic drawing module"), missing_counter(0) { 
  lastpoint.x = 0; 
  lastpoint.y = 0; 
};

void LaserTag::init(Camarea &area, ProjectionWindow &pw) {
  // use default CRANGE criteria
  register_poi(area, 2);
};


void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);
  
  if(poi.size() == 0)
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
  if(poi.size() != 0)
    missing_counter = 0;
};


extern "C" {
  ModInterface *get_module() { 
    return new LaserTag(); 
  };
};
