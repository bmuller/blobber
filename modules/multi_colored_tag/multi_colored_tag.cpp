#include "multi_colored_tag.h"

using namespace std;

MultiColoredTag::MultiColoredTag() : ModInterface("MultiColoredTag"), missing_counter(0) { 
  lastpoint.x = 0; 
  lastpoint.y = 0; 
};


void MultiColoredTag::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  register_poi_criteria(area, CRANGE(COLOR(60, 0, 0)));
};


void MultiColoredTag::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);
  
  if(poi.size() == 0)
    missing_counter++;
  else {
    if(lastpoint.x!=0 && lastpoint.y!=0 && missing_counter < 2) {
      if((poi[0].color.blue - poi[0].color.green) >= 10)
	pw.draw_line(lastpoint, poi[0].coord, BLUE, 1.0);
      else if((poi[0].color.green - poi[0].color.blue) >= 10)    
	pw.draw_line(lastpoint, poi[0].coord, GREEN, 1.0);
    }
    lastpoint.copy(poi[0].coord);
  }

  if(poi.size() != 0)
    missing_counter = 0;
};

