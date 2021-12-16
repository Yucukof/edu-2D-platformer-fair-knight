//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _draw_
#define _draw_ "draw.h"

#include "lib.h"

#include "fonts.h"
#include "sprites.h"

#include "player.h"
#include "monster.h"
#include "bullet.h"
#include "item.h"
#include "level.h"

#include "game.h"

typedef unsigned int uint;

float scrollx,scrolly; // scrolling values to apply when drawing

// FUNCTIONS

// Text Functions
void drawSimpleText(char* text,int x, int y,Font* fonts);
  /* Draw text with default parameters and colour */
void drawCenteredText(char* text, int y, uint size, uint index, colour overlay,Font* fonts);
  /* Draw text that is automatically centered */

// Sprites Functions
void drawPlayer(Player* p, colour overlay);
  /* Draw player sprite */
void drawBullet(Bullet* bullet);
  /* Draw bullet sprite */
void drawMonster(Monster* mob, colour overlay);
  /* Draw monster sprite */
void drawItem(Item* obj);
  /* Draw item sprite*/
void drawTile(Sprite* s, float x, float y);
  /* Draw tile sprite*/
void drawBackground(Sprite* s,colour overlay);
  /* Draw background sprite*/
void drawCenteredSprite(Sprite* s, float w, float h, colour overlay, direction orientation);
  /* Draw sprite on a centered quad relative to screen */

// UTILITY

// Text Utility
void drawText(char* text,int x,int y,uint size,int index,colour overlay,Font* fonts);
  /* Draw text based on coordinates, pixel size, font index and colour */
// Sprite Utility
void drawSprite(Sprite* s, float x, float y,float w, float h, colour overlay, direction orientation);
  /* Draw image at specified location with fixed dimensions */

// Other Utility
void drawRectangle(float x, float y, float w, float h,colour overlay);
  /* Draw simple quads with specified coordinates and colours */
void setOverlay(colour overlay);
  /* Set current gl_colour */

#endif
