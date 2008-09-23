#include "blobber.h"
//<ctime> (time.h) included in "blobber.h"
//<cstdio> (stdio.h) included in "blobber.h"

//#ifndef COLOR_RANDOMIZER_H
//#define COLOR_RANDOMIZER_H

using namespace blobber;

class ColorRandomizer : public ModInterface {
 public:
  ColorRandomizer();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  //This could be a double or float if we want to take
  //advantage of fractions of a second
  int waitTime;
  Glib::Rand rand;
  time_t last, now;
  COLOR color;
};

//#endif
