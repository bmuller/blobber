namespace blobber {
  using namespace std;

  class Configuration {
  private:
    Glib::KeyFile config;
    string filename, directory;
  public:
    Configuration();
    ~Configuration();
    void save();

    // get/set values for main program
    void set(string key, string value, string groupname="blobber");
    void set(string key, vector<string> values, string groupname="blobber");   
    void get(string key, string &value, string groupname="blobber");
    void get(string key, vector<string> &values, string groupname="blobber");

    // get/set values for modules
    void module_set(string key, string value, string modname);
    void module_set(string key, vector<string> values, string modname);   
    void module_get(string key, string &value, string modname);
    void module_get(string key, vector<string> &values, string modname);
  };
};
