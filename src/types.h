using namespace std;

struct buffer {
  void *                  start;
  size_t                  length;
};

struct COLOR {
  int red, green, blue;
  COLOR() {
    red = green = blue = 0;
  };
  COLOR(int r, int g, int b) {
    red=r; green=g; blue=b;
  };
  COLOR(const COLOR &c) {
    copy(c);
  };
  void copy(const COLOR &c) {
    red=c.red; green=c.green; blue=c.blue;
  }
  double cairo_red() { return float(red) / 255.0; };
  double cairo_green() { return float(green) / 255.0; };
  double cairo_blue() { return float(blue) / 255.0; };
};

#define RED COLOR(255,0,0)
#define GREEN COLOR(0,255,0)
#define BLUE COLOR(0,0,255)
#define BLACK COLOR(0,0,0)
#define WHITE COLOR(255,255,255)
#define LIGHT_BLUE COLOR(50,100,255)

// color range
struct CRANGE {
  COLOR lower, upper;
  CRANGE(COLOR l, COLOR u) {
    lower.copy(l); 
    upper.copy(u);
  };
  CRANGE(COLOR l) {
    lower.copy(l);
    upper.copy(WHITE);
  };
  bool contains(COLOR &c) {
    return c.red >= lower.red && c.red <= upper.red && \
      c.green >= lower.green && c.green <= upper.green && \
      c.blue >= lower.blue && c.blue <= upper.blue;
  }
};

struct COORD {
  int x, y;
  COORD(int _x, int _y) {
    x = _x; y = _y;
  };
  COORD() {
    x = y = 0;
  };
  void copy(COORD &c) {
    x = c.x; y = c.y;
  };
  void to_s(string &s) {
    string xs, ys;
    num_to_string(x, xs);
    num_to_string(y, ys);
    s = "(" + xs + ", " + ys + ")";
  };
  double distance_from(COORD c) {
    double xdistance = fabs(double(c.x - x));
    double ydistance = fabs(double(c.y - y));
    return sqrt(pow(xdistance, 2.0) + pow(ydistance, 2.0));
  };
  double distance_from(int x, int y) {
    return distance_from(COORD(x, y));
  };
};

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
  void from_coords(COORD &one, COORD &two) {
    top = min(one.y, two.y);
    bottom = max(one.y, two.y);
    left = min(one.x, two.x);
    right = max(one.x, two.x);    
  };
  void update(COORD &c) {
    top = min(top, c.y);
    bottom = max(bottom, c.y);
    left = min(left, c.x);
    right = max(right, c.x);    
  };
};

