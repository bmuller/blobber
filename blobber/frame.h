#ifndef __FRAME_H
#define __FRAME_H

namespace blobber {

  // The frame class encapsulates all the information necessary to keep track of a single frame.
  class Frame { 
  public:
    int width;
    int height;
    int bytesperline;
    int sizeimage;
    void *data;
    Frame(int w, int h, int bpl, int si);
    ~Frame();
  };

};

#endif /* __FRAME_H */

