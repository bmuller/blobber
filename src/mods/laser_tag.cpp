#include "../blobber.h"

using namespace std;

LaserTag::LaserTag() : ModInterface("LaserTag") { 
  lastpoint.x = 0; 
  lastpoint.y = 0; 
};

void LaserTag::init_poi(Camarea &area) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with all three above the thresholds you set will be considered the input.
  register_poi_criteria(area, CRANGE(COLOR(60, 0, 0)));
};

void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  vector<COORD> poi;
  get_poi(area, poi);
  for(unsigned int i=0; i<poi.size(); i++) {
    if(lastpoint.x!=0 && lastpoint.y!=0 && lastpoint.distance_from(poi[i]) <= 15.0)
      pw.draw_line(lastpoint, poi[i], pw.colors[pw.preferred_color], 5.0);    
    lastpoint.copy(poi[i]);
  };
};

