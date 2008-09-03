using namespace std;

class ProjectionWindow : public Gtk::Window {
 public:
  int height, width, preferred_color; // preferred_color is index in colors
  vector<COLOR> colors;
  bool is_fullscreen;
  bool need_alignment; 
  // These are the bounds of the visible projection area - see 
  // http://trac.butterfat.net/public/blobber/wiki/DevDocs
  BOUNDS vprojbounds;
  COLOR current_color;

  ProjectionWindow(int cw, int ch);
  void draw_line(COORD source, COORD sink, COLOR c, double line_width=2.0);
  void draw_point(COORD coords, COLOR c);
  void draw_circle(COORD coords, int radius, COLOR c);
  void draw_box(int left, int right, int top, int bottom, COLOR c);
  void show_alignment_graphics();
  void hide_alignment_graphics();
  void set_bounds(BOUNDS &b);
  void set_background(COLOR c);
  void clear(COLOR c=BLACK);
  bool on_key_press_event(GdkEventKey* eventData);
  void show_message(string msg, COLOR c=WHITE);
  void set_color(Cairo::RefPtr<Cairo::Context> cr, COLOR c);
 protected:
  int cam_height, cam_width;
  // this var tells the window to draw the align graphics (green circles) on expose
  bool on_expose_event(GdkEventExpose* event);
  bool get_context(Cairo::RefPtr<Cairo::Context> &cr);
  void draw_alignment_graphics();
  // translate coordinates from camera coordinates to projection window coordinates
  void translate_coordinates(COORD camcords, COORD &projcoords);
};
