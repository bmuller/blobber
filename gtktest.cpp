#include <gtkmm.h>
#include "nerdtag.h"

class CamScreen {
public:
  FrameGrabber *fg;
  Frame * frame;
  int bpp;
  Glib::RefPtr<Gdk::Pixbuf> pb;

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
    cout << "frame wxh: " << frame->width << " " frame->height << endl;
    //Glib::signal_idle().connect( sigc::mem_fun(*this, &CamScreen::on_idle) );
    //pb = Gdk::Pixbuf::create_from_data((const guint8 *) frame->data, Gdk::COLORSPACE_RGB, false, 8, fg->window.width, fg->window.height, fg->window.width * fg->picture.depth);
    pb = Gdk::Pixbuf::create_from_data((const guint8 *) frame->data, Gdk::COLORSPACE_RGB, false, 8, frame->width, frame->height, frame->width * 4);
    //pb = Gdk::Pixbuf::create_from_file("/home/bmuller/.gajim/avatars/hacker@klever.net-.png");
  };
  ~CamScreen() { delete frame; delete fg; };
  void update() { 
    fg->grabFrame(frame); 
  };
};


int main( int argc, char* argv[] ) {
  Gtk::Main kit(argc, argv);
  CamScreen * cs = new CamScreen("/dev/video0");
  
  Gtk::Window w;
  Gtk::Image::Image i(cs->pb);    
  i.set(cs->pb);
  Gtk::Frame frame;
  frame.add(i);
  w.add(frame);
  w.show_all_children();
  Gtk::Main::run(w);
  return 0;
}
