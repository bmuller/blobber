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

#include "sprite.h"

namespace blobber {

  Sprite::Sprite(string _filelocation, COORD center, COLOR background) : MovableObject(center, background) {
    filelocation = _filelocation;
    img = Gdk::Pixbuf::create_from_file(filelocation);
    dimensions.width = img->get_width();
    dimensions.height = img->get_height();
    // half_dimensions is used to figure out where the center of the dimensions is
    half_dimensions.scale(HALF_PROPORTION, dimensions);
    untranslated_half_dimensions.copy(half_dimensions);
    blkimg = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, img->get_bits_per_sample(), 
				 dimensions.width, dimensions.height);
    blkimg->fill(color.to_int());
  };

  bool Sprite::in_bounds(COORD c) {
    return c.x < (center.x + untranslated_half_dimensions.width) \
      && c.x > (center.x - untranslated_half_dimensions.width) \
      && c.y > (center.y - untranslated_half_dimensions.height) \
      && c.y < (center.y + untranslated_half_dimensions.height);
  };

  void Sprite::paint(ProjectionWindow &pw) {
    // convert coords which are middle to top left
    COORD old_coord(center.x - untranslated_half_dimensions.width, center.y - untranslated_half_dimensions.height);
    COORD new_coord;
    pw.translate_coordinates(old_coord, new_coord);
    pw.get_window()->draw_pixbuf(pw.get_style()->get_black_gc(), img, 0, 0, new_coord.x, 
				 new_coord.y, dimensions.width, dimensions.height, Gdk::RGB_DITHER_NORMAL, 0, 0);
  };

  void Sprite::clear(ProjectionWindow &pw) {
    // convert coords which are middle to top left
    COORD old_coord(center.x - untranslated_half_dimensions.width, center.y - untranslated_half_dimensions.height);
    COORD new_coord;
    pw.translate_coordinates(old_coord, new_coord);
    pw.get_window()->draw_pixbuf(pw.get_style()->get_black_gc(), blkimg, 0, 0, new_coord.x, 
				 new_coord.y, dimensions.width, dimensions.height, Gdk::RGB_DITHER_NORMAL, 0, 0);
  };

  void Sprite::get_bounds(BOUNDS &bounds) {
    BOUNDS b(center.y - untranslated_half_dimensions.height, center.y + untranslated_half_dimensions.height, 
	     center.x - untranslated_half_dimensions.width, center.x + untranslated_half_dimensions.width);
    bounds.copy(b);
  };

  void Sprite::scale(PROPORTION p, ProjectionWindow &pw) {
    // reload image to prevent loosing data if window is downsized
    img = Gdk::Pixbuf::create_from_file(filelocation);
    dimensions.scale(p, pw.dimensions);
    half_dimensions.scale(HALF_PROPORTION, dimensions);
    img = img->scale_simple(dimensions.width, dimensions.height, Gdk::INTERP_HYPER);
    blkimg = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, img->get_bits_per_sample(), 
				 dimensions.width, dimensions.height);
    blkimg->fill(color.to_int());
  };
};
