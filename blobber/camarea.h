#ifndef __CAMAREA_H
#define __CAMAREA_H

#include <gtkmm/drawingarea.h>

#include "types.h"
#include "framegrabber.h"

namespace blobber {

using namespace std;

 class Camarea : public Gtk::DrawingArea {
 private:
   string device;
   FrameGrabber *fg;
   Cairo::RefPtr< Cairo::ImageSurface > surface;
   Cairo::RefPtr< Cairo::ImageSurface > noCam;
   COORD mouse_click;
   // points of interest for the modules
   std::map<string, vector<PIXEL> > poi;
   // modules register criteria for defining a poi
   std::map<string, vector<CRANGE> > poi_criteria;
   // modules register maximum number of poi
   std::map<string, int> maxPoints;
   // a list of all mods that just want to use the default criteria (as specified in config)
   std::vector<string> default_criteria_mods;
   // default criteria
   CRANGE default_criteria;
   // size of the default criteria window - range is picked color += default_criteria_window
   int default_criteria_window;
   void update_poi(unsigned char *data, string modname, CRANGE &criteria);
   void update_default_poi(unsigned char *data);
   bool find_the_blob(COLOR &blob, BOUNDS &bounds, unsigned char *data);

 public:
   bool hascam, manual_align;
   DIMENSION dimensions;
   Frame *frame;
   // bounds are the visible bounds in the camera's field of vision, 
   // and mouse_drawing is the bounds being drawn by the mouse 
   // (which will exist every time mouse_clicked is true)
   BOUNDS bounds, mouse_drawing;
   bool mouse_clicked;
   
   Camarea();
   ~Camarea();
   bool on_button_press_event(GdkEventButton* event);
   bool on_button_release_event(GdkEventButton* event);
   void set_brightness(int brightness);
   void set_contrast(int contrast);
   void update_screen();
   void update_frame();
   void set_device(string _device);
   void set_bounds(BOUNDS &b);
   void draw_bounds(BOUNDS &b, COLOR c=RED);
   void register_poi_criteria(string modname, CRANGE range, int maxPoints);
   bool on_motion_notify_event (GdkEventMotion* event);  
   // register poi but use the default criteria
   void register_poi(string modname, int maxPoints);
   void get_poi(string modname, vector<PIXEL> &poi);
   Cairo::RefPtr< Cairo::ImageSurface > getSurface();
 };

};

#endif /* __CAMAREA_H */

