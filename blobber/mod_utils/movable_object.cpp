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
};
