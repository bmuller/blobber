#include "blobber.h"

using namespace blobber;

class GreenScreen : public ModInterface {
 public:
  GreenScreen();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
  protected:
  unsigned char * frameRef;
  int waitCycle;
  Glib::RefPtr<Gdk::Pixbuf> image;
  string filename;
  unsigned char green_red_diff;
  unsigned char green_blue_diff;
  int sensitivity;
};

