using namespace std;

class Projection : public Gtk::DrawingArea {
public:
  FrameGrabber *fg;
  Frame * frame;
  int height, width;
  Projection();
  virtual bool on_motion_notify_event (GdkEventMotion* event);
  virtual bool on_button_press_event(GdkEventButton* event);
  virtual bool on_button_release_event(GdkEventButton* event);
  bool update();
};
