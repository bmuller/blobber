#include "config.h"

#ifdef HAVE_V4L2
#include <linux/videodev2.h>

#include "config.h"
#include "colormap.h"

#include <cstdlib>
#include <cstring>

namespace blobber {
  colorspace colorspaces[] = {
    { V4L2_PIX_FMT_BGR24,  &map_bgr24  },
    { V4L2_PIX_FMT_RGB24,  &map_rgb24  },
    { V4L2_PIX_FMT_BGR32,  &map_bgr32  },
    { V4L2_PIX_FMT_RGB32,  &map_rgb32  },
    { V4L2_PIX_FMT_YUYV,   &map_yuyv   },
    { V4L2_PIX_FMT_UYVY,   &map_yuyv   },
    { V4L2_PIX_FMT_RGB565, &map_rgb565 },
    { V4L2_PIX_FMT_RGB555, &map_rgb555 },
    { V4L2_PIX_FMT_GREY,   &map_grey   },
    { -1 }
  };

  void map_rgb24(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    memcpy(dst, src, sizeimage);
  }

  void map_bgr24(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      dstbuf[0] = srcbuf[2];
      dstbuf[1] = srcbuf[1];
      dstbuf[2] = srcbuf[0];
      dstbuf += 4;
      srcbuf += 4;
    }
  }

  void map_rgb32(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    memcpy(dst, src, sizeimage);
  }

  void map_bgr32(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      dstbuf[0] = srcbuf[2];
      dstbuf[1] = srcbuf[1];
      dstbuf[2] = srcbuf[0];
      dstbuf += 4;
    srcbuf += 4;
    }
  }

  void map_grey(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      *dstbuf++ = *srcbuf;
      *dstbuf++ = *srcbuf;
      *dstbuf++ = *srcbuf++;
      dstbuf++;
    }
  }

  void map_yuyv(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    unsigned char u, v;
    int i;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      u = srcbuf[1];
      v = srcbuf[3];
      for (i = 0; i < 2; i++) {
	*dstbuf++ = CLIP((int) *srcbuf + 2.029 * ((int) u - 128));
	*dstbuf++ = CLIP((int) *srcbuf - 0.396 * ((int) u - 128) - 0.581 * ((int) v - 128));
	*dstbuf++ = CLIP((int) *srcbuf + 1.140 * ((int) v - 128));
	dstbuf++;
	srcbuf += 2;
      }
    }
  }
  
  void map_rgb565(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      /* rrrrrggg gggbbbbb -> gggbbbbb rrrrrggg */
      *dstbuf++ = (srcbuf[0] << 3) | (srcbuf[0] & 0x07);
      *dstbuf++ = (srcbuf[1] << 5) | ((srcbuf[0] >> 3) & 0x1c) | ((srcbuf[0] >> 5) & 0x03);
      *dstbuf++ = (srcbuf[1] & 0xf8) | ((srcbuf[1] >> 3) & 0x07);
      dstbuf++;
      srcbuf += 2;
    }
  }
  
  void map_rgb555(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      /* 0rrrrrgg gggbbbbb -> gggbbbbb 0rrrrrgg */
      *dstbuf++ = (srcbuf[0] << 3) | (srcbuf[0] & 0x07);
      *dstbuf++ = (srcbuf[1] << 6) | ((srcbuf[0] >> 2) & 0x38) | ((srcbuf[0] >> 5) & 0x07);
      *dstbuf++ = ((srcbuf[1] << 1) & 0xf8) | ((srcbuf[1] >> 2) & 0x07);
      dstbuf++;
      srcbuf += 2;
    }
  }
  
}

#endif
