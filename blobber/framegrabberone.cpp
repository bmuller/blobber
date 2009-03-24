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
#include "exception.h"
#include "utils.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cstring>


namespace blobber {

using namespace std;

FrameGrabberOne::FrameGrabberOne(string dev) : cur_frame(-1) {
  fd = open(dev.c_str(), O_RDONLY);
  if (fd == -1 ) {
    throw NoSuchVideoDeviceException(" open video device \"" + dev + "\" failed");
  }
  
  // For n-ary buffering
  cur_frame = -1;

  // Get the device capabilities
  if( ioctl(fd, VIDIOCGCAP, &caps) < 0 ) {
    close(fd);
    throw CameraReadException(" query capabilities failed");
  }
  debug("V4L1: Found camera: " + string(caps.name));

  // Read info for all input sources
  source = 0;
  for (int i = 0; i < caps.channels; i++ ) {
    video_channel vc;
    ioctl(fd, VIDIOCGCHAN, &vc );
    sources.push_back(vc);
    debug("V4L1: Found video source: " + string(vc.name));
  }

  // Don't really care about tuning capabilities
  if(VID_TYPE_TUNER) debug("V4L1: Your video device can be tuned");
  // Read info about tuner
  tuner.tuner = 0;
  if ((caps.type & VID_TYPE_TUNER ) && ioctl(fd, VIDIOCGTUNER, &tuner) < 0 )
    debug("V4L1: cannot get tuner info (not present?)");

  if(ioctl(fd, VIDIOCGWIN, &window) < 0) {
    close(fd);
    throw CameraReadException(" set default window attrs failed");
  }

  // Set default window to max size
  window.x = 0;
  window.y = 0;
  window.width = caps.maxwidth;
  window.height = caps.maxheight;
  window.chromakey = 0;
  window.flags = 0;
  window.clips = NULL;
  window.clipcount = 0;

  if(ioctl(fd, VIDIOCSWIN, &window) < 0) {
    close(fd);
    throw CameraReadException(" set default window attrs failed");
  }

  picture.brightness = 16384;
  picture.hue        = 0;
  picture.colour     = 0;
  picture.contrast   = 8192;
  picture.whiteness  = 0;
  picture.depth      = 32;
  picture.palette    = VIDEO_PALETTE_RGB32;
  //picture.depth      = 24;
  //picture.palette    = VIDEO_PALETTE_RGB24;

  if (ioctl(fd, VIDIOCSPICT, &picture) < 0 ) {
    close(fd);
    throw CameraReadException(" set picture attributes failed");
  }

  // Get frame buffer info
  if ( ioctl(fd, VIDIOCGFBUF, &fbuffer) < 0 ) {
    close(fd);
    throw CameraReadException(" get framebuffer failed");
  }

  // Get the memory buffer info
  if ( ioctl(fd, VIDIOCGMBUF, &mbuf) < 0 ) {
    close(fd);
    throw CameraReadException(" get memory buffer failed");
  }

  // Memory map the video buffer
  mb_map = mmap(0, mbuf.size, PROT_READ, MAP_SHARED, fd, 0);

  if (mb_map == MAP_FAILED) {
    close(fd);
    throw CameraReadException(" mmap buffer not mmapped");
  }
};

FrameGrabberOne::~FrameGrabberOne() {
  close(fd);
};

void FrameGrabberOne::set_contrast(int contrast) {
  string scontrast;
  num_to_string(contrast, scontrast);
  debug("setting contrast to " + scontrast);
  picture.contrast = 256 * contrast;
  if (ioctl(fd, VIDIOCSPICT, &picture) < 0 ) {
    close(fd);
    throw CameraReadException(" set picture attributes failed");
  }
};

void FrameGrabberOne::set_brightness(int brightness) {
  string sbrightness;
  num_to_string(brightness, sbrightness);
  debug("setting brightness to " + sbrightness);
  picture.brightness = 256 * brightness;
  if (ioctl(fd, VIDIOCSPICT, &picture) < 0 ) {
    close(fd);
    throw CameraReadException(" set picture attributes failed");
  }
};


Frame * FrameGrabberOne::makeFrame() {
  int bpp = 0;
  switch (picture.palette) {
  case VIDEO_PALETTE_RGB24:
    // 3 bytes per pixel                                                                                            
    bpp = 3;
    break;
  case VIDEO_PALETTE_RGB32:
    // 4 bytes (1 int) per pixel                                                                                    
    bpp = 4;
    break;
  case VIDEO_PALETTE_RGB565:
    // 2 bytes (16 bits) per pixel                                                                                  
    bpp = 2;
    break;
  case VIDEO_PALETTE_RGB555:
    // 2 bytes (15 bits) per pixel                                                                                  
    bpp = 2;
    break;
  case VIDEO_PALETTE_YUV422:
    // 1 byte (8 bits) per pixel                                                                                    
    bpp = 1;
    break;
  default:
    // Unsupported!                                                                                                 
    throw UnsupportedFrameFormatException(" unsupported frame type");
    bpp = 1;
  }

  // bytes per line = bytes per pixel * width of window
  int bytesperline = bpp * window.width;
  // image size = height * width * bytes per pixel
  int size = window.height * window.width * bpp;

  Frame * fr = new Frame(window.width, window.height, bytesperline, size);
  return fr;
};

void FrameGrabberOne::grabFrame(Frame *frame) {
  int capture_frame = cur_frame + 1;

  // Very first time only
  if (mbuf.frames > 1 && cur_frame == -1 ) {
    cur_frame = 1;

    // Set up capture parameters
    vmmap.format = picture.palette;
    vmmap.frame  = cur_frame;
    vmmap.width  = window.width;
    vmmap.height = window.height;

    // Start capture
    if (ioctl(fd, VIDIOCMCAPTURE, &vmmap) < 0 ) {
      close(fd);
      throw CameraReadException(" failed to capture frame");
    }
  }

  // Start capturing next frame
  // Wrap counter
  if ( capture_frame >= mbuf.frames )
    capture_frame = 0;

  // Set up capture parameters
  vmmap.format = picture.palette;
  vmmap.frame  = capture_frame;
  vmmap.width  = window.width;
  vmmap.height = window.height;

  // Start capture
  if (ioctl(fd, VIDIOCMCAPTURE, &vmmap) < 0) {
    close(fd);
    throw CameraReadException(" failed to capture frame");
  }

  // Wait for end of frame
  if (ioctl(fd, VIDIOCSYNC, &cur_frame) < 0 ) {
    close(fd);
    throw CameraReadException(" failed to sync frame");
  }

  // Save video buffer into our own memory
  memcpy(frame->data, ((unsigned char *) mb_map + mbuf.offsets[cur_frame]), frame->sizeimage);

  // Move along to the next one
  cur_frame = capture_frame;
};

};
