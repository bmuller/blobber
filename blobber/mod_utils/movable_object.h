
namespace blobber {
  // basically, a moveable sprite
  class MovableObject {
  public:
    MovableObject(COORD middle, COLOR c);
    // @param first_clear Specify whether or not old position should be cleared
    void move(COORD newcenter, ProjectionWindow &pw, bool first_clear=true);
    virtual bool in_bounds(COORD c) = 0;
    virtual void paint(ProjectionWindow &pw) = 0;
    virtual void clear(ProjectionWindow &pw);
    virtual void get_bounds(BOUNDS &bounds);
    bool selected;
    COORD center;
    COLOR color;
  };
};
