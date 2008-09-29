#include "blobber.h"
using namespace blobber;

typedef enum {YARPER = 0} alien_t;
typedef enum {ATTACK = 0} pose_t;

typedef struct {
  alien_t type;
  pose_t pose;
  int speed;
  COORD center;
  COLOR color;
} Alien;

void init_alien(Alien * a, Camarea &area) {
  a->type = YARPER;
  a->pose = ATTACK;
  a->speed = 20 + rand() % 100;
  a->center.x = area.bounds.left + rand() % area.width;
  a->center.y = area.bounds.top;
  a->color.copy(RED);
}

bool in_bounds(Alien * a, COORD c) { return(sqrt( pow( (float) (c.x - a->center.x), 2) + pow( (float) (c.y - a->center.y), 2 ) ) < 5.0); }


class Invaders : public ModInterface {
 public:
  Alien * aliens;
  vector<COORD> explosions;
  clock_t alien_timer;
  clock_t update_timer;

  Invaders();
  ~Invaders();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

  void draw_alien(ProjectionWindow &pw, Cairo::RefPtr<Cairo::Context> cr, Alien * a, bool erase = false);
  void draw_explosions(ProjectionWindow &pw);
};

