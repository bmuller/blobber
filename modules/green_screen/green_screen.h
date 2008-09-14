#include "blobber.h"

using namespace blobber;

class GreenScreen : public ModInterface {
 public:
  GreenScreen();
  ~GreenScreen();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  Glib::RefPtr<Gdk::Pixbuf> image;
  string filename;
  unsigned char green_red_diff;
  unsigned char green_blue_diff;
};

