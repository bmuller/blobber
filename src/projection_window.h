using namespace std;

class ProjectionWindow : public Gtk::Window {
 public:
  ProjectionWindow();
 protected:
  virtual bool on_expose_event(GdkEventExpose* event);
  Gtk::DrawingArea drawing_area;
};
