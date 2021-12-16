//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "player.h"

#define CALLER "player"

// INITIALISATION

Player* initPlayer(void) {

  logger(INFO,CALLER,"instanciating player...");
  // Allocate space memory for player struct
  Player* player = malloc(sizeof(Player));
  // Check if instantiation was successful
  if (player != NULL) {
    // Set initial values
    player->baseHealth    = 5;
    player->currentHealth = player->baseHealth;
    player->baseSpeed     = 3;
    player->currentSpeed  = player->baseSpeed;
    player->baseAttack    = 1;
    player->currentAttack = player->baseAttack;
    player->powerbank     = NULL;
    player->sprites       = NULL;
    player->coord         = initCoord();
    logger(INFO,CALLER,"player instanciated.\n");
  }
  // return struct
  return player;
}

// FUNCTIONS

bool setPlayerSprites(Player* player,type type, int name, anim anim, Asset* assets) {
  if (isValidPlayer(player)) {
    player->sprites = loadAssetSprites(type,name,anim,assets);
    // Check if sprites were successfully loaded
    if (isValidSprites(player->sprites)) {
      // Set player hitbox size
      player->coord->w = player->coord->x+(float)player->sprites->width;
      player->coord->h = player->coord->y+(float)player->sprites->height;
      // Return success
      return true;
    } else {
      // If anim or coord failed to load, output error and return failure
      logger(ERR,CALLER,"failed to load sprites!");
      return false;
    }
  }
  // Output error
  logger(ERR,CALLER,"unable to set sprites");
  // Return failure
  return false;
}

void setStatus(Player* player, int status, int timer){
  logger(WARN,CALLER,"should have been set invincible.\n\t\ttoo bad it has not been implemented yet...");
}

void hitPlayer(Player* player, int hitpoint) {
  // Check if passed input is valid
  if (isValidPlayer(player)) {
    // Check if player can be hurt
    if (!isInvinciblePlayer(player)) {
      // IF yes, then proceed to health reduction
      player->currentHealth -= hitpoint;
      logger(INFO,CALLER,"was hit.");
      // Reset player coordinates to start point
      setStatus(player,INVULNERABILITY,DEFAULTTIMER);
      setCoord(player->coord,16,16,RIGHT);
      if (isDeadPlayer(player)) {
        logger(INFO,CALLER,"is dead.");
      }
    } else {
      // ELSE output invicibility message
      logger(INFO,CALLER,"resisted hit.");
    }
  } else {
    // Else output error message to screen
    logger(ERR,CALLER,"could not be hit!");
  }
}

// TESTS

bool isValidPlayer(Player* player) {
  // Check if passed parameter is valid
  if (player != NULL) {
    // Allocate memory for test response
    bool isValid = true;
    // Validate inner characteristics
    if (player->baseHealth    <= 0
     || player->currentHealth <  0
     || player->baseSpeed     <= 0
     || player->currentSpeed  <  0
     || player->baseAttack    <= 0
     || player->currentAttack <  0 ) {
        isValid = false;
      }
      // Validate substructures
      if (player->coord !=NULL && !isValidCoord(player->coord)) {
        isValid = false;
      }
      if (player->sprites != NULL && !isValidSprites(player->sprites)) {
        isValid = false;
      }
      // Validate powerbank only if exist
      if (player->powerbank != NULL && !isValidPower(player->powerbank)) {
        isValid = false;
      }
      // Evaluate test result
      if (!isValid) {
        logger(WARN,CALLER,"invalid found!");
      }
      // Return test value
      return isValid;
    } else {
      // Output error and return failure
      logger(ERR,CALLER,"unable to validate!");
      return false;
    }
  }

bool isInvinciblePlayer(Player* player) {
  // ALlocate memory for function return
  bool output = false;
  // Check if passed input is valid
  if (isValidPlayer(player)) {
    // Check if player is actually invincible
    if (0) { // to implement
      output = true;
    }
  } else {
    // Otherwise, print error to screen
    logger(ERR,CALLER,"unable to check invicibility!");
  }
  // Return test value
  return output;
}

bool isDeadPlayer(Player* player) {
  // check if passed input is valid
  if (isValidPlayer(player)) {
    // Check if player is alive
    if (player->currentHealth > 0) {
      return false;
    } else {
      return true;
    }
  } else {
    logger(ERR,CALLER,"unable to check if dead!");
    return false;
  }
}

// DESTROY

void destroyPlayer(Player* player) {
  // Free player coord
  destroyCoordinates(player->coord);
  // Free player from memory
  free(player);
  logger(WARN,CALLER,"destroyed.");
}

// UTILITY

void printPlayer(Player* player) {
  // Check if player exists
  if (isValidPlayer(player)) {
    char buffer[MAXSTRING];
    // Output player info to buffer
    sprintf(buffer,"\n\t\t~HP:%i/%i\n\t\t~Attack: %i\n\t\t~Speed: %i"
    ,player->currentHealth,player->baseHealth
    ,player->currentAttack
    ,player->currentSpeed);
    //  Output buffer;
    logger(INFO,CALLER,buffer);
    // Output player coord
    printCoord(player->coord);
    printSprite(player->sprites);
  } else {
    logger(ERR,CALLER,"unable to print!");
  }
}
