#include "blobber.h"

using namespace blobber;

class Pong : public ModInterface {
 public:
  Pong();
  ~Pong();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

 private:
   COORD laser[2];
   float  vPercent, // Pixels in one percent of the vertical
          hPercent; // and horizontal for scaling

   struct Ball {
     COORD coord;
     float radius,
           angle,  // Ball velocity vector angle
           speed;  // Ball velocity magnitude (screen fraction/sec)
   } ball;

   struct Paddle {
     COORD coord;
     float height,
           width,
           speed; // Paddle velocity magnitude (screen fraction/sec)
   } paddle[2];
};
