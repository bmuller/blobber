
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
    Gtk::VBox modsBox;
    Camarea *area;
    std::map<string,string> availableModules;
  };
};
