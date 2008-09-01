#include "../blobber.h"

using namespace std;

typedef union PIXEL {
  unsigned long pixeldata;
  __u8 rgbo[4];
};

void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  // following methods are organized by order of speed, fastest to slowest

  unsigned char * data = (unsigned char *) area.frame->data;
  for(int x=area.bounds.left; x<area.bounds.right; x++) {
    for(int y=area.bounds.top; y<area.bounds.bottom; y++) {
      int index = x*y;
      if(data[index*4] > 100 && data[index*4+1] > 100 && data[index*4+2] > 100) {
	points.push_back(COORD(x, y));
      }
    }
  }

  pw.clear();
  if(points.size() > 1) {
    COORD source = points[0];
    for(int i=1; i<points.size(); i++) {
      pw.draw_line(source, points[i], BLUE); 
      source = points[i];
    }
  }

  // following is second fastest
  /*
  unsigned int * data = (unsigned int *) area.frame->data;
  for(int index=0; index < area.frame->sizeimage; index++) {  
    if((data[index] & 16777215) > 13158600) // (data[index] & 00000000111111111111111111111111) > 00000000110010001100100011001000-? 200 200 200
      data[index] = 0;
  } 
  */
 

  // following is faster than last one
  /*
  unsigned int * data = (unsigned int *) area.frame->data;
  for(int index=0; index < area.frame->sizeimage; index++) {  
    if((data[index] & 255) > 200 && ((data[index] >> 8) & 255) > 200 && ((data[index] >> 16) & 255) > 200)
      data[index] = 0;
  } 
  */

  // following works but is slow
  /*
  PIXEL * data = (PIXEL *) area.frame->data;
  for(int index=0; index < area.frame->sizeimage; index++) {
    if(data[index].rgbo[0] > 200  && data[index].rgbo[1] > 200 && data[index].rgbo[2] > 200)
      data[index].pixeldata = 0;
  }
  */
};

