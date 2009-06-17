#ifndef __TEXT_BOX_H
#define __TEXT_BOX_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/textview.h>

namespace blobber {
  using namespace std;

  class TextBox : public Gtk::TextView {
  public:	
    TextBox();
    void add_big_text(string txt, bool newline=true);
    void add_text(string txt, bool newline=true);
    void add_image(string filename, bool newline=true);
    void add_newline();
    void add_section(string header, string txt, bool newlines=true);
  };
};

#endif /* __TEXT_BOX_H */

