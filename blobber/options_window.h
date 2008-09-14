
namespace blobber {
  using namespace std;

  class OptionsWindow : public Gtk::Window {
  public:
    OptionsWindow();
    void ok();
  protected:
    Gtk::Label lblCamDevice;
    Gtk::VBox mainBox;
    Gtk::HBox camDevice;
    Gtk::ComboBoxEntryText cboCamDevice;
//    Gtk::ButtonBox modules;
    Gtk::Button okButton;
  };
};
