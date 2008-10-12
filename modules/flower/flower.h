#include "blobber.h"
using namespace blobber;

#define NORMAL 0x00
#define PAUSED 0x01
#define RESET  0x02
#define WAIT   0x03
#define SQRT_18 4.242640687

class Entity {
 public:
  int state;
  COORD loc;
  Entity(COORD coord) { loc.copy(coord); state = 0; }
};

bool in_bounds(Entity * a, COORD c) { return(sqrt( pow( (float) (c.x - (a->loc.x + 30)), 2) + pow( (float) (c.y - (a->loc.y+40)), 2 ) ) < 30); }

class Flower : public ModInterface {
 public:
  vector<Entity> flowers;
  Glib::RefPtr<Gdk::Pixbuf> * image; 
  Glib::RefPtr<Gdk::Pixbuf> scr;
  double scale[2];

  int win_height;
  int win_width;

  Flower();
  ~Flower();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
  void resize();

  void key(GdkEventKey * event);
  void expose(GdkEventExpose* event);
};

