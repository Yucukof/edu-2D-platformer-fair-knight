  //
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "display.h"

#define CALLER "display"

int glut(int argc, char **argv) {
  logger(INFO,CALLER,"initialisation of GLUT...");
  // Initialize OpenGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(DISPLAYWIDTH, DISPLAYHEIGHT);
  glutInitWindowPosition(100,100);
  glutCreateWindow("PROJECT ALPHA");
  // Call initialiser
  init();

  // Register Callback functions
    // Keyboard
    glutKeyboardFunc(keyIn);
    glutKeyboardUpFunc(keyUp);
    glutIgnoreKeyRepeat(1);
    // Display
    glutDisplayFunc(draw);
  // Output success
  logger(INFO,CALLER,"GLUT initialized.");
  // Loop back
  glutMainLoop();
  // Close log file upon exit
  closeLog();
  // Exit
  return 0;
}

void init() {

  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);

  // Enable anti-aliasing
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  // Enabble bidimensional representation
  glViewport(0.f, 0.f, (GLfloat) DISPLAYWIDTH, DISPLAYHEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.f, (GLfloat) DISPLAYWIDTH, 0.f, (GLfloat) DISPLAYHEIGHT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
  // Set reset colour to black
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClearDepth(1.0);

  // Start screen update functions
  updateFrame(0);
  updatePhase(0);
  updateFlashing(0);
  updateAnim(0);

}
