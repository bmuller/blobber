#include "blobber.h"

class Blopper {
private:
  OptionsWindow win;
  ProjectionWindow proj;
  vector<ModInterface*> mods;
public:
  Blopper() : win("/dev/video0") {
    proj.set_transient_for(win);
    proj.show();
    Glib::signal_idle().connect(sigc::mem_fun(*this, &Blopper::on_idle));
  };

  ~Blopper() {
    debug("Freeing all modules...");
    for(unsigned int i=0; i<mods.size(); i++) {
      debug("Freeing " + mods[i]->name + "...");
      delete mods[i];
    }
  };

  bool add_mod(ModInterface * mi) {
    mods.push_back(mi);
  };

  bool on_idle() {
    win.area.update_frame();
    for(unsigned int i=0; i<mods.size(); i++) {
      mods[i]->update(win.area, proj);
    }
    win.area.update_screen();
    return true;
  };

  void run() {
    Gtk::Main::run(win);
  };
};


int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  Blopper b;
  LaserTag *lt = new LaserTag();
  b.add_mod(lt);
  b.run();
  return 0;
}
