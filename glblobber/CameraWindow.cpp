// FIXME - need a clean way to prevent resizing this window

#include"CameraWindow.h"

// Initialize static data
Frame *      CameraWindow::frame         = NULL;
int          CameraWindow::display       = 0;
bool         CameraWindow::glIsSetup     = false;
unsigned int CameraWindow::videoTexture  = 0;
AlignmentBox CameraWindow::alignmentBox;
bool         CameraWindow::aligningBox   = false;
POI *        CameraWindow::poi           = NULL;
bool         CameraWindow::aligningColor = false;
bool         CameraWindow::showOverlay   = false;
time_t       CameraWindow::start         = 0;
time_t       CameraWindow::now           = 0;
int          CameraWindow::frameCount    = 0;
char         CameraWindow::fpsData[3]    = "";

CameraWindow::CameraWindow(Frame * frm,
                           POI * p,
                           string name,
                           int x,
                           int y)
{
  frame = frm;
  poi = p;

  alignmentBox = poi->getBox();

  // FIXME - default some camera settings
  // This will be pulled from a config file eventually
  frame->setBrightness(90);
  frame->setContrast(80);
  frame->setExposure(5, false);
  frame->setSaturation(80);

  display = glutCreateWindow(name.c_str());
  glutPositionWindow(x, y);
  glutReshapeWindow(frame->getWidth(), frame->getHeight());

  // Register Callbacks
  glutDisplayFunc(displayCB);
  glutKeyboardFunc(keyboardCB);
  glutMouseFunc(mouseButtonCB);
  glutMotionFunc(mouseDragCB);

  time(&start);
}

CameraWindow::~CameraWindow()
{}

int CameraWindow::getGLUTDisplay()
{
  return display;
}

void CameraWindow::displayCB()
{
  if (!glIsSetup)
    {
      // Setup some OpenGL Stuff
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glOrtho(0, frame->getWidth(), 0, frame->getHeight(), -1.0f, 1.0f);
      
      // Generate the video texture for painting later
      glGenTextures(1, &videoTexture);
      glBindTexture(GL_TEXTURE_RECTANGLE_ARB, videoTexture);

      // Define texture wraping, etc
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);

      // This prevents the drawing color from affecting the texture
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

      glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0,
                   GL_RGB, frame->getWidth(), frame->getHeight(), 0,
                   GL_RGB, GL_UNSIGNED_BYTE,
                   static_cast<unsigned char*>(frame->getCapture()));

      glIsSetup = true;
    }

  // Draw the current video frame
  glEnable(GL_TEXTURE_RECTANGLE_ARB);
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB, videoTexture);

  glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0,
                  frame->getWidth(), frame->getHeight(),
                  GL_RGB, GL_UNSIGNED_BYTE,
                  static_cast<unsigned char *>(frame->getCapture()));

  glBegin(GL_QUADS);
    glTexCoord2i(0, frame->getHeight());
    glVertex2i(0, 0);
    glTexCoord2i(frame->getWidth(), frame->getHeight());
    glVertex2i(frame->getWidth(),0);
    glTexCoord2i(frame->getWidth(), 0);
    glVertex2i(frame->getWidth(),frame->getHeight());
    glTexCoord2i(0, 0);
    glVertex2i(0, frame->getHeight());
  glEnd();

  glDisable(GL_TEXTURE_RECTANGLE_ARB);

  // Draw Alignment Box (Selection Box)
  // We will draw couter clockwise as a
  // standard to support backface culling
  glBegin(GL_LINE_LOOP);
    aligningBox ?
      glColor3f(0.0f, 0.0f, 1.0f) :
      glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(alignmentBox.xStart, frame->getHeight() - alignmentBox.yStart);
    glVertex2i(alignmentBox.xStop, frame->getHeight() - alignmentBox.yStart);
    glVertex2i(alignmentBox.xStop, frame->getHeight() - alignmentBox.yStop);
    glVertex2i(alignmentBox.xStart, frame->getHeight() - alignmentBox.yStop);
  glEnd();

  // Update Refresh Rate data
  time(&now);
  frameCount++;

  if (difftime(now, start) > 1)
  {
    fpsData[0] = (frameCount / 100) + 48;
    fpsData[1] = (frameCount % 100) / 10 + 48;
    fpsData[2] = ((frameCount % 100) % 10) + 48;
    time(&start);
    frameCount = 0;
  }

  // Draw the overlay
  // Not using an OpenGL overlay for max portablility (hardware)
  if (showOverlay)
  {
    string textData = "";
    
    textData += fpsData;
    textData += " FPS";

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(0, frame->getHeight() - 10);
    drawText(textData);
    // More stuff will go in here
    // Brightness, Contrast, Exposure, etc
  }

  // Swap front-back buffers
  glutSwapBuffers();
}

void CameraWindow::keyboardCB(unsigned char key,
                              int x,
                              int y)
{
  unsigned char color[3] = {0, 0, 0};
  unsigned char * reference = static_cast<unsigned char *>(frame->getCapture());

  switch (key)
  {
    // Auto color alignment
    case 'a':
    case 'A':
      for (int i = 0; i < frame->getHeight(); i++)
        for (int j = 0; j < frame->getWidth(); j++)
        {
          if (*(reference + 0) + *(reference + 1) + *(reference + 2) >
              color[0] + color[1] + color[2])
          {
            color[0] = *(reference + 0);
            color[1] = *(reference + 1);
            color[2] = *(reference + 2);
          }
          reference += 3;
        }
      poi->setColor(color);
      break;

    // Manual color alignment
    case 'm':
    case 'M':
      aligningColor = true;
      break;

    // Toggle Overlay
    case 9:  //Horizontal Tab
      showOverlay = !showOverlay;
      break;

    // Quit keys
    case 27: //ESC
    case 'q':
    case 'Q':
      exit(0);
      break;
  } 
}

void CameraWindow::mouseButtonCB(int button,
                                 int state,
                                 int x,
                                 int y)
{
  if (button == GLUT_LEFT_BUTTON && 
      state == GLUT_DOWN &&
      aligningColor)
  {
    unsigned char color[3];
    glReadPixels(x, frame->getHeight() - y, 1, 1,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 color);
    poi->setColor(color);
    aligningColor = false;
  }

  if (button == GLUT_LEFT_BUTTON &&
      state == GLUT_UP &&
      aligningBox)
  {
    alignmentBox.xStop = x;
    alignmentBox.yStop = y;

    // Make sure we have our standard coordinates for use in POI
    int buf;
    if (alignmentBox.xStart > alignmentBox.xStop)
    {
      buf = alignmentBox.xStart;
      alignmentBox.xStart = alignmentBox.xStop;
      alignmentBox.xStop = buf;
    }
    if (alignmentBox.yStart > alignmentBox.yStop)
    {
      buf = alignmentBox.yStart;
      alignmentBox.yStart = alignmentBox.yStop;
      alignmentBox.yStop = buf;
    }
    if (alignmentBox.xStart < 0)
      alignmentBox.xStart = 0;
    if (alignmentBox.xStop > frame->getWidth())
      alignmentBox.xStop = frame->getWidth();
    if (alignmentBox.yStart < 0)
      alignmentBox.yStart = 0;
    if (alignmentBox.yStop > frame->getHeight())
      alignmentBox.yStop = frame->getHeight();

    poi->setBox(alignmentBox);
    aligningBox = false;
  }
}

void CameraWindow::mouseDragCB(int x,
                               int y)
{
  if (!aligningBox)
  {
    aligningBox = true;
    alignmentBox.xStop = alignmentBox.xStart = x;
    alignmentBox.yStop = alignmentBox.yStart = y;
  }
  else
  {
    alignmentBox.xStop = x;
    alignmentBox.yStop = y;
  }
}

void CameraWindow::drawText(string text)
{
  for (int i = 0; i < text.length(); i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
  }
}
