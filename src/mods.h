class ModInterface {
 public:
  string name;
  ModInterface(string n);
  virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
  virtual void clear() {};
  virtual void init_poi(Camarea &area) {};
 protected:
  void register_poi_criteria(Camarea &area, CRANGE crange);
  void get_poi(Camarea &area, vector<COORD> &modpoi);
};

class LaserTag : public ModInterface {
 public:
  LaserTag();
  void update(Camarea &area, ProjectionWindow &pw);
  void init_poi(Camarea &area);
 protected:
  COORD lastpoint;
};


class GreenScreen : public ModInterface {
 public:
  GreenScreen(const std::string filename);
  ~GreenScreen();
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  Gtk::Image image;
  unsigned char green_red_diff;
  unsigned char green_blue_diff;
};
