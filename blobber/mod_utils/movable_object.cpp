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
  MovableObject::MovableObject(COORD middle, COLOR c) {
    center.copy(middle);
    color.copy(c);
    selected = false;
  };

  void MovableObject::clear(ProjectionWindow &pw) {
    COLOR oldcolor;
    oldcolor.copy(color);
    color.copy(BLACK);
    paint(pw);
    color.copy(oldcolor);
  };

  void MovableObject::move(COORD newcenter, ProjectionWindow &pw, bool first_clear) {
    if(first_clear)
      clear(pw);
    center.copy(newcenter);
    paint(pw);
  };

  /** 
   * Move object a certain distance toward a newcenter
   * 
   * @param distance Distance to move
   * @param newcenter "direction" to move toward
   * @param pw Projection window to draw on
   * @param first_clear Whether or not the clear method should be called first
   */
  void MovableObject::move(double distance, COORD newcenter, ProjectionWindow &pw, bool first_clear) {
    double ratio = distance / center.distance_from(newcenter);
    double xdistance = fabs(double(center.x - newcenter.x));
    double ydistance = fabs(double(center.y - newcenter.y));
    newcenter.x = ratio * xdistance;
    newcenter.y = ratio * ydistance;
    move(newcenter, pw, first_clear);
  };

  // assume the movable object is just one pixel in size
  void MovableObject::get_bounds(BOUNDS &bounds) {
    BOUNDS b(center.y, center.y, center.x, center.x);
    bounds.copy(b);
  };
};
