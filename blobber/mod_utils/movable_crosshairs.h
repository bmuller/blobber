namespace blobber {
  class MovableCrosshairs : public MovableObject {
  public:
    MovableCrosshairs(int _size, COORD center, COLOR c);
    bool in_bounds(COORD c);
    void paint(ProjectionWindow &pw);
  private:
    int size;
  };

};
