#include "blobber.h"

using namespace blobber;

class Pong : public ModInterface {
 public:
  Pong();
  ~Pong();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
  void updateFactors(ProjectionWindow &pw);
  void reset();
  void move();
  void collision(ProjectionWindow &pw);

 private:
   COORD laser[2]; // Local laser coordinates
   short int score[2];  // Player score
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
	   angle, // Paddle velocity vector angle
           speed; // Paddle velocity magnitude (screen fraction/sec)
   } paddle[2];
};
