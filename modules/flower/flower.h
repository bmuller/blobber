#include "blobber.h"
using namespace blobber;

class Entity {
 public:
  int state;
  COORD loc;
  Entity(COORD coord) { loc.copy(coord); state = 0; }
};

class Flower : public ModInterface {
 public:
  vector<Entity> flowers;
  Glib::RefPtr<Gdk::Pixbuf> * image; 
  bool clear;

  Flower();
  ~Flower();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

  void key(GdkEventKey * event);
  void expose(GdkEventExpose* event);
};

