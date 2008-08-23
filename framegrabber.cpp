#include "nerdtag.h"

using namespace std;

FrameGrabber::FrameGrabber(string dev) : cur_frame(-1) {
  fd = open(dev.c_str(), O_RDONLY);
  if (fd == -1 ) {
    cerr << "open video device failed" << endl;
  }
  // Make sure child processes don't inherit video (close on exec)
  fcntl(fd, F_SETFD, FD_CLOEXEC );

  // For n-ary buffering
  cur_frame = -1;

  // Get the device capabilities
  if( ioctl(fd, VIDIOCGCAP, &caps) < 0 ) {
    cerr << "query capabilities failed" << endl;
  }

  // Read info for all input sources
  source = 0;
  for (int i = 0; i < caps.channels; i++ ) {
    video_channel vc;
    ioctl(fd, VIDIOCGCHAN, &vc );
    sources.push_back(vc);
    debug("Found video source: " + string(vc.name));
  }

  if(VID_TYPE_TUNER) debug("Your video device can be tuned");

  // Read info about tuner
  tuner.tuner = 0;
  if ((caps.type & VID_TYPE_TUNER ) && ioctl(fd, VIDIOCGTUNER, &tuner) < 0 ) {
    cerr << "warning: cannot get tuner info (not present?)";
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
    cerr << "fg_open(): set default window attrs failed" << endl;
  }

  // Set default picture attributes (50%)
  picture.brightness = US50PC;
  picture.hue        = US50PC;
  picture.colour     = US50PC;
  picture.contrast   = US50PC;
  picture.whiteness  = US50PC;
  picture.depth      = 32;
  picture.palette    = VIDEO_PALETTE_RGB32;

  if (ioctl(fd, VIDIOCSPICT, &picture) < 0 ) {
    cerr << "set picture attributes failed" << endl;
  }

  // Get frame buffer info
  if ( ioctl(fd, VIDIOCGFBUF, &fbuffer) < 0 ) {
    cerr << "get framebuffer failed" << endl;
  }

  // Get the memory buffer info
  if ( ioctl(fd, VIDIOCGMBUF, &mbuf) < 0 ) {
    cerr << "get memory buffer" << endl;
  }

  // Memory map the video buffer
  mb_map = mmap(0, mbuf.size, PROT_READ, MAP_SHARED, fd, 0);

  if ( mb_map == MAP_FAILED ) {
    cerr << "mmap buffer not mmapped" << endl;
  }
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
    if (ioctl(fd, VIDIOCMCAPTURE, &vmmap) < 0 ) {
      cerr << "failed to capture frame" << endl;
      return;
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
    cerr << "failed to capture frame" << endl;
    return;
  }

  // Save current frame

  // Wait for end of frame
  if (ioctl(fd, VIDIOCSYNC, &cur_frame) < 0 ) {
    cerr << "failed to sync frame" << endl;
    return;
  }

  // Save video buffer into our own memory
  memcpy(frame->data, ((char *) mb_map + mbuf.offsets[cur_frame]), frame->getSize());

  // Move along to the next one
  cur_frame = capture_frame;
};

