#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "camera_window.h"
#include "projection_window.h"
#include "mod_interface.h"

namespace blobber {

  class Application {
  private:
    CameraWindow win;
    ProjectionWindow proj;
    BOUNDS visible_bounds;
    vector<ModInterface*> mods;
    bool aligned;
    Configuration *config;

    void load_modules();
    Application();
    ~Application();
  public:
    static Application * _inst;
    static Application * get_app();
    static void delete_app();
    void reload_config();
    bool module_loaded(ModInterface *mi);
    void add_mod(ModInterface * mi);
    bool on_idle();
    void run();
    void align();
  };

};

#endif /* __APPLICATION_H */

