#ifndef __PROJECTION_WINDOW_H
#define __PROJECTION_WINDOW_H

#include <gtkmm/window.h>
#include "types.h"
#include "configuration.h"

namespace blobber {
  using namespace std;

  class ProjectionWindow : public Gtk::Window {
  public:
    DIMENSION dimensions;
    int preferred_color; // preferred_color is index in colors
    vector<COLOR> colors;
    bool is_fullscreen;
    bool need_alignment; 
    bool i_exposed_myself;
    string current_message;
    // These are the bounds of the visible projection area - see 
    // http://trac.butterfat.net/public/blobber/wiki/DevDocs
    BOUNDS vprojbounds;
    COLOR current_color;
    
    ProjectionWindow(DIMENSION _cam_dimensions);
    void finish();
    void draw_line(COORD source, COORD sink, COLOR c, double line_width=2.0);
    void draw_point(COORD coords, COLOR c);
    void draw_circle(COORD coords, int radius, COLOR c, bool fill=false);
    void draw_box(COORD coord, int width, int height, COLOR c, bool fill=false);
    void draw_box_absolute(COORD coord, int width, int height, COLOR c, bool fill=false);
    void draw_curve(vector<COORD> points, COLOR c, double line_width=2.0);
    void show_alignment_graphics();
    void hide_alignment_graphics();
    void set_bounds(BOUNDS &b);
    void set_background(COLOR c);
    void set_background_image(string filelocation);
    void clear(COLOR c=BLACK);
    bool on_key_press_event(GdkEventKey* eventData);
    void show_message(string msg, COLOR c=WHITE, COLOR clear=BLACK);
    void set_color(Cairo::RefPtr<Cairo::Context> cr, COLOR c);
    void get_drawing_area_dimensions(DIMENSION &d);

    bool in_visible_bounds(COORD &location);
    bool in_visible_bounds(BOUNDS &bounds);
    
    // translate coordinates from camera coordinates to projection window coordinates
    void translate_coordinates(COORD camcords, COORD &projcoords);
  protected:
    DIMENSION cam_dimensions;
    Configuration *config;
    // this var tells the window to draw the align graphics (green circles) on expose
    bool on_expose_event(GdkEventExpose* event);
    bool get_context(Cairo::RefPtr<Cairo::Context> &cr);
    void draw_alignment_graphics();
  private:
    void save_to_file();
  };
};

#endif /* __PROJECTION_WINDOW_H */

