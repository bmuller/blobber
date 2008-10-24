namespace blobber {

  class Sprite : public MovableObject {
  public:
    Sprite(string spriteimagelocation, COORD center, COLOR background = BLACK);
    bool in_bounds(COORD c);
    void paint(ProjectionWindow &pw);
    void clear(ProjectionWindow &pw);
  private:
    Glib::RefPtr<Gdk::Pixbuf> img;
    Glib::RefPtr<Gdk::Pixbuf> blkimg;
    int width, height, half_width, half_height;
  };

};
