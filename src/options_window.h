
using namespace std;

class OptionsWindow : public Gtk::Window {
public:
  Camarea area;
  OptionsWindow(string device);
  void exit();
protected:
  Gtk::Button exitButton;
  Gtk::Table table;
};
