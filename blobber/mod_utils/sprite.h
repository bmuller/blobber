namespace blobber {

  class Sprite : public MovableObject {
  public:
    Sprite(string spriteimagelocation, COORD center, COLOR background = BLACK);
    bool in_bounds(COORD c);
    void paint(ProjectionWindow &pw);
    void clear(ProjectionWindow &pw);
    void get_bounds(BOUNDS &bounds);
    void scale(PROPORTION p, ProjectionWindow &pw);
    DIMENSION dimensions, half_dimensions;
  private:
    Glib::RefPtr<Gdk::Pixbuf> img;
    Glib::RefPtr<Gdk::Pixbuf> blkimg;
    string filelocation;
  };

};
