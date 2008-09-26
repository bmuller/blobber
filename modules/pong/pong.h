//#include "pongmod.h"
//#include "blobber.h"
//#include "ball.h"



using namespace blobber;

class Pong : public ModInterface {
 public:
  Pong();
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

 private:
  Ball ball;

};


