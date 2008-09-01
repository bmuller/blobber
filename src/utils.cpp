#include "blobber.h"

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
