#include "../blobber.h"

using namespace std;

typedef union PIXEL {
  unsigned long pixeldata;
  __u8 rgbo[4];
};

void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  unsigned int * data = (unsigned int *) area.frame->data;
  for(int index=0; index < area.frame->sizeimage; index++) {  
    if((data[index] & 255) > 200 && ((data[index] >> 8) & 255) > 200 && ((data[index] >> 16) & 255) > 200)
      data[index] = 0;
  } 

  // following works but is slow
  /*
  PIXEL * data = (PIXEL *) area.frame->data;
  for(int index=0; index < area.frame->sizeimage; index++) {
    if(data[index].rgbo[0] > 200  && data[index].rgbo[1] > 200 && data[index].rgbo[2] > 200)
      data[index].pixeldata = 0;
  }
  */
};

