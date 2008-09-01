struct buffer {
  void *                  start;
  size_t                  length;
};

struct COLOR {
  int red, green, blue;
  COLOR(int r, int g, int b) {
    red=r; green=g; blue=b;
  };
};

#define RED COLOR(255,0,0)
#define GREEN COLOR(0,255,0)
#define BLUE COLOR(0,0,255)
#define BLACK COLOR(0,0,0)
#define WHITE COLOR(255,255,255)

struct BOUNDS {
  int top, bottom, left, right;
  BOUNDS() {
    top = bottom = left = right = 0;
  };
  BOUNDS(int t, int b, int l, int r) {
    top=t; bottom=b; left=l; right=r;
  };  
  void copy(BOUNDS &b) {
    top=b.top; bottom=b.bottom; left=b.left; right=b.right;
  };
  int width() {
    return right - left;
  };
  int height() {
    // yup, bottom - top, don't change
    return bottom - top;
  };
};

struct COORD {
  int x, y;
  COORD(int _x, int _y) {
    x = _x; y = _y;
  };
  COORD() {
    x = y = 0;
  };
};
