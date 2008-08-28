#include "../blobber.h"

using namespace std;

void LaserTag::update(Camarea &area, ProjectionWindow &pw) {
  return;
  unsigned int * data = (unsigned int *) area.frame->data;
  /*
  int location = area.frame->width * area.frame->height / 2;
  data[location] = 0xff0000;   
  location = (area.frame->width * area.frame->height / 2) + 1;
  data[location] = 0xff0000;   
  location = (area.frame->width * area.frame->height / 2) + 2;
  data[location] = 0xff0000;   


  location = area.frame->width * area.frame->height / 2;
  data[location] = 0xff0000;   
  location = (area.frame->width * area.frame->height / 2) + area.frame->width + 1;
  data[location] = 0xff0000;   
  location = (area.frame->width * area.frame->height / 2) + area.frame->width + 2;
  data[location] = 0xff0000;   

  location = area.frame->width * area.frame->height - area.frame->width;
  data[location] = 0xff0000;   
  location = area.frame->width * area.frame->height + 1  - area.frame->width;
  data[location] = 0xff0000;   
  location = area.frame->width * area.frame->height + 2  - area.frame->width;
  data[location] = 0xff0000;   

  location = area.frame->width * area.frame->height - area.frame->width;
  data[location] = 0xff0000;   
  location = area.frame->width * area.frame->height + 1  - area.frame->width;
  data[location] = 0xff0000;   
  location = area.frame->width * area.frame->height + 2  - area.frame->width;
  data[location] = 0xff0000;   
  */

  //2457616 is size
  //101376 is wxh
  
  int max = 0;
  for(int y=0; y < area.frame->height; y++) {
    for(int x=0; x < area.frame->width; x++) {
      int index = x + (y * area.frame->width);
      if(data[index] > max) max = data[index];
      //if(data[index] > 0xFF0000 && y > 20)
      int yourmom = data[index];
      if(yourmom > 14000000)
	data[index] = 0x3366FF;
    }
  }
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

