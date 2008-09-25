#include "blobber.h"
using namespace blobber;

class Invaders : public ModInterface {
 public:
  vector<COORD> aliens;
  vector<COORD> laser_beams;
  vector<COORD> alien_beams; 
  COORD laser;
  clock_t timer;

  Invaders();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

  void draw_laser(Camarea &area, ProjectionWindow &pw,  COORD location);
  void draw_aliens(ProjectionWindow &pw, vector<COORD> &locations);
  void draw_beams(ProjectionWindow &pw, vector<COORD> &locations);
  void draw_explosions(ProjectionWindow &pw, vector<COORD> &locations);
};

