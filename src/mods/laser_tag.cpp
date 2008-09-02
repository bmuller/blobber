#include "../blobber.h"

using namespace std;

typedef union PIXEL {
  unsigned long pixeldata;
  __u8 rgbo[4];
};

void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  unsigned char * data = (unsigned char *) area.frame->data;
  
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with all three above the thresholds you set will be considered the input.
  int red = 100;
  int green = 100;
  int blue = 100;

  for(int x=area.bounds.left; x<area.bounds.right; x++) {
    for(int y=area.bounds.top; y<area.bounds.bottom; y++) {

      int index = x+(y*area.width);
      if(data[index*4] > blue && data[index*4+1] > green && data[index*4+2] > red) {
	if(lastpoint.x!=0 && lastpoint.y!=0 && lastpoint.distance_from(x, y) <= 15.0)
	  pw.draw_line(lastpoint, COORD(x, y), LIGHT_BLUE);
	lastpoint.x = x;
	lastpoint.y = y;
	data[index*4] = 0;
	data[index*4+1] = 0;
	data[index*4+2] = 0;
	return;
      }

    }
  }


};

