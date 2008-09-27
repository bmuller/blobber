#include "pongmod.h"

Paddle::Paddle()  {
  pos.x = 20;
  pos.y = 100;
  width = 10;
  height = 50;
  speed = 5;
}

void Paddle::moveUp()  {
  if(pos.y-3 >= 0)
    pos.y -= speed;
}

void Paddle::moveDown()  {
  if(pos.y+height+3 <= court_height)
    pos.y += speed;
}

void Paddle::printInfo()  {
  
}

int Paddle::getLeft()  {
  return 0;
}

int Paddle::getRight()  {
  return 0;
}


int Paddle::getTop()  {
  return 0;
}

int Paddle::getBottom()  {
  return 0;
}

void Paddle::setCourtHeight(int h)  {
  court_height = h;
}

void Paddle::setCourtWidth(int w)  {
  court_width = w;
}

int Paddle::getCourtHeight()  {
  return court_height;
}

int Paddle::getCourtWidth()  {
  return court_width;
}
