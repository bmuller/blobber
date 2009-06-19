#ifndef __OPTIONS_WINDOW_H
#define __OPTIONS_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include <gtkmm/button.h>
#include <gtkmm/table.h>

#include "additional_options_tab.h"
#include "camera_options_tab.h"
#include "module_options_tab.h"
#include "poi_options_tab.h"

namespace blobber {
  using namespace std;

  class OptionsWindow : public Gtk::Window {
  public:	
    OptionsWindow(Camarea *cam);
    void ok();
    void cancel();
  protected:
    ModulesOptionsTab mot;
    CameraOptionsTab cot;
    POIOptionsTab poiot;
    AdditionalOptionsTab aot;
    Gtk::Notebook nb;
    Gtk::VBox mainBox;
    Gtk::Table buttons;
    Gtk::Button okButton, cancelButton;
  };
};

#endif /* __OPTIONS_WINDOW_H */

