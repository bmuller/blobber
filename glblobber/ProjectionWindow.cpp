#include"ProjectionWindow.h"

//Initialize Static Data Members
int     ProjectionWindow::display    = 0;
int     ProjectionWindow::fsdisplay  = 0;
bool    ProjectionWindow::fullscreen = false;
POI *   ProjectionWindow::poi        = NULL;
Frame * ProjectionWindow::frame      = NULL;

ProjectionWindow::ProjectionWindow(Frame * frm,
                                   POI * p,
                                   char* title,
                                   int x,
                                   int y,
                                   int width,
                                   int height)
{
  poi = p;
  frame = frm;
  display = glutCreateWindow(title);
  glutPositionWindow(x, y);
  glutReshapeWindow(width, height);

  //Set graphics to be drawn in quadrant 1
  gluOrtho2D(0.0, width, 0.0, height);
  glShadeModel(GL_SMOOTH);

  
  //Use a seperate method to register callbacks
  //because every time fullscreen is entered, 
  //callbacks should be re-registered.
  registerCallbacks();
}

ProjectionWindow::~ProjectionWindow() {}

int ProjectionWindow::getGLUTDisplay()
{
  if (!fullscreen) return display;
  else return fsdisplay;
}

bool ProjectionWindow::isFullscreen()
{
  return fullscreen;
}

void ProjectionWindow::registerCallbacks()
{
  glutDisplayFunc(renderDisplay);
  glutKeyboardFunc(keyboard);
} 

void ProjectionWindow::renderDisplay()
{
  glDrawBuffer(GL_BACK);
  
  // This is where the module interface will
  // do its work... eventually
//  glLineWidth(5.0);
//  glEnable(GL_LINE_SMOOTH);

  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINES);
    int xo = poi->oldx,
        xf = poi->x,
        yo = frame->getHeight() - poi->oldy,
        yf = frame->getHeight() - poi->y;
  glVertex2i(xo,yo);
  glVertex2i(xf,yf);
  glEnd();

  glutSwapBuffers();
}

void ProjectionWindow::keyboard(unsigned char key,
                                int x,
                                int y)
{
  switch (key)
  {
    //Spacebar clears the screen
    case ' ':
      glClear(GL_COLOR_BUFFER_BIT);
      break;
    //Toggle Fullscreen
    case 'f':
    case 'F':
      if (!fullscreen)
      {
        glutSetWindow(getGLUTDisplay());
        glutEnterGameMode();
        glutSetCursor(GLUT_CURSOR_NONE);
        fsdisplay = glutGetWindow();
        registerCallbacks();

        //Set graphics to be drawn in quadrant 1
        gluOrtho2D(0.0, 640, 0.0, 480);
      }

      else
      {
        glutLeaveGameMode();
        glutSetWindow(display);
      }

      fullscreen = !fullscreen;
      glutPostRedisplay();
      break;

    //Quit keys
    case 27: //ESC
    case 'q':
    case 'Q':
      if (fullscreen) glutLeaveGameMode();
      exit(0);
      break;
  }  
}
