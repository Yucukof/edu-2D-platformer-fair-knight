//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _controls_
#define _controls_ value "controls.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include "lib.h"
#include "coord.h"
#include "motion.h"
#include "player.h"
#include "monster.h"
#include "bullet.h"
#include "builder.h"
#include "main.h"
#include "keyboard.h"

void keysGame(Game* game);
  /* Keyboard input general handler function*/
void keysMenu(Menu* menu);
  /* List of keys and actions related to menu manipulation */
void keysContext(Context* context);
  /*List of keys and actions related to context and allowing user to play game */
void keysResources(Resources* resources);
  /* List of keys and actions related to resources */

#endif
