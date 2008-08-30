using namespace std;

class ProjectionWindow : public Gtk::Window {
 public:
  int height, width;
  ProjectionWindow(int cw, int ch);
  void draw_line(int source_x, int source_y, int sink_x, int sink_y, COLOR c);
  void draw_point(int x, int y, COLOR c);
  void draw_circle(int x, int y, int radius, COLOR c);
  void draw_box(int left, int right, int top, int bottom, COLOR c);
  void show_alignment_graphics();
  void hide_alignment_graphics();
 protected:
  int cam_height, cam_width;
  // this var tells the window to draw the align graphics (green circles) on expose
  bool need_alignment_graphics; 
  bool on_expose_event(GdkEventExpose* event);
  bool get_context(Cairo::RefPtr<Cairo::Context> &cr);
  void draw_alignment_graphics();
};
