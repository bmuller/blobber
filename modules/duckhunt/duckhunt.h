#include "blobber.h"

using namespace blobber;

class Duckhunt : public ModInterface {
 public:
  Duckhunt();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
  void projection_window_exposed(ProjectionWindow &pw);
  void destroy();
 protected:
  Sprite * duck;
  PROPORTION duck_proportion;
  int direction;
};
