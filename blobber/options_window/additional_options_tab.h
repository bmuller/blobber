#ifndef __ADDITIONAL_OPTIONS_TAB_H
#define __ADDITIONAL_OPTIONS_TAB_H */

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>

#include "configuration.h"

namespace blobber {
  using namespace std;

  class AdditionalOptionsTab : public Gtk::VBox {
  public:
    AdditionalOptionsTab();
    void save();
  protected:
    Configuration *config;
    Gtk::Label description;    
    Gtk::Label lblSavedImagesDir;
    Gtk::HBox savedImagesDirBox;
    Gtk::Entry savedImagesDir;
  };
};

#endif /* __ADDITIONAL_OPTIONS_TAB_H */

