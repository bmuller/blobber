class ModInterface {
 public:
  string name;
  ModInterface(string n);
  virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
  virtual void clear() {};
  virtual void init(Camarea &area, ProjectionWindow &pw) {};
  virtual void projection_window_exposed(ProjectionWindow &pw) {};
 protected:
  void register_poi_criteria(Camarea &area, CRANGE crange);
  void get_poi(Camarea &area, vector<PIXEL> &modpoi);
};
/*
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


class LaserTag : public ModInterface {
 public:
  LaserTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  COORD lastpoint;
  unsigned int missing_counter;
  vector<COORD> points;
};

class MultiColoredTag : public ModInterface {
 public:
  MultiColoredTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  COORD lastpoint;
  unsigned int missing_counter;
  vector<COORD> points;
};

class MultiLaserTag : public ModInterface {
 public:
  MultiLaserTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  vector<PIXEL> lastpoints;
};

class GreenScreen : public ModInterface {
 public:
  GreenScreen(const std::string filename);
  ~GreenScreen();
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  Glib::RefPtr<Gdk::Pixbuf> image;
  unsigned char green_red_diff;
  unsigned char green_blue_diff;
};
*/
