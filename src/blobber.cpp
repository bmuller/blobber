#include "blobber.h"

class Blopper {
private:
  OptionsWindow win;
  ProjectionWindow proj;
  BOUNDS visible_bounds;
  vector<ModInterface*> mods;
  bool aligned, doalignment;
public:
  Blopper(string device, bool da=true) : win(device), proj(win.area.width, win.area.height), aligned(false), doalignment(da) {
    proj.set_transient_for(win);
    proj.show();
    Glib::signal_idle().connect(sigc::mem_fun(*this, &Blopper::on_idle));

    // if we're not going to be doing an alignment, just set the projection area and camarea bounds to be the 
    // full visible area of camera
    if(!doalignment) {
      debug("Not doing an alignment, assuming camera's visible area is the projected image and nothing else...");
      BOUNDS b(0, win.area.height, 0, win.area.width);
      win.area.set_bounds(b);
      proj.set_bounds(b);
    } else {
      debug("Performing alignment.  Looking for camarea bounds...");
    }
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
   
    if(win.area.hascam && doalignment) {
      if(!aligned)
	align();
      else
	draw_bounds(visible_bounds);
    }

    // (if we are aligned or we don't have to do an alignment) and there is a camera
    if((aligned || !doalignment) && win.area.hascam) {
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
    int x, y;
    b.top = win.area.height;
    b.left = win.area.width;
    b.right = b.bottom = 0;
    unsigned char * data = (unsigned char *) win.area.frame->data;
    for(int index=0; index < (win.area.height*win.area.width); index++) {
      // looking for blue
      if(data[index*4] > 205 && data[index*4+1] < 190 && data[index*4+2] < 150) {
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
    } else {
      aligned = true;
      win.area.set_bounds(b);
      proj.set_bounds(b);
      visible_bounds.copy(b);
      proj.hide_alignment_graphics();
      debug("Found camera bounds!  Go get a beer.");
    }
  };

  void draw_bounds(BOUNDS b) {
    unsigned char * data = (unsigned char *) win.area.frame->data;

    // These will place a border on the camera window to show what it "sees"
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
  };
};


int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  Blopper b("/dev/video0", false);
  LaserTag *lt = new LaserTag();
  b.add_mod(lt);
  b.run();
  return 0;
}
