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

#include "blobber.h"

namespace blobber {

template<class T>
string showbits(T val) {
  string out;
  char cmp, *ptr = (char*) &val;
  for(unsigned int i = 0; i < sizeof(T); i++) {
    cmp = 0x01;
    for(unsigned int j = 0; j < 8; j++) {
      if( (*(ptr + i) & (cmp << j) ) > 0) { out += '1'; }
      else { out += '0'; }
    }
  }
  return(out);
}

  using namespace std;

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

  FrameGrabberTwo::FrameGrabberTwo(string dev) : n_buffers(0) {
    fd = open(dev.c_str(), O_RDWR, 0); //removed  | O_NONBLOCK to prevent EBUSY's
    if (fd == -1 ) {
      close(fd);
      throw NoSuchVideoDeviceException(" open video device \"" + dev + "\" failed");
    }

    // Get the device capabilities
    if(xioctl(fd, VIDIOC_QUERYCAP, &caps) < 0 ) {
      close(fd);
      throw CameraReadException(" query capabilities failed");
    }
    debug("V4L2: Found camera");
    
    if(!(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
      close(fd);
      throw CameraReadException(" " + dev + " is not a video capture device");
    }

    // Get the pixelformat supported by the device
    v4l2_fmtdesc fmtd;
    CLEAR(fmtd);
    fmtd.index              = 0;
    fmtd.type               = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    if( xioctl(fd, VIDIOC_ENUM_FMT, &fmtd) < 0 ) { 
      close(fd);
      throw CameraReadException(" query pixelformat failed");
    }

    CLEAR(fmt);
    fmt.fmt.pix.pixelformat = fmtd.pixelformat;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    fmt.fmt.pix.height      = 480;  // These can be changed, but my cam defaults to 120x160
    fmt.fmt.pix.width       = 640;
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
      close(fd);
      throw CameraReadException(" error setting format");
    }
    
    switch ( fmt.fmt.pix.pixelformat ) {
    case V4L2_PIX_FMT_RGB32 : break;
    case V4L2_PIX_FMT_YUYV  : conv = new YUYVtoRGB32(fmt.fmt.pix.height, fmt.fmt.pix.width); break;
    default                 : throw CameraReadException(" pixel format not supported");
    }


    // Request that the device set up some buffers
    CLEAR(req);
    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
      close(fd);
      throw CameraReadException(" device does not support memory mapping");
    }

    if (req.count < 2) {
      close(fd);
      throw CameraReadException(" insufficient memory");
    }

    // Map the userspace buffers
    buffers = (buffer *) calloc(req.count, sizeof(*buffers));
    
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
      struct v4l2_buffer buf;
      CLEAR (buf);
      buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory      = V4L2_MEMORY_MMAP;
      buf.index       = n_buffers;
      
      if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
	close(fd);
	throw CameraReadException(" error reading video buffer");
      }
      
      buffers[n_buffers].length = buf.length;
      buffers[n_buffers].start = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
      if (MAP_FAILED == buffers[n_buffers].start) {
	close(fd);
	throw CameraReadException(" mmap buffer not mmapped");
      }
    }
    
    // Queue the video buffers
    for(int i = 0; i < n_buffers; ++i) {
      struct v4l2_buffer buf;
      CLEAR (buf);
      buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory      = V4L2_MEMORY_MMAP;
      buf.index       = i;
      if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
	close(fd);
	throw CameraReadException(" error reading video buffer");
      }
    }
    
    // Start streaming video
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
      close(fd);
      throw CameraReadException(" error starting video stream");
    }
  };
  
  FrameGrabberTwo::~FrameGrabberTwo() {
    close(fd);
  };

  Frame * FrameGrabberTwo::makeFrame() {
    Frame * fr = new Frame(fmt.fmt.pix.width, fmt.fmt.pix.height, 4 * fmt.fmt.pix.width, 4 * fmt.fmt.pix.height * fmt.fmt.pix.width);
    return fr;
  };

  void FrameGrabberTwo::grabFrame(Frame *frame) {
    struct v4l2_buffer buf;
    CLEAR(buf); 
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    
    int res;
    //while ((res = ioctl(fd, VIDIOC_DQBUF, &buf)) < 0 && ((errno == EAGAIN) || (errno == EINTR)));
    //if (res < 0) 
    if(tioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
      close(fd);
      throw CameraReadException(" error dequeuing buffer");
    }
  
    // If YUYV, do conversion
    if(fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
      conv->convert( (unsigned char*) buffers[buf.index].start, (unsigned char*) frame->data );
    }
  
    // If RGB32, copy directly
    if(fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB32) {
      memcpy(frame->data, buffers[buf.index].start, buf.bytesused);
    }

    if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
      close(fd);
      throw CameraReadException(" error queuing buffer");
    }
  };

  void FrameGrabberTwo::set_contrast(int contrast) {\
    struct v4l2_queryctrl qcon;
    struct v4l2_control con;
    qcon.id = V4L2_CID_CONTRAST;
    // query the control structure for contrast
    if (ioctl(fd, VIDIOC_QUERYCTRL, &qcon) < 0) {
      close(fd);
      throw CameraReadException(" query contrast control failed: " + string(strerror(errno)));
    }
    if(qcon.flags & (V4L2_CTRL_FLAG_DISABLED | V4L2_CTRL_FLAG_GRABBED | V4L2_CTRL_FLAG_READ_ONLY) ) { return; }
    con.id = qcon.id;
    if(contrast < qcon.minimum) { con.value = qcon.minimum; }
    else if(contrast > qcon.maximum) { con.value = qcon.maximum; }
    else { con.value = contrast; }
    // set the control structure for contrast
    if (ioctl(fd, VIDIOC_S_CTRL, &con) < 0) {
      close(fd);
      throw CameraReadException(" error setting contrast: " + string(strerror(errno)));
    }
  };

  void FrameGrabberTwo::set_brightness(int brightness) {
    struct v4l2_queryctrl qcon;
    struct v4l2_control con;
    qcon.id = V4L2_CID_BRIGHTNESS;
    // query the control structure for brightness
    if (ioctl(fd, VIDIOC_QUERYCTRL, &qcon) < 0) {
      close(fd);
      throw CameraReadException(" query brightness control failed: " + string(strerror(errno)));
    }
    if(qcon.flags & (V4L2_CTRL_FLAG_DISABLED | V4L2_CTRL_FLAG_GRABBED | V4L2_CTRL_FLAG_READ_ONLY) ) { return; }
    con.id = qcon.id;
    if(brightness < qcon.minimum) { con.value = qcon.minimum; }
    else if(brightness > qcon.maximum) { con.value = qcon.maximum; }
    else { con.value = brightness; }
    // set the control structure for brightness
    if (ioctl(fd, VIDIOC_S_CTRL, &con) < 0) {
      close(fd);
      throw CameraReadException(" error setting brightness: " + string(strerror(errno)));
    }
  };
}; //  blobber namespace
