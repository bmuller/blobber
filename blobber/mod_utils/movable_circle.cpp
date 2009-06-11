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

#include "movable_circle.h"

namespace blobber {
  
  /** 
   * 
   * 
   * @param _size radius
   * @param center 
   * @param c 
   */
  MovableCircle::MovableCircle(PROPORTION _prop, COORD center, COLOR c) : MovableObject(center, c) {
    radius = 0;
    prop.copy(_prop);
  }

  bool MovableCircle::in_bounds(COORD c) {
    return center.distance_from(c) <= radius;
  };

  void MovableCircle::paint(ProjectionWindow &pw) {
    // since we are an object with equal height and width, our proportion will be to height+width/2 of the
    // projection window
    radius = (int) (double(prop.convert_width(pw.dimensions.width) + prop.convert_height(pw.dimensions.height)) / 2.0);
    pw.draw_circle(center, radius, color, true);
  };

  void MovableCircle::clear(ProjectionWindow &pw) {
    // no idea why small single pixel border left when clearing
    pw.draw_circle(center, radius+1, background, true);
  };

};
