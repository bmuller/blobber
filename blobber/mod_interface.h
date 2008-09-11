
namespace blobber {

class ModInterface {
 public:
  string name;
  ModInterface(string n);
  virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
  virtual void clear() {};
  virtual void init(Camarea &area, ProjectionWindow &pw) {};
  virtual void projection_window_exposed(ProjectionWindow &pw) {};
  static ModInterface * load_module(string modname);
 protected:
  void register_poi_criteria(Camarea &area, CRANGE crange);
  void get_poi(Camarea &area, vector<PIXEL> &modpoi);
};

};


