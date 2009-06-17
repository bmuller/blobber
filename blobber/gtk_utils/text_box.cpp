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

#include <gtkmm/textbuffer.h>

#include "exception.h"

#include "config.h"
#include "utils.h"
#include "text_box.h"

namespace blobber {
  using namespace std;

  TextBox::TextBox() {
    set_left_margin(10);
    set_right_margin(10);
    set_cursor_visible(false);
    set_wrap_mode(Gtk::WRAP_WORD);
    set_editable(false);

    Glib::RefPtr<Gtk::TextBuffer> tb = get_buffer();
    Glib::RefPtr<Gtk::TextTag> bigtag = Gtk::TextTag::create("big");
    bigtag->property_scale() = 1.3;
    tb->get_tag_table()->add(bigtag);

    Glib::RefPtr<Gtk::TextTag> heading = Gtk::TextTag::create("heading");
    heading->property_scale() = 1.2;
    heading->property_underline() = Pango::UNDERLINE_SINGLE;
    tb->get_tag_table()->add(heading);
  };

  void TextBox::add_big_text(string txt, bool newline) {
    Glib::RefPtr<Gtk::TextBuffer> tb = get_buffer();
    tb->insert_with_tag(tb->end(), txt, tb->get_tag_table()->lookup("big"));
    if(newline) 
      add_newline();
  };

  void TextBox::add_text(string txt, bool newline) {
    Glib::RefPtr<Gtk::TextBuffer> tb = get_buffer();
    tb->insert(tb->end(), txt);
    if(newline) 
      add_newline();
  };

  void TextBox::add_image(string filename, bool newline) {
    Glib::RefPtr<Gtk::TextBuffer> tb = get_buffer();    
    try {
      tb->insert_pixbuf(tb->end(), Gdk::Pixbuf::create_from_file(filename));
    } catch(Glib::FileError &e) {
      error("Problem loading filename " + filename + ": " + e.what());
    }
    if(newline) 
      add_newline();
  };

  void TextBox::add_newline() {
    Glib::RefPtr<Gtk::TextBuffer> tb = get_buffer();    
    tb->insert(tb->end(), "\n");
  };

  void TextBox::add_section(string header, string txt, bool newlines) {
    Glib::RefPtr<Gtk::TextBuffer> tb = get_buffer();
    tb->insert_with_tag(tb->end(), header, tb->get_tag_table()->lookup("heading"));
    add_newline();
    add_text(txt, false);
    if(newlines) {
      add_newline();
      add_newline();
    }
  };
};
