#include "blobber.h"

using namespace blobber;

class MultiLaserTag : public ModInterface {
 public:
  MultiLaserTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  BVector<PIXEL> lastpoints;
};
