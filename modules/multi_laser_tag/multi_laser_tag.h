#include "blobber.h"

class MultiLaserTag : public ModInterface {
 public:
  MultiLaserTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  vector<PIXEL> lastpoints;
};
