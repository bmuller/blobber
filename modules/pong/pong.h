using namespace blobber;

class Pong : public ModInterface {
 public:
  Pong();
  COORD origin;
  void init(Camarea &area, ProjectionWindow &pw);
  void update(Camarea &area, ProjectionWindow &pw);

 private:
  Ball ball;
  Paddle left_paddle;
  Paddle right_paddle;

  //temp variables to be removed later
  short int paddle_moving_up;
};


