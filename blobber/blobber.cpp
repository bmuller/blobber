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

using namespace blobber;

int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  Application *b = Application::get_app();
  b->run();
  Application::delete_app();
  return 0;
}

