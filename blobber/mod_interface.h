
namespace blobber {

  class ModInterface {
  public:
    string name;
    ModInterface(string n);
    virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
    virtual void clear() {};
    virtual void init(Camarea &area, ProjectionWindow &pw, Configuration *config) {};
    virtual void projection_window_exposed(ProjectionWindow &pw) {};
    static ModInterface * load_module(string modname);
    static void get_available_modules(vector<string> &mods);
    // called right before init
    inline void set_config(Configuration *c) { config = c; };
    void config_set(string key, string value);
    void config_set(string key, vector<string> values);
    void config_get(string key, string &value, string vdefault);
    void config_get(string key, vector<string> &values);

  protected:
    Configuration *config;
    void register_poi_criteria(Camarea &area, CRANGE crange);
    void get_poi(Camarea &area, vector<PIXEL> &modpoi);
  };

};


