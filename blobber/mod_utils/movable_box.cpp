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

#include "movable_box.h"

namespace blobber {

  MovableBox::MovableBox(PROPORTION _prop, COORD center, COLOR c) : MovableObject(center, c) {
    prop.copy(_prop);
    height = width = 0;
  }

  bool MovableBox::in_bounds(COORD c) {
    int hwidth = width / 2;
    int hheight = height / 2;
    return c.x < (center.x + hwidth) && c.x > (center.x - hwidth) && \
      c.y > (center.y - hheight) && c.y < (center.y + hheight);
  };

  void MovableBox::paint(ProjectionWindow &pw) {
    width = prop.convert_width(pw.dimensions.width);
    height = prop.convert_height(pw.dimensions.height);
    int hsize_width = width / 2;
    int hsize_height = height / 2;
    COORD topleft((center.x-hsize_width), (center.y-hsize_height));
    pw.draw_box(topleft, width, height, color, true);
  };

  void MovableBox::clear(ProjectionWindow &pw) {
    // no idea why small single pixel border left when clearing a box
    width+=1;
    height+=1;
    int hsize_width = width / 2;
    int hsize_height = height / 2;
    COORD topleft((center.x-hsize_width), (center.y-hsize_height));
    pw.draw_box(topleft, width, height, background, true);
    width-=1;
    height-=1;
  };

};
