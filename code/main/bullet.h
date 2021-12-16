//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _bullet_
#define _bullet_

#include "lib.h"
#include "coord.h"
#include "sprites.h"
#include "assets.h"

#define BULLETTIMER  20 // Duration of bullets before disappearing
#define MAXBULLETS   10 // Maximum number of bullet items allowed
#define BULLETATTACK  1 // Base attack point for new bullet
#define BULLETSPEED   6 // Base speed point for new bullet

#define structdef(NAME) typedef struct NAME NAME; struct NAME

structdef(Bullet){
  Coordinates*  coord;          // Position on field
          int   baseAttack;     // Default attack
          int   currentAttack;  // Modified attack value
          int   baseSpeed;      // Default speed
          int   currentSpeed;   // Modified speed value
          int   timer;          // Bullet life timer
       Bullet*  next;           // Chain link
       Sprite*  sprites;        // Link to sprite resource
};

// INITIALISATION

Bullet* initBullet(void);
  /* Initialise new bullet struct */

// FILES

// FUNCTIONS

Bullet* newBullet(float x, float y, direction orientation);
  /* Create new bullet at position with specifed direction */
void appendBullet(Bullet* bullet, Bullet** bullets);
  /* Add bullet to chain as last item and return chain star */
void popFirstBullet(Bullet** bullets);
  /* Remove first bullet from chain and return new start */
void removeBullet(Bullet* bullet, Bullet** bullets);
  /* Remove bullet from chain and return chain start */
bool setBulletSprites(Bullet* bullet, Asset* assets);
  /* Load bullet struct sprites from assets */
int getBulletChainSize(Bullet* bullets);
  /* Get bullet chain size */

// TESTS

bool isValidBullets(Bullet* bullets);
  /* Check if all bullets in chain are valid */
bool isValidBullet(Bullet* bullet);
  /* Check if bullet struct is valid */
bool isBulletChainFull(Bullet* bullets);
  /* Check if chain is full */

// DESTROY

void destroyBullets(Bullet* bullets);
  /* Destroy array of bullets from pointer */
void destroyBullet(Bullet* bullet);
  /* Free bullet struct from memory */

// UTILITY

void printBullets(Bullet* bullets);
  /* Output array of bullets to screen */
void printBullet(Bullet* bullet);
  /* Print bullet info to screen */


#endif
