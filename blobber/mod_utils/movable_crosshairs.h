#ifndef MOVABLE_CROSSHAIRS_H
#define MOVABLE_CROSSHAIRS_H

#include "movable_object.h"

namespace blobber {
  class MovableCrosshairs : public MovableObject {
  public:
    MovableCrosshairs(int _size, COORD center, COLOR c, COLOR bg=BLACK);
    bool in_bounds(COORD c);
    void paint(ProjectionWindow &pw);
    void clear(ProjectionWindow &pw);
  private:
    int size;
  };

};

#endif /* MOVABLE_CROSSHAIRS_H */
