/*
    Blobber is webcam based blob tracking software
    Copyright (C) 2008 Charleston, SC Linux Users Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "config.h"

// only include this file if we have v4l2
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

  void map_rgb24_special(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      dstbuf[2] = srcbuf[0];
      dstbuf[1] = srcbuf[1];
      dstbuf[0] = srcbuf[2];
      dstbuf[3] = 0;
      dstbuf += 4;
      srcbuf += 3;
    }
  }

  void map_rgb24(unsigned char * dst, unsigned char * src, unsigned int sizeimage) {
    unsigned char *dstbuf, *dstend, *srcbuf;
    dstbuf = dst;
    dstend = dstbuf + sizeimage; 
    srcbuf = src;
    while (dstbuf < dstend) {
      dstbuf[0] = srcbuf[0];
      dstbuf[1] = srcbuf[1];
      dstbuf[2] = srcbuf[2];
      dstbuf[3] = 0;
      dstbuf += 4;
      srcbuf += 3;
    }
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
