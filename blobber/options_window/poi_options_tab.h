#ifndef __POI_OPTIONS_TAB_H
#define __POI_OPTIONS_TAB_H

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/scale.h>
#include <gtkmm/frame.h>

#include "configuration.h"
#include "camarea.h"

namespace blobber {
  using namespace std;

  class POIOptionsTab : public Gtk::VBox {
  public:
    POIOptionsTab(Camarea *_cam);
    void save();
  protected:
    Camarea *cam;
    Configuration *config;
    Gtk::Label description, redLabel, greenLabel, blueLabel;    
    Gtk::Frame poiCriteriaWindowFrame, poiCriteriaFrame;
    Gtk::VBox poiCriteriaBox;
    Gtk::HScale poiCriteriaWindow, poiCriteriaRed, poiCriteriaGreen, poiCriteriaBlue;
  };
};

#endif /* __POI_OPTIONS_TAB_H */

