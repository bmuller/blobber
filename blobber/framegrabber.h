#ifndef __FRAMEGRABBER_H
#define __FRAMEGRABBER_H

#include "config.h"

#ifdef HAVE_V4L1
#include <linux/videodev.h>
#endif

#ifdef HAVE_V4L2
#include <linux/videodev2.h>
#endif

#include "frame.h"
#include "colormap.h"

#include <vector>
#include <iostream>

namespace blobber {

using namespace std;

class FrameGrabber {
 public:
  virtual Frame * makeFrame() = 0;
  virtual void grabFrame(Frame *frame) = 0;
  virtual ~FrameGrabber() {};
  virtual void set_contrast(int contrast) {};
  virtual void set_brightness(int brightness) {};
};

class FrameGrabberFactory {
 public:
  static FrameGrabber * create(string dev);
};


// v4l 1
#ifdef HAVE_V4L1
class FrameGrabberOne : public FrameGrabber {
 public:
  int                     fd;         ///< File handle for open device                                                    
  struct video_capability caps;       ///< Capabilities                                                                   
  vector<video_channel>   sources;    ///< Input sources (eg. TV, SVideo)                                                 
  int                     source;     ///< Currently selected source                                                      
  int                     cur_frame;  ///< Currently capuring frame no.         
  struct video_tuner      tuner;      ///< TV or Radio tuner                                                              
  struct video_window     window;     ///< Capture window           
  struct video_picture    picture;    ///< Picture controls (eg. bright)                                                  
  struct video_buffer     fbuffer;    ///< Frame buffer                                                                   
  struct video_mbuf       mbuf;       ///< Memory buffer #frames, offsets                                  
  void*                   mb_map;     ///< Memory-mapped buffer                           
  struct video_mmap       vmmap;      ///< Memory-mapped info   
  FrameGrabberOne(string dev);
  ~FrameGrabberOne();
  Frame * makeFrame();
  void grabFrame(Frame *frame);
  void set_contrast(int contrast);
  void set_brightness(int brightness);
};
#endif

// v4l 2
#ifdef HAVE_V4L2

#define FG2_DEFAULT_BUFFERS  5        ///< Default number of buffers
#define FG2_DEFAULT_WIDTH    640      ///< Default frame width
#define FG2_DEFAULT_HEIGHT   480      ///< Default frame height 
#define FG2_GRAB_FRAME       (1<<0)   ///< Flag set on first call to grabFrame

class FrameGrabberTwo : public FrameGrabber {
 protected:
  int                     fd;         ///< File handle for open device                                                    
  struct v4l2_capability  caps;       ///< Capabilities        
  struct v4l2_fmtdesc *   fmtds;      ///< Available formats
  unsigned int            nfmtd;      ///< Number of available formats
  unsigned int            sfmtd;      ///< Index of the selected format                                            
  struct v4l2_format      fmt;        ///< Buffer/pixel format
  struct buffer *         bufs;       ///< Memory buffers
  unsigned int            nbuf;       ///< Number of memory buffers
  struct v4l2_buffer      cbuf;       ///< Current buffer info
  colormap *              cmap;       ///< Colormapping function pointer
  unsigned int            flags;      ///< Flags used internally
 public:
  FrameGrabberTwo(string dev);
  ~FrameGrabberTwo();
  Frame * makeFrame();
  void grabFrame(Frame * frame);
  void set_contrast(int contrast);
  void set_brightness(int brightness);
};
#endif

};

#endif /* __FRAMEGRABBER_H */

