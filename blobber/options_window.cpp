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

  OptionsWindow::OptionsWindow() : okButton("OK"), lblCamDevice("Camera devices: ") { 
    resize(100, 100);
    okButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::ok));
    
    add(mainBox);
    mainBox.add(camDevice);
    camDevice.add(lblCamDevice);
    cboCamDevice.append_text("one");
    cboCamDevice.append_text("two");
    cboCamDevice.append_text("three");
    camDevice.add(cboCamDevice);
  //  mainBox.add(modules);
    show_all_children();
  };

  void OptionsWindow::ok() {
    hide();
  };
};
