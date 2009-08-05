using namespace blobber;

class Pong : public ModInterface {
 public:
  Pong();
  COORD origin;
  DIMENSION court;
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

 private:
  Ball ball;
  Paddle left_paddle;
  Paddle right_paddle;

  int scale(int pw, int real_val, int coord);
  COORD scale(DIMENSION pw, DIMENSION real_val, COORD coord);
 
  //temp variables to be removed later
  short int paddle_moving_up;
};


