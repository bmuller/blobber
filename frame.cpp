#include "nerdtag.h"

using namespace std;

int Frame::getSize() {
  int pixels = width * height;
  int size = 0;

  switch ( format ) {
    case VIDEO_PALETTE_RGB24:
      // 3 bytes per pixel                                                                                            
      size = pixels * 3;
      break;

    case VIDEO_PALETTE_RGB32:
      // 4 bytes (1 int) per pixel                                                                                    
      size = pixels * 4;
      break;

    case VIDEO_PALETTE_RGB565:
      // 2 bytes (16 bits) per pixel                                                                                  
      size = pixels * 2;
      break;

    case VIDEO_PALETTE_RGB555:
      // 2 bytes (15 bits) per pixel                                                                                  
      size = pixels * 2;
      break;

    case VIDEO_PALETTE_YUV422:
      // 1 byte (8 bits) per pixel                                                                                    
      size = pixels;
      break;

    default:
      // Unsupported!                                                                                                 
      fprintf( stderr, "frame_get_size(): Unsupported type!\n" );
      size = -1;
  }
  return size;
};

void Frame::allocateBuffer() {
  data = malloc(getSize());
};
