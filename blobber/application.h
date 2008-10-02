namespace blobber {

  class Application {
  private:
    CameraWindow win;
    ProjectionWindow proj;
    BOUNDS visible_bounds;
    vector<ModInterface*> mods;
    bool aligned;
    Configuration *config;

    void load_modules();
    Application();
    ~Application();
  public:
    static Application * _inst;
    static Application * get_app();
    static void delete_app();
    void reload_config();
    bool module_loaded(ModInterface *mi);
    void add_mod(ModInterface * mi);
    bool on_idle();
    void run();
    void align();
  };

};
