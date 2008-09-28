
namespace blobber {
  // basically, a moveable sprite
  class MovableObject {
  public:
    MovableObject(COORD middle, COLOR c);
    void move(COORD newcenter, ProjectionWindow &pw);
    virtual bool in_bounds(COORD c) = 0;
    virtual void paint(ProjectionWindow &pw) = 0;
    virtual void clear(ProjectionWindow &pw);
    bool selected;
  protected:
    COORD center;
    COLOR color;
  };
};
