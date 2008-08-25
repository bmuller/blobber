#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <iostream>

using namespace std;

class Projection : public Gtk::DrawingArea
{
public:
  Projection() {
    add_events( Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
  }

protected:
  virtual bool on_motion_notify_event (GdkEventMotion* event) {};
  virtual bool on_button_press_event(GdkEventButton* event) { cout << "press x: " << event->x << " y: " << event->y << endl; };
  virtual bool on_button_release_event(GdkEventButton* event) { cout << "release x: " << event->x << " y: " << event->y << endl; };

  //Override default signal handler:
  virtual bool on_expose_event(GdkEventExpose* event) {
    // This is where we draw on the window
    Glib::RefPtr<Gdk::Window> window = get_window();
    if(window) {
      Gtk::Allocation allocation = get_allocation();
      const int width = allocation.get_width();
      const int height = allocation.get_height();

      // coordinates for the center of the window
      int xc, yc;
      xc = width / 2;
      yc = height / 2;

      Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
      cr->set_line_width(10.0);

      // clip to the area indicated by the expose event so that we only redraw
      // the portion of the window that needs to be redrawn
      cr->rectangle(event->area.x, event->area.y,
		    event->area.width, event->area.height);
      cr->clip();

      cr->save();
      cr->set_source_rgb(0, 0, 0);
      cr->paint();
      cr->restore();

      // draw red lines out from the center of the window
      cr->set_source_rgb(0.8, 0.0, 0.0);
      cr->move_to(0, 0);
      cr->line_to(xc, yc);
      cr->line_to(0, height);
      cr->move_to(xc, yc);
      cr->line_to(width, yc);
      cr->stroke();
    }
  };
};

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  Gtk::Window win;
  win.set_title("DrawingArea");

  Projection area;
  win.add(area);
  area.show();

  Gtk::Main::run(win);

  return 0;
}
