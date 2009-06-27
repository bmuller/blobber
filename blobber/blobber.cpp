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

/**
\mainpage Blobber :: The Blob Tracker
\section desc Description
Blobber is a program that tracks points of light (lasers, LEDs) and projects reactions.
Given a webcam, projector, and a computer, the idea is to track lights/colors with the camera and 
then project "reactions" onto a large surface with the projector. Right now, a framework has been 
created to allow the creation of modules to handle output "reactions" based on what the camera sees. 
These modules accept as input the pixel data from the camera and can draw on a Cairo::Context in a 
separate window (this method is called ~ 30 times/sec). Many modules can be loaded at once. You can 
also easily create your own modules. 
 
You can find out more at http://trac.butterfat.net/public/blobber
\section Authors
\li Brian Muller <bmuller@butterfat.net>
\li Matt Shotwell <shotwelm@musc.edu>
\li Zenko B. Klapko Jr. <CovertCodeOp1@gmail.com>
*/

#include "application.h"
#include <gtkmm/main.h>
#include <iostream>

using namespace blobber;
using namespace std;

int main(int argc, char** argv) {
  Glib::OptionContext option_context;
  Glib::OptionGroup main_group("main", "Main Options", "Show main options");

  bool print_version = false;
  Glib::OptionEntry version_entry;
  version_entry.set_short_name('v');
  version_entry.set_long_name("version");
  version_entry.set_description("Print the version info and return.");
  main_group.add_entry(version_entry, print_version);

  bool debug = false;
  Glib::OptionEntry debug_entry;
  debug_entry.set_short_name('d');
  debug_entry.set_long_name("debug");
  debug_entry.set_description("Print debugging information.");
  main_group.add_entry(debug_entry, debug);

  option_context.set_main_group(main_group);
  Gtk::Main main(argc, argv); 
  main.add_gtk_option_group(option_context); 

  // parse options
  try {
    option_context.parse(argc, argv);
  } catch (Glib::OptionError& error) {
    cout << error.what() << endl << endl << option_context.get_help() << endl;
    return -1;
  }

  // handle option choices
  if(print_version) {
    cout << PACKAGE_STRING << endl;
    return 0;
  }
  Configuration *config = Configuration::get_config();
  config->settmp("debug", (debug ? "1" : "0"));

  // run application
  Application *b = Application::get_app();
  b->run();
  Application::delete_app();
  return 0;
}

