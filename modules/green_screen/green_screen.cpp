/*
    Blobber is webcam based blob tracking software
    Copyright (C) 2008 Charleston, SC Linux Users Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "green_screen.h"
#include <fstream>

using namespace blobber;
using namespace std;

GreenScreen::GreenScreen() : ModInterface("GreenScreen") {
}

void GreenScreen::init(Camarea &area, ProjectionWindow &pw) {
  ///////////////////
  waitCycle = 0;
  frameRef = (unsigned char *)(malloc(area.frame->sizeimage));
  ///////////////////

  config_get_set("image_file", filename, string(DATAROOTDIR) + "/green_screen/stone-640x480.jpg");
  Glib::RefPtr<Gdk::Pixbuf> temp_buf;
  try { temp_buf = Gdk::Pixbuf::create_from_file(filename); }
  catch(Glib::FileError err) { 
    throw ModuleFileException(" file " + filename + "  not found!");
  }  
  image = temp_buf->scale_simple(area.frame->width, area.frame->height, Gdk::INTERP_BILINEAR);
/*  string grd, gbd;
  config_get_set("green_red_diff", grd, "30");
  config_get_set("green_blue_diff", gbd, "20");
  green_red_diff = (unsigned char) string_to_int(grd);
  green_blue_diff = (unsigned char) string_to_int(gbd);*/
}  

GreenScreen::~GreenScreen() {}

void GreenScreen::update(Camarea &area, ProjectionWindow &pw) {

  unsigned char * data;

  //gotta have this to let the camera come online before the baseline image is captured
  //a hot key would be beneficial here...
  if (waitCycle < 50) waitCycle++;
  //after wait, get the frame data and put it in memory
  if (waitCycle == 50){
    int ctr = 0;
    for( data = (unsigned char *) area.frame->data; 
         data - (unsigned char *) area.frame->data < ( area.frame->sizeimage ); 
         data++ )
      {
        *(frameRef + ctr) = *data;
        ctr ++;
      } 
      waitCycle++; // stop checking
  }

  int frameRefDelta = (unsigned char *)area.frame->data - frameRef;

  Glib::RefPtr<Gdk::Pixbuf> buf = image;
  guint8 * image_data = buf->get_pixels();

  for( data = (unsigned char *) area.frame->data; 
       data - (unsigned char *) area.frame->data < ( area.frame->sizeimage ); 
       data+=4 ) {
      //if( *(data+1) - *data > green_red_diff && *(data+1) - *(data+2) > green_blue_diff ) {
      //This line uses a sensitivity of 30 hard coded
      //We could set an optinon here that can be tweaked real-time
      if( (*(data) - *(data - frameRefDelta)) < 30 && (*(data) - *(data - frameRefDelta)) > -30){
      *data = *(image_data+2); 
      *(data+1) = *(image_data+1); 
      *(data+2) = *(image_data); 
      *(data+3) = 0x00;
    }
    image_data+=3;
  }
};

extern "C" { 
  ModInterface *get_module() { return new GreenScreen(); }; 
}; 
