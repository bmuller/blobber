#include <gtkmm.h>
#include "nerdtag.h"

class CamScreen {
public:
  FrameGrabber *fg;
  Frame * frame;
  int bpp;

  CamScreen(string device) {
    fg = new FrameGrabber(device);
    frame = fg->makeFrame();
    switch ( frame->format ) {
    case VIDEO_PALETTE_RGB24:
	bpp = 24;
	break;
    case VIDEO_PALETTE_RGB32:
      bpp = 32;
      break;
    default:
      cerr << "Unsupported frame format: " << frame->format << endl;
    }
    cout << "bpp is " << bpp << endl;
    //Glib::signal_idle().connect( sigc::mem_fun(*this, &CamScreen::on_idle) );
  };
  ~CamScreen() { delete frame; delete fg; };
  void update() { 
    fg->grabFrame(frame); 
  };
};


int main( int argc, char* argv[] ) {
  Gtk::Main kit(argc, argv);
  CamScreen cs("/dev/video0");

  Glib::RefPtr<Gdk::Pixbuf> pb = Gdk::Pixbuf::create_from_data((const guint8*) cs.frame->data, Gdk::COLORSPACE_RGB, false, 8, cs.frame->width, cs.frame->height, cs.fg->window.width * cs.fg->picture.depth);

  Gtk::Window w;
  Gtk::Image::Image i(pb);    
  Gtk::Frame frame;
  frame.add(i);
  w.add(frame);
  //window.add(i);
  w.show_all_children();
  Gtk::Main::run(w);
  return 0;
}
