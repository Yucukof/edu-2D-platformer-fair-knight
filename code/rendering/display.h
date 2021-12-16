//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _display_
#define _display_ "display.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include "lib.h"
#include "update.h"
#include "draw.h"
#include "UI.h"
#include "keyboard.h"

int glut(int argc, char **argv);
  /* Start GLUT */

// INITIALISATION

void init();
  /* Initialise GLUT and register special functions */

#endif
