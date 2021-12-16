//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _keyboard_
#define _keyboard_ value "keyboard.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include "lib.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Keyboard key status tracker
structdef(Key){
  bool pressed;   // is key currently pressed?
  bool inactive;  // has key already been taken into account and used?
};

void keyIn(unsigned char key, int x, int y);
  /* GLUT function to capture pressed keys */
void keyUp(unsigned char key, int x, int y);
  /* GLUT function to capture released keys */

// FUNCTIONS
void pressKey(int index);
  /* Turn pressed status ON for key index */
void unpressKey(int index);
  /* Turn pressed status OFF for key index and reset inactive status */
void deactivateKey(int index);
  /* Turn key into inactive mode so it will be considered unpressed even if still physically pressed */

// TESTS

bool isActiveKey(int index);
  /* Check if key at index is currently pressed and not previously deactivated */
bool isPressedKey(int index);
  /* Check if key is currently in pressed state, regardless of inactive status */

#endif
