#include "blobber.h"

using namespace blobber;

class FrameRate : public ModInterface {
 public:
  FrameRate();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  time_t start, now;
  string rate;
  int count;
};
