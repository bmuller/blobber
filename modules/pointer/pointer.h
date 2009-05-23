#include "blobber.h"
#include <string>
#include <ctime>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#define POINTER_MODE_TIMED 0

using namespace blobber;

class Pointer : public ModInterface {
 public:
  Pointer();
  ~Pointer();

  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);
  void update_mode_timed(Camarea &area, ProjectionWindow &pw);
  void translate_coordinates( Camarea &area, COORD &camera, COORD &screen );
  void key(GdkEventKey * event);

 private:
  int     mod;   //current mode
  int     sch;   //X screen height 
  int     scw;   //X screen width
  COORD   loc;   //last location of pointer
  clock_t clk;   //clock at last location
  
};

