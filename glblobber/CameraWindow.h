/*
 * Using a good bit of static linkage here.
 * All because of GLUT
 * Is there a better way?
 */
#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include<GL/glut.h>
#include<string>
#include<cstdlib>
#include<ctime>
#include"Frame.h"
#include"utility.h"
#include"POI.h"

using namespace std;

class CameraWindow
{
  public:
    CameraWindow(Frame * frm,
                 POI * p,
                 string name = "Camera Window",
                 int x = 0,
                 int y = 0);
    ~CameraWindow();

    static int getGLUTDisplay();

  protected:
    // Static variables for GLUT
    static int display;    // Holds the display ID for the window
    static Frame * frame;
    static bool glIsSetup; // "First Pass" flag in displayCB
    static unsigned int videoTexture;
    static AlignmentBox alignmentBox;
    static bool aligningBox;
    static POI * poi;
    static bool aligningColor;
    static bool showOverlay;
    static time_t start, now; // Time Data for refresh rate
    static int frameCount;
    static char fpsData[3];

    // Utility
    static void drawText(string text);

    // GLUT Callbacks
    static void displayCB();
    static void keyboardCB(unsigned char key,
                           int x,
                           int y);
    static void mouseButtonCB(int button,
                              int state,
                              int x,
                              int y);
    static void mouseDragCB(int x,
                            int y);
};

#endif        
