#include "pongmod.h"
#include <cstdlib>

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong") { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {

}

void Pong::update(Camarea &area, ProjectionWindow &pw) {
  pw.draw_circle(ball.pos, ball.diameter+2, BLACK);  
  ball.move();
  pw.draw_circle(ball.pos, ball.diameter, BLUE);
  ball.printInfo();


  /**  pw.draw_circle(pos, BALL_DIAMETER+2, BLACK);
  pos.x += x_dir*speed;
  pos.y += y_dir*speed;
  pw.draw_circle(pos, BALL_DIAMETER, BLUE);

  if(pos.x + BALL_DIAMETER > WIDTH)  {
    pos.x = WIDTH;
    x_dir = -1;
  }  
  else if(pos.x < 0)  {
    pos.x = 0;
    x_dir = 1;
  }
  
  if(pos.y + BALL_DIAMETER > HEIGHT)  {
    pos.y = HEIGHT;
    y_dir = -1;
  }
  else if(pos.y < 0)  {
    pos.y = 0;
    y_dir = 1;
  }

  cout << pos.x << "," << pos.y << endl;**/

}

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
