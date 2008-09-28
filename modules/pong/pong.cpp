#include "pongmod.h"
#include <cstdlib>

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong") { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {
  right_paddle = Paddle(ball.getCourtWidth()-30, 100, 10, 50, 5);
  left_paddle = Paddle(20, 100, 10, 50, 5);

  //used to draw court -- temporary
  origin.x = 1;
  origin.y = 1;

  //the ball is currently specifying the court, but we'll fix that soon
  left_paddle.setCourtHeight(ball.getCourtHeight());
  left_paddle.setCourtWidth(ball.getCourtWidth());
  right_paddle.setCourtHeight(ball.getCourtHeight());
  right_paddle.setCourtWidth(ball.getCourtWidth());

  paddle_moving_up = 1;

}

void Pong::update(Camarea &area, ProjectionWindow &pw) {
  pw.draw_box(origin ,ball.getCourtWidth(),ball.getCourtHeight(), BLUE);  //draw the court (TEMPORARY)

  /** Clear, move, and draw ball **/
  pw.draw_circle(ball.pos, ball.radius+2, BLACK);  //clear the circle
  ball.move();
  pw.draw_circle(ball.pos, ball.radius, WHITE);  //draw the new circle
  /** end clear, move and draw ball **/

  /** This is could be WAY better -- clearing out the previous paddles **/
  COORD temp;
  temp.x = left_paddle.pos.x-1;
  temp.y = left_paddle.pos.y-1;
  pw.draw_box(temp, left_paddle.width+2, left_paddle.height+2, BLACK, true);

  temp.x = right_paddle.pos.x-1;
  temp.y = right_paddle.pos.y-1;
  pw.draw_box(temp, right_paddle.width+2, right_paddle.height+2, BLACK, true);
  /** End "clearing paddle" part **/

  /** Testing the paddle out without interaction **/
  if(rand()%40 == 5)
    paddle_moving_up = -paddle_moving_up;
  if(paddle_moving_up == 1) {
    left_paddle.moveUp();
    right_paddle.moveDown();
  }
  else {
    left_paddle.moveDown();
    right_paddle.moveUp();
  }
  /** End testing paddle **/

  /** Drawing Paddles **/
  pw.draw_box(left_paddle.pos, left_paddle.width, left_paddle.height, WHITE, true);
  pw.draw_box(right_paddle.pos, right_paddle.width, right_paddle.height, WHITE, true);
  /** End Drawing Paddles **/

  //ball.printInfo();

  ball.processCollision(&left_paddle);
  ball.processCollision(&right_paddle);
}

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
