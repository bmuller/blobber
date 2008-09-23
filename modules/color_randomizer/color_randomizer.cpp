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

#include "color_randomizer.h"

using namespace blobber;
using namespace std;

ColorRandomizer::ColorRandomizer() : ModInterface("ColorRandomizer") {}

void ColorRandomizer::init(Camarea &area, ProjectionWindow &pw) {
	
	string wait;
	config_get_set("ColorShiftTime", wait, "2");
	waitTime = string_to_int(wait);

	pw.colors.push_back(color);
	//Initially set color to random color
	//User can change at will
	pw.preferred_color = (pw.colors.size() - 1);

	time(&last);

}

void ColorRandomizer::update(Camarea &area, ProjectionWindow &pw) {

	time(&now);

	if ( difftime(now, last) > waitTime)
	{
		//use a narrow color band so that colors aren't too dark
		//to see and lasertag won't track lines drawn
		color.red = static_cast<int>(rand.get_int_range(70, 250));
		color.green = static_cast<int>(rand.get_int_range(70, 250));
		color.blue = static_cast<int>(rand.get_int_range(70, 250));
		//set pw color
		pw.colors.pop_back();
		pw.colors.push_back(color);

		//reset last
		time(&last);
	}
};

extern "C" {
	ModInterface *get_module() { return new ColorRandomizer(); }; 
}; 
