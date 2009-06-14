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

#include "utils.h"
#include "config.h"

#include <cstdlib>
#include <iostream>

#include <gtkmm/messagedialog.h>

namespace blobber {
  using namespace std;

  void debug(string msg) {
#ifdef DEBUG
    string time_s = "";
    time_t rawtime = time(NULL);
    tm *tm_t = localtime(&rawtime);
    char rv[40];
    if(strftime(rv, sizeof(rv)-1, "%a %b %d %H:%M:%S %Y", tm_t))
      time_s = "[" + string(rv) + "] ";
    cout << time_s << "[" << string(PACKAGE_NAME) << "] " << msg << endl;
    cout.flush();
#endif
  };

  void error(string msg) {
    Gtk::MessageDialog md(string(PACKAGE_STRING) + " Error", false, Gtk::MESSAGE_WARNING);
    md.set_secondary_text(msg);
    md.set_title("Error");
    md.run();
  };

  void alert(string msg) {
    Gtk::MessageDialog md(string(PACKAGE_STRING) + " Information", false);
    md.set_secondary_text(msg);
    md.set_title("Information");
    md.run();
  };

  /** convert integer to string */
  void num_to_string(int i, string& s) {
    char c_int[100];
    sprintf(c_int, "%ld", i);
    s = string(c_int);
  };

  void num_to_string(double i, string& s) {
    char c_int[100];
    sprintf(c_int, "%f", i);
    s = string(c_int);
  };

  int string_to_int(string s) {
    return atoi(s.c_str());
  };

  double string_to_double(string s) {
    return atof(s.c_str());
  };

  /** convert integer to string */
  void int_to_string(int i, string& s) {
    char c_int[100];
    sprintf(c_int, "%ld", i);
    s = string(c_int);
  };

  /** Split a string s into parts by value e */
  vector<string> explode(string s, string e) {
    vector<string> ret;
    int iPos = s.find(e, 0);
    int iPit = e.length();
    while(iPos>-1) {
      if(iPos!=0)
	ret.push_back(s.substr(0,iPos));
      s.erase(0,iPos+iPit);
      iPos = s.find(e, 0);
    }
    if(s!="")
      ret.push_back(s);
    return ret;
  };

  /** 
   * Restrict a given integer to an upper/lower range
   * 
   * @param x Integer to be restricted
   * @param lower The lower bounds (inclusive)
   * @param upper The upper bounds (inclusive)
   * 
   * @return An integer value within the given range.
   */
  int restrict_range(int x, int lower, int upper) {
    return max(min(x, upper), lower);
  };

  bool ends_with(string s, string end) {
    int ssize = s.size();
    int esize = end.size();
    if(esize > ssize)
      return false;
    return s.substr((ssize-esize), ssize) == end;
  };
};
