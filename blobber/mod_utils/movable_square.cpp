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

#include "../blobber.h"

namespace blobber {

  // @param _size is the width and the height
  MovableSquare::MovableSquare(int _size, COORD center, COLOR c) : MovableObject(center, c) {
    size = _size;
  }

  bool MovableSquare::in_bounds(COORD c) {
    int hsize = size / 2;
    return c.x < (center.x + hsize) && c.x > (center.x - hsize) && \
      c.y > (center.y - hsize) && c.y < (center.y + hsize);
  };

  void MovableSquare::paint(ProjectionWindow &pw) {
    int hsize = size / 2;
    COORD topleft((center.x-hsize), (center.y-hsize));
    pw.draw_box(topleft, size, size, color, true);
  };

  void MovableSquare::clear(ProjectionWindow &pw) {
    // no idea why small single pixel border left when clearing a box
    size+=2;
    MovableObject::clear(pw);
    size-=2;
  };

};
