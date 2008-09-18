#include "blobber.h"

using namespace blobber;

class Pong : public ModInterface {
 public:
  Pong();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
  void projection_window_exposed(ProjectionWindow &pw);
 private:
  COORD ball;
  int bar, ball_direction;
  bool moving_right;
};
