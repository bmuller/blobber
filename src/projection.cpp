#include "nerdtag.h"

using namespace std;

Projection::Projection() {
  add_events( Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
  fg = new FrameGrabber("/dev/video0");
  frame = fg->makeFrame();
  width = frame->width;
  height = frame->height;
  Glib::signal_idle().connect( sigc::mem_fun(*this, &Projection::update) );
};

bool Projection::on_motion_notify_event (GdkEventMotion* event) { };
bool Projection::on_button_press_event(GdkEventButton* event) { cout << "press x: " << event->x << " y: " << event->y << endl; };
bool Projection::on_button_release_event(GdkEventButton* event) { cout << "release x: " << event->x << " y: " << event->y << endl; };

bool Projection::update() {
  fg->grabFrame(frame); 
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window) {
    Cairo::RefPtr<Cairo::Context> context = get_window()->create_cairo_context();
    Cairo::RefPtr< Cairo::ImageSurface > surface = Cairo::ImageSurface::create((unsigned char *) frame->data, Cairo::FORMAT_RGB24, frame->width, frame->height, fg->window.width * 4); 
    context->set_source(surface, 0.0, 0.0);
    context->rectangle (0.0, 0.0, width, height);
    context->clip();
    context->paint(); 
    return true;
  } else {
    return false;
  }
};

