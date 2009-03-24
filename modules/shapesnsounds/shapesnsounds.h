#include "BeeThree.h"
#include "RtAudio.h"
#include "blobber.h"

using namespace blobber;

// The TickData structure holds all the class instances and data that 
// are shared by the various processing functions.                                                                                                                    
struct TickData {
  Instrmnt *instrument;
  StkFloat frequency;
  TickData() : instrument(0) {}
};


class ShapesNSounds : public ModInterface {
 public:
  ShapesNSounds();
  ~ShapesNSounds();
  void projection_window_exposed(ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  float min_frequency, max_frequency;
  int missing_counter;
  RtAudio dac;
  TickData data;
  RtAudio::StreamParameters parameters;
  unsigned int bufferFrames;
};
