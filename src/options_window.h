
using namespace std;

class OptionsWindow : public Gtk::Window {
public:
  OptionsWindow();
  void exit();
protected:
  Gtk::Button exitButton;
  Gtk::Table table;
  Camarea area;
};
