//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "bullet.h"

#define CALLER "bullet"

// INITIALISATION

Bullet* initBullet(void){
  // Allocate space for Bullet struct in  memory
  Bullet* bullet = malloc(sizeof(Bullet));
  // Check if instantiation is successful
  if (bullet != NULL) {
    // Set initial values
    bullet->baseAttack    = BULLETATTACK;
    bullet->currentAttack = bullet->baseAttack;
    bullet->baseSpeed     = BULLETSPEED;
    bullet->currentSpeed  = bullet->baseSpeed;
    bullet->timer         = BULLETTIMER;
    bullet->next          = NULL;
    bullet->sprites       = NULL;
    bullet->coord         = initCoord();
  } else {
    // ELSE, output error
    logger(ERR,CALLER,"failed to initialise bullet!");
  }
  // Return struct pointer
  return bullet;
}

// FILES

// FUNCTIONS

Bullet* newBullet(float x, float y, direction orientation){
  // Allocate memory space for output
  Bullet* output = initBullet();
  // Check if instantiation was successful
  if (isValidBullet(output)) {
    // Specify bullet offset from character throwing it
    int offset = -UNIT;
    if (orientation == LEFT) {
      offset = -UNIT;
    } else {
      offset = UNIT*1/4;
    }
    setCoord(output->coord,x+offset,y,orientation);
    setMoving(output->coord,orientation);
  } else {
    // Otherwise, return empty bullet
    output = NULL;
  }
  return output;
}

void appendBullet(Bullet* bullet, Bullet** bullets) {
  // check if input is valid
  if (isValidBullet(bullet)) {
    // Check if chain exists
    if (*bullets == NULL) {
      // If not, start chain
      *bullets = bullet;
    } else {
      // Else, check if chain is valid
      if (isValidBullets(*bullets)) {
        // Check if chain can accomodate with new bullet
        if (isBulletChainFull(*bullets)) {
          // Otherwise remove first bullet
          popFirstBullet(bullets);
        } else {
          // Store chain start pointer
          Bullet* cursor = *bullets;
          // Read through chain to find append position
          while (cursor->next != NULL) {
            cursor = cursor->next;
          }
          // Append bullet to chain
          cursor->next = bullet;
        }
      }
    }
  } else {
    // Output failure
    logger(ERR,CALLER,"unable to append bullet");
  }
}

void popFirstBullet(Bullet** bullets) {
  // Store first bullet for deletion
  Bullet* toDestroy = *bullets;
  // Check if first item of chain is valid
  if (isValidBullets(*bullets)) {
    // Output warning
    logger(WARN,CALLER,"popping first bullet of chain");
    // Transfer chain start to next bullet;
    *bullets = toDestroy->next;
    // Destroy first bullet;
    destroyBullet(toDestroy);
  }
}

void removeBullet(Bullet* bullet, Bullet** bullets) {
  // Check if passed chain is valid
  if (isValidBullet(bullet) && *bullets != NULL && isValidBullets(*bullets)) {
    // Make cursor
    Bullet* cursor = *bullets;
    // Reserve space for previous address
    Bullet* previous = NULL;
    // Read through chain
    while (cursor != NULL && cursor != bullet) {
      // Keep previous bullet of chain in memory
      previous = cursor;
      // Switch to next bullet
      cursor = cursor->next;
    }
    // Check if bullet was found
    if (cursor != NULL) {
      // Check if bullet was first in chain
      if (previous == NULL) {
        // if so, return next item as new start
        *bullets = cursor->next;
      } else {
        // Otherwise skip bullet to destroy in chain
        previous->next = cursor->next;
      }
      logger(WARN,CALLER,"removing bullet");
      // Delete bullet
      destroyBullet(bullet);
    } else {
      // Output failure and return untouched chain
      logger(ERR,CALLER,"unable to remove bullet!");
    }
  }
}

int getBulletChainSize(Bullet* bullets) {
  // Allocate memory and initialise value for output
  int output = 0;
  // Check if input is valid
  if (isValidBullets(bullets)) {
    // Make cursor
    Bullet* cursor = bullets;
    // Read through bullets
    while (cursor != NULL) {
      cursor = cursor->next;
      output++;
    }
  }
  return output;
}

bool setBulletSprites(Bullet* bullet, Asset* assets) {
  if (bullet != NULL && bullet->coord !=  NULL) {
    bullet->sprites = loadAssetSprites(OBJ,BULLET,IDLE,assets);
    // Check if sprites were successfully loaded
    if (bullet->sprites != NULL) {
      // Set bullet hitbox size
      bullet->coord->w   = bullet->coord->x+bullet->sprites->width;
      bullet->coord->h   = bullet->coord->y+bullet->sprites->height;
      // Return success
      return true;
    } else {
      // If anim or coord failed to load, output error and return failure
      logger(ERR,CALLER,"failed to load bullet sprites!");
      return false;
    }
  }
  // Output error
  logger(ERR,CALLER,"unable to set bullet sprites!");
  // Return failure
  return false;
}

// TESTS

bool isValidBullets(Bullet* bullets) {
  // Check if passed parameter is valid
  if (bullets != NULL) {
    // Allocate memory space for test response
    bool isValid = true;
    // Read through bullet chain
    while (bullets != NULL) {
      // Check single bullet
      if (!isValidBullet(bullets)) {
        isValid = false;
      }
      // switch to next bullet in chain
      bullets = bullets->next;
    }
    // check if successful
    if (!isValid) {
      // If not, then output error
      logger(ERR,CALLER,"invalid bullet chain!");
    }
    // Return test value
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate bullets!");
    return false;
  }
}

bool isValidBullet(Bullet* bullet) {
  // Check if passed parameter is valid
  if (bullet != NULL) {
    // Allocate memory space for test response
    bool isValid = true;

    if (bullet->coord && !isValidCoord(bullet->coord)) {
      isValid = false;
    }
    if (bullet->sprites != NULL && !isValidSprites(bullet->sprites)) {
      isValid = false;
    }

    // Return test value
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate bullet");
    return false;
  }
}

bool isBulletChainFull(Bullet* bullets) {
  // Allocate memory for output
  bool output = false;
  // Check if input is valid
  if (isValidBullets(bullets)) {
    // check if chain size exceeds max size
    if (getBulletChainSize(bullets) >= MAXBULLETS) {
      // return true
      return true;
    }
  } else {
    // Print error
    logger(ERR,CALLER,"unable to check chain emptiness!");
  }
  // return value
  return output;
}

// DESTROY

void destroyBullets(Bullet* bullets) {
char buffer[MAXSTRING];

logger(WARN,CALLER,"destroying bullets...");
// Set bullet to destroy
Bullet* toDestroy = bullets;

int i = 0; // Destroyed bullets count
while (toDestroy != NULL) {
  // Put next bullet in buffer
  Bullet* next = toDestroy->next;
  // Free current bullet
  destroyBullet(toDestroy);
  // Set next bullet as current
  toDestroy = next;
  // Increment destroyed bullets counter
  i++;
  }
  // Feedback: number of bullets effectively destroyed
  sprintf(buffer,"%i bullets destroyed.\n",i);
  logger(WARN,CALLER,buffer);
}

void destroyBullet(Bullet* bullet) {
  // Free bullet struct from memory
  destroyCoordinates(bullet->coord);
  free(bullet);
  logger(TRACE,CALLER,"bullet destroyed.");
}

// UTILITY

void printBullets(Bullet* bullets) {
  // Read array of bullets and print out each entry
  while (bullets!= NULL) {
    printBullet(bullets);
    bullets = bullets->next;
  }
}

void printBullet(Bullet* bullet) {
  char buffer[MAXSTRING];
  // Put bullet info in buffer
  sprintf(buffer,"Bullet\n\t\t~AP: %i/%i\n\t\t~SP: %i/%i\n\t\t~timer: %i"
         ,bullet->currentAttack,bullet->baseAttack
         ,bullet->currentSpeed ,bullet->baseSpeed
         ,bullet->timer);
  // Output buffer to logger
  logger(INFO,CALLER,buffer);
  // Print bullet coordinates
  printCoord(bullet->coord);
  printSprite(bullet->sprites);
}
