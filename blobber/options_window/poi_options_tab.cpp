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

#include "poi_options_tab.h"
#include "config.h"

namespace blobber {
  using namespace std;

  POIOptionsTab::POIOptionsTab(Camarea *_cam) :
    cam(_cam),
    poiCriteriaWindow(1, 100, 1.0),
    poiCriteriaWindowFrame("Laser Color Window")
  {
    description.set_text("These settings affect which light is tracked.  The RGB settings for the "
			 "point of interest criteria can be set automatically by drawing a box on "
			 "the camera window with the right mouse button (see the help documentation "
			 "for more information).  The Laser Color Window is the amount (plus and minus) "
			 "to either side of the RGB values that are still considered of interest.");
    description.set_line_wrap(true);
    description.set_line_wrap_mode(Pango::WRAP_WORD);
    set_spacing(10);
    pack_start(description, Gtk::PACK_SHRINK, 20);

    config = Configuration::get_config();

    pack_start(poiCriteriaWindowFrame, Gtk::PACK_SHRINK);
    poiCriteriaWindowFrame.add(poiCriteriaWindow);

    string slidePosition;
    config->get_set("default_criteria_window", slidePosition, DEFAULT_CRITERIA_WINDOW_SIZE);
    poiCriteriaWindow.set_value(string_to_int(slidePosition));
  };

  void POIOptionsTab::save() {
    string slider;
    num_to_string(poiCriteriaWindow.get_value(), slider);
    config->set("default_criteria_window", slider);
    cam->reload_default_criteria();
  };
};
