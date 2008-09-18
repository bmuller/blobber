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

#include "pong.h"

using namespace std;
using namespace blobber;

Pong::Pong() : ModInterface("Pong") { 

};

void Pong::projection_window_exposed(ProjectionWindow &pw) {

};

void Pong::init(Camarea &area, ProjectionWindow &pw) {
  // adjust these as necessary for different light/laser sources.  Any pixel
  // with the threshold you set will be considered in the input.
  string red, green, blue;
  // get config values - and set them to defaults if they are not set already
  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");

  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range);


  Glib::Rand rand;
  ball_direction = rand.get_int_range(0, 360);
  moving_right = true;
  ball.x = area.width / 2;
  ball.y = area.height / 2;
};


void Pong::update(Camarea &area, ProjectionWindow &pw) {
  pw.draw_circle(ball, 5, BLACK);
  if(ball.x <= 0) {
    moving_right = true;
    ball.x += 5;
  } else if(ball.x >= area.width) {
    moving_right = false;
    ball.x -= 5;
  } else if(moving_right) {
    ball.x += 5;
  } else {
    ball.x -= 5;
  }
  pw.draw_circle(ball, 5, BLUE);  
};


extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
