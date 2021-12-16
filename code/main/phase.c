//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "phase.h"

#define CALLER "phase"

void playerPhase(Context* context){
  // Check if player is valid
  if (isValidPlayer(context->player)) {
    // Process player movements according to its moving status
    motion(context->player->coord,context->player->currentSpeed,context);
    // Collect items that collide and recover their score value
    int points = collectItems(context->player->coord,&context->items);
    // Check if items were effectively collected
    if (points > 0) {
      // If so, increase score accordingly
      updateScore(context->score,points,0);
    }
  } else {
    // if player is not valid, output error message and exit
    logger(ERR,CALLER,"failed to process player phase!");
  }
}

void monsterPhase(Context* context){
  Player* player = context->player;
  // Check if passed input is valid
  if (isValidMonster(context->monsters)) {
    // IF yes, make cursor to read trough chain
    Monster* monster = context->monsters;
    while(monster) {
      // check if monster is V
      if (isValidMonster(monster)) {
        // Store next monster address
        Monster* next = monster->next;
        // check if monster is not dead
        if (!isDeadMonster(monster)) {
          // Check if monster is able to do something
          if (!monster->isStunned) {
            // Check if player is visible  or in rang and turn attack mode on if needed
            if ((isVisible(monster->coord,player->coord,monster->sight) || monster->isAttacking)
            && isInRange(monster->coord,player->coord,monster->sight)) {
              setAttacking(monster,true);
            } else {
              setAttacking(monster,false);
            }
            // if monster is trying to attack player
            if (monster->isAttacking) {
              // If so, try to close in
              closeIn(monster->coord, player->coord);
              if (atArmReach(monster->coord,player->coord,monster->reach)) {
                hitPlayer(context->player,monster->currentAttack);
              }
            } else {
              // ELSE, move randomly
              setMoveRandom(monster->coord,context);
            }
            motion(monster->coord,monster->currentSpeed,context);
          } else {
            monster->timer--;
            if (monster->timer == 0) {
              monster->isStunned = false;
            }
          }
        } else {
          // if mosnter is dead, increase score and
          updateScore(context->score,monster->scoreValue,1);
          // Remove dead monster from context
          removeMonster(monster,&context->monsters);
          // Output confirmation
          logger(INFO,CALLER,"monster is dead.");
        }
        // Switch to next monster in chain
        monster = next;
      }
    }
  } else {
    // Output error to screen
    logger(ERR,CALLER,"failed to process monster phase.");
  }
}

void bulletPhase(Context* context) {
  // Check if bullet phase should be done
  if (context->bullets) {
    // Make cursor
    Bullet* bullet = context->bullets;
    // Check if chain is valid
    if (isValidBullets(bullet)) {
      // Read through chain
      while (bullet) {
        // Store next bullet address
        Bullet* next = bullet->next;
        Coordinates* targetCoord = nextCoord(bullet->coord,bullet->coord->look,context);
        Monster* target = getFirstHitMonster(targetCoord,context->monsters);
        // Check if monster hit
        if (target) {
          // Then process HP degradation
          hitMonster(target,bullet->currentAttack);
          // delete bullet from chain
          removeBullet(bullet,&context->bullets);
        } else {
          // Else, check if bullet has ran out of time
          if (bullet->timer-- < 0) {
            // If out of time, then remove bullet
            removeBullet(bullet,&context->bullets);
          } else {
            // Process bullet movement
            move(bullet->coord,bullet->baseSpeed,context);
          }
        }
        // Move on to next bullet in chain
        bullet = next;
      }
    } else {
      // Output error message and exit
      logger(ERR,CALLER,"failed to process bullet phase!");
    }
  }
}
