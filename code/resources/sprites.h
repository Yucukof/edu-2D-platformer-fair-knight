//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _sprites_
#define _sprites_ "sprites.h"

#include "lib.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Sprites storing entry
structdef(Sprite){
     char   path[MAXSTRING];
      int   frame;
      int   width;
      int   height;
   GLuint   texture;
   Sprite*  next;
};

// INITIALISATION

Sprite* initSprite(void);
  /* Initialise Sprite struct */

// FILES

void loadSprites(Sprite* sprites, int frame);
  /* Load all sprites to make frames */
void loadTexture(Sprite* sprite);
  /* Convert sprite to OpenGL texture */

// FUNCTIONS

Sprite* buildSprites(char* path,int frames);
  /* Generate multi-frames sprite */

// TESTS
bool isValidSprites(Sprite* sprites);
  /* Check if chained sprites are valid */
bool isValidSprite(Sprite* sprite);
  /* Check if sprites struc is valid */
bool isValidTexture(Sprite* sprite);

// DESTROY

void destroySprites(Sprite* sprites);
  /* Free array of sprites beyond pointer */
void destroySprite(Sprite* sprite);
  /* Free sprite from memory */

// UTILITY

void printSprite(Sprite* sprite);
  /* Output sprite info to screen */
#endif
