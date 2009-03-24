#ifndef __TYPES_H
#define __TYPES_H

#include "utils.h"
#include <cmath>
#include <glibmm/random.h>

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
      red = restrict_range(r, 0, 255); 
      green = restrict_range(g, 0, 255); 
      blue = restrict_range(b, 0, 255);
    };
    COLOR(const COLOR &c) {
      copy(c);
    };
    void copy(const COLOR &c) {
      red=c.red; green=c.green; blue=c.blue;
    }
    void set(int r, int g, int b) {
      red=r; green=g; blue=b;
    };
    /**
       This will convert the color to a RGBA int with no alpha (used throughout Gdk libs).
    */
    int to_int() {
      return ((red & 0xff) << 24) | ((green & 0xff) << 16) | ((blue & 0xff) << 8);
    };
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

    /** 
     * A function to determine whether or not the given color is brighter than 
     * the current color.  Returns true if the total difference for each color
     * is positive.
     * 
     * @param c The COLOR in question
     * 
     * @return A boolean result
     */
    bool brighter_than(COLOR &c) {  
      return ((red - c.red) + (green - c.green) + (blue - c.blue)) > 0;
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
    /** 
	@param center The color to center the range on
	@param plusminus The range should be plusminus above and below the center
    */
    CRANGE(COLOR &center, int plusminus) {
      COLOR newlower(center.red - plusminus, center.green - plusminus, center.blue - plusminus);
      COLOR newupper(center.red + plusminus, center.green + plusminus, center.blue + plusminus);
      lower.copy(newlower);
      upper.copy(newupper);
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
    void to_string(string &s) {
      string supper, slower;
      lower.to_string(slower);
      upper.to_string(supper);
      s = "CRANGE(LOWER(" + slower + "), UPPER(" + supper + "))";
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
    void to_string(string &s) {
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
      return c.x >= left && c.x <= right && c.y >= top && c.y <= bottom;
    };
    bool contains(BOUNDS &b) {
      return b.left >= left && b.right <= right && b.top >= top && b.bottom <= bottom;
    };
    void random_coordinate(COORD &c) {
      Glib::Rand rand;
      c.x = rand.get_int_range(left, right+1);
      c.y = rand.get_int_range(top, bottom+1);
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

  struct PROPORTION {
    double width_percent, height_percent;
    PROPORTION() {
      width_percent = 1.0;
      height_percent = 1.0;
    };
    PROPORTION(double width, double height) {
      width_percent = width;
      height_percent = height;
    };
    void copy(PROPORTION &p) {
      width_percent = p.width_percent;
      height_percent = p.height_percent;
    };
    int convert_width(int width) {
      return (int) (width * width_percent);
    };
    int convert_height(int height) {
      return (int) (height * height_percent);
    };
  };

  struct DIMENSION {
    int width, height;
    DIMENSION() {
      width = 0;
      height = 0;
    };
    DIMENSION(int _width, int _height) {
      width = _width;
      height = _height;
    };
    DIMENSION(const DIMENSION &d) {
      width = d.width;
      height = d.height;
    };
    void copy(DIMENSION &d) {
      width = d.width;
      height = d.height;
    };
    /**
       Reset our height and width to be some proportion of d's
       height and width
    */
    void scale(PROPORTION p, DIMENSION d) {
      width = p.convert_width(d.width);
      height = p.convert_height(d.height);
    };
    /**
       Reset our height and width to be some proportion of our current
       height and width
    */
    void scale(PROPORTION p) {
      width = p.convert_width(width);
      height = p.convert_height(height);      
    };
    /**
       Get the proportion of this dimension to another
    */
    void get_proportion(PROPORTION &p, DIMENSION other) {
      p.height_percent = ((double) height) / ((double) other.height);
      p.width_percent = ((double) width) / ((double) other.width);
    };
  };

#define HALF_PROPORTION PROPORTION(0.5, 0.5)

  struct TIMEKEEPER {
    clock_t current;
    int clocks_per_millisecond;
    TIMEKEEPER() {
      current = clock();
      clocks_per_millisecond = (CLOCKS_PER_SEC / 1000);
    };
    // get milliseconds since last called
    long int set_marker() {
      clock_t old = current;   
      current = clock();
      return (current - old) / clocks_per_millisecond;
    };
  };
};



#endif /* __TYPES_H */

