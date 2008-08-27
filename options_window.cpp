#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/cairomm.h>
#include <iostream>
#include <glib.h>
#include "nerdtag.h"

using namespace std;

class OptionsWindow : public Gtk::Window {
  Gtk::Button exitButton;
  
  OptionsWindow() { 
    Projection area;
    resize(area.width, area.height+50);
    add(area);
    exitButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::exit) );
    show_all_children();
  };

  void exit() {
    cout << "exit!\n";
  };
};

int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  OptionsWindow win;
  Gtk::Main::run(win);
  return 0;
}
