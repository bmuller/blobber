#include "green_screen.h"
#include <fstream>

using namespace std;

GreenScreen::GreenScreen() : ModInterface("GreenScreen") {
  string filename = string(DATAROOTDIR) + "/green_screen/stone-640x480.jpg";
  try { image = Gdk::Pixbuf::create_from_file(filename); }
  catch(Glib::FileError err) { 
    cout << filename << " File not found!" << endl; 
    exit(1);
  }  
}

GreenScreen::~GreenScreen() {}

void GreenScreen::update(Camarea &area, ProjectionWindow &pw) {
  green_red_diff = 35; 
  green_blue_diff = 25; 
  unsigned char * data;
  Glib::RefPtr<Gdk::Pixbuf> buf = image;
  guint8 * image_data = buf->get_pixels();

  for( data = (unsigned char *) area.frame->data; 
       data - (unsigned char *) area.frame->data < ( area.frame->sizeimage ); 
       data+=4 ) {
    if( *(data+1) - *data > green_red_diff && *(data+1) - *(data+2) > green_blue_diff ) {
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
