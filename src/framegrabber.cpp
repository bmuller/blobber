#include "blobber.h"

using namespace std;

FrameGrabber::FrameGrabber(string dev) : cur_frame(-1) {
  fd = open(dev.c_str(), O_RDWR); //O_RDONLY);
  if (fd == -1 ) 
    throw NoSuchVideoDeviceException("open video device \"" + dev + "\" failed");
  
  // Make sure child processes don't inherit video (close on exec)
  fcntl(fd, F_SETFD, FD_CLOEXEC );

  // For n-ary buffering
  cur_frame = -1;

  // Get the device capabilities
  if( ioctl(fd, VIDIOCGCAP, &caps) < 0 ) 
    throw CameraReadException("query capabilities failed");

  // Read info for all input sources
  source = 0;
  for (int i = 0; i < caps.channels; i++ ) {
    video_channel vc;
    ioctl(fd, VIDIOCGCHAN, &vc );
    sources.push_back(vc);
    debug("Found video source: " + string(vc.name));
  }

  /* Don't really care about tuning capabilities
  if(VID_TYPE_TUNER) debug("Your video device can be tuned");
  // Read info about tuner
  tuner.tuner = 0;
  if ((caps.type & VID_TYPE_TUNER ) && ioctl(fd, VIDIOCGTUNER, &tuner) < 0 ) {
    cerr << "warning: cannot get tuner info (not present?)";
  }
  */

  if(ioctl(fd, VIDIOCGWIN, &window) < 0) 
    throw NoSuchVideoDeviceException("set default window attrs failed");

  // Set default window to max size
  window.x = 0;
  window.y = 0;
  window.width = caps.maxwidth;
  window.height = caps.maxheight;
  window.chromakey = 0;
  window.flags = 0;
  window.clips = NULL;
  window.clipcount = 0;

  if(ioctl(fd, VIDIOCSWIN, &window) < 0) 
    throw CameraReadException("set default window attrs failed");


  // Set default picture attributes (50%)
  // these following attrs made the fps very very small
  /*
  picture.brightness = US50PC;
  picture.hue        = US50PC;
  picture.colour     = US50PC;
  picture.contrast   = US50PC;
  picture.whiteness  = US50PC;
  picture.depth      = 32;
  picture.palette    = VIDEO_PALETTE_RGB32;
  */

  picture.brightness = 16384;
  picture.hue        = 0;
  picture.colour     = 0;
  picture.contrast   = 8192;
  picture.whiteness  = 0;
  picture.depth      = 32;
  picture.palette    = VIDEO_PALETTE_RGB32;
  //picture.depth      = 24;
  //picture.palette    = VIDEO_PALETTE_RGB24;

  if (ioctl(fd, VIDIOCSPICT, &picture) < 0 ) 
    throw CameraReadException("set picture attributes failed");

  // Get frame buffer info
  if ( ioctl(fd, VIDIOCGFBUF, &fbuffer) < 0 ) 
    throw CameraReadException("get framebuffer failed");

  // Get the memory buffer info
  if ( ioctl(fd, VIDIOCGMBUF, &mbuf) < 0 ) 
    throw CameraReadException("get memory buffer failed");

  // Memory map the video buffer
  mb_map = mmap(0, mbuf.size, PROT_READ, MAP_SHARED, fd, 0);

  if ( mb_map == MAP_FAILED )
    throw CameraReadException("mmap buffer not mmapped");
};


Frame * FrameGrabber::makeFrame() {
  Frame * fr = new Frame(window.width, window.height, picture.palette);
  return fr;
};

void FrameGrabber::grabFrame(Frame *frame) {
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
    if (ioctl(fd, VIDIOCMCAPTURE, &vmmap) < 0 ) 
      throw CameraReadException("failed to capture frame");
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
  if (ioctl(fd, VIDIOCMCAPTURE, &vmmap) < 0)
    throw CameraReadException("failed to capture frame");

  // Wait for end of frame
  if (ioctl(fd, VIDIOCSYNC, &cur_frame) < 0 ) 
    throw CameraReadException("failed to sync frame");

  // Save video buffer into our own memory
  memcpy(frame->data, ((char *) mb_map + mbuf.offsets[cur_frame]), frame->getSize());

  // Move along to the next one
  cur_frame = capture_frame;
};

