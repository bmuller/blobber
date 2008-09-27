#include "pongmod.h"
#include <cstdlib>

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong") { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {
  origin.x = 1;
  origin.y = 1;

  //the ball is currently specifying the court, but we'll fix that soon
  left_paddle.setCourtHeight(ball.getCourtHeight());
  left_paddle.setCourtWidth(ball.getCourtWidth());

  paddle_moving_up = 1;

}

void Pong::update(Camarea &area, ProjectionWindow &pw) {
  if(rand()%40 == 5)
    paddle_moving_up = -paddle_moving_up;

  pw.draw_box(origin ,ball.getCourtWidth(),ball.getCourtHeight(), BLUE);
  pw.draw_circle(ball.pos, ball.radius+2, BLACK);  //clear the circle
  ball.move();
  pw.draw_circle(ball.pos, ball.radius, WHITE);  //draw the new circle

  /** This is hacked together -- don't know a better way to do it off the top of my head **/
  COORD temp;
  temp.x = left_paddle.pos.x-1;
  temp.y = left_paddle.pos.y-1;
  pw.draw_box(temp, left_paddle.width+2, left_paddle.height+2, BLACK, true);
  if(paddle_moving_up == 1)
    left_paddle.moveUp();
  else
    left_paddle.moveDown();
  pw.draw_box(left_paddle.pos, left_paddle.width, left_paddle.height, WHITE, true);

  //ball.printInfo();
}

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
