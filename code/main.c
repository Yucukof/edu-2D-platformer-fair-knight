//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "main.h"

#define CALLER "main"

int main(int argc, char **argv) {
  // Activate logger
  activateLog(true);
  // Initialise main game variable and subsequent structs
  initGame(&game);
  // Start GLUT
  glut(argc,argv);

  return EXIT_SUCCESS;
}
