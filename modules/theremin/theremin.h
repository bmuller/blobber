#include "BeeThree.h"
#include "RtAudio.h"
#include "blobber.h"

using namespace blobber;

// The TickData structure holds all the class instances and data that                                                                                                 
// are shared by the various processing functions.                                                                                                                    
struct TickData {
  Instrmnt *instrument;
  StkFloat frequency;
  StkFloat scaler;
  long counter;
  bool done;

  // Default constructor.                                                                                                                                             
  TickData()
  : instrument(0), scaler(1.0), counter(0), done( false ) {}
};



class Theremin : public ModInterface {
 public:
  Theremin();
  ~Theremin();
  void projection_window_exposed(ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
  void note_on(double frequency);
  void note_off();
 protected:
  RtAudio dac;
  TickData data;
  bool note_is_on;
  RtAudio::StreamParameters parameters;
  unsigned int bufferFrames;
};
