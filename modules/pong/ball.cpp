#include "pongmod.h"
#include <iostream>

using namespace std;

Ball::Ball()  {
  pos.x = 0;
  pos.y = 0;
  x_dir = 1;
  y_dir = 1;
  speed = 5;
  diameter = 5;
  court_height = 250;
  court_width = 150;
}

void Ball::move()  {
  pos.x += x_dir*speed;
  if(pos.x >= court_width || pos.x <= 0)
    x_dir = -x_dir;

  pos.y += y_dir*speed;
  if(pos.y >= court_height || pos.y <= 0)
    y_dir = -y_dir;
}

void Ball::printInfo()  {
  cout << pos.x << " " << pos.y  <<endl;
}

void Ball::setCourtHeight(int h)  {
  court_height = h;
}

void Ball::setCourtWidth(int w)  {
  court_width = w;
}
