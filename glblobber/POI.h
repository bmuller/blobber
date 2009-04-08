#ifndef POI_H
#define POI_H

#include"Frame.h"
#include"utility.h"

class POI
{
  public:
    POI(Frame * frm);
    ~POI();
    void update();
    void setColor(unsigned char color[3]);
    void setBox(AlignmentBox box);
    AlignmentBox getBox();
    int translate(int in, bool isX);
    int x;
    int y;
    int oldx;
    int oldy;

  protected:
    int window;
    bool newRef;
    Frame * frame;
    unsigned char alignBlobColor[3];
    AlignmentBox alignmentBox;
}; 

#endif
