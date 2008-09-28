#include "blobber.h"

using namespace blobber;
using namespace std;

class MovableShapes : public ModInterface {
 public:
  MovableShapes();
  ~MovableShapes();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
  void projection_window_exposed(ProjectionWindow &pw);
 private:
  vector<MovableObject*> shapes;
  int missing_point_count;
};
