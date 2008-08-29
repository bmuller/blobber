#include "blobber.h"

class Blopper {
private:
  OptionsWindow win;
  ProjectionWindow proj;
  vector<ModInterface*> mods;
public:
  Blopper(string device) : win(device), proj(win.area.width, win.area.height) {
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

  void add_mod(ModInterface * mi) {
    debug("Module " + mi->name + " loaded...");
    mods.push_back(mi);
  };

  bool on_idle() {
    win.area.update_frame();
    for(unsigned int i=0; i<mods.size(); i++) {
      if(win.area.hascam) 
	mods[i]->update(win.area, proj);
    }
    win.area.update_screen();
    return true;
  };

  void run() {
    Gtk::Main::run(win);
    debug("finished running");
  };
};


int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  Blopper b("/dev/video0");
  LaserTag *lt = new LaserTag();
  b.add_mod(lt);
  b.run();
  return 0;
}
