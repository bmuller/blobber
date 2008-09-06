class ModInterface {
 public:
  string name;
  ModInterface(string n);
  virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
  virtual void clear() {};
  virtual void init(Camarea &area, ProjectionWindow &pw) {};
 protected:
  void register_poi_criteria(Camarea &area, CRANGE crange);
  void get_poi(Camarea &area, vector<COORD> &modpoi);
};


class ProjectionOptions : public ModInterface {
 public:
  ProjectionOptions() : ModInterface("LaserTag") {};
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
};


class LaserTag : public ModInterface {
 public:
  LaserTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init(Camarea &area, ProjectionWindow &pw);
 protected:
  COORD lastpoint;
  unsigned int missing_counter;
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
