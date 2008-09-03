#include "blobber.h"

using namespace std;

Camarea::Camarea(string _device) : device(_device), hascam(true), mouse_clicked(false), manual_align(false) {
  add_events(Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
  try {
    fg = new FrameGrabber(device);
    frame = fg->makeFrame();
    width = frame->width;
    height = frame->height;
  } catch(NoSuchVideoDeviceException &e) {
    noCam = Cairo::ImageSurface::create_from_png("nocam.png");
    hascam = false;
    width = 352;
    height = 288;    
  }
};

Camarea::~Camarea() {
  if(hascam) {
    delete fg;
    delete frame; 
  }
};

void Camarea::set_device(string _device) { 
  device = _device;
  delete fg;
  fg = new FrameGrabber(device);
};

void Camarea::set_bounds(BOUNDS &b) {
  bounds.copy(b);
};

bool Camarea::on_motion_notify_event (GdkEventMotion* event) { 
  if(mouse_clicked && hascam) {
    BOUNDS b;
    COORD c((int) event->x, (int) event->y);
    b.from_coords(c, mouse_click);
    draw_bounds(b);
  };
};

bool Camarea::on_button_press_event(GdkEventButton* event) { 
#ifdef DEBUG
  cout << "camarea mouse press (" << event->x << ", " << event->y << ")";
  if(hascam) {
    unsigned char * data = (unsigned char *) frame->data;
    int index = (width * (int) event->y) + (int) event->x;
    cout << " R:" << (int) data[index*4+2] << " G:" << (int) data[index*4+1] << " B:" << (int) data[index*4] << endl;
  } else {
    cout << endl;
  }
  cout.flush();
#endif
  if(hascam) {
    mouse_clicked = true;
    mouse_click.x = (int) event->x;
    mouse_click.y = (int) event->y;
  }
};

bool Camarea::on_button_release_event(GdkEventButton* event) { 
#ifdef DEBUG
  cout << "camarea mouse release (" << event->x << ", " << event->y << ")" << endl; 
  cout.flush();
#endif
  if(hascam) {
    COORD c((int) event->x, (int) event->y);
    bounds.from_coords(c, mouse_click);
    manual_align = true;
  }
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
      surface = Cairo::ImageSurface::create((unsigned char *) frame->data, Cairo::FORMAT_RGB24, frame->width, frame->height, frame->bytesperline); 
    } else {
      surface = noCam;
    }
    context->set_source(surface, 0.0, 0.0);
    context->rectangle (0.0, 0.0, width, height);
    context->clip();
    context->paint(); 
  }
};


void Camarea::draw_bounds(BOUNDS &b) {
  unsigned char * data = (unsigned char *) frame->data;

  // These will place a border on the camera window to show what it "sees"                                                                                          
  for(int index=(b.top*width+b.left); index<(b.top*width+b.right); index++) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }

  for(int index=(b.bottom*width+b.left); index<(b.bottom*width+b.right); index++) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }

  for(int index=(b.top*width+b.left); index<(b.bottom*width+b.left); index+=width) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }

  for(int index=(b.top*width+b.right); index<(b.bottom*width+b.right); index+=width) {
    data[index*4] = 0;
    data[index*4+1] = 0;
    data[index*4+2] = 210;
  }
};
