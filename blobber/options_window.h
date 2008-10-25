
namespace blobber {
  using namespace std;

  class OptionsWindow : public Gtk::Window {
  public:	
    OptionsWindow(Camarea *cam);
    void ok();
  protected:
    Gtk::Label lblCamDevice;
    Gtk::VBox mainBox;
    Gtk::HBox camDevice;
    Gtk::ComboBoxEntryText cboCamDevice;
    Gtk::Button okButton;
    Gtk::Frame modsFrame;
    Gtk::Frame brightnessFrame;
    Gtk::Frame contrastFrame;
    Gtk::Frame poiCriteriaWindowFrame;
    Gtk::VBox modsBox;
    Gtk::HScale brightnessScale;
    Gtk::HScale contrastScale;
    // this is for the default_criteria_window config option
    Gtk::HScale poiCriteriaWindow;
    Camarea *area;
    // module description by module name
    std::map<string, string> availableModules;
    // module check button pointer by module name
    std::map<string, Gtk::CheckButton*> modButtons;
    // module file name by module name;
    std::map<string, string> modFiles;
    Configuration *config;
  };
};
