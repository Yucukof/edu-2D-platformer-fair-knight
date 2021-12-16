//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _motion_
#define _motion_ "motion.h"

#include "lib.h"

#include "coord.h"
#include "context.h"
#include "player.h"
#include "monster.h"

// FUNCTIONS

void motion(Coordinates* coord,int speed,Context* context);
  /* Main motion function, do movements and falls */

void setMoveRandom(Coordinates* origin, Context* context);
  /* Randomly set coordinates movement status */
void move(Coordinates* origin, int speed, Context* context);
  /* Launches movement step accordin to motion */

void jump(Coordinates* origin, int speed, Context* context);
  /* Set jump timer and make upward step */
void fall(Coordinates* origin, Context* context);
  /* Stop jump and make fall steps */
void steps(Coordinates* origin, int speed, direction orientation, Context* context);
  /* Chain steps according to speed */
void step(Coordinates* origin,direction orientation,Context* context);
  /* Take actual steps on board */

Coordinates* nextCoord(Coordinates* origin, direction orientation, Context* context);
  /* Define target coordinates on board, based on origin, direction and map */
void amendCoord(Coordinates* coord, Context* context);
  /* Adapt potential x, y to map boudaries */
void swapCoord(Coordinates* origin,Coordinates* next,Context* context);
  /* exchange coordinates on board */

// TESTS

bool isFalling(Coordinates* origin, Context* context);
  /* Check if coordinate implies falling */
bool isBlocked(Coordinates* next, direction orientation, Context* context);
  /* Check if future coordinate is blocked (prevent moving) */
bool isCollidingWithMonsters(Coordinates* origin, Coordinates* next, Monster* monsters);
  /* Check if current movement causes collision with any monster in array */
bool isCollidingWithPlayer(Coordinates* origin, Coordinates* next, Player* player);
  /* Check if curent movement causes collision with player */


#endif
