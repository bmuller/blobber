#include "blobber.h"

using namespace blobber;

//BACKGROUND mode uses initial reference frame
//GREENCOLOR mode is original greenscreen 
typedef enum {
  BACKGROUND = 0, 
  GREENCOLOR = 1
} rep_mode_t;

class GreenScreen : public ModInterface {
 public:
  GreenScreen();
  ~GreenScreen();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  Glib::RefPtr<Gdk::Pixbuf> image;
  rep_mode_t rep_mode;
  int sensitivity;
  int wait_cycle;
  unsigned char * frame_ref;
  unsigned char green_red_diff;
  unsigned char green_blue_diff;
};

