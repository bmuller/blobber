#ifndef MOVABLE_BOX_H
#define MOVABLE_BOX_H

#include "movable_object.h"

namespace blobber {

  class MovableBox : public MovableObject {
  public:
    MovableBox(PROPORTION _prop, COORD center, COLOR c);
    bool in_bounds(COORD c);
    void paint(ProjectionWindow &pw);
    void clear(ProjectionWindow &pw);
  private:
    PROPORTION prop;
    int width, height;
  };

};

#endif  /* MOVABLE_BOX_H */
