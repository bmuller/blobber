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
  line_width = 5.0;
};

void LaserTag::init(Camarea &area, ProjectionWindow &pw) {
  // use default CRANGE criteria
  register_poi(area, 2);
  timekeeper.set_marker();
};


void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);
  
  if(poi.size() == 0)
    missing_counter++;
  else {
    long int interval = timekeeper.set_marker();
    // if the time since our last call is under two seconds, set line width appropriately
    if(interval < 2000) {
      line_width = (double) interval / 500.0;
    } else {
      line_width = 5.0;
    }

    if(lastpoint.x!=0 && lastpoint.y!=0 && missing_counter < 2)
      pw.draw_line(lastpoint, poi[0].coord, pw.colors[pw.preferred_color], line_width);    
    lastpoint.copy(poi[0].coord);
  }

  if(poi.size() != 0)
    missing_counter = 0;
};


extern "C" {
  ModInterface *get_module() { 
    return new LaserTag(); 
  };
};
