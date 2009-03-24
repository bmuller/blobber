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

#include "framegrabber.h"
#include "utils.h"
#include "exception.h"

namespace blobber {

  FrameGrabber * FrameGrabberFactory::create(string dev) {
#ifdef HAVE_V4L1
    // test v1, return if works
    debug("Trying V4L1");
    try {
      return new FrameGrabberOne(dev);
    } catch ( CameraReadException ex ) {
      debug("No V4L1 support:" + string(ex.what()));
    }
#endif

#ifdef HAVE_V4L2
    // test v2, return if works
    debug("Trying V4L2");
    try {
      return new FrameGrabberTwo(dev);
    } catch ( CameraReadException ex ) {
      debug("No V4L2 support:" + string(ex.what()));
    } 
#endif
    // and if we're and neither worked, die loudly
    throw BlobberException(" no V4L support found");
    return (FrameGrabber *) NULL;
  };

};
