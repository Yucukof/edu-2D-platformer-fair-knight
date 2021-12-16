//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _power_
#define _power_ "power.h"

#include "lib.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Power structure
structdef(Power){
  effect  type;     // Type of power
    bool  active;   // Current status
     int  timer;    // Life timer
   Power* next;     // Link to next power
};

// INITIALISATION

Power* initPower(void);
  /* Create new power struct */

// FILES

// FUNCTIONS

// TESTS

bool isValidPowers(Power* powers);
  /* Check if array of power is valid */
bool isValidPower(Power* power);
  /* Check if power is valid */

// DESTROY

void destroyPowers(Power* powers);
  /* Free chain of powers from pointer and after */
void destroyPower(Power* power);
  /* Free power struct from memory */

// UTILITY
void printPowers(Power* powers);
  /* Output chain of powers to screen */
void printPower(Power* power);
  /* Print power info to screen */

#endif
