class ModInterface {
 public:
  string name;
  ModInterface(string n) : name(n) { debug("Module \"" + n + "\" just created"); };
  virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
};

class LaserTag : public ModInterface {
 public:
  LaserTag() : ModInterface("LaserTag") {};
  void update(Camarea &area, ProjectionWindow &pw);
 protected:
  vector<COORD> points;
};
