#include "blobber.h"

using namespace blobber;

class Duckhunt : public ModInterface {
 public:
  Duckhunt();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  Sprite duck;
};
