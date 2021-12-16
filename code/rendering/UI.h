//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _UI_
#define _UI_ "UI.h"

#include "lib.h"
#include "game.h"
#include "draw.h"
#include "sprites.h"

void draw(void);
  /* Main fuction, reorients to the right drawing method (MENU or GAME) */

// MENU

void drawMenu(Game *game);

void drawHome(Game *game);
void drawCanevas(Game *game);
void drawCredits(Game *game);
void drawScores(Game *game);
void drawRules(Game *game);

// GAME

void drawUI(Game *game);

void drawBack(Game *game);
void drawField(Game *game);
void drawParty(Game *game);
void drawFront(Game *game);

void drawPause(Game *game);
void drawGameOver(Game *game);
void drawVictory(Game *game);

#endif
