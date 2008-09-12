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

  OptionsWindow::OptionsWindow() : exitButton("Exit"), table(2, 15) { 
    resize(100, 100);
    exitButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::exit) );
    
    add(table);
    Gtk::CheckButton cb("Enable", true);
    table.attach(cb, 0, 1, 0, 1);
    table.attach(exitButton, 0, 1, 14, 15, Gtk::EXPAND, Gtk::EXPAND); 
    show_all_children();
  };

  void OptionsWindow::exit() {
    hide();
  };
};
