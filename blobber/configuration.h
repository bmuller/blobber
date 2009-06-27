#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "types.h"
#include <glibmm/keyfile.h>

namespace blobber {
  using namespace std;

  class Configuration {
  private:
    // temporary configuration values (like from the command line) are stored in tmpconfig
    Glib::KeyFile config, tmpconfig;
    string filename, directory;
    Configuration();
    ~Configuration();
  public:
    static Configuration * _inst;
    static Configuration * get_config();
    static void delete_config();
    void save();

    // get/set values for main program
    void set(string key, string value, string groupname="blobber");
    // set a temporary value that will not be saved
    void settmp(string key, string value, string groupname="blobber");
    void set(string key, vector<string> values, string groupname="blobber");   
    bool is_set(string key, string groupname="blobber");
    // get from a set value, if not found, check a set tmp value, if not found, return vdefault
    void get(string key, string &value, string vdefault, string groupname="blobber");
    // get if exists, set if not
    void get_set(string key, string &value, string vdefault, string groupname="blobber");
    void get(string key, vector<string> &values, string groupname="blobber");

    void get_keys(vector<string> &keys, string groupname);

    // get/set values for modules
    void module_set(string key, string value, string modname);
    void module_set(string key, vector<string> values, string modname);   
    void module_get(string key, string &value, string vdefault, string modname);
    void module_get(string key, vector<string> &values, string modname);

    // storing colors
    void get_color(COLOR &c, COLOR &vdefault, string prefix, string groupname="blobber");
    void get_set_color(COLOR &c, COLOR &vdefault, string prefix, string groupname="blobber");
    void set_color(COLOR &c, string prefix, string groupname="blobber");

    // storing bounds
    void get_bounds(BOUNDS &b, BOUNDS &vdefault, string prefix, string groupname="blobber");
    void get_set_bounds(BOUNDS &c, BOUNDS &vdefault, string prefix, string groupname="blobber");
    void set_bounds(BOUNDS &c, string prefix, string groupname="blobber");
    bool bounds_are_set(string prefix, string groupname="blobber");
  };
};

#endif /* __CONFIGURATION_H */

