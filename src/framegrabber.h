#include "pixconverter.h"
using namespace std;

//class FrameGrabber {}

class FrameGrabber {
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
  FrameGrabber(string dev);
  ~FrameGrabber();
  Frame * makeFrame();
  void grabFrame(Frame *frame);
};

class FrameGrabberTwo {
 public:
  int fd;                            ///< File handle for open device                                                    
  struct v4l2_capability caps;       ///< Capabilities                                                                   
  struct v4l2_format fmt;
  struct v4l2_requestbuffers req;
  struct buffer *buffers;
  unsigned int n_buffers;
  YUYVtoRGB32* conv;
  FrameGrabberTwo(string dev);
  ~FrameGrabberTwo();
  Frame * makeFrame();
  void grabFrame(Frame *frame);
};
