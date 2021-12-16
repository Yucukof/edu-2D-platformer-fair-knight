//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _game_
#define _game_ "game.h"

#include "lib.h"
#include "context.h"
#include "resources.h"
#include "support.h"
#include "menu.h"
#include "builder.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Frame structure
structdef(Game){
  Resources* resources;
    Support* support;
    Context* context;
       Menu* menu;
      state  state;
};

// GLOBAL VARIABLE

Game game;

// INITIALISATION

bool initGame(Game *game);

// FUNCTIONS

void startGame(Game *game);
void stopGame(Game *game);
void exitGame(Game *game);

// TESTS
bool isValidGame(Game *game);
  /* Check if game is ready */

// UTILITY

#endif
