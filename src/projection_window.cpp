#include "blobber.h"

using namespace std;

ProjectionWindow::ProjectionWindow(int cw, int ch) : cam_width(cw), cam_height(ch), need_alignment_graphics(false) {
  resize(cam_width, cam_height);
};

bool ProjectionWindow::get_context(Cairo::RefPtr<Cairo::Context> &cr) {
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window) {
    Gtk::Allocation allocation = get_allocation();
    width = allocation.get_width();
    height = allocation.get_height();

    cr = window->create_cairo_context();
    return true;
  }
  debug("no cairo context yet");
  return false;
};

bool ProjectionWindow::on_expose_event(GdkEventExpose* event) {
  debug("projection window expose event");
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return true;

  cr->set_source_rgb(0, 0, 0);
  cr->paint();

  if(need_alignment_graphics)
    draw_alignment_graphics();
  return true;
};

void ProjectionWindow::draw_line(int source_x, int source_y, int sink_x, int sink_y, COLOR c) {

};

void ProjectionWindow::draw_point(int x, int y, COLOR c) {

};

void ProjectionWindow::draw_alignment_graphics() {
  draw_circle(20, 20, 20, GREEN);
  draw_circle(width-20, height-20, 20, GREEN);
};

void ProjectionWindow::show_alignment_graphics() {
  need_alignment_graphics = true;
  draw_alignment_graphics();
};

void ProjectionWindow::hide_alignment_graphics() {
  need_alignment_graphics = false;
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->set_source_rgb(0, 0, 0);
  cr->paint();
};

void ProjectionWindow::draw_circle(int x, int y, int radius, COLOR c) {
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->save();
  cr->arc(x, y, radius, 0.0, 2.0 * M_PI); // full circle                                                        
  cr->set_source_rgb(c.red, c.green, c.blue);
  cr->fill_preserve();
  cr->restore();
  cr->stroke();
};

void ProjectionWindow::draw_box(int left, int right, int top, int bottom, COLOR c) {
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->save();
  cr->set_source_rgb(c.red, c.green, c.blue);
  cr->set_line_width(10.0);

  cr->move_to(left, top);
  cr->line_to(right, top);
  cr->line_to(right, bottom);
  cr->line_to(left, bottom);
  cr->line_to(left, top);
  
  cr->restore();
  cr->stroke();
}
