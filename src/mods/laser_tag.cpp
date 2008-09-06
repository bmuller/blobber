#include "../blobber.h"

using namespace std;

LaserTag::LaserTag() : ModInterface("LaserTag"), missing_counter(0) { 
  lastpoint.x = 0; 
  lastpoint.y = 0; 
};


void LaserTag::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  register_poi_criteria(area, CRANGE(COLOR(60, 0, 0)));
};


void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  vector<COORD> poi;
  get_poi(area, poi);
  
  if(poi.size() == 0)
    missing_counter++;
  else {
    if(lastpoint.x!=0 && lastpoint.y!=0 && missing_counter < 3)
      pw.draw_line(lastpoint, poi[0], pw.colors[pw.preferred_color], 1.0);    
    lastpoint.copy(poi[0]);
  }

  /*
  for(unsigned int i=0; i<poi.size(); i++) {
    if(lastpoint.x!=0 && lastpoint.y!=0 && missing_counter < 3)
      pw.draw_line(lastpoint, poi[i], pw.colors[pw.preferred_color], 1.0);    
    lastpoint.copy(poi[i]);
  };
  */

  if(poi.size() != 0)
    missing_counter = 0;
};

