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
#include <gtkmm/scrolledwindow.h>

#include "config.h"

#include "help_window.h"

namespace blobber {
  using namespace std;

  HelpWindow::HelpWindow() : okButton(Gtk::Stock::OK) { 
    set_title(string(PACKAGE_NAME) + " Help");
    set_resizable(true);
    
    okButton.signal_clicked().connect(sigc::mem_fun(*this, &HelpWindow::ok));

    textBox.add_newline();
    textBox.add_image(Glib::build_filename(string(DATAROOTDIR), "blobber.png"));
    textBox.add_newline();
    textBox.add_section("Help Introduction",
			string(PACKAGE_STRING) + " is a program that tracks light and displays results "
			"(based on selected modules) in a window.  In order to use it, you will need a "
			"webcam, a computer (running a recent version of the linux kernel), and a "
			"projector."
			);
    textBox.add_section("Initialization",
			"Once it's running, there are two windows. One is what the camera sees, and the "
			"other is the projection window (it often comes up right behind the camera "
			"window, so you might have to move that to see it). Make sure that the computers "
			"projected screen can be seen by the camera (move the camera as necessary so that "
			"the whole projection can be seen by the camera). Click on the projection window and "
			"then press \"f\" - this will make it go fullscreen. Then press \"a\" - this will "
			"flash blue and attempt to align the camera and the projector. Press \"f\" again "
			"(to unfullscreen the projected window) - and make sure that the red lines surround "
			"the projected window. If not, press \"f\" and \"a\" again - to attempt another "
			"alignment. Once the red lines (roughly) surround the projected image, you're ready "
			"to start using the laser. You can also manually specify the location of the "
			"projection in the camera window by drawing a box with your mouse."
			);

    //Gtk::ScrolledWindow sw;
    //sw.add(textBox);
    mainBox.set_size_request(400, 500);
    add(mainBox);
    //mainBox.pack_start(sw, Gtk::PACK_SHRINK);
    mainBox.pack_start(textBox, Gtk::PACK_SHRINK);
    mainBox.pack_end(okButton, Gtk::PACK_SHRINK);

    show_all_children();
  };

  void HelpWindow::ok() {
    hide();
  };

};
