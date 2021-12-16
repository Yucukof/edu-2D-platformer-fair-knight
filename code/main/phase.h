//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _bulletPhase_
#define _bulletPhase_ "bulletPhase.h"

#include "lib.h"
#include "motion.h"
#include "player.h"
#include "monster.h"
#include "bullet.h"
#include "item.h"
#include "context.h"

// FUNCTIONS

void playerPhase(Context* context);
  /* Move player across field and collect items if needed */
void monsterPhase(Context* context);
  /* Move monsters randomly across field, start attacking if player in sight */
void bulletPhase(Context* context);
  /* Move bullets across field until they find a target or run out of time */

#endif
