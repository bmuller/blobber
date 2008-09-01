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

void ProjectionWindow::set_bounds(BOUNDS &b) {
  vprojbounds.copy(b);
};

bool ProjectionWindow::on_expose_event(GdkEventExpose* event) {
  debug("projection window expose event");
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return true;

  set_background(BLACK);

  if(need_alignment_graphics)
    draw_alignment_graphics();
  return true;
};

void ProjectionWindow::set_background(COLOR c) {
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->set_source_rgb(c.red, c.green, c.blue);
  cr->paint();
};

void ProjectionWindow::draw_line(COORD source, COORD sink, COLOR c) {
  COORD projcoords_source, projcoords_sink;
  translate_coordinates(COORD(source.x, source.y), projcoords_source);
  translate_coordinates(COORD(sink.x, sink.y), projcoords_sink);
  
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->set_source_rgb(c.red, c.green, c.blue);
  cr->move_to(projcoords_source.x, projcoords_source.y);
  cr->line_to(projcoords_sink.x, projcoords_sink.y);
};

void ProjectionWindow::draw_point(COORD cords, COLOR c) {
  // this might work?
  draw_line(cords, cords, c);
};

void ProjectionWindow::draw_alignment_graphics() {
  set_background(BLUE);
};

void ProjectionWindow::show_alignment_graphics() {
  need_alignment_graphics = true;
  draw_alignment_graphics();
};

void ProjectionWindow::hide_alignment_graphics() {
  need_alignment_graphics = false;
  set_background(BLACK);
};

void ProjectionWindow::draw_circle(COORD coords, int radius, COLOR c) {
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->save();
  cr->arc(coords.x, coords.y, radius, 0.0, 2.0 * M_PI); // full circle
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
};

// see http://trac.butterfat.net/public/blobber/wiki/DevDocs
void ProjectionWindow::translate_coordinates(COORD camcords, COORD &projcoords) { 
  projcoords.x = ((camcords.x - vprojbounds.left) / vprojbounds.width()) * width;
  projcoords.y = ((camcords.y - vprojbounds.top) / vprojbounds.height()) * width;
};

void ProjectionWindow::clear(COLOR c) {
  set_background(c);
};
