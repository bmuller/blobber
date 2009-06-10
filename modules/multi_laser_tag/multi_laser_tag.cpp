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

#include "multi_laser_tag.h"

using namespace std;
using namespace blobber;

MultiLaserTag::MultiLaserTag() : ModInterface("MultiLaserTag") { 

};


void MultiLaserTag::init(Camarea &area, ProjectionWindow &pw) {
  // use default CRANGE criteria
  register_poi(area, 15);
};


void MultiLaserTag::update(Camarea &area, ProjectionWindow &pw) {
  BVector<PIXEL> poi;
  get_poi(area, poi);

  for(unsigned int i=0; i<lastpoints.size(); i++) {

    BVector<PIXEL>::iterator iter = poi.begin();
    while(iter != poi.end()) {
      if((*iter).coord.distance_from(lastpoints[i].coord) < 15.0) {
	pw.draw_line(lastpoints[i].coord, (*iter).coord, pw.colors[pw.preferred_color], 1.0);
	poi.erase(iter);
	// break from loop
	iter = poi.end();
      } else {
	iter++;
      }
    }

  }


  lastpoints = poi; 
};

extern "C" {
  ModInterface *get_module() { return new MultiLaserTag(); };
};

