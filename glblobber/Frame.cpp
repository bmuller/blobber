#include"Frame.h"

Frame::Frame(string devName) :
  devID(-1)
{
  setDevice(devName);
}

Frame::~Frame()
{
  closeDevice();
}

//From V4L2 spec:
int Frame::xioctl(int fd,
                  int request,
                  void* arg)
{
  int r;

  do r = ioctl(fd, request, arg);
  while (r == -1 && EINTR == errno);

  return r;
}

//From blobber
int Frame::tioctl(int fd,
                  int request,
                  void* arg)
{
  int r;

  do r = ioctl(fd, request, arg);
  while (r == -1 && EAGAIN == errno);

  return r;
}

void Frame::openDevice()
{
  // Stat the device to see if it exists
  struct stat statBuffer;
  if (stat(device.c_str(), &statBuffer) == -1)
  {
    cout<<"No such device: "<<device<<endl;
    exit(0);
  }

  // Open the device
  devID = open(device.c_str(), O_RDWR, 0);
  if (devID == -1)
  {
    cout<<"Device open error: "<<device<<endl;
    exit(0);
  }
}

void Frame::closeDevice()
{
  if (devID != -1)
  {
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(devID, VIDIOC_STREAMOFF, &type) == -1)
    {
      cout<<"Could not stop streaming"<<endl;
      exit(1);
    }

    for (int i = 0; i < numBuffers; ++i)
    {
      if (munmap(buffers[i].start, buffers[i].length) == -1)
      {
        cout<<"Failed to release buffer memory!"<<endl;
        exit(1);
      }
    }
  }

  if (close(devID) == -1)
  {
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    cout<<"Device close error!"<<endl;
    exit(0);
  }

  devID = -1;
}

void Frame::initDevice()
{
  if (xioctl(devID, VIDIOC_QUERYCAP, &capabilities) == -1)
  {
    cout<<"Query capability failure!"<<endl;
    closeDevice();
    exit(0);
  }

  if (!(capabilities.capabilities & V4L2_CAP_VIDEO_CAPTURE) == -1)
  {
    cout<<"Not a video capture device"<<endl;
    closeDevice();
    exit (0);
  }

  if (!(capabilities.capabilities & V4L2_CAP_READWRITE) == -1)
  {
    cout<<"Video device does not support read functionality"<<endl;
    closeDevice();
    exit(0);
  }

  // Get the pixelformat supported by the device
  v4l2_fmtdesc fmtd;
  CLEAR(fmtd);
  fmtd.index = 0;
  fmtd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
  if(xioctl(devID, VIDIOC_ENUM_FMT, &fmtd) == -1)
  {
    cout<<"Error determining supported video format!"<<endl;
    closeDevice();
    exit(0);
  }

  // Set the video format
  // Don't ask for more than 640x480
  CLEAR(format);
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width = 640;
  format.fmt.pix.height = 480;
  format.fmt.pix.pixelformat = fmtd.pixelformat;
  format.fmt.pix.field = V4L2_FIELD_INTERLACED;

  if (xioctl(devID, VIDIOC_S_FMT, &format))
  {
    cout<<"Video format error!"<<endl;
    closeDevice();
    exit(0);
  }

  switch ( format.fmt.pix.pixelformat ) 
  {
    case V4L2_PIX_FMT_RGB32 :
      break;
    case V4L2_PIX_FMT_YUYV:
      convert = new YUYVtoGL_RGB(format.fmt.pix.height, format.fmt.pix.width); 
      break;
    default:
      cout<<"Pixel format not supported!"<<endl;
      closeDevice();
      exit(0);
  }

  // Setup buffers
  CLEAR(requestBuffers);
  requestBuffers.count = 4;
  requestBuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  requestBuffers.memory = V4L2_MEMORY_MMAP;

  if (xioctl(devID, VIDIOC_REQBUFS, &requestBuffers) == -1)
  {
    cout<<"Device does not support memory mapping!"<<endl;
    closeDevice();
    exit(0);
  }

  if (requestBuffers.count < 2)
  {
    cout<<"Insufficient buffer memory!"<< endl;
    closeDevice();
    exit(0);
  }

  buffers = static_cast<buffer*>(calloc(requestBuffers.count, sizeof(*buffers)));

  capture = malloc(requestBuffers.count * format.fmt.pix.width * format.fmt.pix.height);

  if (!buffers || !capture)
  {
    cout<<"Out of memory!"<<endl;
    closeDevice();
    exit(0);
  }

  for (numBuffers = 0; numBuffers < requestBuffers.count; ++numBuffers)
  {
    struct v4l2_buffer buf;

    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = numBuffers;

    if (xioctl(devID, VIDIOC_QUERYBUF, &buf) == -1)
    {
      cout<<"Buffer query error!"<<endl;
      closeDevice();
      exit(0);
    }

    buffers[numBuffers].length = buf.length;
    buffers[numBuffers].start = mmap(NULL, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, devID, buf.m.offset);

    if (buffers[numBuffers].start == MAP_FAILED)
    {
      cout<<"Error mapping memory!"<<endl;
      closeDevice();
      exit(0);
    }
  }

  for (int i = 0; i < numBuffers; ++i)
  {
    struct v4l2_buffer buf;

    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;

    if (xioctl(devID, VIDIOC_QBUF, &buf) == -1)
    {
      cout<<"Error queueing buffers!"<<endl;
      closeDevice();
      exit(0);
    }
  }

  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(devID, VIDIOC_STREAMON, &type) == -1)
  {
    cout<<"Video streaming failure!"<<endl;
    closeDevice();
    exit(0);
  }
}

void Frame::update()
{
  struct v4l2_buffer buf;

  CLEAR(buf);

  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;

  if (tioctl(devID, VIDIOC_DQBUF, &buf) == -1)
  {
    cout<<"Error dequeueing buffer!"<<endl;
    closeDevice();
    exit(0);
  }
  
  // If YUYV, do conversion
  if(format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV)
  {
    convert->convert( (unsigned char*) buffers[buf.index].start, (unsigned char*) capture);
  }

  // If RGB32, copy directly
  if(format.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB32)
  {
    memcpy(capture, buffers[buf.index].start, buf.bytesused);
  }

  if (ioctl(devID, VIDIOC_QBUF, &buf) == -1)
  {
    cout<<"Error queueing buffers!"<<endl;
    closeDevice();
    exit(0);
  }
/*
  // Fix RGBA for OpenGL (OpenGL is slow...)
  unsigned char RGBArray[getHeight()]
                        [getWidth()]
                        [3];

  unsigned char * reference = static_cast<unsigned char *>(capture);
  for (int i = 0; i < getHeight(); i++)
    for (int j = 0; j < getWidth(); j++)
    {
      RGBArray[getHeight() - i - 1][j][2] = *reference;
      RGBArray[getHeight() - i - 1][j][1] = *(reference + 1);
      RGBArray[getHeight() - i - 1][j][0] = *(reference + 2);
//      RGBArray[getHeight() - i - 1][j][3] = *(reference + 3);
      reference += 4;
    }

  memcpy(capture, RGBArray, sizeof(RGBArray)); */
}

void Frame::setDevice(std::string devName)
{
  if (devID != -1) closeDevice();

  device = devName;
  openDevice();
  initDevice();
}

//We will not exit the program if one of these
//set methods fail. we will return a 0(false) and
//deal with it in the application
bool Frame::setBrightness(double percent)
{
  struct v4l2_queryctrl queryControl;
  struct v4l2_control control;
  queryControl.id = V4L2_CID_BRIGHTNESS;
  if (ioctl(devID, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;

  control.id = queryControl.id;
  control.value = queryControl.minimum +
                  (percent / 100) * 
                  (queryControl.maximum - queryControl.minimum);
//  cout<<"Brightness - "<<percent<<" : "<<control.value<<" : "<<queryControl.maximum<<" : "<<queryControl.minimum<<endl;

  if (ioctl(devID, VIDIOC_S_CTRL, &control) == -1) return 0;

  return 1;
}

bool Frame::setContrast(double percent)
{
  struct v4l2_queryctrl queryControl;
  struct v4l2_control control;
  queryControl.id = V4L2_CID_CONTRAST;
  if (ioctl(devID, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
  control.id = queryControl.id;
  control.value = queryControl.minimum +
                  (percent / 100) * 
                  (queryControl.maximum - queryControl.minimum);
//  cout<<"Contrast - "<<percent<<" : "<<control.value<<" : "<<queryControl.maximum<<" : "<<queryControl.minimum<<endl;

  if (ioctl(devID, VIDIOC_S_CTRL, &control) == -1) return 0;

  return 1;
}

bool Frame::setSaturation(double percent)
{
  struct v4l2_queryctrl queryControl;
  struct v4l2_control control;
  queryControl.id = V4L2_CID_SATURATION;
  if (ioctl(devID, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
  control.id = queryControl.id;
  control.value = queryControl.minimum +
                  (percent / 100) * 
                  (queryControl.maximum - queryControl.minimum);
//  cout<<"Saturation - "<<percent<<" : "<<control.value<<" : "<<queryControl.maximum<<" : "<<queryControl.minimum<<endl;

  if (ioctl(devID, VIDIOC_S_CTRL, &control) == -1) return 0;

  return 1;
}

bool Frame::setExposure(double percent, bool isAuto)
{
  struct v4l2_queryctrl queryControl;
  struct v4l2_control control;

  queryControl.id = V4L2_CID_EXPOSURE_AUTO;
  if (ioctl(devID, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
  control.id = queryControl.id;
  control.value = (isAuto ? V4L2_EXPOSURE_APERTURE_PRIORITY : V4L2_EXPOSURE_MANUAL);
  if (ioctl(devID, VIDIOC_S_CTRL, &control) == -1) return 0;

  queryControl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
  if (ioctl(devID, VIDIOC_QUERYCTRL, &queryControl) == -1) return 0;
  control.id = queryControl.id;
  control.value = queryControl.minimum +
                  (percent / 100) * 
                  (queryControl.maximum - queryControl.minimum);
//  cout<<"Exposure - "<<percent<<" : "<<control.value<<" : "<<queryControl.maximum<<" : "<<queryControl.minimum<<endl;
  if (ioctl(devID, VIDIOC_S_CTRL, &control) == -1) return 0;

  return 1;
}


int Frame::getWidth()
{
  return format.fmt.pix.width;
}

int Frame::getHeight()
{
  return format.fmt.pix.height;
}

//Keep this as a void * so it can be casted as necessary
void * Frame::getCapture()
{
  return capture;
}
