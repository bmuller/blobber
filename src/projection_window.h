using namespace std;

class ProjectionWindow : public Gtk::Window {
 public:
  ProjectionWindow(int cw, int ch);
  void draw_line(int source_x, int source_y, int sink_x, int sink_y, COLOR c);
  void draw_point(int x, int y, COLOR c);
 protected:
  virtual bool on_expose_event(GdkEventExpose* event);
  Gtk::DrawingArea drawing_area;
  int cam_height, cam_width;
};
