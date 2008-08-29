#include "blobber.h"

using namespace std;

#include <errno.h>
static int xioctl(int fd, int request, void *arg) {
  int r;
  do r = ioctl (fd, request, arg);
  while (-1 == r && EINTR == errno);
  return r;
}


FrameGrabber::FrameGrabber(string dev) : n_buffers(0) {
  fd = open(dev.c_str(), O_RDWR | O_NONBLOCK, 0);
  if (fd == -1 ) 
    throw NoSuchVideoDeviceException("open video device \"" + dev + "\" failed");

  // Get the device capabilities
  if(xioctl(fd, VIDIOC_QUERYCAP, &caps) < 0 ) {
    cout << errno << endl;
    throw CameraReadException("query capabilities failed");
  }
  debug("Found camera");

  if(!(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE)) 
    throw CameraReadException(dev + " is not a video capture device");

  CLEAR(fmt);
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

  if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0)
    throw CameraReadException("Error setting format");

  CLEAR(req);
  req.count               = 4;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_MMAP;
  if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0)
    throw CameraReadException("Device does not support memory mapping");

  if (req.count < 2) 
    throw CameraReadException("Insufficient memory");

  buffers = (buffer *) calloc(req.count, sizeof(*buffers));
  
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
    struct v4l2_buffer buf;
    CLEAR (buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;

    if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0)
      throw CameraReadException("Error reading video buffer");

    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    if (MAP_FAILED == buffers[n_buffers].start)
      throw CameraReadException("mmap buffer not mmapped");
  }

  for(int i = 0; i < n_buffers; ++i) {
    struct v4l2_buffer buf;
    CLEAR (buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;
    if (ioctl(fd, VIDIOC_QBUF, &buf) < 0)
      throw CameraReadException("Error reading video buffer");
  }
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(fd, VIDIOC_STREAMON, &type) < 0)
    throw CameraReadException("Error starting video stream");
};

FrameGrabber::~FrameGrabber() {
  close(fd);
};

Frame * FrameGrabber::makeFrame() {
  Frame * fr = new Frame(fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.bytesperline, fmt.fmt.pix.sizeimage);
  return fr;
};

void FrameGrabber::grabFrame(Frame *frame) {
  struct v4l2_buffer buf;
  CLEAR(buf); 
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  
  int res;
  //while ((res = ioctl(fd, VIDIOC_DQBUF, &buf)) < 0 && ((errno == EAGAIN) || (errno == EINTR)));
  //if (res < 0) 
  if(ioctl(fd, VIDIOC_DQBUF, &buf) < 0);
    throw CameraReadException("Error dequeuing buffer");
  
  memcpy(frame->data, buffers[buf.index].start, buf.bytesused);

  if (ioctl(fd, VIDIOC_QBUF, &buf) < 0)
      throw CameraReadException("Error dequeuing buffer");
};
