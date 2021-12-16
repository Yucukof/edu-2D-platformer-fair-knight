//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _player_
#define _player_ "player.h"

#include "coord.h"
#include "power.h"
#include "assets.h"

#include "lib.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Player character info
structdef(Player){
  Coordinates* coord;         // Position on field
        Power* powerbank;     // Powers currently granted to player
          int  baseHealth;    // Default health
          int  currentHealth; // Current health level
          int  baseSpeed;     // Default speed
          int  currentSpeed;  // Modified speed
          int  baseAttack;    // Default attack
          int  currentAttack; // Current attack
       Sprite* sprites;       // Link to sprite resource
};

// INITIALISATION

Player* initPlayer(void);
  /* Create new player struct */

// FUNCTIONS

bool setPlayerSprites(Player* player,type type, int name, anim anim, Asset* assets);
  /* Associate new sprites to player */
void setStatus(Player* player, int status, int timer);
  /*  Activate powerbank with a specific powerup */
  
void hitPlayer(Player* player, int hitpoint);
  /* Inflict damage to plyer according to hitpoint */

// TESTS

bool isValidPlayer(Player* player);
/* Check if player has valid charasteristics */
bool isInvinciblePlayer(Player* player);
  /* Check if invincible */
bool isDeadPlayer(Player* player);
  /* Check if player is dead or alive */

// DESTROY

void destroyPlayer(Player* p);
  /* Free player info and coord from memory */

// UTILITY

void printPlayer(Player* p);
  /* Output player info and coord to screen */

#endif
