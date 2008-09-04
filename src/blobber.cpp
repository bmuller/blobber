#include "blobber.h"

class Blopper {
private:
  CameraWindow win;
  ProjectionWindow proj;
  BOUNDS visible_bounds;
  vector<ModInterface*> mods;
  bool aligned;
public:
  Blopper(string device) : win(device), proj(win.area.width, win.area.height), aligned(false) {
    if(win.area.hascam) {
      proj.set_transient_for(win);
      proj.show();
    }
    Glib::signal_idle().connect(sigc::mem_fun(*this, &Blopper::on_idle));

    // until we do an alignment, just set the projection area and camarea bounds to be the 
    // full visible area of camera
    BOUNDS b(0, win.area.height, 0, win.area.width);
    win.area.set_bounds(b);
    proj.set_bounds(b);
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

    if(win.area.hascam && win.area.manual_align) {
      debug("manually setting camera's bounds");
      aligned = true;
      proj.set_bounds(win.area.bounds);
      visible_bounds.copy(win.area.bounds);
      win.area.manual_align = false; // so we don't keep doing this over and over
    } else if(win.area.hascam && proj.need_alignment) {
      align();
    } 

    if(win.area.hascam && aligned) {
      win.area.draw_bounds(visible_bounds);
    }

    // (if we are aligned or we don't have to do an alignment) and there is a camera
    if(win.area.hascam) {
      for(unsigned int i=0; i<mods.size(); i++) {
	mods[i]->update(win.area, proj);
      }
    }

    win.area.update_screen();
    return true;
  };

  void run() {
    Gtk::Main::run(win);
    debug("finished running");
  };

  void align() {
    proj.show_alignment_graphics();

    BOUNDS b;
    COORD coord;
    b.top = win.area.height;
    b.left = win.area.width;
    b.right = b.bottom = 0;
    unsigned char * data = (unsigned char *) win.area.frame->data;
    for(int index=0; index < (win.area.height*win.area.width); index++) {
      // looking for blue
      if(data[index*4] > 205 && data[index*4+1] < 190 && data[index*4+2] < 150) {
	coord.x = index % win.area.width;
	coord.y = index / win.area.width;
	b.update(coord);
      }
    }

    if(b.right == b.bottom && b.left == win.area.width) {
      b.top = b.left = 0;
      b.bottom = win.area.height;
      b.right = win.area.width;
    } else {
      aligned = true;
      win.area.set_bounds(b);
      proj.set_bounds(b);
      visible_bounds.copy(b);
      proj.hide_alignment_graphics();
      debug("Found camera bounds!  Go get a beer.");
    }
  };
};


int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  Blopper b("/dev/video0");
  LaserTag *lt = new LaserTag();
  b.add_mod(
  //GreenScreen *gs = new GreenScreen("stone-640x480.jpg");
  //b.add_mod(gs);
  b.run();
  return 0;
}
