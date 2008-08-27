using namespace std;

class Projection : public Gtk::Window {
 public:
  Projection();
 protected:
  Gtk::DrawingArea drawing_area;
};
