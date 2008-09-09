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

