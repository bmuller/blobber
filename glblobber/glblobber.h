#ifndef GLVIDEO_H
#define GLVIDEO_H

#include<iostream>

#include"Frame.h"
#include"POI.h"
#include"CameraWindow.h"
#include"ProjectionWindow.h"

using namespace std;

// FIXME - does this need to be static
// should be loaded from config
static Frame frame("/dev/video0");
static POI poi(&frame);

void idleCB();

#endif
