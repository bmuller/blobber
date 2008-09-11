
namespace blobber {

using namespace std;

class CameraWindow : public Gtk::Window {
public:
  Camarea area;
  CameraWindow(string device);
  void exit();
  void about();
  void options();
protected:
  Gtk::Button exitButton;
  Gtk::Table table;
  Gtk::VBox m_Box;
  Glib::RefPtr<Gtk::UIManager> m_UIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_ActionGroup;

};

};
