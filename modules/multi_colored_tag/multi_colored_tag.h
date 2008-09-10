#include "blobber.h"

class MultiColoredTag : public ModInterface {
 public:
  MultiColoredTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  COORD lastpoint;
  unsigned int missing_counter;
  vector<COORD> points;
};
