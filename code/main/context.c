//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "context.h"

#define CALLER "context"

#define SCROLLBASE (UNIT*5)

// INITIALISATION

Context* initContext(void) {
  logger(INFO,CALLER,"initialising context...\n");
  // Initialise context
  Context* context = malloc(sizeof(Context));
  // Check if instantiation was successful
  if (context != NULL) {
    // Set initial values
    context->score       = initScore();
    context->level       = initLevel();
    context->player      = initPlayer();
    context->monsters    = NULL;
    context->bullets     = NULL;
    context->items       = NULL;
    context->isValid     = false;
    context->isSuspended = false;
    // Feedback
    logger(INFO,CALLER,"context initialised.\n");
  } else {
    //ELSE, print error
    logger(FAIL,CALLER,"failed to load context!");
  }
  // Return struct
  return context;
}

// FUNCTIONS

void updateScrolling(Context* context) {
  // CHeck if passed input is valid
  if (isValidContext(context) && context->player != NULL){ //isValidPlayer(context->player)) {
    // Retrieve current player position in order to adapt scrolling to its posisiont
    float scrollx = context->player->coord->x;
    float scrolly = context->player->coord->y;
    // Reduce scrolling in order to have non-immediate scrollig
    scrollx -= SCROLLBASE;
    scrolly -= SCROLLBASE;
    // Check if horizontal scrolling goes beyond map (outgoing left) and if so cease scrolling with border values
    if ( scrollx < 0) {
      scrollx = 0;
      // Below, same (outgoing right)
    } else if (scrollx > (context->level->width*UNIT) - DISPLAYWIDTH) {
        scrollx = (context->level->width*UNIT) - DISPLAYWIDTH;
    }
    // Check for vertical scrolling (outgoing up)
    if ( scrolly < 0) {
      scrolly = 0;
      // and down
    } else if (scrolly > (context->level->height*UNIT) - DISPLAYHEIGHT + BOTTOMBANNER) {
        scrolly = (context->level->height*UNIT) - DISPLAYHEIGHT + BOTTOMBANNER;
    }
    // Save computed scrolling values
    context->level->scrollx = scrollx;
    context->level->scrolly = scrolly;
  } else {
    // If input is invalid, then output error (scrolling is not changed)
    logger(ERR,CALLER,"unable to compute scrolling!");
  }
}

// TESTS

bool isValidContext(Context* context) {
  // Allocate memory space for test result
  bool isValid = false;
  // Check if passed parameter is valid
  if (context != NULL) {
    // Check if context is said valid
    if (context->isValid) {
      isValid = true;
      // Perform checks and set flag off if needed
        if (!isValidPlayer(context->player)) {
          isValid = false;
        }
        if (context->monsters && !isValidMonsters(context->monsters)) {
          isValid = false;
        }
        if (context->items && !isValidItems(context->items)) {
          isValid = false;
        }
        if (context->bullets && !isValidBullets(context->bullets)) {
          isValid = false;
        }
    } else {
      // Otherwise output error
      logger(WARN,CALLER,"context was declared invalid.");
    }
  } else {
    // Output other error
    logger(ERR,CALLER,"unable to validate context!");
  }
  // Return test value
  return isValid;
}

// DESTROY

void destroyContext(Context* context) {
  // Check if passed input is valid
  if (isValidContext(context)) {
    // Proceed to destrution
    logger(WARN,CALLER,"destroying context...\n");
    // Destroy context substructures
    destroyScore(context->score);
    destroyPlayer(context->player);
    destroyMonsters(context->monsters);
    destroyItems(context->items);
    destroyBullets(context->bullets);
    // Free context from memory
    free(context);
    // Output success to screen
    logger(WARN,CALLER,"context destroyed.\n");
  } else {
    // Output error to screen
    logger(ERR,CALLER,"unable to destroy context!");
  }
}
