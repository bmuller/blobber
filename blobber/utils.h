#ifndef __UTILS_H
#define __UTILS_H

#include <string>
#include <vector>

namespace blobber {
  using namespace std;

  void debug(string msg);
  void num_to_string(int i, string& s);
  void num_to_string(double i, string& s);
  int string_to_int(string s);
  double string_to_double(string s);
  void int_to_string(int i, string& s);
  // join a vector of strings with a bit of joiner text
  void join(vector<string> v, string joiner, string& result);
  // break up a string into parts whereever e occurs
  vector<string> explode(string s, string e);
  // return true if s ends with end, false otherwise
  bool ends_with(string s, string end);
  int restrict_range(int x, int lower, int upper);
};


#endif /* __UTILS_H */

