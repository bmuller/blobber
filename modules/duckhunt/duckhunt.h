#include "blobber.h"

using namespace blobber;

class Duckhunt : public ModInterface {
 public:
  Duckhunt();
  ~Duckhunt();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
  void projection_window_exposed(ProjectionWindow &pw);
 protected:
  Sprite * duck;
  Sprite * deadduck;
  PROPORTION duck_proportion;
  BOUNDS sky;
  COORD destination;
  int score, show_dead_count;
};
