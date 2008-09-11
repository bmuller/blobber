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

#include "blobber.h"

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

/** convert integer to string */
void num_to_string(int i, std::string& s) {
  char c_int[100];
  sprintf(c_int, "%ld", i);
  s = std::string(c_int);
};

void num_to_string(double i, std::string& s) {
  char c_int[100];
  sprintf(c_int, "%ld", i);
  s = std::string(c_int);
};
};
