#include "blobber.h"

using namespace blobber;

class GreenScreen : public ModInterface {
 public:
  GreenScreen();
  ~GreenScreen();
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  Glib::RefPtr<Gdk::Pixbuf> image;
  unsigned char green_red_diff;
  unsigned char green_blue_diff;
};

