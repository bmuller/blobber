#include "blobber.h"

using namespace std;

ProjectionWindow::ProjectionWindow() {
};

bool ProjectionWindow::on_expose_event(GdkEventExpose* event) {
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    cr->set_source_rgb(0, 0, 0);
    cr->paint();
  }
  return true;
};


