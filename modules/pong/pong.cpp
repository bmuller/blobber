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
 * Laser "hotspots" are the left and right 10% of the
 * screen.
 *
 * Everything is scaled/interpolated to pw resolution
 */

#include "pong.h"
#include "blobber.h"
#include <cstdlib>
#include <cmath>
#include <string>

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong", "The first-ish video game") { }

Pong::~Pong() { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {
  // Register POI to track
  register_poi(area, 2);

  // Get debug info
  string d;
  Configuration::get_config()->get("debug", d, "0");
  debugEnabled = (d == "0") ? false : true;

  // Initialize scores
  score[0] = score[1] = 0;

  // Seed random number generator
  srand(time(NULL));
  updateFactors(pw);
  reset();
  oldLaser[0] = laser[0] = paddle[0].coord;
  oldLaser[1] = laser[1] = paddle[1].coord;
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

      // Check for reset now because we only store POI
      // when lasers are in the "hotspots"
      if (p.y > (97 * yPercent) &&
          p.x > (45 * xPercent) &&
	  p.x < (55 * xPercent)) {
        pw.clear();
	score[0] = score[1] = 0;
        reset();
	return;
      }

      // Store POI locally
      if (i == 0 && p.x >= 0 && p.x <= 10 * xPercent) {
        oldLaser[0] = laser[0];
        laser[0] = p;
      }
      if (i == 1 && p.x >= 90 * xPercent && p.x <= 100 * xPercent) {
        oldLaser[1] = laser[1];
        laser[1] = p;
      }
    }
  }

  // Check for scores/collisions and move stuff
  grabOldData();
  collision(pw);
  move();

  // ***Draw stuff****
  
  // Using a "clear one - draw one" approach
  // to minimize flicker

  //Clear and Draw Scores
  string scoreString;
  pw.draw_box_absolute(COORD(24 * xPercent, 9 * yPercent),
                       20 * xPercent,
		       12 * yPercent,
		       BLACK,
		       true);
  int_to_string(score[0], scoreString);
  pw.draw_text_absolute(COORD(25 * xPercent, 20 * yPercent),
                        10, scoreString);
  pw.draw_box_absolute(COORD(74 * xPercent, 9 * yPercent),
                       20 * xPercent,
		       12 * yPercent,
		       BLACK,
		       true);
  int_to_string(score[1], scoreString);
  pw.draw_text_absolute(COORD(75 * xPercent, 20 * yPercent),
                        10, scoreString);

  // Draw restart button
  pw.draw_box_absolute(COORD(45 * xPercent, 97 * yPercent),
                       10 * xPercent,
                       3 * yPercent,
		       WHITE,
		       false);
  pw.draw_text_absolute(COORD(48 * xPercent, 99 * yPercent),
                        2, "Restart");

  // Draw center court
  for (int i = 0; i < 100; i += 5) {
    COORD c(50 * xPercent , i * yPercent);

    pw.draw_box_absolute(c, 0.5 * xPercent,
                         yPercent,
			 WHITE,
			 true);
  }

  for (int i = 0; i < 2; i++) {
    if (debugEnabled) {
      // Clear blue laser dots
      pw.draw_circle_absolute(oldLaser[i],
                              2,
                              BLACK,
                              true);

      // Draw blue laser dots for debugging
      pw.draw_circle_absolute(laser[i],
                              1,
                              BLUE,
                              true);
    }

    // Clear paddles
    pw.draw_box_absolute(COORD(oldPaddle[i].coord.x - 1,
                         oldPaddle[i].coord.y - 1),
                         oldPaddle[i].width + 2,
			 oldPaddle[i].height + 2,
			 BLACK,
			 true);

    //Draw paddles
    pw.draw_box_absolute(paddle[i].coord,
                         paddle[i].width,
			 paddle[i].height,
			 WHITE,
			 true);
  }

  // Clear ball
  pw.draw_circle_absolute(oldBall.coord,
                          oldBall.radius + 1,
			  BLACK,
			  true);

  // Draw ball
  pw.draw_circle_absolute(ball.coord,
                          ball.radius,
			  WHITE,
			  true);
}

void Pong::updateFactors(ProjectionWindow &pw) {
  xPercent = static_cast<float>(pw.dimensions.width) / 100;
  yPercent = static_cast<float>(pw.dimensions.height) / 100;

  for (int i = 0; i < 2; i++) {
    paddle[i].width = 0.5 * xPercent;
    paddle[i].height = 10 * yPercent;
  }

  paddle[0].coord.x = 4.5 * xPercent;
  paddle[1].coord.x = 95 * xPercent;

  ball.radius = 0.5 * xPercent;
}

void Pong::reset() {
  ball.coord.x = 50 * xPercent;
  ball.coord.y = 50 * yPercent;
  ball.angle = (rand() % 360);
  do {
    ball.speed = (rand() % 15);
  }
  while (ball.speed < 8);

  paddle[0].coord.x = 4.5 * xPercent;
  paddle[0].coord.y = 50 * yPercent;

  paddle[1].coord.x = 95 * xPercent;
  paddle[1].coord.y = 50 * yPercent;
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
      paddle[i].speed = abs((laser[i].y - paddle[i].coord.y) / 7);
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
  if (ball.coord.x <= 2 * xPercent) {
    score[1]++;
    reset();
    return;
  }

  if (ball.coord.x >= 98 * xPercent) {
    score[0]++;
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

  else {

    // Did we hit the left paddle?
    if ((ball.coord.x - static_cast<int>(ball.radius)) <=
        (paddle[0].coord.x + static_cast<int>(paddle[0].width)) &&
        (ball.coord.y >= paddle[0].coord.y) &&
	(ball.coord.y <= (paddle[0].coord.y +
	 static_cast<int>(paddle[0].height))) &&
	(ball.angle < 90 || ball.angle > 270)) {
      if (paddle[0].speed > 0) {
        // Calculate resultant vectors from collision
	// while paddle is in motion
        float x, y;
        x = (paddle[0].speed * cos((paddle[0].angle * PI) / 180)) +
            (ball.speed * cos((ball.angle * PI) / 180));
        y = (paddle[0].speed * sin((paddle[0].angle * PI) / 180)) +
            (ball.speed * sin((ball.angle * PI) / 180));
        ball.speed = sqrt(x * x + y * y);
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
	 static_cast<int>(paddle[1].height))) &&
	(ball.angle >= 90 && ball.angle <= 270)) {
      if (paddle[1].speed > 0) {
        // Calculate resultant vectors from collision
	// while paddle is in motion
        float x, y;
        x = (paddle[1].speed * cos((paddle[1].angle * PI) / 180)) +
            (ball.speed * cos((ball.angle * PI) / 180));
        y = (paddle[1].speed * sin((paddle[1].angle * PI) / 180)) +
            (ball.speed * sin((ball.angle * PI) / 180));
        ball.speed = sqrt(x * x + y * y);
	ball.angle = 180 * (atan(y / x) / PI);
      }
      ball.angle = (ball.angle > 180) ?
	            540 - ball.angle : 180 - ball.angle;
    }
  }
  // Set some "video game" limits here for speed and angle
  ball.speed = (ball.speed > 15) ? 15 : ball.speed;
  ball.speed = (ball.speed < 8) ? 8 : ball.speed;
  if (ball.angle > 80 && ball.angle < 90) ball.angle = 80;
  if (ball.angle >= 90 && ball.angle < 100) ball.angle = 100;
  if (ball.angle > 260 && ball.angle < 270) ball.angle = 260;
  if (ball.angle >= 270 && ball.angle < 280) ball.angle = 280;
}

void Pong::grabOldData() {
  // Before we start drawing, get old data
  // for screen blanking
  oldBall = ball;
  oldPaddle[0] = paddle[0];
  oldPaddle[1] = paddle[1];
}

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
