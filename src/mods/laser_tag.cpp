#include "../blobber.h"

using namespace std;

void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  unsigned int * data = (unsigned int *) area.frame->data;
  return;
  //2457616 is size
  //101376 is wxh
  /*
  for(int x=(5*area.frame->width); x<((5*area.frame->width)+10); x++) {
    int index = x + (5 * area.frame->width);
    if(data[index] > 13800000 && data[index] < 15000000) {
      //cout << data[index] << endl;
      data[index] = 0xB3FF66;
    }
  }
  */
  //cout << data[(10 * area.frame->width) + 10] << " " << data[(100 * area.frame->width) + 10] << endl;
  cout << data[0] << " " << data[(100 * area.frame->width) + 10] << endl;
  //data[(10 * area.frame->width) + 10] = 0xB3FF66;
  //data[(100 * area.frame->width) + 10] = 0xB3FF66;
  //return;
  return;
  //int max = 0;
  for(int y=0; y < area.frame->height; y++) {
    for(int x=0; x < area.frame->width; x++) {
      int index = x + (y * area.frame->width);
      //if(data[index] < 0)
      //data[index] = 0xFF0000;
      if(data[index] > 13800000 && data[index] < 16000000) {
	//data[index] = 0xB3FF66;
	points.push_back(index);
	if((area.frame->width * area.frame->height - area.frame->width) == index)
	  points.clear();
      }

      if(area.frame->height == (y+1) || 0 == y || area.frame->width == (x+1) || x == 0)
	data[index] = 0xFF0000;

    }
  }

  for(unsigned int x=0; x<points.size(); x++) {
    int index = points[x];
    data[index] = 0xB3FF66;
    data[index + 1] = 0xB3FF66;
    data[index + area.frame->width] = 0xB3FF66;
    data[index + area.frame->width + 1] = 0xB3FF66;
  }

  //cout << max << endl;
  return;

  /*
  int max = 0;
  for(int y=0; y < area.frame->height; y++) {
    for(int x=0; x < area.frame->width; x++) {
      int index = x + (y * area.frame->width);
      if(data[index] > max) max = data[index];
      //if(data[index] > 0xFF0000 && y > 20)
      int yourmom = data[index];
      if(yourmom > 0xCCCCCC)
	data[index] = 0x3366FF;
    }
  }
  */
  //cout << max << endl;
  return;

  /*
  int limit = area.frame->height * area.frame->width;
  int max = 0;
  for(int index=0; index < limit; index++) {
    if(data[index] > max) max = data[index];
    //if(data[index] >= -15265004)  
    // data[index] = -10327220;   
  } 
  cout << max << endl;
  */
  /*
  for(int y=0; y < area.frame->height; y++) {
    for(int x=0; x < area.frame->width; x++) {
      int lineoffset = y * (1408 / 4);
      //int lineoffset = y * area.surface->get_stride();
      //int lineoffset = y * area.width;
      //lineoffset = y * (app->screen->pitch / 4);


      if(data[lineoffset + x] >= 0xD0D0D0) 
	data[lineoffset + x] = 0xff0000;   
      //if(y > 100) data[(24 * area.frame->width * y) + x] = 0xff0000;
    }
  } 
  */
};

