
using namespace std;

class CameraWindow : public Gtk::Window {
public:
  Camarea area;
  CameraWindow(string device);
  void exit();
protected:
  Gtk::Button exitButton;
  Gtk::Table table;
};
