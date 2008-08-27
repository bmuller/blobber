#include "blobber.h"

int main(int argc, char** argv) {
  Gtk::Main kit(argc, argv);
  OptionsWindow win;
  Gtk::Main::run(win);
  return 0;
}
