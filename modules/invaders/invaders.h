#include "blobber.h"
using namespace blobber;

#define NORMAL 0x00
#define PAUSED 0x01
#define RESET  0x02
#define WAIT   0x03
#define SQRT_18 4.242640687

typedef struct {
  int speed;
  int state;
  COORD loc;
} Entity;

bool in_bounds(Entity * a, COORD c) { return(sqrt( pow( (float) (c.x - (a->loc.x + 30)), 2) + pow( (float) (c.y - (a->loc.y+40)), 2 ) ) < 30); }

class Invaders : public ModInterface {
 public:
  char paused;
  int level;
  int score;
  COORD cross;
  Entity * aliens;
  int aliens_size;
  vector<Entity> bams;
  //clock_t timer;
  Glib::RefPtr<Gdk::Pixbuf> * bam;
  Glib::RefPtr<Gdk::Pixbuf> * gho;
  Glib::RefPtr<Gdk::Pixbuf> scr;
  Glib::RefPtr<Gdk::Pixbuf> pau;
  Glib::RefPtr<Gdk::Pixbuf> cro;
  
  double scale[2];
  double trans;

  int win_height;
  int win_width;

  Invaders();
  ~Invaders();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
  void resize();

  void key(GdkEventKey * event);
  void expose(GdkEventExpose* event);
};

