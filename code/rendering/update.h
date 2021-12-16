//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _update_
#define _update_ "update.h"

#include "lib.h"
#include "context.h"
#include "game.h"
#include "main.h"
#include "draw.h"
#include "controls.h"
#include "phase.h"

// FUNCTIONS

void updateFrame(int state);
  /* Update display */
void updatePhase(int state);
  /* Handles game phases */
void updateFlashing(int state);
  /* Set flashing colour */
void updateAnim(int state);
  /* Control sprite animation */

#endif
