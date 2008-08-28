#include "blobber.h"

using namespace std;

OptionsWindow::OptionsWindow(string device) : exitButton("Exit"), area(device), table(2, 15) { 
  resize(area.width, area.height+50);   
  exitButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::exit) );

  add(table);
  table.attach(area, 0, 2, 0, 14);
  table.attach(exitButton, 0, 1, 14, 15, Gtk::EXPAND, Gtk::EXPAND); 
  show_all_children();
};

void OptionsWindow::exit() {
  hide();
};
