#include "../blobber.h"

using namespace std;

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
};


void ProjectionOptions::update(Camarea &area, ProjectionWindow &pw) {
  vector<COORD> poi;
  get_poi(area, poi);
  
  for(unsigned int i=0; i<poi.size(); i++) {
    // handle clear screen
    if(poi[i].x > (pw.width - 20) && poi[i].y > (pw.height - 20))
      pw.clear();
  };
};

