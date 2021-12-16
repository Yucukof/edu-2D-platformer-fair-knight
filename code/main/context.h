//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _context_
#define _context_ "context.h"

#include "lib.h"

#include "scores.h"
#include "level.h"
#include "player.h"
#include "monster.h"
#include "bullet.h"
#include "item.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME
structdef(Context){
      Score* score;       // Points and kills Currently collected by player
      Level* level;       // Level currently loaded
     Player* player;      // Player tab
    Monster* monsters;    // Monsters tab list
     Bullet* bullets;     // Bullets tab list
       Item* items;       // Items tab list
       bool  isValid;     // Context validity status
       bool  isSuspended; // Context state
       bool  isWon;       // Context victory state
};

// INITIALISATION

Context* initContext (void);
  /* Create new context Struture */

// FUNCTIONS

void updateScrolling(Context* context);
  /* Update scrolling x and Y values accoridng to player position */

// TESTS

bool isValidContext(Context* context);
  /* Check if context is valid */

// DESTROY

void destroyContext(Context* context);
  /* Free Context structure contents from memoy */

#endif
