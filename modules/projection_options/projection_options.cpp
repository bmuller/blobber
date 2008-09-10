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
