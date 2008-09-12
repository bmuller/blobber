#include "blobber.h"

namespace blobber {

  Configuration::Configuration() : config() {
    directory =  Glib::build_filename(Glib::get_user_config_dir(), "blobber");
    filename = "conf.ini";
    try {
      config.load_from_file(Glib::build_filename(directory, filename));
    } catch(Glib::Error e) {};
  };

  Configuration::~Configuration() {
    save();
  }

  void Configuration::save() {
    try {
      Glib::Dir::Dir dirio(directory);
    } catch(Glib::FileError fe) {
      mkdir(directory.c_str(), S_IRWXU);
    };
    ofstream fio(Glib::build_filename(directory, filename).c_str(), fstream::out | fstream::trunc);
    string data = config.to_data();
    fio.write(data.c_str(), data.size());
    fio.close();
  };

  void Configuration::set(string key, string value, string groupname) {
    config.set_string(groupname, key, value);
  };

  void Configuration::set(string key, vector<string> values, string groupname) {

  };

  void Configuration::get(string key, string &value, string groupname) {
    value = config.get_string(groupname, key);
  };

  void Configuration::get(string key, vector<string> &values, string groupname) {

  };

  void Configuration::module_set(string key, string value, string modname) {
    set(key, value, "mod_" + modname);
  };

  void Configuration::module_set(string key, vector<string> values, string modname) {
    set(key, values, "mod_" + modname);
  };

  void Configuration::module_get(string key, string &value, string modname) {
    get(key, value, "mod_" + modname);
  };

  void Configuration::module_get(string key, vector<string> &values, string modname) {
    get(key, values, "mod_" + modname);    
  };

};
