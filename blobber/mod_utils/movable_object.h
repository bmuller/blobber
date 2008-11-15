
namespace blobber {
  // basically, a moveable sprite
  class MovableObject {
  public:
    MovableObject(COORD middle, COLOR c, COLOR bg=BLACK);
    // @param first_clear Specify whether or not old position should be cleared
    void move(COORD newcenter, ProjectionWindow &pw, bool first_clear=true);
    // move distance toward newcenter rather than just moving to newcenter
    void move(double distance, COORD newcenter, ProjectionWindow &pw, bool first_clear=true);

    /** 
     * Same as in_bounds(COORD c) but translates coordinates first.
     * 
     * @param c Coordinate to translate, then check if in bounds of object
     * @param pw Projection window
     * 
     * @return True if in bounds of object, false otherwise
     */
    virtual bool in_bounds(COORD c, ProjectionWindow &pw);
    /** 
     * The Following accepts a coordinate in the projection window and returns true if that coordinate
     * is in the bounds of the object.  No translation of the point occurs.
     * 
     * @param c The Coordinate to test
     * 
     * @return True if coordinate is in bounds
     */
    virtual bool in_bounds(COORD c) = 0;
    virtual void paint(ProjectionWindow &pw) = 0;
    virtual void clear(ProjectionWindow &pw);
    virtual void get_bounds(BOUNDS &bounds);
    bool selected;
    COORD center;
    COLOR color, background;
  };
};
