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
 *       180----------0
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
#include <iostream>

using namespace blobber;
using namespace std;

Pong::Pong() : ModInterface("Pong", "The first-ish video game") { }

Pong::~Pong() { }

void Pong::init(Camarea &area, ProjectionWindow &pw) {
  register_poi(area, 2);

  ball.coord.x = 50 * hPercent;
  ball.coord.y = 50 * vPercent;

}

void Pong::update(Camarea &area, ProjectionWindow &pw) {
  
  // Adjust scaling - FIXME Should only be done if win geometry changes
  hPercent = pw.dimensions.width / 100;
  vPercent = pw.dimensions.height / 100;

  ball.coord.x = 50 * hPercent;
  ball.coord.y = 50 * vPercent;

  for (int i = 0; i < 2; i++) {
    paddle[i].width = 3 * hPercent;
    paddle[i].height = 10 * vPercent;
  }
 
  ball.radius = hPercent;

  // Handle POI
  vector<PIXEL> poi;
  get_poi(area, poi);

  if(poi.size() == 0) {
    //debug("no poi");
  }
  else {
    //debug("poi found!");
    for(int i = 0; i < poi.size(); i++) {

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

      if (i == 0 && p.x <= 10 * hPercent)
        laser[0] = p;
      if (i == 1 && p.x >= 90 * hPercent)
        laser[1] = p;

    }
  }

  /** move stuff **/

  //here is where we'll need to move the left paddle based on the poi

  /** end move stuff **/

  /** Check for stuff **/
  /** end check for stuff **/

  /** Clear stuff **/
  pw.clear();
  /** end clear stuff **/

  /** draw stuff **/
  for (int i = 0; i < 2; i++)
    pw.draw_circle_absolute(laser[i], 1, BLUE, true);
//  pw.draw_box_absolute(origin,
//		       pw.dimensions.width - 10,
//		       pw.dimensions.height - 10,
//		       BLUE);  //draw the court (TEMPORARY)

  pw.draw_circle_absolute(ball.coord,
			  ball.radius,
			  WHITE,
			  true);
/*
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

// Interpolate POI

extern "C" {
  ModInterface *get_module() { return new Pong(); };
};
