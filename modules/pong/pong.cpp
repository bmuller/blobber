#include "pongmod.h"
#include <cstdlib>
#include <iostream>

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong") { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {

  register_poi(area, 2);  //????

  court.width = 400;
  court.height = 150;

  ball = Ball(6,6,5,10,court);
  right_paddle = Paddle(court.width-30, 100, 10, 50, 5, court);
  left_paddle = Paddle(20, 100, 10, 50, 5, court);

  debug("Greetings from the Pong Module!");

  //used to draw court -- temporary
  origin.x = 0;
  origin.y = 0;

  paddle_moving_up = 1; //TEMPORARY, JUST TO GET STUFF HAPPENING
}

void Pong::update(Camarea &area, ProjectionWindow &pw) {
  vector<PIXEL> poi;
  get_poi(area, poi);

  if(poi.size() == 0) {
    //debug("no poi");
  }
  else {
    //debug("poi found!");
    for(int i = 0; i < poi.size(); ++i) {

    }
  }

  //cout << "num poi: " << poi.size() << endl;

  /** move stuff **/
  ball.move();

  //here is where we'll need to move the left paddle based on the poi

  //TEMPORARY MOVEMENT OF PADDLES, JUST TO GET STUFF HAPPENING
  if(rand()%10 == 5)
    paddle_moving_up = -paddle_moving_up;
  if(paddle_moving_up == 1) {
    left_paddle.moveUp();
    right_paddle.moveDown();
  }
  else {
    left_paddle.moveDown();
    right_paddle.moveUp();
  }
  //END TEMPORARY MOVEMENT OF PADDLES
  /** end move stuff **/

  /** Check for stuff **/
  ball.processCollision(&left_paddle);
  ball.processCollision(&right_paddle);
  /** end check for stuff **/

  /** Clear stuff **/
  pw.clear();
  /** end clear stuff **/

  /** draw stuff **/
  pw.draw_box_absolute(origin,
		       pw.dimensions.width,
		       pw.dimensions.height,
		       BLUE);  //draw the court (TEMPORARY)

  pw.draw_circle_absolute(scale(pw.dimensions, court, ball.pos),
			  scale(pw.dimensions.width, court.width, ball.radius),
			  WHITE,
			  true);

  pw.draw_box_absolute(scale(pw.dimensions, court, left_paddle.pos),
		       scale(pw.dimensions.width, court.width, left_paddle.width),
		       scale(pw.dimensions.height, court.height, left_paddle.height),
		       WHITE,
		       true);

  pw.draw_box_absolute(scale(pw.dimensions, court, right_paddle.pos),
		       scale(pw.dimensions.width, court.width, right_paddle.width),
		       scale(pw.dimensions.height, court.height, right_paddle.height),
		       WHITE,
		       true);
  /** end draw stuff **/
}

int Pong::scale(int pw, int real_val, int coord) {
  float scale_factor = (float) pw/real_val;
  return (int) coord*scale_factor;
}

COORD Pong::scale(DIMENSION pw, DIMENSION real_val, COORD coord) {
  COORD return_val;
  return_val.x = scale(pw.width, real_val.width, coord.x);
  return_val.y = scale(pw.height, real_val.height, coord.y);
  return return_val;
}

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
