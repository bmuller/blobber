#include "pongmod.h"

Paddle::Paddle(int posx, int posy, int w, int h, int s, DIMENSION c) {
  pos.x = posx;
  pos.y = posy;
  width = w;
  height = h;
  speed = s;
  court_height = c.height;
  court_width = c.width;
}

Paddle::Paddle()  {
  pos.x = 20;
  pos.y = 100;
  width = 10;
  height = 50;
  speed = 5;
}

void Paddle::moveUp()  {
  if(pos.y-3 >= 0) {
    pos.y -= speed;
  }
}

void Paddle::moveDown()  {
  if(pos.y+height+3 <= court_height)
    pos.y += speed;
}

void Paddle::printInfo()  {
  
}

int Paddle::getLeft()  {
  return pos.x;
}

int Paddle::getRight()  {
  return pos.x+width;
}


int Paddle::getTop()  {
  return pos.y;
}

int Paddle::getBottom()  {
  return pos.y+height;
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
