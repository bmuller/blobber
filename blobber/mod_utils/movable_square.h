#include "movable_object.h"

namespace blobber {

  class MovableSquare : public MovableObject {
  public:
    MovableSquare(int _size, COORD center, COLOR c);
    bool in_bounds(COORD c);
    void paint(ProjectionWindow &pw);
    void clear(ProjectionWindow &pw);
  private:
    int size;
  };

};
