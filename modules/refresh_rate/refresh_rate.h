#include "blobber.h"

using namespace blobber;

class RefreshRate : public ModInterface {
 public:
  RefreshRate();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  time_t start, now;
  string rate;
  int count;
};
