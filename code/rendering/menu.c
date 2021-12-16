//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "menu.h"

#define CALLER "menu"

// INITIALISATION

Menu* initMenu(void) {
  // Allocate memory
  Menu* menu = malloc(sizeof(menu));
  // Check if instantiation was successful
  if (menu != NULL) {
    // If yes, then set initial values
    menu->id    = 0;
    menu->focus = 1;
  } else {
    // Otherwise output error to screen
    logger(ERR,CALLER,"unable to initialise menu");
  }
  // Return new menu address
  return menu;
}

// FUNCTION

void setMenu(Menu* menu, int select) {
  /// check if passed input is valid
  if (isValidMenu(menu)) {
    if (isValidFocus(select)) {
      // if yes, then set new menu
      menu->id = select;
    }
  } else {
    // Otherwise output error to screen
    logger(ERR,CALLER,"unable to set menu");
  }
}

void setFocus(Menu* menu,int focus) {
  // Check if passed input is valid
  if (isValidMenu(menu) && isValidFocus(focus)) {
    // Set new focus
    menu->focus = focus;
  } else {
    // Else, output error to screen
    logger(ERR,CALLER,"unable to choose menu");
  }
}

void chooseMenu(Menu* menu) {
  // Check if passed input is valid
  if (isValidMenu(menu)) {
    // Check if the focus is valid
    if (isValidFocus(menu->focus)) {
      // If so, then change selected menu to focused menu
      menu->id = menu->focus;
    }
  } else {
    // Else, output error to screen
    logger(ERR,CALLER,"unable to set menu");
  }
}

// TESTS

bool isValidMenu(Menu* menu) {
  // Allocate memory space for test value
  bool isValid = false;
  // Check if passed input is valid
  if (menu != NULL) {
    // Check against its values
    isValid = true;
    if (menu->id < 0 || menu->id >= MAXMENU) {
      logger(ERR,CALLER,"invalid menu id!");
      isValid = false;
    } else {
      if (!isValidFocus(menu->focus)) {
        isValid = false;
      }
    }
  } else {
    logger(ERR,CALLER,"unable to validate menu!");
  }
  return isValid;
}

bool isValidFocus(int focus) {
  // ALlocate memory for test output
  bool isValid = true;
  // Check if focus value is acceptable
  if (focus < 0) {
    logger(ERR,CALLER,"has negative focus!");
    isValid = false;
  } else if (focus >= MAXMENU) {
    logger(ERR,CALLER,"has focus too high!");
    isValid = false;
  }
  // Return test result
  return isValid;
}

bool isMenuFocused(Menu* menu) {
  // Allocate memory space for test output
  bool isValid = false;
  if (isValidMenu(menu)) {
    // Check if current menu and focus are aligned
    if (menu->id == menu->focus) {
      // If yes, menu is focused
      return true;
    }
  } else {
    logger(ERR,CALLER,"unable to check menu focus!");
  }
  // Return test result
  return isValid;
}

// DESTROY

void destroyMenu(Menu* menu) {
  // Check if passed input is valid
  if (menu) {
    // free menu struct from memory
    free(menu);
    logger(WARN,CALLER,"menu destroyed.");
  } else {
    // Otherwise output error to screen
    logger(ERR,CALLER,"unable to destroy menu!");
  }
}

// UTILITY

void printMenu(Menu* menu) {
  char buffer[MAXSTRING];
  // Check if passed input is valid
  if (isValidMenu(menu)) {
    // Print value to screen
    sprintf(buffer,"ID %i (selected: %i)",menu->id,menu->focus);
    logger(INFO,CALLER,buffer);
  } else {
    // Otherwise, output error
    logger(ERR,CALLER,"unable to print menu!");
  }
}
