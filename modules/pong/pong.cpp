/* Origin is set at upper left hand corner of screen
 *
 * Vectors are converted to rectangular coordinates
 * using:
 *     delta x = speed * cos(angle)
 *     delta y = speed * sin(angle)
 * Rectangular coordinates are converted to vectors
 * using:
 *     speed = sqrt[x^2 + y^2]
 *     angle = tan^-1(y/x)
 *
 *              90
 *              |
 *              |
 *        0----------180
 *              |
 *              |
 *             270
 *
 * Laser "hotspots are the left and right 10% of the
 * screen.
 *
 * Everything is scaled/interpolated to pw resolution
 */

#include "pong.h"
#include "blobber.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

#define PI 3.14159265

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong", "The first-ish video game") { }

Pong::~Pong() { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {
  register_poi(area, 2);
  // Seed random number generator
  srand(time(NULL));
  updateFactors(pw);
  reset();
}

void Pong::update(Camarea &area, ProjectionWindow &pw) {
  updateFactors(pw);
 
  // Handle POI
  vector<PIXEL> poi;
  get_poi(area, poi);

  if (poi.size() > 0) {
    for (int i = 0; i < 2; i++) {

      // Interpolate POI
      COORD p;
      p.x = static_cast<int>((
        static_cast<float>(poi[i].coord.x - area.bounds.left) /
	static_cast<float>(area.bounds.width())) *
	static_cast<float>(pw.dimensions.width));
      p.y = static_cast<int>((
        static_cast<float>(poi[i].coord.y - area.bounds.top) /
	static_cast<float>(area.bounds.height())) *
	static_cast<float>(pw.dimensions.height));

      // Store POI locally
      if (i == 0 && p.x <= 10 * hPercent)
        laser[0] = p;
      if (i == 1 && p.x >= 90 * hPercent)
        laser[1] = p;
    }
  }

  collision(pw);
  move();

  // Draw stuff
  pw.clear();

  for (int i = 0; i < 2; i++) {
    pw.draw_circle_absolute(laser[i],
                            1,
			    BLUE,
			    true);

    pw.draw_box_absolute(paddle[i].coord,
                         paddle[i].width,
			 paddle[i].height,
			 WHITE,
			 true);
  }

  pw.draw_circle_absolute(ball.coord,
                          ball.radius,
			  WHITE,
			  true);
}

void Pong::updateFactors(ProjectionWindow &pw) {
  hPercent = pw.dimensions.width / 100;
  vPercent = pw.dimensions.height / 100;

  for (int i = 0; i < 2; i++) {
    paddle[i].width = hPercent;
    paddle[i].height = 20 * vPercent;
  }

  paddle[0].coord.x = 5 * hPercent;
  paddle[1].coord.x = 95 * hPercent;

  ball.radius = hPercent;
}

void Pong::reset() {
  ball.coord.x = 50 * hPercent;
  ball.coord.y = 50 * vPercent;
  ball.angle = 60;//(rand() % 360);
  do {
    ball.speed = (rand() % 10);
  }
  while (ball.speed < 5);
  ball.radius = hPercent;

  laser[0].x = paddle[0].coord.x = 5 * hPercent;
  laser[0].y = paddle[0].coord.y = 50 * vPercent;

  laser[1].x = paddle[1].coord.x = 95 * hPercent;
  laser[1].y = paddle[1].coord.y = 50 * vPercent;
}

void Pong::move() {
  // Check for misalignment between laser points and paddles
  // Determine speed and angle of paddle movement - then move
  for (int i = 0; i < 2; i++) {
    if ((laser[i].y < paddle[i].coord.y) ||
        (laser[i].y > paddle[i].coord.y + 
         static_cast<int>(paddle[i].height))) {
      paddle[i].angle = (laser[i].y < paddle[i].coord.y) ? 
                         90 : 270;
      paddle[i].speed = abs((laser[i].y - paddle[i].coord.y) / 10);
      paddle[i].coord.y -= (paddle[i].speed *
                            sin((paddle[i].angle * PI) / 180));
    }
    else paddle[i].speed = 0;
  }

  // Move the ball
  ball.coord.x -= (ball.speed * cos((ball.angle * PI) / 180));
  ball.coord.y -= (ball.speed * sin((ball.angle * PI) / 180));
}

void Pong::collision(ProjectionWindow &pw) {
  // Check for a score
  if (ball.coord.x <= 2 * hPercent) {
    score[0]++;
    reset();
    return;
  }

  if (ball.coord.x >= 98 * hPercent) {
    score[1]++;
    reset();
    return;
  }

  // Did we hit the roof or floor?
  if (((ball.coord.y - static_cast<int>(ball.radius)) <= 0 &&
	ball.angle < 180) ||
      ((ball.coord.y + static_cast<int>(ball.radius)) >=
        pw.dimensions.height) &&
	ball.angle > 180)
    ball.angle = 360 - ball.angle;

  else { // FIXME top/bottom/outside sides of paddles?

    // Did we hit the left paddle?
    if ((ball.coord.x - static_cast<int>(ball.radius)) <=
        (paddle[0].coord.x + static_cast<int>(paddle[0].width)) &&
        (ball.coord.y >= paddle[0].coord.y) &&
	(ball.coord.y <= (paddle[0].coord.y +
	 static_cast<int>(paddle[0].height)))) {
      if (paddle[0].speed > 0) {
        // Calculate resultant vectors from collision
	// while paddle is in motion
        float x, y;
        x = (paddle[0].speed * cos((paddle[0].angle * PI) / 180)) +
            (ball.speed * cos((ball.angle * PI) / 180));
        y = (paddle[0].speed * sin((paddle[0].angle * PI) / 180)) +
            (ball.speed * sin((ball.angle * PI) / 180));
        ball.speed = sqrt(x * x + y * y);
	// Set some realistic limits here
        ball.speed = (ball.speed > 10) ? 10 : ball.speed;
	ball.angle = 180 * (atan(y / x) / PI);
      }
      ball.angle = (ball.angle > 180) ?
	            540 - ball.angle : 180 - ball.angle;
    }

    // Did we hit the right paddle?
    if ((ball.coord.x + static_cast<int>(ball.radius)) >=
        (paddle[1].coord.x) &&
        (ball.coord.y >= paddle[1].coord.y) &&
	(ball.coord.y <= (paddle[1].coord.y +
	 static_cast<int>(paddle[0].height)))) {
      if (paddle[1].speed > 0) {
        // Calculate resultant vectors from collision
	// while paddle is in motion
        float x, y;
        x = (paddle[1].speed * cos((paddle[1].angle * PI) / 180)) +
            (ball.speed * cos((ball.angle * PI) / 180));
        y = (paddle[1].speed * sin((paddle[1].angle * PI) / 180)) +
            (ball.speed * sin((ball.angle * PI) / 180));
        ball.speed = sqrt(x * x + y * y);
	// Set some realistic limits here
        ball.speed = (ball.speed > 10) ? 10 : ball.speed;
	ball.angle = 180 * (atan(y / x) / PI);
      }
      ball.angle = (ball.angle > 180) ?
	            540 - ball.angle : 180 - ball.angle;
    }
  }
}

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
