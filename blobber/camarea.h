
namespace blobber {

using namespace std;

 class Camarea : public Gtk::DrawingArea {
 private:
   string device;
   FrameGrabber *fg;
   Cairo::RefPtr< Cairo::ImageSurface > surface;
   Cairo::RefPtr< Cairo::ImageSurface > noCam;
   COORD mouse_click;
   bool mouse_clicked;
   // points of interest for the modules
   std::map<string, vector<PIXEL> > poi;
   // modules register criteria for defining a poi
   std::map<string, vector<CRANGE> > poi_criteria;
   // modules register maximum number of poi
   std::map<string, int > maxPoints;
   void update_poi(unsigned char *data, string modname, CRANGE &criteria);

 public:
   bool hascam, manual_align;
   int height, width;
   Frame *frame;
   BOUNDS bounds;
   
   Camarea(string _device);
   ~Camarea();
   bool on_motion_notify_event (GdkEventMotion* event);
   bool on_button_press_event(GdkEventButton* event);
   bool on_button_release_event(GdkEventButton* event);
   void update_screen();
   void update_frame();
   void set_device(string _device);
   void set_bounds(BOUNDS &b);
   void draw_bounds(BOUNDS &b);
   void register_poi_criteria(string modname, CRANGE range, int maxPoints);
   void get_poi(string modname, vector<PIXEL> &poi);
   Cairo::RefPtr< Cairo::ImageSurface > getSurface();
 };

};
