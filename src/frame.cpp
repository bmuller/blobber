#include "blobber.h"

Frame::Frame(int w, int h, int bpl, int si) : width(w), height(h), bytesperline(bpl), sizeimage(si) {
  data = malloc(sizeimage); 
};

