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

/** I'm really sorry about this, what a freakin' mess **/
void Ball::processCollision(Paddle* p) {
  if(getBottom() >= p->getTop() && getTop() <= p->getBottom()) {
    if(getLeft() <= p->getRight() && getRight() >= p->getLeft()) {

      //overlap from the left and the top
      if(getLeft() <= p->getLeft() && getTop() <= p->getTop()) {
	if(getRight()-p->getLeft() <= getBottom()-p->getBottom()) {
	  //move the ball left
	  pos.x = p->getLeft()-(radius+1);
	  //flip the direction
	  x_dir = -x_dir;
	}
	else {
	  //move the ball up
	  pos.y = p->getTop()-(radius+1);
	  //flip the direction
	  y_dir = -y_dir;
	}
      }

      //overlap from the left and the bottom
      else if(getLeft() <= p->getLeft() && getBottom() >= p->getBottom()) {
	if(getRight()-p->getLeft() <= p->getBottom()-getTop()) {
	  //move the ball left
	  pos.x = p->getLeft()-(radius+1);
	  //flip the direction
	  x_dir = -x_dir;
	}
	else {
	  //move the ball down
	  pos.y = p->getBottom()+(radius+1);
	  //flip the direction
	  y_dir = -y_dir;
	}
      }

      //overlap from the right and the bottom
      else if(getRight() >= p->getRight() && getBottom() >= p->getBottom()) {
	if(p->getRight()-getLeft() <= p->getBottom()-getTop()) {
	  //move the ball right
	  pos.x = p->getRight()+(radius+1);
	  //flip the direction
	  x_dir = -x_dir;
	}
	else {
	  //move the ball down
	  pos.y = p->getBottom()+(radius+1);
	  //flip the direction
	  y_dir = -y_dir;
	}
      }

      //overlap from the right and the top
      else if(getRight() >= p->getRight() && getTop() <= p->getTop()) {
	if(p->getRight()-getLeft() <= getBottom()-p->getBottom()) {
	  //move the ball right
	  pos.x = p->getRight()+(radius+1);
	  //flip the direction
	  x_dir = -x_dir;
	}
	else {
	  //move the ball up
	  pos.y = p->getTop()-(radius+1);
	  //flip the direction
	  y_dir = -y_dir;
	}
      }

      //else just a direct collision in a direction
      else if(getRight() >= p->getRight() || getLeft() <= p->getLeft()) {
	x_dir = -x_dir;
	pos.x = (getRight() >= p->getRight())?p->getRight()+(radius+1):p->getLeft()-(radius+1);
      }
      else {
	y_dir = -y_dir;
	pos.y = (getBottom() >= p->getBottom())?p->getBottom()+(radius+1):p->getTop()-(radius+1);
      }
    }
  }
}
