//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "game.h"

#define CALLER "game"

Game game;

bool initGame(Game *game) {

  logger(INFO,CALLER,"game initialisation...\n");
  // Initialisation of Game structure
  game->resources      = readResources();
  game->support        = readSupport();
  game->menu           = initMenu();
  game->state          = MENU;
  game->context        = NULL;

  if (isValidGame(game)) {
    logger(INFO,CALLER,"game initialised.\n");
    return true;
  } else {
    logger(ERR,CALLER,"failed to initialise game!");
    exit(EXIT_FAILURE);
    return false;
  }
}

void startGame(Game *game) {
  // Check if game not already started
  if (game->state != GAME) {
    logger(INFO,CALLER,"game starting...\n");
    // load first level
    game->context = buildContext(game->resources,1);
    if (isValidContext(game->context)) {
      // Change game state to start game
      game->state = GAME;
    }
  }
}

void stopGame(Game *game) {
  if (game->state == GAME) {
    logger(INFO,CALLER,"game stopping...\n");
    // Return to menu
    game->context->isSuspended = true;
    registerScore(&game->context->score,&game->support->scores);
    writeScores(game->support->scores);
    game->state = MENU;
    // Remove context
    destroyContext(game->context);
    game->context = NULL;
  }
}

void exitGame(Game *game) {
  logger(INFO,CALLER,"exiting game...\n");
  // Free memory
  destroyResources(game->resources);
  // Exit fashionly
  closeLog();
  exit(EXIT_SUCCESS);
}

// TESTS

bool isValidGame(Game *game) {
  // Allocate space for test result
  bool isValid = true;
  // Check if passed input is valid
  if (game != NULL) {
    // Check game internal validity
    if (game->state < 0 || game->state >= MAXSTATE) {
      logger(ERR,CALLER,"invalid state");
      isValid = false;
    }
    // Validate game resources
    if (!isValidResources(game->resources)) {
      logger(ERR,CALLER,"invalid resources!");
      isValid = false;
    }
    // Validate support resources
    if (!isValidSupport(game->support)) {
      logger(ERR,CALLER,"invalid support!");
      isValid = false;
    }
    // Check if context should be checked and if exists, check its consistency
    if (game->context && !isValidContext(game->context)) {
      logger(WARN,CALLER,"invalid context!");
      isValid = false;
    }
  } else {
    // Otherwise, output error to screen
    logger(ERR,CALLER,"unable to validate game!");
  }
  // Return test value
  return isValid;
}
