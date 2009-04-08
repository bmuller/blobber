//Using a whole lot of static linkage based on
//GLUT. This is not very robust, but considering
//we will usually only need one instance of this
//object, this is ok.

#ifndef PROJECTION_WINDOW_H
#define PROJECTION_WINDOW_H

#include<GL/glut.h>
#include<cstdlib>
#include"POI.h"
#include"Frame.h"

class ProjectionWindow
{
  public:
    ProjectionWindow(Frame * frm,
                     POI * p,
                     char * name = "Projection Window",
                     int x = 400,
                     int y = 20,
                     int width = 640,
                     int height = 480);
    ~ProjectionWindow();

    static int getGLUTDisplay();
    static bool isFullscreen();
    
  protected:
    //Static Data Members
    static int display;
    static int fsdisplay;
    static bool fullscreen;
    static POI * poi;
    static Frame * frame;
    
    //Utility Functions
    static void registerCallbacks();
    
    //GLUT Callbacks
    static void renderDisplay();
    static void keyboard(unsigned char key,
                         int x,
                         int y);
};

#endif
