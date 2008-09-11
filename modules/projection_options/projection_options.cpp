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

#include "projection_options.h"

using namespace std;

ProjectionOptions::ProjectionOptions() : ModInterface("ProjectionOptions") {
  colors_showing_count = 0;
};

void ProjectionOptions::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  register_poi_criteria(area, CRANGE(COLOR(60, 0, 0)));

  // draw clear screen box
  projection_window_exposed(pw);
};


void ProjectionOptions::projection_window_exposed(ProjectionWindow &pw) {
  // draw clear screen box
  COORD c(pw.width-20, pw.height-20);
  pw.draw_box(c, 15, 15, GREY);
  
  if(colors_showing_count == 0) {
    // draw "show colors" box
    pw.draw_box(COORD(pw.width-20, 10), 15, 15, GREY);
  } else {
    int size = pw.height / (pw.colors.size() + 1);
    for(unsigned int i=0; i<pw.colors.size(); i++) 
      pw.draw_box(COORD(pw.width-20, i*size+5), 15, size-5, pw.colors[i], (i != pw.preferred_color));  
  }
};


void ProjectionOptions::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);

  for(unsigned int i=0; i<poi.size(); i++) {
    COORD c;
    pw.translate_coordinates(poi[i].coord, c);

    // handle clear screen
    if(c.x > (pw.width - 20) && c.y > (pw.height - 20)) {
      pw.clear();   
      projection_window_exposed(pw);
    } else if(c.x > (pw.width - 20) && c.y < 35 && colors_showing_count == 0) {
      // handle show colors box
      colors_showing_count = 1;
      projection_window_exposed(pw);
    } else if(c.x > (pw.width - 20) && colors_showing_count > 0) {
      
      // picking a color
      int size = pw.height / (pw.colors.size() + 1);
      for(unsigned int ci=0; ci<pw.colors.size(); ci++) {
	BOUNDS b(ci*size+5, (ci*size+5)+(size-5), pw.width-20, pw.width-5);
	if(b.contains(c)) {
	  pw.preferred_color = ci;
	  colors_showing_count = 1;
	  pw.draw_box(COORD(pw.width-25, 0), 25, pw.height, BLACK, true);
	  projection_window_exposed(pw);
	}
      }
      
    }
  }

  if(colors_showing_count > 0)
    colors_showing_count++;
  if(colors_showing_count == 50) {
    colors_showing_count = 0;
    // clear colors boxes
    pw.draw_box(COORD(pw.width-25, 0), 25, pw.height, BLACK, true);
    projection_window_exposed(pw);
  }
};

extern "C" {
  ModInterface *get_module() { return new ProjectionOptions(); };
};
