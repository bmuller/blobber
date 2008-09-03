using namespace std;

class Camarea : public Gtk::DrawingArea {
 private:
  string device;
  FrameGrabber *fg;
  Cairo::RefPtr< Cairo::ImageSurface > surface;
  Cairo::RefPtr< Cairo::ImageSurface > noCam;
  COORD mouse_click;
  bool mouse_clicked;

 public:
  bool hascam, manual_align;
  int height, width;
  Frame *frame;
  BOUNDS bounds;

  Camarea(string _device);
  ~Camarea();
  bool on_motion_notify_event (GdkEventMotion* event);
  bool on_button_press_event(GdkEventButton* event);
  bool on_button_release_event(GdkEventButton* event);
  void update_screen();
  void update_frame();
  void set_device(string _device);
  void set_bounds(BOUNDS &b);
  void draw_bounds(BOUNDS &b);
};
