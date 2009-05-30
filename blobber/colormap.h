#ifndef __COLORMAP_H
#define __COLORMAP_H

#define CLIP(color) (unsigned char) (((color)>0xFF)?0xFF:(((color)<0)?0:(color)))

namespace blobber {
  typedef void (colormap) (unsigned char * , unsigned char *, unsigned int);

  /* set of functions for mapping to rgb24 (32 bit) */
  colormap map_rgb24_special;
  colormap map_rgb24;
  colormap map_bgr24;
  colormap map_rgb32;
  colormap map_bgr32;
  colormap map_grey;
  colormap map_yuyv;
  colormap map_rgb565;
  colormap map_rgb555;

  struct colorspace {
    int          color;        
    colormap *   map;
  };

  extern struct colorspace colorspaces[];
}
#endif /* __COLORMAP_H */

