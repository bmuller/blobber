using namespace blobber;

class Ball  {
 public:
  Ball();

  void move();
  void setCourtHeight(int h);
  void setCourtWidth(int w);
  void printInfo();
  int getLeft();
  int getTop();
  int getRight();
  int getBottom();
  int getCourtHeight();
  int getCourtWidth();
  void processCollision(Paddle* p);

  /**I probably should make these private eventually**/
  COORD pos;
  short int x_dir;
  short int y_dir;
  short int speed;
  short int radius;
  int court_height;
  int court_width;

};

