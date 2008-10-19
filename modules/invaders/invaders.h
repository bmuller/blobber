#include "blobber.h"
using namespace blobber;

#define NORMAL 0x00
#define PAUSED 0x01
#define RESET  0x02
#define WAIT   0x03
#define SQRT_18 4.242640687

class Alien {
 public:
  int state;
  int speed;
  COORD loc;
  Alien(COORD _loc);
  void reset(COORD _loc);
  bool in_bounds(COORD c);
};

class Explosion {
 public:
  int state;
  COORD loc;
  Explosion(COORD _loc);
};

class Invaders : public ModInterface {
 public:
  char paused;
  int level;
  int score;
  COORD cross;
  vector<Alien> aliens;
  vector<Explosion> kills;
  Glib::RefPtr<Gdk::Pixbuf> kill;
  Glib::RefPtr<Gdk::Pixbuf> kill_blk;
  Glib::RefPtr<Gdk::Pixbuf> * ali;
  Glib::RefPtr<Gdk::Pixbuf> * ali_blk;
  Glib::RefPtr<Gdk::Pixbuf> pau;
  Glib::RefPtr<Gdk::Pixbuf> pau_blk;

  Invaders();
  ~Invaders();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

  void key(GdkEventKey * event);
  void expose(GdkEventExpose* event);
};

