#include "blobber.h"

using namespace blobber;

class LaserTag : public ModInterface {
 public:
  LaserTag();
  ~LaserTag() {};
  double smooth(double previous, double next);
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  COORD lastpoint;
  unsigned int missing_counter;
  double line_width;
};
