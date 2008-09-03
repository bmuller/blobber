#include "blobber.h"

FrameGrabber * FrameGrabberFactory::create(string dev) {
  // eventually, this line will go away, following would be used
  return new FrameGrabberOne(dev);

#ifdef HAVE_V4L1
  // test v1, return if works
#endif

#ifdef HAVE_V4L2
  // test v2, return if works
#endif

  // and if we're here, then neither worked, so die loudly
};

