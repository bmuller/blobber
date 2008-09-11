
namespace blobber {

#define CLIP(color) (unsigned char)(((color)>0xFF)?0xff:(((color)<0)?0:(color)))

class YUYVtoRGB32 {
 public:
  YUYVtoRGB32(unsigned int height, unsigned int width);
  ~YUYVtoRGB32();
  void convert(unsigned char* yuyv, unsigned char* rgb32);
 private:
  unsigned int yuyv_bytes;
  int *LutRv;
  int *LutGu;
  int *LutGv;
  int *LutBu;
  inline unsigned char R_FROMYV(unsigned char y, unsigned char v) { return CLIP((y) + LutRv[(v)]); }
  inline unsigned char G_FROMYUV(unsigned char y, unsigned char u, unsigned char v) { return CLIP((y) + LutGu[(u)] + LutGv[(v)]); }
  inline unsigned char B_FROMYU(unsigned char y, unsigned char u) { return CLIP((y) + LutBu[(u)]); }
};

};
  
    
