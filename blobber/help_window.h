#ifndef __HELP_WINDOW_H
#define __HELP_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/textview.h>

namespace blobber {
  using namespace std;

  class HelpWindow : public Gtk::Window {
  public:	
    HelpWindow();
    void ok();
  protected:
    Gtk::VBox mainBox;
    Gtk::Button okButton;
    Gtk::TextView textView;
  };
};

#endif /* __HELP_WINDOW_H */

