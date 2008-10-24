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

  Sprite::Sprite(string filelocation, COORD center, COLOR background) : MovableObject(center, BLACK) {
    img = Gdk::Pixbuf::create_from_file(filelocation);
    width = img->get_width();
    height = img->get_height();
    half_height = height / 2;
    half_width = width / 2;
    blkimg = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, img->get_bits_per_sample(), width, height);
    blkimg->fill(background.to_int());
  };

  bool Sprite::in_bounds(COORD c) {
    return c.x < (center.x + half_width) && c.x > (center.x - half_width) && \
      c.y > (center.y - half_height) && c.y < (center.y + half_height);
  };

  void Sprite::paint(ProjectionWindow &pw) {
    // convert coords which are middle to top left
    int x = center.x - half_width;
    int y = center.y - half_height;
    pw.get_window()->draw_pixbuf(pw.get_style()->get_black_gc(), img, 0, 0, x, y, width, height, Gdk::RGB_DITHER_NONE, 0, 0);
  };

  void Sprite::clear(ProjectionWindow &pw) {
    // convert coords which are middle to top left
    int x = center.x - half_width;
    int y = center.y - half_height;
    pw.get_window()->draw_pixbuf(pw.get_style()->get_black_gc(), blkimg, 0, 0, x, y, width, height, Gdk::RGB_DITHER_NONE, 0, 0);
  };
};
