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
  wait_cycle = 0;
  frame_ref = (unsigned char *)(malloc(area.frame->sizeimage));
  string filename, mode, grd, gbd, stivity;
  config_get_set("image_file", filename, string(DATAROOTDIR) + "/green_screen/stone-640x480.jpg");
  config_get_set("mode", mode, "background"); 
  config_get_set("green_red_diff", grd, "30");
  config_get_set("green_blue_diff", gbd, "20");
  config_get_set("sensitivity", stivity, "30");
  if( mode.compare("background") == 0 ) { rep_mode = BACKGROUND; }
  else if ( mode.compare("greencolor") == 0 ) { rep_mode = GREENCOLOR; }
  green_red_diff = (unsigned char) string_to_int(grd);
  green_blue_diff = (unsigned char) string_to_int(gbd);
  sensitivity = string_to_int(stivity);
  
  // error check sensitivity
  if (sensitivity < 0) sensitivity *= -1;
  if (sensitivity > 100) sensitivity = 100;

  Glib::RefPtr<Gdk::Pixbuf> temp_buf;
  try { temp_buf = Gdk::Pixbuf::create_from_file(filename); }
  catch(Glib::FileError err) { 
    throw ModuleFileException(" file " + filename + "  not found!");
  }  
  
  // scale image to camera resolution
  image = temp_buf->scale_simple(area.frame->width, area.frame->height, Gdk::INTERP_BILINEAR);
}  

void GreenScreen::update(Camarea &area, ProjectionWindow &pw) {

  unsigned char * data;

  //gotta have this to let the camera come online before the baseline image is captured
  //a hot key would be beneficial here...
  if (wait_cycle < 50) { wait_cycle++; return; }
  else if (wait_cycle == 50){
    memcpy( (void *) frame_ref, area.frame->data, (size_t) area.frame->sizeimage ); 
    wait_cycle++; // stop checking
  }
  
  int frame_ref_offset = ( (unsigned char *) area.frame->data ) - frame_ref;
  guint8 * image_data = image->get_pixels();

  for( data = (unsigned char *) area.frame->data; 
       data - (unsigned char *) area.frame->data < ( area.frame->sizeimage ); 
       data+=4 ) {
    if( (rep_mode == GREENCOLOR && *(data+1) - *data > green_red_diff && *(data+1) - *(data+2) > green_blue_diff) || 
        (rep_mode == BACKGROUND && labs( *(data) - *(data - frame_ref_offset)) < sensitivity )){
      *data = *(image_data+2); 
      *(data+1) = *(image_data+1); 
      *(data+2) = *(image_data); 
      *(data+3) = 0x00;
    }
    image_data+=3;
  }  
}

extern "C" { 
  ModInterface *get_module() { return new GreenScreen(); }; 
}
