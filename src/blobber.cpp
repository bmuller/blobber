#include "blobber.h"

class Blopper {
private:
  OptionsWindow win;
  ProjectionWindow proj;
  vector<ModInterface*> mods;
  bool aligned;
public:
  Blopper(string device) : win(device), proj(win.area.width, win.area.height), aligned(false) {
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

    if(!aligned) {
      align();
      aligned = true;
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
    int x, y;
    b.top = win.area.height;
    b.left = win.area.width;
    b.right = b.bottom = 0;
    unsigned char * data = (unsigned char *) win.area.frame->data;
    for(int index=0; index < (win.area.height*win.area.width); index++) {
      // if blue
      if(data[index*4] > 205 && data[index*4+1] < 150 && data[index*4+2] < 150) {
	x = index % win.area.width;
	y = index / win.area.width;
	if(x > b.right) b.right = x;
	if(x < b.left) b.left = x;
	if(y < b.top) b.top = y;
	if(y > b.bottom) b.bottom = y;
      }
    }

    if(b.right == b.bottom && b.left == win.area.width) {
      b.top = b.left = 0;
      b.bottom = win.area.height;
      b.right = win.area.width;
      debug("Trouble finding camera bounds...");
    }

    for(int index=(b.top*win.area.width+b.left); index<(b.top*win.area.width+b.right); index++) {
      data[index*4] = 0;
      data[index*4+1] = 0;
      data[index*4+2] = 210;
    }

    for(int index=(b.bottom*win.area.width+b.left); index<(b.bottom*win.area.width+b.right); index++) {
      data[index*4] = 0;
      data[index*4+1] = 0;
      data[index*4+2] = 210;
    } 

    for(int index=(b.top*win.area.width+b.left); index<(b.bottom*win.area.width+b.left); index+=win.area.width) {
      data[index*4] = 0;
      data[index*4+1] = 0;
      data[index*4+2] = 210;
    } 

    for(int index=(b.top*win.area.width+b.right); index<(b.bottom*win.area.width+b.right); index+=win.area.width) {
      data[index*4] = 210;
      data[index*4+1] = 0;
      data[index*4+2] = 210;
    } 

    win.area.set_bounds(b);
    proj.hide_alignment_graphics();
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
