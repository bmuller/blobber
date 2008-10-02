namespace blobber {
  using namespace std;

  class Configuration {
  private:
    Glib::KeyFile config;
    string filename, directory;
    Configuration();
    ~Configuration();
  public:
    static Configuration * _inst;
    static Configuration * get_config();
    static void delete_config();
    void save();

    // get/set values for main program
    void set(string key, string value, string groupname="blobber");
    void set(string key, vector<string> values, string groupname="blobber");   
    bool is_set(string key, string groupname="blobber");
    void get(string key, string &value, string vdefault, string groupname="blobber");
    void get(string key, vector<string> &values, string groupname="blobber");

    void get_keys(vector<string> &keys, string groupname);

    // get/set values for modules
    void module_set(string key, string value, string modname);
    void module_set(string key, vector<string> values, string modname);   
    void module_get(string key, string &value, string vdefault, string modname);
    void module_get(string key, vector<string> &values, string modname);
  };
};
