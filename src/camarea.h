using namespace std;

class Camarea : public Gtk::DrawingArea {
public:
  FrameGrabber *fg;
  Frame *frame;
  int height, width;
  string device;
  bool hascam;
  Cairo::RefPtr< Cairo::ImageSurface > surface;
  Cairo::RefPtr< Cairo::ImageSurface > noCam;
  BOUNDS bounds;

  Camarea(string _device);
  ~Camarea();
  virtual bool on_motion_notify_event (GdkEventMotion* event);
  virtual bool on_button_press_event(GdkEventButton* event);
  virtual bool on_button_release_event(GdkEventButton* event);
  unsigned char * get_data() { return surface->get_data(); };
  void update_screen();
  void update_frame();
  void set_device(string _device);
  void set_bounds(BOUNDS &b);
};
