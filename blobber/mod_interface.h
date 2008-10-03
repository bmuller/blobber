
namespace blobber {

  class ModInterface {
  public:
    string name, description;
    ModInterface(string n, string d="No description");
    virtual void update(Camarea &area, ProjectionWindow &pw) = 0;
    virtual void clear() {};
    virtual void init(Camarea &area, ProjectionWindow &pw) {};
    virtual void projection_window_exposed(ProjectionWindow &pw) {};
    static ModInterface * load_module(string modname, bool make_resident=true);
    static void get_available_modules(map<string, string> &mods, map<string, string> &files);
    // called right before init
    inline void set_config(Configuration *c) { config = c; };
    void config_set(string key, string value);
    void config_set(string key, vector<string> values);
    void config_get(string key, string &value, string vdefault);
    void config_get(string key, vector<string> &values);

    bool operator==(const ModInterface& other);
    bool operator!=(const ModInterface& other);

    // this method is useful when you want to get the config value - and in the
    // case that it's not set and you get the default to set it to that value
    void config_get_set(string key, string &value, string vdefault);
  protected:
    Configuration *config;
    void register_poi_criteria(Camarea &area, CRANGE crange, int maxPoi = -1);
    void get_poi(Camarea &area, vector<PIXEL> &modpoi);
  };

};


