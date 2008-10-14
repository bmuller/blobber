
namespace blobber {
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
  void to_string(string &s) {
    string sred, sgreen, sblue;
    num_to_string(red, sred);
    num_to_string(blue, sblue);
    num_to_string(green, sgreen);
    s = sred + "," + sblue + "," + sgreen;
  };
  void from_string(string s) {
    vector<string> parts = explode(s, ",");
    if(parts.size() != 3)
      return;
    red = string_to_int(parts[0]);
    blue = string_to_int(parts[1]);
    green = string_to_int(parts[2]);
  };
};

#define RED COLOR(255,0,0)
#define GREEN COLOR(0,255,0)
#define BLUE COLOR(0,0,255)
#define BLACK COLOR(0,0,0)
#define WHITE COLOR(255,255,255)
#define LIGHT_BLUE COLOR(50,100,255)
#define GREY COLOR(200,200,200)
#define BROWN COLOR(102,51,0)

// color range
struct CRANGE {
  COLOR lower, upper;
  CRANGE() {
    lower.copy(BLACK);
    upper.copy(WHITE);
  };
  CRANGE(COLOR l) {
    lower.copy(l);
    upper.copy(WHITE);
  };
  CRANGE(COLOR l, COLOR u) {
    lower.copy(l); 
    upper.copy(u);
  };
  bool contains(COLOR &c) {
    return c.red >= lower.red && c.red <= upper.red && \
      c.green >= lower.green && c.green <= upper.green && \
      c.blue >= lower.blue && c.blue <= upper.blue;
  };
  void copy(CRANGE &c) {
    lower.copy(c.lower);
    upper.copy(c.upper);
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
  void operator+=(COORD &c) {
    x += c.x; y += c.y;
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
  bool contains(COORD &c) {
    return c.x > left && c.x < right && c.y > top && c.y < bottom;
  };
};

struct PIXEL {
  COLOR color;
  COORD coord;
  PIXEL(COLOR _color, COORD _coord) {
    color.copy(_color);
    coord.copy(_coord);
  };
};
};
