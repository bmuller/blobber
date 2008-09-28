#include "pongmod.h"
#include <iostream>

using namespace std;

Ball::Ball()  {
  pos.x = 6;
  pos.y = 6;
  x_dir = 1;
  y_dir = 1;
  speed = 5;
  radius = 5;
  court_height = 150;
  court_width = 250;
}



void Ball::move()  {
  pos.x += x_dir*speed;
  if(getRight() >= court_width || getLeft() <= 0)  {
    x_dir = -x_dir;
    pos.x = (getLeft() <= 0)?0+(radius+1):court_width-(radius+1);
  }

  pos.y += y_dir*speed;
  if(getBottom() >= court_height || getTop() <= 0)  {
    y_dir = -y_dir;
    pos.y = (getTop() <= 0)?0+(radius+1):court_height-(radius+1);
  }
}

void Ball::printInfo()  {
  cout << "x:" << pos.x << ", Left:" << getLeft() << ", y:" << pos.y  << endl;
}

int Ball::getLeft()  {
  return pos.x - (radius+1);
}

int Ball::getRight()  {
  return pos.x + (radius+1);
}

int Ball::getTop()  {
  return pos.y - (radius+1);
}

int Ball::getBottom()  {
  return pos.y + (radius+1);
}

void Ball::setCourtHeight(int h)  {
  court_height = h;
}

void Ball::setCourtWidth(int w)  {
  court_width = w;
}

int Ball::getCourtHeight()  {
  return court_height;
}

int Ball::getCourtWidth()  {
  return court_width;
}


void Ball::processCollision(Paddle* p) {
  //do they have any overlap in the y direction?
  if(getBottom() >= p->getTop() && getTop() <= p->getBottom()) {
    if(getLeft() <= p->getRight() && getRight() >= p->getLeft())
      x_dir = -x_dir;
  }
}
