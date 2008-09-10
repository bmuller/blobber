#include "blobber.h"

class ProjectionOptions : public ModInterface {
 public:
  ProjectionOptions();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
  void projection_window_exposed(ProjectionWindow &pw);
 private:
  // the number of times the colors have been showing
  int colors_showing_count;
};

