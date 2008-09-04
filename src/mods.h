class ModInterface {
 public:
  string name;
  ModInterface(string n) : name(n) { debug("Module \"" + n + "\" just created"); };
  virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
  virtual void clear() {};
};

class LaserTag : public ModInterface {
 public:
  LaserTag() : ModInterface("LaserTag") { lastpoint.x = 0; lastpoint.y = 0; };
  void update(Camarea &area, ProjectionWindow &pw);
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
