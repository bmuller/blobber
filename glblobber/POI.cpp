#include"POI.h"

POI::POI(Frame * frm) :
         x(0),
         y(0),
         oldx(0),
         oldy(0),
         window(100),
         newRef(true),
         frame(frm)
{
  // FIXME - Should be loaded from config
  alignBlobColor[0] = 60;
  alignBlobColor[1] = 0;
  alignBlobColor[2] = 0;
  alignmentBox.xStart = 10;
  alignmentBox.yStart = 10;
  alignmentBox.xStop = frame->getWidth() - 10;
  alignmentBox.yStop = frame->getHeight() - 10;
}

POI::~POI() {}

void POI::update()
{
  // Grab a snapshot of the box in case
  // an alignment occurs during an update
  // --Not multithreading anyomore.... is this needed?
  int xo = alignmentBox.xStart;
  int xf = alignmentBox.xStop;
  int yo = alignmentBox.yStart;
  int yf = alignmentBox.yStop;

  for (int i = yo; i < yf; i++)
  { 
    unsigned char *reference = static_cast<unsigned char*>(frame->getCapture()) + 
                         3 * (i) * frame->getWidth() +
                         3 * xo; // Set starting point for line in buffer
    for (int j = xo; j < xf; j++)
    {
      unsigned char r = *(reference);
      unsigned char g = *(reference + 1);
      unsigned char b = *(reference + 2);
      if (alignBlobColor[0] - window < r && r < alignBlobColor[0] + window &&
          alignBlobColor[1] - window < g && g < alignBlobColor[1] + window &&
          alignBlobColor[2] - window < b && b < alignBlobColor[2] + window)
      {
        if (newRef)
        {
          oldx = translate(j, true);
          oldy = translate(i, false);
          newRef = false;
        }
        else
        {
          oldx = x;
          oldy = y;
        }
        x = translate(j, true);
        y = translate(i, false);
//	cout<<x<<":"<<y<<endl;
        return;
      }
      reference += 3; // Skip to next block of data
    }
  }
  oldx = x;
  oldy = y;
  newRef = true;
}

void POI::setColor(unsigned char *color)
{
  for (int i = 0; i < 3; i++)
    alignBlobColor[i] = color[i];
  cout<<(int)color[0]<<":"<<(int)color[1]<<":"<<(int)color[2]<<endl;
}

void POI::setBox(AlignmentBox box)
{
  alignmentBox.xStart = box.xStart;
  alignmentBox.xStop = box.xStop;
  alignmentBox.yStart = box.yStart;
  alignmentBox.yStop = box.yStop;

  // Reset POI
  x = oldx = y = oldy = 0;
}

AlignmentBox POI::getBox()
{
  return alignmentBox;
}

int POI::translate(int in, bool isX)
{
  float xRatio, yRatio, out;

/*cout<<alignmentBox.xStart<<" "
  <<alignmentBox.xStop<<" "
  <<alignmentBox.yStart<<" "
  <<alignmentBox.yStop<<" "<<in<<endl;*/

  
  xRatio = (float)frame->getWidth() / (float)(alignmentBox.xStop - alignmentBox.xStart);
  yRatio = (float)frame->getHeight() / (float)(alignmentBox.yStop - alignmentBox.yStart);

//cout<<xRatio<<" "<<yRatio<<endl;

  if (isX)
    out = (in - alignmentBox.xStart) * xRatio;
  else
    out = (in - alignmentBox.yStart) * yRatio;

  return static_cast<int>(out);
}
