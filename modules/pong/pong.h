#include "blobber.h"

#define PI 3.14159265

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
  void grabOldData();

 private:
   COORD laser[2]; // Local laser coordinates
   COORD oldLaser[2];
   short int score[2];  // Player score
   float  yPercent, // Pixels in one percent of the vertical
          xPercent; // and horizontal for scaling
   bool debugEnabled;

   struct Ball {
     COORD coord;
     float radius,
           angle,  // Ball velocity vector angle
           speed;  // Ball velocity magnitude (screen fraction/sec)
   };
   Ball ball,
        oldBall;

   struct Paddle {
     COORD coord;
     float height,
           width,
	   angle, // Paddle velocity vector angle
           speed; // Paddle velocity magnitude (screen fraction/sec)
   };
   Paddle paddle[2],
          oldPaddle[2];
};
