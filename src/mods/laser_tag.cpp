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

