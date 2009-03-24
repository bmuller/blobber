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

  FrameGrabberTwo::FrameGrabberTwo(string dev) : nbuf(0) {

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
    for(unsigned int i = 0; i < nfmtd; i++ ) {
      for(colorspace * col = colorspaces; col->color != -1; col++) { 
        if( fmtds[i].pixelformat == col->color ) { 
          sfmtd = i; 
          cmap = col->map;
        }
      }
    }
    if( sfmtd < 0 ) { close(fd); throw CameraReadException(" pixelformat not supported"); }
    else { cam_debug("V4L2: Using ", fmtds[sfmtd].description, 32); }

    CLEAR(fmt);
    fmt.fmt.pix.pixelformat = fmtds[sfmtd].pixelformat;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    fmt.fmt.pix.height      = FG2_DEFAULT_HEIGHT;
    fmt.fmt.pix.width       = FG2_DEFAULT_WIDTH;
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
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
    close(fd);
  };

  Frame * FrameGrabberTwo::makeFrame() {
    Frame * fr = new Frame(fmt.fmt.pix.width, fmt.fmt.pix.height, 4 * fmt.fmt.pix.width, 4 * fmt.fmt.pix.height * fmt.fmt.pix.width);
    return fr;
  };

  void FrameGrabberTwo::grabFrame(Frame * frame) {
 
    // queue last buffer
    if( flags & FG2_GRAB_FRAME ) { 
      if( ioctl(fd, VIDIOC_QBUF, &cbuf) < 0 ) {
        close(fd); throw CameraReadException(" error queueing buffer");
      }
    }
    else { flags |= FG2_GRAB_FRAME; }  

    // dequeue next buffer 
    memset( (void *) &cbuf, 0, sizeof(struct v4l2_buffer));
    cbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    cbuf.memory = V4L2_MEMORY_MMAP;
    cbuf.index = (cbuf.index + 1) % nbuf;
    if( ioctl(fd, VIDIOC_DQBUF, &cbuf) < 0) {
      close(fd); throw CameraReadException(" error queueing buffer");
    }
   
    // convert to rgb24 (32 bit)
    (*cmap)( (unsigned char *) frame->data, (unsigned char *) bufs[cbuf.index].start, frame->sizeimage);
      
  };

  void FrameGrabberTwo::set_contrast(int contrast) {\
    struct v4l2_queryctrl qcon;
    struct v4l2_control con;
    qcon.id = V4L2_CID_CONTRAST;

    // query the control structure for contrast
    if (ioctl(fd, VIDIOC_QUERYCTRL, &qcon) < 0) {
      close(fd); throw CameraReadException(" query contrast control failed: " + string(strerror(errno)));
    }
    if(qcon.flags & (V4L2_CTRL_FLAG_DISABLED | V4L2_CTRL_FLAG_GRABBED | V4L2_CTRL_FLAG_READ_ONLY) ) { return; }
    con.id = qcon.id;
    if(contrast < qcon.minimum) { con.value = qcon.minimum; }
    else if(contrast > qcon.maximum) { con.value = qcon.maximum; }
    else { con.value = contrast; }

    // set the control structure for contrast
    if (ioctl(fd, VIDIOC_S_CTRL, &con) < 0) {
      close(fd); throw CameraReadException(" error setting contrast: " + string(strerror(errno)));
    }
  };

  void FrameGrabberTwo::set_brightness(int brightness) {
    struct v4l2_queryctrl qcon;
    struct v4l2_control con;
    qcon.id = V4L2_CID_BRIGHTNESS;

    // query the control structure for brightness
    if (ioctl(fd, VIDIOC_QUERYCTRL, &qcon) < 0) {
      close(fd); throw CameraReadException(" query brightness control failed: " + string(strerror(errno)));
    }
    if(qcon.flags & (V4L2_CTRL_FLAG_DISABLED | V4L2_CTRL_FLAG_GRABBED | V4L2_CTRL_FLAG_READ_ONLY) ) { return; }
    con.id = qcon.id;
    if(brightness < qcon.minimum) { con.value = qcon.minimum; }
    else if(brightness > qcon.maximum) { con.value = qcon.maximum; }
    else { con.value = brightness; }

    // set the control structure for brightness
    if (ioctl(fd, VIDIOC_S_CTRL, &con) < 0) {
      close(fd); throw CameraReadException(" error setting brightness: " + string(strerror(errno)));
    }
  };
}; //  blobber namespace
