//e#include "blobber.h"
#include "ball.h"

using namespace blobber;

class Pong : public ModInterface {
 public:
  Pong();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

 private:
  Ball ball;
  unsigned short int x_pos;
  unsigned short int y_pos;
  short int x_dir;
  short int y_dir;
  short int speed;
  COORD pos;

};


