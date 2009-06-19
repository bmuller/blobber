#ifndef __MODULE_OPTIONS_TAB_H
#define __MODULE_OPTIONS_TAB_H

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/checkbutton.h>

#include "configuration.h"

namespace blobber {
  using namespace std;

  class ModulesOptionsTab : public Gtk::VBox {
  public:
    ModulesOptionsTab();
    void save();
  protected:
    Configuration *config;
    Gtk::Label description;
    Gtk::Frame modsFrame;
    Gtk::VBox modsBox;
    // module description by module name
    std::map<string, string> availableModules;
    // module check button pointer by module name
    std::map<string, Gtk::CheckButton*> modButtons;
    // module file name by module name;
    std::map<string, string> modFiles;
  };  
};

#endif /* __MODULE_OPTIONS_TAB_H */

