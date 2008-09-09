#include "../blobber.h"

using namespace std;

MultiLaserTag::MultiLaserTag() : ModInterface("MultiLaserTag") { 

};


void MultiLaserTag::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  register_poi_criteria(area, CRANGE(COLOR(60, 0, 0)));
};


void MultiLaserTag::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);

  for(unsigned int i=0; i<lastpoints.size(); i++) {

    vector<PIXEL>::iterator iter = poi.begin();
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

