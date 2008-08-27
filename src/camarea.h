using namespace std;

class Camarea : public Gtk::DrawingArea {
public:
  FrameGrabber *fg;
  Frame * frame;
  int height, width;
  string device;
  bool hascam;

  Camarea(string _device);
  virtual bool on_motion_notify_event (GdkEventMotion* event);
  virtual bool on_button_press_event(GdkEventButton* event);
  virtual bool on_button_release_event(GdkEventButton* event);
  bool update();
  void set_device(string _device);
};
