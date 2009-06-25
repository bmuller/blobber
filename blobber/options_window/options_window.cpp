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

#include <gtkmm/stock.h>

#include "options_window.h"
#include "application.h"

namespace blobber {
  using namespace std;

  OptionsWindow::OptionsWindow(Camarea *cam) : 
    cot(cam),
    poiot(cam),
    okButton(Gtk::Stock::OK),
    cancelButton(Gtk::Stock::CANCEL),
    buttons(1, 4, true)
  { 
    set_title(string(PACKAGE_NAME) + " Configuration");
    set_resizable(false);
    set_size_request(400, 500);

    okButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::ok));
    cancelButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::cancel));

    nb.append_page(mot, "Enabled Modules");
    nb.append_page(cot, "Camera");
    nb.append_page(poiot, "Tracking");
    nb.append_page(aot, "Additional");
    mainBox.pack_start(nb);

    buttons.set_size_request(400, 35);    
    buttons.attach(okButton, 2, 3, 0, 1);
    buttons.attach(cancelButton, 3, 4, 0, 1);

    mainBox.pack_start(buttons);
    add(mainBox);
    show_all_children();
  };


  void OptionsWindow::ok() {
    try {
      mot.save();
      cot.save();
      poiot.save();
      aot.save();

      // reload config if no exceptions thrown above
      Application::get_app()->reload_config();
      hide();
    } catch(exception &e) {
      // if error, display error
      error(e.what());
    }
  };

  void OptionsWindow::cancel() {
    hide();
  };

};
