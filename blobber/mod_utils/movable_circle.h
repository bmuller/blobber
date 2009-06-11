#ifndef MOVABLE_CIRCLE_H
#define MOVABLE_CIRCLE_H

#include "movable_object.h"

namespace blobber {

  class MovableCircle : public MovableObject {
  public:
    MovableCircle(PROPORTION _prop, COORD center, COLOR c);
    bool in_bounds(COORD c);
    void paint(ProjectionWindow &pw);
    void clear(ProjectionWindow &pw);
  private:
    int radius;
    PROPORTION prop;
  };

};

#endif  /* MOVABLE_CIRCLE_H */
