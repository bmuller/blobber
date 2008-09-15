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
    string devDir = "/dev";
    Glib::Dir listDev(devDir);
    string isVideo;
    isVideo = listDev.read_name();
    do
    {
      if(isVideo.substr(0,5).compare("video") == 0)
        cboCamDevice.append_text(devDir + "/" + isVideo);
      isVideo = listDev.read_name();
    } while(isVideo.compare("") != 0);

    camDevice.add(cboCamDevice);
  //  mainBox.add(modules);
    show_all_children();
  };

  void OptionsWindow::ok() {
    hide();
  };
};
