using namespace std;

class FrameGrabber {
 public:
  int                     fd;         ///< File handle for open device                                                    
  struct v4l2_capability caps;       ///< Capabilities                                                                   
  struct v4l2_format fmt;
  struct v4l2_requestbuffers req;
  struct buffer *buffers;
  struct v4l2_buffer current_buffer;
  unsigned int n_buffers;
  FrameGrabber(string dev);
  ~FrameGrabber();
  Frame * makeFrame();
  void grabFrame(Frame *frame);
};
