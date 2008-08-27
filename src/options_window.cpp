#include "nerdtag.h"

using namespace std;

class OptionsWindow : public Gtk::Window {
public:
  OptionsWindow() { 
    Camarea area("/dev/video0");
    resize(area.width, area.height+50);
    add(area);
    exitButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::exit) );
    show_all_children();
  };

  void exit() {
    cout << "exit!\n";
  };
protected:
  Gtk::Button exitButton;
};

int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  OptionsWindow win;
  Gtk::Main::run(win);
  return 0;
}
