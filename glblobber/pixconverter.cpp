#include "pixconverter.h"

YUYVtoRGB32::YUYVtoRGB32(unsigned int height, unsigned int width) :
  yuyv_bytes(height * width * 2) {
  LutRv = new int[256];
  LutGu = new int[256];
  LutGv = new int[256];
  LutBu = new int[256];
  for (int i = 0; i < 256; i++){
    LutRv[i] = (i - 128) * 1402 / 1000;
    LutBu[i] = (i - 128) * 1772 / 1000;
    LutGu[i] = (128 - i) * 714 / 1000;
    LutGv[i] = (128-i) * 344 / 1000;
  }
};


YUYVtoRGB32::~YUYVtoRGB32() {
  delete [] LutRv;
  delete [] LutGu;
  delete [] LutGv;
  delete [] LutBu;
};

void YUYVtoRGB32::convert(unsigned char* yuyv, unsigned char* rgb32) {
  unsigned char* y;
  unsigned char* r = rgb32;
  for(y = yuyv; y - yuyv < yuyv_bytes; y += 4) { 
        *r = R_FROMYV ( *y,  *(y+1) );
    *(r+1) = G_FROMYUV( *y,  *(y+3),  *(y+1) );
    *(r+2) = B_FROMYU ( *y,  *(y+3) );
    *(r+3) = (unsigned char) 0;
    *(r+4) = R_FROMYV ( *(y+2),  *(y+1) );
    *(r+5) = G_FROMYUV( *(y+2),  *(y+3),  *(y+1) );
    *(r+6) = B_FROMYU ( *(y+2),  *(y+3) );
    *(r+7) = (unsigned char) 0;
    r+=8;
  }  
};

YUYVtoGL_RGB::YUYVtoGL_RGB(unsigned int height, unsigned int width) :
  yuyv_bytes(height * width * 2) {
  LutRv = new int[256];
  LutGu = new int[256];
  LutGv = new int[256];
  LutBu = new int[256];
  for (int i = 0; i < 256; i++){
    LutRv[i] = (i - 128) * 1402 / 1000;
    LutBu[i] = (i - 128) * 1772 / 1000;
    LutGu[i] = (128 - i) * 714 / 1000;
    LutGv[i] = (128-i) * 344 / 1000;
  }
};


YUYVtoGL_RGB::~YUYVtoGL_RGB() {
  delete [] LutRv;
  delete [] LutGu;
  delete [] LutGv;
  delete [] LutBu;
};

void YUYVtoGL_RGB::convert(unsigned char* yuyv, unsigned char* rgb24) {
  unsigned char* y;
  unsigned char* r = rgb24;
  for(y = yuyv; y - yuyv < yuyv_bytes; y += 4) { 
    *(r+2) = R_FROMYV ( *y,  *(y+1) );
    *(r+1) = G_FROMYUV( *y,  *(y+3),  *(y+1) );
    *(r)   = B_FROMYU ( *y,  *(y+3) );
    *(r+5) = R_FROMYV ( *(y+2),  *(y+1) );
    *(r+4) = G_FROMYUV( *(y+2),  *(y+3),  *(y+1) );
    *(r+3) = B_FROMYU ( *(y+2),  *(y+3) );
    r+=6;
  }  
};
