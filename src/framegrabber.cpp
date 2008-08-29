#include "blobber.h"

using namespace std;

FrameGrabber::FrameGrabber(string dev) : n_buffers(0) {
  fd = open(dev.c_str(), O_RDWR | O_NONBLOCK, 0);
  if (fd == -1 ) 
    throw NoSuchVideoDeviceException("open video device \"" + dev + "\" failed");
  
  // Get the device capabilities
  if(xioctl(fd, VIDIOC_QUERYCAP, &caps) < 0 ) 
    throw CameraReadException("query capabilities failed");
  debug("Found camera: " + string(caps.card));

  if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) 
    throw CameraReadException(dev + " is not a video capture device");

  CLEAR(fmt);

  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

  if (xioctl(fd, VIDIOC_S_FMT, &fmt) < 0)
    throw CameraReadException("Error setting format");

  CLEAR(req);

  req.count               = 4;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_MMAP;
  if (xioctl(fd, VIDIOC_REQBUFS, &req) < 0)
    throw CameraReadException("Device does not support memory mapping");

  if (req.count < 2) 
    throw CameraReadException("Insufficient memory");

  buffers = calloc(req.count, sizeof (*buffers));
  
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
    struct v4l2_buffer buf;
    CLEAR (buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;

    if (xioctl(fd, VIDIOC_QUERYBUF, &buf) < 0)
      throw CameraReadException("Error reading video buffer");

    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    if (MAP_FAILED == buffers[n_buffers].start)
      throw CameraReadException("mmap buffer not mmapped");
  }

  for (i = 0; i < n_buffers; ++i) {
    struct v4l2_buffer buf;
    CLEAR (buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;
    if (xioctl(fd, VIDIOC_QBUF, &buf) < 0)
      throw CameraReadException("Error reading video buffer");
  }
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(fd, VIDIOC_STREAMON, &type) < 0)
    throw CameraReadException("Error starting video stream");

  current_buffer = NULL;
};

FrameGrabber::~FrameGrabber() {
  close(fd);
};

Frame * FrameGrabber::makeFrame() {
  Frame * fr = new Frame(window.width, window.height, picture.palette);
  return fr;
};

void FrameGrabber::grabFrame(Frame *frame) {
  //here
  struct v4l2_buffer buf;
  CLEAR (buf);

  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;

  if (xioctl(fd, VIDIOC_DQBUF, &buf) < 0)
    throw CameraReadException("Error queuing buffer");

  process_image (buffers[buf.index].start);

  if (xioctl(fd, VIDIOC_QBUF, &buf) < 0)
    throw CameraReadException("Error dequeuing buffer");
};
