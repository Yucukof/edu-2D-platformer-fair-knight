//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _builder_
#define _builder_ "builder.h"

#include "lib.h"
#include "resources.h"
#include "context.h"

// FUNCTIONS

Context* buildContext(Resources* resources, int levelIndex);
  /* Use context values to build new game context */
void reloadContext(Context** context, Resources* resources);
  /* Reload context from resources */
void updateContext(Context** context, Resources* resources);
  /* update context to next level */

Monster* spawnMonster(mob type, float x, float y, Resources* resources);
  /* Create monster with specified coordinates */
Item* spawnItem(obj type, float x, float y, Resources* resources);
  /* Create item with specified coordinates */

#endif
