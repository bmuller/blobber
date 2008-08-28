#include "blobber.h"

using namespace std;

Camarea::Camarea(string _device) : device(_device), hascam(true) {
  add_events(Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
  try {
    fg = new FrameGrabber(device);
    frame = fg->makeFrame();
    width = frame->width;
    height = frame->height;
  } catch(NoSuchVideoDeviceException &e) {
    hascam = false;
    width = 352;
    height = 288;    
  }
};

void Camarea::set_device(string _device) { 
  device = _device;
  delete fg;
  fg = new FrameGrabber(device);
};

bool Camarea::on_motion_notify_event (GdkEventMotion* event) { 

};

bool Camarea::on_button_press_event(GdkEventButton* event) { 
  cout << "press x: " << event->x << " y: " << event->y << endl; 
};

bool Camarea::on_button_release_event(GdkEventButton* event) { 
  cout << "release x: " << event->x << " y: " << event->y << endl; 
};

void Camarea::update_frame() {
  if(hascam)
    fg->grabFrame(frame); 
};

void Camarea::update_screen() {
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window) {
    Cairo::RefPtr<Cairo::Context> context = get_window()->create_cairo_context();
    if(hascam) {
      surface = Cairo::ImageSurface::create((unsigned char *) frame->data, Cairo::FORMAT_RGB24, frame->width, frame->height, fg->window.width * 4); 
    } else {
      surface = Cairo::ImageSurface::create_from_png("nocam.png");
    }
    context->set_source(surface, 0.0, 0.0);
    context->rectangle (0.0, 0.0, width, height);
    context->clip();
    context->paint(); 
  }
};

