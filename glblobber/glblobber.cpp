#include"glblobber.h"

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

  CameraWindow cameraWindow(&frame, &poi,
                            "glblobber::camera");
  ProjectionWindow projectionWindow(&frame, &poi,
                                    "glblobber::projection");

  glutIdleFunc(idleCB);

  glutMainLoop();
  return 0;
}

// This is the actual "loop"
void idleCB()
{
  frame.update();
  poi.update();
  glutSetWindow(CameraWindow::getGLUTDisplay());
  glutPostRedisplay();
  glutSetWindow(ProjectionWindow::getGLUTDisplay());
  glutPostRedisplay();
}
