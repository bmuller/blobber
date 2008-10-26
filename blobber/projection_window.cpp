/*
    Blobber is webcam based blob tracking software
    Copyright (C) 2008 Charleston, SC Linux Users Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "blobber.h"

namespace blobber {
  using namespace std;

  ProjectionWindow::ProjectionWindow(int cw, int ch) : is_fullscreen(false), cam_width(cw), cam_height(ch), need_alignment(false) {
    resize(cam_width, cam_height);
    i_exposed_myself = false;

    config = Configuration::get_config();
    string groupname = "default_colors";
    string key, value;
    vector<string> colornames;
    config->get_keys(colornames, groupname);

    // initialize height, width
    dimensions.width = cam_width;
    dimensions.height = cam_height;

    // set default colors if non existant
    if(colornames.size() == 0) {
      vector<COLOR> default_colors;
      default_colors.push_back(BLUE);
      default_colors.push_back(RED);
      default_colors.push_back(GREEN);
      default_colors.push_back(WHITE);
      default_colors.push_back(LIGHT_BLUE);
      default_colors.push_back(BROWN);
      for(int i=0; i<default_colors.size(); i++) {
	num_to_string(i, key);
	default_colors[i].to_string(value);
	config->set("color_"+key, value, groupname);
      }
    }

    // load colors in config
    string sblack;
    BLACK.to_string(sblack);
    for(unsigned int i=0; i<colornames.size(); i++) {
      COLOR color;
      // default is black
      config->get(colornames[i], value, sblack, groupname);
      color.from_string(value);
      colors.push_back(color);
    }
    config->get("preferred_color", value, "4"); 
    preferred_color = string_to_int(value);
  };

  void ProjectionWindow::finish() {
    string key, value;
    string groupname = "default_colors";
    for(int i=0; i<colors.size(); i++) {
      num_to_string(i, key);
      colors[i].to_string(value);
      config->set("color_"+key, value, groupname);
    }
    num_to_string(preferred_color, value);
    config->set("preferred_color", value);
  };

  void ProjectionWindow::save_to_file() {
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    Cairo::RefPtr<Cairo::Context> cr;
    Cairo::RefPtr<Cairo::Surface> sr;
    string filepath;
    time_t rawTime;
    string nameTime;

    if(!get_context(cr))
      return;
    sr = cr->get_target();
    filepath =  Glib::build_filename(Glib::get_user_data_dir(), "blobber");
    rawTime = time(NULL);
    nameTime = string(ctime(&rawTime));
    filepath = Glib::build_filename(filepath, nameTime.substr(0, nameTime.length()-1)+ "_proj.png");
    debug("Saving projection screen capture to " + filepath);
    sr->write_to_png(filepath);
#else
    throw NoSuchFeatureException("Cairo must be compiled with PNG support to save screen captures")
#endif	
  };

  int ProjectionWindow::get_drawing_area_height() {
    return cam_height;
  };

  int ProjectionWindow::get_drawing_area_width() {
    return cam_width;
  };

  bool ProjectionWindow::in_visible_bounds(COORD &location) {
    BOUNDS b(0, cam_height, 0, cam_width);
    return b.contains(location);
  };

  bool ProjectionWindow::in_visible_bounds(BOUNDS &bounds) { 
    BOUNDS b(0, cam_height, 0, cam_width);
    return b.contains(bounds);
  };
  
  bool ProjectionWindow::on_key_press_event(GdkEventKey* eventData) {
#ifdef DEBUG
    string ks;
    num_to_string((int) eventData->keyval, ks);
    debug("Key pressed on projection window: "+ks);
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
    case 115: // s for save to file
      save_to_file();
      break;
    case 65307: // ESC
      unfullscreen();
      is_fullscreen = false;
      break;
    case 97:
      need_alignment = true;
      break;
    case 99:
      preferred_color = (preferred_color + 1) % colors.size();
      show_message("Color changed", colors[preferred_color]);
      break;
    };
  };

  void ProjectionWindow::set_color(Cairo::RefPtr<Cairo::Context> cr, COLOR c) {
    cr->set_source_rgb(c.cairo_red(), c.cairo_green(), c.cairo_blue());
    current_color.copy(c);
  };

  void ProjectionWindow::show_message(string msg, COLOR c) {
    Cairo::RefPtr<Cairo::Context> cr;
    if(!get_context(cr))
      return;

    COLOR old_color(current_color);
    cr->move_to(10, 10);
    set_color(cr, c);
    cr->show_text(msg);
    set_color(cr, old_color);
  };
  
  bool ProjectionWindow::get_context(Cairo::RefPtr<Cairo::Context> &cr) {
    Glib::RefPtr<Gdk::Window> window = get_window();
    if(window) {
      Gtk::Allocation allocation = get_allocation();
      dimensions.width = allocation.get_width();
      dimensions.height = allocation.get_height();
      
      cr = window->create_cairo_context();
      return true;
    }
    return false;
  };

  void ProjectionWindow::set_bounds(BOUNDS &b) {
    vprojbounds.copy(b);
  };

  bool ProjectionWindow::on_expose_event(GdkEventExpose* event) {
    debug("Projection window expose event");
    i_exposed_myself = true;
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
    set_color(cr, c);
    cr->paint();
  };

  void ProjectionWindow::set_background_image(string filelocation) {
    Glib::RefPtr<Gdk::Pixbuf> img = Gdk::Pixbuf::create_from_file(filelocation);
    Glib::RefPtr<Gdk::Pixbuf> scaled = img->scale_simple(dimensions.width, dimensions.height, Gdk::INTERP_HYPER);
    get_window()->draw_pixbuf(get_style()->get_black_gc(), scaled, 0, 0, 0, 0, 
			      dimensions.width, dimensions.height, Gdk::RGB_DITHER_NONE, 0, 0);
  };

  void ProjectionWindow::draw_line(COORD source, COORD sink, COLOR c, double line_width) {
    COORD projcoords_source, projcoords_sink;
    translate_coordinates(COORD(source.x, source.y), projcoords_source);
    translate_coordinates(COORD(sink.x, sink.y), projcoords_sink);
    
    Cairo::RefPtr<Cairo::Context> cr;
    if(!get_context(cr))
      return;
    
    cr->set_line_width(line_width);
    set_color(cr, c);
    cr->move_to(projcoords_source.x, projcoords_source.y);
    cr->line_to(projcoords_sink.x, projcoords_sink.y);
    cr->stroke();
  };


  void ProjectionWindow::draw_curve(vector<COORD> points, COLOR c, double line_width) {
    Cairo::RefPtr<Cairo::Context> cr;
    if(!get_context(cr) || points.size()!=4)
      return;
    
    cr->set_line_width(line_width);
    set_color(cr, c);
    
    COORD convcoord;
    translate_coordinates(points[0], convcoord);
    cr->move_to(convcoord.x, convcoord.y);
    
    COORD first, second, third;
    translate_coordinates(points[1], first);
    translate_coordinates(points[2], second);
    translate_coordinates(points[3], third);
    cr->curve_to(first.x, first.y, second.x, second.y, third.x, third.y);
    cr->stroke();
  };
  
  void ProjectionWindow::draw_point(COORD cords, COLOR c) {
    Cairo::RefPtr<Cairo::Context> cr;
    if(!get_context(cr))
      return;
    
    COORD translated;
    translate_coordinates(COORD(cords.x, cords.y), translated);
    set_color(cr, c);
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

  void ProjectionWindow::draw_circle(COORD coords, int radius, COLOR c, bool fill) {
    Cairo::RefPtr<Cairo::Context> cr;
    if(!get_context(cr))
      return;

    COORD translated;
    translate_coordinates(coords, translated);
    cr->arc(translated.x, translated.y, radius, 0.0, 2.0 * M_PI); // full circle
    set_color(cr, c);
    if(fill)
      cr->fill_preserve();
    cr->stroke();
  };

  void ProjectionWindow::draw_box(COORD coord, int width, int height, COLOR c, bool fill) {
    Cairo::RefPtr<Cairo::Context> cr;
    if(!get_context(cr))
      return;
    
    COORD translated;
    translate_coordinates(coord, translated);

    COORD trans_opposite;
    COORD opposite(coord.x + width, coord.y + height);
    translate_coordinates(opposite, trans_opposite);

    int transwidth = trans_opposite.x - translated.x;
    int transheight = trans_opposite.y - translated.y;

    set_color(cr, c);
    cr->set_line_width(1.0);
    cr->rectangle(translated.x, translated.y, transwidth, transheight);
    if(fill)
      cr->fill_preserve();
    cr->stroke();
  };

  void ProjectionWindow::draw_box_absolute(COORD coord, int width, int height, COLOR c, bool fill) {
    Cairo::RefPtr<Cairo::Context> cr;
    if(!get_context(cr))
      return;
    
    set_color(cr, c);
    cr->set_line_width(1.0);
    cr->rectangle(coord.x, coord.y, width, height);
    if(fill)
      cr->fill_preserve();
    cr->stroke();
  };

  // see http://trac.butterfat.net/public/blobber/wiki/DevDocs
  void ProjectionWindow::translate_coordinates(COORD camcords, COORD &projcoords) { 
    projcoords.x = (int) ((float(camcords.x - vprojbounds.left) / float(vprojbounds.width())) * dimensions.width);
    projcoords.y = (int) ((float(camcords.y - vprojbounds.top) / float(vprojbounds.height())) * dimensions.height);
  };

  void ProjectionWindow::clear(COLOR c) {
    set_background(c);
  };

};
