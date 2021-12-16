//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "keyboard.h"

#define CALLER "keyboard"

Key keyboard[128];

// GLUT

void keyIn(unsigned char key, int x, int y) {
  // Record input
  pressKey(key);
}
void keyUp(unsigned char key, int x, int y) {
  // Record de-input
  unpressKey(key);
}

void pressKey(int index) {
  // Check if passed input is valid
  if (index >= 0 || index < 128) {
    // Turn key inactive
    keyboard[index].pressed = true;
  } else {
    // Else, output error
    char buffer[MAXSTRING];
    sprintf(buffer,"invalid press attempt (%i)!",index);
    logger(ERR,CALLER,buffer);
  }
}

void unpressKey(int index) {
  // Check if passed input is valid
  if (index >= 0 || index < 128) {
    // Turn key inactive
    keyboard[index].pressed  = false;
    keyboard[index].inactive = false;
  } else {
    // Else, output error
    char buffer[MAXSTRING];
    sprintf(buffer,"invalid unpress attempt (%i)!",index);
    logger(ERR,CALLER,buffer);
  }
}

void deactivateKey(int index) {
  // Check if passed input is valid
  if (index >= 0 || index < 128) {
    // Turn key inactive
    keyboard[index].inactive = true;
  } else {
    // Else, output error
    char buffer[MAXSTRING];
    sprintf(buffer,"invalid deactivation attempt (%i)!",index);
    logger(ERR,CALLER,buffer);
  }
}

// TESTS

bool isActiveKey(int index) {
  // Allocate memory space for test output
  bool isActive = false;
  // Check if passed input is valid
  if (index >= 0 || index < 128) {
    // Check if key is currently pressed and not previously deactivated
    if (keyboard[index].pressed && !keyboard[index].inactive) {
      // If yes, key is said active
      isActive = true;
    }
  } else {
    // Otherwise output error to screen
    char buffer[MAXSTRING];
    sprintf(buffer,"invalid key active (%i)!",index);
    logger(ERR,CALLER,buffer);
  }
  // Return test value
  return isActive;
}

bool isPressedKey(int index) {
  // Allocate memory space for test output
  bool isPressed = false;
  // Check if passed input is valid
  if (index >= 0 || index < 128) {
    // Check if key is currently pressed and not previously deactivated
    if (keyboard[index].pressed) {
      // If yes, key is said active
      isPressed = true;
    }
  } else {
    // Otherwise output error to screen
    char buffer[MAXSTRING];
    sprintf(buffer,"invalid key press test (%i)!",index);
    logger(ERR,CALLER,buffer);
  }
  // Return test value
  return isPressed;
}
