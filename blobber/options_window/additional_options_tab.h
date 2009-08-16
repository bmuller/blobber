#ifndef __ADDITIONAL_OPTIONS_TAB_H
#define __ADDITIONAL_OPTIONS_TAB_H

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/button.h>
#include <gtkmm/stock.h>

#include "configuration.h"

namespace blobber {
  using namespace std;

  class AdditionalOptionsTab : public Gtk::VBox {
  public:
    AdditionalOptionsTab();
    void save();
    void browseClicked();
  protected:
    Configuration *config;
    Gtk::Label description;    
    Gtk::Label lblSavedImagesDir;
    Gtk::VBox savedImagesDirBox;
    Gtk::Entry savedImagesDir;
    Gtk::HButtonBox buttonBox;
    Gtk::Button browseButton;
  };
};

#endif /* __ADDITIONAL_OPTIONS_TAB_H */

