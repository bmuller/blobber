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

#include "configuration.h"
#include "exception.h"

#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>

#include <fcntl.h>

#include <fstream>

namespace blobber {
  Configuration * Configuration::_inst = 0;

  Configuration * Configuration::get_config() {
    if(_inst == 0)
      _inst = new Configuration();
    return _inst;
  };

  void Configuration::delete_config() {
    if(_inst != 0) {
      delete _inst;
      _inst = 0;
    }
  };

  Configuration::Configuration() : config(), tmpconfig() {
    directory =  Glib::build_filename(Glib::get_user_config_dir(), "blobber");
    filename = "conf.ini";
    try {
      string conflocation = Glib::build_filename(directory, filename);
      config.load_from_file(conflocation);
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

  void Configuration::settmp(string key, string value, string groupname) {
    tmpconfig.set_string(groupname, key, value);
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
    else if(tmpconfig.has_group(groupname) && tmpconfig.has_key(groupname, key))
      value = tmpconfig.get_string(groupname, key);      
    else
      value = vdefault;
  };

  void Configuration::get_set(string key, string &value, string vdefault, string groupname) {
    get(key, value, vdefault, groupname);
    set(key, value, groupname);
  };

  void Configuration::get(string key, vector<string> &values, string groupname) {
    if(config.has_group(groupname) && config.has_key(groupname, key)) {
      vector<string> l = config.get_string_list(groupname, key);
      values.assign(l.begin(), l.end());
    } else {
      values.clear();      
    }
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

  void Configuration::set_color(COLOR &c, string prefix, string groupname) {
    string red, green, blue;
    num_to_string(c.red, red);
    num_to_string(c.green, green);
    num_to_string(c.blue, blue);

    set(prefix + "_red", red, groupname);
    set(prefix + "_green", green, groupname);
    set(prefix + "_blue", blue, groupname);
  };

  void Configuration::get_color(COLOR &c, COLOR &vdefault, string prefix, string groupname) {
    string red, green, blue;
    string dred, dgreen, dblue;

    num_to_string(vdefault.red, dred);
    num_to_string(vdefault.green, dgreen);
    num_to_string(vdefault.blue, dblue);

    get(prefix + "_red", red, dred, groupname);
    get(prefix + "_green", green, dgreen, groupname);
    get(prefix + "_blue", blue, dblue, groupname);
    
    COLOR tmp(string_to_int(red), string_to_int(green), string_to_int(blue));
    c.copy(tmp);
  };

  void Configuration::get_set_color(COLOR &c, COLOR &vdefault, string prefix, string groupname) {
    get_color(c, vdefault, prefix, groupname);
    set_color(c, prefix, groupname);
  };

  void Configuration::get_bounds(BOUNDS &b, BOUNDS &vdefault, string prefix, string groupname) {
    string top, bottom, left, right;
    string dtop, dbottom, dleft, dright;

    num_to_string(vdefault.top, dtop);
    num_to_string(vdefault.bottom, dbottom);
    num_to_string(vdefault.left, dleft);
    num_to_string(vdefault.right, dright);

    get(prefix + "_top", top, dtop, groupname);
    get(prefix + "_bottom", bottom, dbottom, groupname);
    get(prefix + "_left", left, dleft, groupname);
    get(prefix + "_right", right, dright, groupname);
    
    BOUNDS tmp(string_to_int(top), string_to_int(bottom), string_to_int(left), string_to_int(right));
    b.copy(tmp);
  };

  void Configuration::get_set_bounds(BOUNDS &c, BOUNDS &vdefault, string prefix, string groupname) {
    get_bounds(c, vdefault, prefix, groupname);
    set_bounds(c, prefix, groupname);
  };

  void Configuration::set_bounds(BOUNDS &c, string prefix, string groupname) {
    string top, bottom, left, right;
    num_to_string(c.top, top);
    num_to_string(c.bottom, bottom);
    num_to_string(c.left, left);
    num_to_string(c.right, right);

    set(prefix + "_top", top, groupname);
    set(prefix + "_bottom", bottom, groupname);
    set(prefix + "_left", left, groupname);
    set(prefix + "_right", right, groupname);
  };

  bool Configuration::bounds_are_set(string prefix, string groupname) {
    return is_set(prefix+"_top",groupname) && is_set(prefix+"_bottom",groupname) \
      && is_set(prefix+"_left", groupname) && is_set(prefix+"_right",groupname);
  };

  void Configuration::get_keys(vector<string> &values, string groupname) {
    if(!config.has_group(groupname)) {
      values.clear();
      return;
    }  
    vector<string> keys = config.get_keys(groupname);
    values.assign(keys.begin(), keys.end());
  };
};
