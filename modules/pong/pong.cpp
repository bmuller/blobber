#include "pongmod.h"
#include <cstdlib>

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong") { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {
  right_paddle = Paddle(ball.getCourtWidth()-30, 100, 10, 50, 5);
  left_paddle = Paddle(20, 100, 10, 50, 5);

  //used to draw court -- temporary
  origin.x = 0;
  origin.y = 0;

  //the ball is currently specifying the court, but we'll fix that soon
  left_paddle.setCourtHeight(ball.getCourtHeight());
  left_paddle.setCourtWidth(ball.getCourtWidth());
  right_paddle.setCourtHeight(ball.getCourtHeight());
  right_paddle.setCourtWidth(ball.getCourtWidth());

  paddle_moving_up = 1;

}

void Pong::update(Camarea &area, ProjectionWindow &pw) {
  /** move stuff **/
  ball.move();
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
  /** end move stuff **/

  /** Check for stuff **/
  ball.processCollision(&left_paddle);
  ball.processCollision(&right_paddle);
  /** end check for stuff **/

  /** Clear stuff **/
  pw.draw_box(origin, pw.vprojbounds.bottom, pw.vprojbounds.right, BLACK, true);
  /** end clear stuff **/

  /** draw stuff **/
  pw.draw_box(origin ,ball.getCourtWidth(),ball.getCourtHeight(), BLUE);  //draw the court (TEMPORARY)
  pw.draw_circle(ball.pos, ball.radius, WHITE, true);  //draw the new circle
  pw.draw_box(left_paddle.pos, left_paddle.width, left_paddle.height, WHITE, true);
  pw.draw_box(right_paddle.pos, right_paddle.width, right_paddle.height, WHITE, true);
  /** end draw stuff **/
}

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
