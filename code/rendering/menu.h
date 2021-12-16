//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _menu_h_
#define _menu_h_ "menu"

#include <GL/glew.h>
#include <GL/glut.h>

#include "lib.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Menu state tracker
structdef(Menu){
  int id;         // Selected menu index
  int focus;      // Focused menu index
};

// INITIALISATION

Menu* initMenu(void);
  /* Initialise new menu structure */

// FUNTIONS

void setMenu(Menu* menu, int select);
  /* Set menu according to select */
void setFocus(Menu* menu, int focus);
  /* Select new focus */
void chooseMenu(Menu* menu);
  /* Set menu according to menu focus */

// TESTS

bool isValidMenu(Menu* menu);
  /* Check if menu is valid */
bool isValidFocus(int focus);
  /* Check if */
bool isMenuFocused(Menu* menu);
  /* Check if current menu is currently focused */

// DESTROY

void destroyMenu(Menu* menu);
  /* Free menu from memory */

// UTILITY

void printMenu(Menu* menu);
  /* Output menu info to screen */

#endif
