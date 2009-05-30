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
#include "types.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

namespace blobber {

  using namespace std;

  /*
  static int xioctl(int fd, int request, void *arg) {
    int r;
    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);
    return r;
  }

  static int tioctl(int fd, int request, void *arg) {
    int r;
    do r = ioctl (fd, request, arg);
    while(-1 == r && EAGAIN == errno);
    return(r);
  }
  */

  static void cam_debug(string mesg, __u8 * info, unsigned int length) {
    char cstr[256];
    memcpy( (void *) cstr, (const void *) info, (length < 256)?length:255);
    cstr[length] = '\0';
    debug(mesg + cstr);
  };

  FrameGrabberTwo::FrameGrabberTwo(string dev) : nbuf(0), flags(0) {

    char desc[32];

    // Open the device, removed "| O_NONBLOCK" to prevent EBUSY
    fd = open(dev.c_str(), O_RDWR, 0); 
    if ( fd == -1 ) {
      close(fd); throw NoSuchVideoDeviceException(" open video device \"" + dev + "\" failed");
    }

    // Get the device capabilities
    if( ioctl(fd, VIDIOC_QUERYCAP, &caps) < 0 ) {
      close(fd); throw CameraReadException(" query capabilities failed");
    } 
    cam_debug("V4L2: Driver: ", caps.driver, 16);
    cam_debug("V4L2: Camera: ", caps.card, 32);
    cam_debug("V4L2: Bus info: ", caps.bus_info, 32);
    if(!(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
      close(fd); throw CameraReadException(" " + dev + " is not a video capture device");
    }

    // Get the pixelformats supported by the device
#ifdef USE_LIBV4LCONVERT
    cond = v4lconvert_create(fd);
    CLEAR(sfmt);
    CLEAR(dfmt);
    dfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    dfmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    dfmt.fmt.pix.height      = FG2_DEFAULT_HEIGHT;
    dfmt.fmt.pix.width       = FG2_DEFAULT_WIDTH;
    dfmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if( v4lconvert_try_format( cond, &dfmt, &sfmt ) < 0 ) {
      close(fd); throw CameraReadException(" pixelformat not supported");
    }
    tbuf.length = dfmt.fmt.pix.height * dfmt.fmt.pix.width * 3;
    tbuf.start = (unsigned char *) malloc(tbuf.length);
    if( tbuf.start == NULL ) { close(fd); throw CameraReadException(" memory allocation failed"); }
#else
    sfmtd = -1;
    nfmtd = -1;
    fmtds = NULL;
    do {
      nfmtd++;
      fmtds = (struct v4l2_fmtdesc *) realloc( (void *) fmtds, (nfmtd + 1) * sizeof(struct v4l2_fmtdesc) );
      memset( (void *) (fmtds + nfmtd) , 0, sizeof(struct v4l2_fmtdesc));
      fmtds[nfmtd].type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      fmtds[nfmtd].index = nfmtd; 
    } while( ioctl(fd, VIDIOC_ENUM_FMT, &fmtds[nfmtd]) >= 0 ); 
    // Select a format for which there is a mapping (to RGB24) function 
    for(unsigned int i = 0; i < nfmtd; i++) {
      for(colorspace * col = colorspaces; col->color != -1; col++) { 
        if( fmtds[i].pixelformat == col->color ) { 
          sfmtd = i; 
          cmap = col->map;
        }
      }
    }
    if( sfmtd < 0 ) { close(fd); throw CameraReadException(" pixelformat not supported"); }
    else { cam_debug("V4L2: Using ", fmtds[sfmtd].description, 32); }

    CLEAR(sfmt);
    sfmt.fmt.pix.pixelformat = fmtds[sfmtd].pixelformat;
    sfmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    sfmt.fmt.pix.height      = FG2_DEFAULT_HEIGHT;
    sfmt.fmt.pix.width       = FG2_DEFAULT_WIDTH;
    sfmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
#endif    
    if (ioctl(fd, VIDIOC_S_FMT, &sfmt) < 0) {
      close(fd); throw CameraReadException(" error setting format");
    }
    
    // Request that the device set up some buffers
    struct v4l2_requestbuffers req;
    CLEAR(req);
    req.count               = FG2_DEFAULT_BUFFERS;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
      close(fd); throw CameraReadException(" device does not support memory mapping");
    }

    // Map the userspace buffers
    bufs = (struct buffer *) calloc(req.count, sizeof(struct buffer));   
    for (nbuf = 0; nbuf < req.count; ++nbuf) {
      struct v4l2_buffer buf;
      CLEAR (buf);
      buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory      = V4L2_MEMORY_MMAP;
      buf.index       = nbuf;
      if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
	close(fd); throw CameraReadException(" error reading video buffer");
      }
      bufs[nbuf].length = buf.length;
      bufs[nbuf].start  = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
      if (MAP_FAILED == bufs[nbuf].start) {
	close(fd); throw CameraReadException(" mmap buffer failed");
      }
    }
    
    // Queue the video buffers
    for(int i = 0; i < nbuf; ++i) {
      CLEAR (cbuf);
      cbuf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      cbuf.memory      = V4L2_MEMORY_MMAP;
      cbuf.index       = i;
      if (ioctl(fd, VIDIOC_QBUF, &cbuf) < 0) {
	close(fd); throw CameraReadException(" error queueing buffer");
      }
    }
    
    // Start streaming video
    cbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &cbuf.type) < 0) {
      close(fd); throw CameraReadException(" error starting video stream");
    }

    // Setup current buffer
    cbuf.type      = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    cbuf.memory    = V4L2_MEMORY_MMAP;
    cbuf.index     = 0;
  };
  
  FrameGrabberTwo::~FrameGrabberTwo() {
    // Stop streaming video
    cbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMOFF, &cbuf.type) < 0) {
      close(fd); throw CameraReadException(" error stopping video stream");
    }

    // Free memory map
    for (int i = 0; i < nbuf; ++i) {
      if (munmap(bufs[i].start, bufs[i].length) == -1) {
        close(fd); throw CameraReadException(" error releasing memory");
      }
    }
    close(fd);
#ifdef USE_LIBV4LCONVERT
    v4lconvert_destroy( cond );
    free( tbuf.start );
#endif
  };

  Frame * FrameGrabberTwo::makeFrame() {
#ifdef USE_LIBV4LCONVERT
    Frame * fr = new Frame(dfmt.fmt.pix.width, dfmt.fmt.pix.height, 4 * dfmt.fmt.pix.width, 4 * dfmt.fmt.pix.height * dfmt.fmt.pix.width); 
#else
    Frame * fr = new Frame(sfmt.fmt.pix.width, sfmt.fmt.pix.height, 4 * sfmt.fmt.pix.width, 4 * sfmt.fmt.pix.height * sfmt.fmt.pix.width);
#endif
    return fr;
  };

  void FrameGrabberTwo::grabFrame(Frame * frame) {
 
    // queue last buffer if grabFrame has been called before (FG2_GRAB_FRAME flag set)
    if( flags & FG2_GRAB_FRAME ) { 
      if( ioctl(fd, VIDIOC_QBUF, &cbuf) < 0 ) {
        close(fd); throw CameraReadException(" error queueing buffer");
      }
    }
    // if this is first call to grabFrame set FG2_GRAB_FRAME flag
    else { flags |= FG2_GRAB_FRAME; }

    // dequeue next buffer 
    memset( (void *) &cbuf, 0, sizeof(struct v4l2_buffer));
    cbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    cbuf.memory = V4L2_MEMORY_MMAP;
    cbuf.index = (cbuf.index + 1) % nbuf;
    if( ioctl(fd, VIDIOC_DQBUF, &cbuf) < 0) {
      close(fd); throw CameraReadException(" error queueing buffer");
    }
   
#ifdef USE_LIBV4LCONVERT
    // v4lconvert native format to RGB24 (24bit)
    if( v4lconvert_convert( cond, &sfmt, &dfmt,\
        (unsigned char *) bufs[cbuf.index].start, bufs[cbuf.index].length,\
        (unsigned char *) tbuf.start, tbuf.length ) < 0 ) {
      close(fd); throw CameraReadException( v4lconvert_get_error_message( cond ) );
    }
    
    // since Cairo::FORMAT_RGB24 is 32bit (last byte is empty), 
    // which is also how blobber interprets an image,
    // but v4lconvert RGB24 is a 24bit value, we must
    // convert again, which is an extra unnecessary conversion per frame FIXME
    // bbbbbbbbggggggggrrrrrrrr -> rrrrrrrrggggggggbbbbbbbb00000000
    map_rgb24_special( (unsigned char *) frame->data, (unsigned char *) tbuf.start, frame->sizeimage );
#else
    // convert to rgb24 (32 bit)
    (*cmap)( (unsigned char *) frame->data, (unsigned char *) bufs[cbuf.index].start, frame->sizeimage);
#endif      
  };

  bool FrameGrabberTwo::set_brightness(double percent)
  {
    string spercent;
    num_to_string(percent, spercent);
    debug("setting brightness to " + spercent + "%");

    struct v4l2_queryctrl queryControl;
    struct v4l2_control control;
    queryControl.id = V4L2_CID_BRIGHTNESS;
    if (ioctl(fd, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;

    control.id = queryControl.id;
    control.value = queryControl.minimum +
                    (percent / 100) * 
                    (queryControl.maximum - queryControl.minimum);

    if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) return 0;

    return 1;
  }

  bool FrameGrabberTwo::set_contrast(double percent)
  {
    string spercent;
    num_to_string(percent, spercent);
    debug("setting contrast to " + spercent + "%");

    struct v4l2_queryctrl queryControl;
    struct v4l2_control control;
    queryControl.id = V4L2_CID_CONTRAST;
    if (ioctl(fd, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
    control.id = queryControl.id;
    control.value = queryControl.minimum +
                    (percent / 100) * 
                    (queryControl.maximum - queryControl.minimum);

    if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) return 0;

    return 1;
  }

  bool FrameGrabberTwo::set_saturation(double percent)
  {
    string spercent;
    num_to_string(percent, spercent);
    debug("setting saturation to " + spercent + "%");

    struct v4l2_queryctrl queryControl;
    struct v4l2_control control;
    queryControl.id = V4L2_CID_SATURATION;
    if (ioctl(fd, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
    control.id = queryControl.id;
    control.value = queryControl.minimum +
                    (percent / 100) * 
                    (queryControl.maximum - queryControl.minimum);

    if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) return 0;

    return 1;
  }

  bool FrameGrabberTwo::set_exposure(double percent, bool isAuto)
  {
    string spercent;
    num_to_string(percent, spercent);
    debug("setting exposure to " + spercent + "%");
    if (isAuto) debug("Auto exposure selected");
    else debug("Manual Exposure selected");

    struct v4l2_queryctrl queryControl;
    struct v4l2_control control;

    queryControl.id = V4L2_CID_EXPOSURE_AUTO;
    if (ioctl(fd, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
    control.id = queryControl.id;
    control.value = (isAuto ? V4L2_EXPOSURE_APERTURE_PRIORITY : V4L2_EXPOSURE_MANUAL);
    if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) return 0;

    queryControl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    if (ioctl(fd, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
    control.id = queryControl.id;
    control.value = queryControl.minimum +
                    (percent / 100) * 
                    (queryControl.maximum - queryControl.minimum);
    if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) return 0;

    return 1;
  }
}; //  blobber namespace
