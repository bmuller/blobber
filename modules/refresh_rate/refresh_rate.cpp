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

#include "refresh_rate.h"

using namespace blobber;
using namespace std;

RefreshRate::RefreshRate() : ModInterface("RefreshRate") {}

void RefreshRate::init(Camarea &area, ProjectionWindow &pw) {
	time(&start);
	count = 0;
}

void RefreshRate::update(Camarea &area, ProjectionWindow &pw) {

	time(&now);
	count +=1;
		
	// Display frame rate every ~3 sec
	if (difftime(now, start) > 3)
	{
		// Leaving the show_message call in here makes awkward pauses
		// when the pw is drawn/redrawn, leaving it here to minimize time load
		// Erase old message FIXME: is there a better way?
		pw.show_message("                              Refresh Rate: " + rate + " cycles/sec", BLACK);

		// Calculate frame rate
		int_to_string(count / static_cast<int>(difftime(now, start)), rate);

		// Display to the right of the color indicator in case key pressed
		pw.show_message("                              Refresh Rate: " + rate + " cycles/sec");

		// Reset timer and counter
		time(&start);
		count = 0;
	}
};

extern "C" {
	ModInterface *get_module() { return new RefreshRate(); }; 
}; 
