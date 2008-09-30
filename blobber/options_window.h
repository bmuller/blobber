
namespace blobber {
  using namespace std;

  class OptionsWindow : public Gtk::Window {
  public:	
    OptionsWindow(Camarea *cam);
    void ok();
    void refreshAvailableModules();
  protected:
    Gtk::Label lblCamDevice;
    Gtk::VBox mainBox;
    Gtk::HBox camDevice;
    Gtk::ComboBoxEntryText cboCamDevice;
    Gtk::Button okButton;
    Gtk::Frame modsFrame;
    Gtk::VBox modsBox;
    Camarea *area;
    vector<string> availableModules;
  };
};
