using namespace blobber;

class Paddle  {
 public:
  Paddle();
  Paddle(int posx, int posy, int w, int h, int s, DIMENSION c);

  void moveUp();
  void moveDown();
  void setCourtHeight(int h);
  void setCourtWidth(int w);
  void printInfo();
  int getLeft();
  int getTop();
  int getRight();
  int getBottom();
  int getCourtHeight();
  int getCourtWidth();

  /**I probably should make these private eventually**/
  COORD pos;
  short int speed;
  short int width;
  short int height;
  int court_height;
  int court_width;

};
