#include "blobber.h"

class GreenScreen : public ModInterface {
 public:
  GreenScreen(const std::string filename);
  ~GreenScreen();
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  Glib::RefPtr<Gdk::Pixbuf> image;
  unsigned char green_red_diff;
  unsigned char green_blue_diff;
};

