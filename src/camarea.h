using namespace std;

class Camarea : public Gtk::DrawingArea {
public:
  FrameGrabber *fg;
  Frame *frame;
  int height, width;
  string device;
  bool hascam;
  Cairo::RefPtr< Cairo::ImageSurface > surface;
 
  Camarea(string _device);
  ~Camarea() { if(hascam) delete frame; };
  virtual bool on_motion_notify_event (GdkEventMotion* event);
  virtual bool on_button_press_event(GdkEventButton* event);
  virtual bool on_button_release_event(GdkEventButton* event);
  unsigned char * get_data() { return surface->get_data(); };
  void update_screen();
  void update_frame();
  void set_device(string _device);
};
