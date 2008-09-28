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
  
  // @param _size is radius
  MovableCircle::MovableCircle(int _size, COORD center, COLOR c) : MovableObject(center, c) {
    radius = _size;
  }

  bool MovableCircle::in_bounds(COORD c) {
    return center.distance_from(c) <= radius;
  };
  
  void MovableCircle::paint(ProjectionWindow &pw) {
    pw.draw_circle(center, radius, color, true);
  };

  void MovableCircle::clear(ProjectionWindow &pw) {
    // no idea why small single pixel border left when clearing
    radius+=1;
    MovableObject::clear(pw);
    radius-=1;
  };

};
