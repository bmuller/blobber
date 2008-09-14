#include "blobber.h"

namespace blobber {

  Configuration::Configuration() : config() {
    directory =  Glib::build_filename(Glib::get_user_config_dir(), "blobber");
    filename = "conf.ini";
    try {
      string conflocation = Glib::build_filename(directory, filename);
      config.load_from_file(conflocation);
      debug("Successfully loaded config file " + conflocation);
    } catch(Glib::Error e) {};
  };

  Configuration::~Configuration() {
    save();
  }

  void Configuration::save() {
    try {
      Glib::Dir::Dir dirio(directory);
    } catch(Glib::FileError fe) {
      if(mkdir(directory.c_str(), S_IRWXU) != 0)
	throw ConfigurationException("Could not make configuration directory " + directory);
    };
    string conflocation = Glib::build_filename(directory, filename);
    debug("Writing configuration file " + conflocation);
    ofstream fio(conflocation.c_str(), fstream::out | fstream::trunc);
    string data = config.to_data();
    fio.write(data.c_str(), data.size());
    fio.close();
  };

  void Configuration::set(string key, string value, string groupname) {
    config.set_string(groupname, key, value);
  };

  void Configuration::set(string key, vector<string> values, string groupname) {
    config.set_string_list(groupname, key, values);
  };

  void Configuration::get(string key, string &value, string vdefault, string groupname) {
    if(config.has_group(groupname) && config.has_key(groupname, key))
      value = config.get_string(groupname, key);
    else
      value = vdefault;
  };

  void Configuration::get(string key, vector<string> &values, string groupname) {
    if(config.has_group(groupname) && config.has_key(groupname, key))
      values = config.get_string_list(groupname, key);
    else
      values.clear();      
  };

  bool Configuration::is_set(string key, string groupname) {
    return (config.has_group(groupname) && config.has_key(groupname, key));
  };

  void Configuration::module_set(string key, string value, string modname) {
    set(key, value, "mod_" + modname);
  };

  void Configuration::module_set(string key, vector<string> values, string modname) {
    set(key, values, "mod_" + modname);
  };

  void Configuration::module_get(string key, string &value, string vdefault, string modname) {
    get(key, value, vdefault, "mod_" + modname);
  };

  void Configuration::module_get(string key, vector<string> &values, string modname) {
    get(key, values, "mod_" + modname);    
  };

};
