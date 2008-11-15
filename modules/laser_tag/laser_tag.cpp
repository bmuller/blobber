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
};

void LaserTag::projection_window_exposed(ProjectionWindow &pw) {
  string background;
  config_get_set("background", background, "");
  if(background != "")
    pw.set_background_image(background);
};

/** 
 * Smooth out the line.
 * 
 * @param previous Previous width
 * @param next Next (new) width
 * 
 * @return fixed, smoothed version of next
 */
double LaserTag::smooth(double previous, double next) {
  int sfactor = 1;
  // if getting bigger
  if(next > previous && (next - previous) > sfactor)
    return previous + sfactor;
  else if(previous > next && (previous - next) > sfactor)
    return previous - sfactor;
  return next;
};

void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);
  
  if(poi.size() == 0)
    missing_counter++;
  else {
    if(lastpoint.x!=0 && lastpoint.y!=0 && missing_counter < 2) {
      double previous = line_width;
      line_width = 35.0 / (double) lastpoint.distance_from(poi[0].coord);
      line_width = min(line_width, 15.0);
      line_width = smooth(previous, line_width);
      pw.draw_line(lastpoint, poi[0].coord, pw.colors[pw.preferred_color], line_width);    
    } else line_width = 15.0;
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
