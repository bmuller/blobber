#include "../blobber.h"

using namespace std;

void ProjectionOptions::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  register_poi_criteria(area, CRANGE(COLOR(60, 0, 0)));

  // draw clear screen box
  pw.draw_box(area.width-10, area.height-10, area.width, area.height, GREY);
};


void ProjectionOptions::update(Camarea &area, ProjectionWindow &pw) {
  vector<COORD> poi;
  get_poi(area, poi);
  
  for(unsigned int i=0; i<poi.size(); i++) {
    // handle clear screen
    if(poi[i].x > (area.width - 10) && poi[i].y > (area.height - 10))
      pw.clear();
  };

  if(pw.i_exposed_myself)
    pw.draw_box(area.width-10, area.height-10, area.width, area.height, WHITE);
};

