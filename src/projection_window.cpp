#include "blobber.h"

using namespace std;

ProjectionWindow::ProjectionWindow(int cw, int ch) : is_fullscreen(false), cam_width(cw), cam_height(ch), need_alignment(false) {
  resize(cam_width, cam_height);
};

bool ProjectionWindow::on_key_press_event(GdkEventKey* eventData) {
#ifdef DEBUG
  string ks;
  num_to_string((int) eventData->keyval, ks);
  debug("key pressed on projection window: "+ks);
#endif
  switch(eventData->keyval) {
  case 32: // spacebar
    clear();
    break;
  case 102: // f
    if(is_fullscreen)
      unfullscreen();
    else
      fullscreen();
    is_fullscreen = !is_fullscreen;
    break;
  case 65307: // ESC
    unfullscreen();
    is_fullscreen = false;
    break;
  case 97:
    need_alignment = true;
    break;
  };
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

  if(need_alignment)
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

  cr->set_source_rgb(c.cairo_red(), c.cairo_green(), c.cairo_blue());
  cr->move_to(projcoords_source.x, projcoords_source.y);
  cr->line_to(projcoords_sink.x, projcoords_sink.y);
  cr->stroke();
};

void ProjectionWindow::draw_point(COORD cords, COLOR c) {
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  COORD translated;
  translate_coordinates(COORD(cords.x, cords.y), translated);
  cr->set_source_rgb(c.cairo_red(), c.cairo_green(), c.cairo_blue());
  cr->move_to(translated.x, translated.y);
  cr->line_to(translated.x+1, translated.y);
  cr->stroke();
};

void ProjectionWindow::draw_alignment_graphics() {
  set_background(BLUE);
};

void ProjectionWindow::show_alignment_graphics() {
  need_alignment = true;
  draw_alignment_graphics();
};

void ProjectionWindow::hide_alignment_graphics() {
  need_alignment = false;
  set_background(BLACK);
};

void ProjectionWindow::draw_circle(COORD coords, int radius, COLOR c) {
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->save();
  cr->arc(coords.x, coords.y, radius, 0.0, 2.0 * M_PI); // full circle
  cr->set_source_rgb(c.cairo_red(), c.cairo_green(), c.cairo_blue());
  cr->fill_preserve();
  cr->restore();
  cr->stroke();
};

void ProjectionWindow::draw_box(int left, int right, int top, int bottom, COLOR c) {
  Cairo::RefPtr<Cairo::Context> cr;
  if(!get_context(cr))
    return;

  cr->save();
  cr->set_source_rgb(c.cairo_red(), c.cairo_green(), c.cairo_blue());
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
  projcoords.x = (int) ((float(camcords.x - vprojbounds.left) / float(vprojbounds.width())) * width);
  projcoords.y = (int) ((float(camcords.y - vprojbounds.top) / float(vprojbounds.height())) * height);
  /*
  cout << "((" << camcords .y << " - " << vprojbounds.top << ") / " << vprojbounds.height() << ") * " << width << endl;
  string cs, ps;
  camcords.to_s(cs);
  projcoords.to_s(ps);
  debug("Converted " + cs + " to " + ps);
  */
};

void ProjectionWindow::clear(COLOR c) {
  set_background(c);
};
