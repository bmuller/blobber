#ifndef __HELP_WINDOW_H
#define __HELP_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/scrolledwindow.h>

#include "gtk_utils/text_box.h"

namespace blobber {
  using namespace std;

  class HelpWindow : public Gtk::Window {
  public:	
    HelpWindow();
    void ok();
  protected:
    Gtk::VBox mainBox;
    Gtk::Button okButton;
    Gtk::ScrolledWindow sw;
    TextBox textBox;
  };
};

#endif /* __HELP_WINDOW_H */

