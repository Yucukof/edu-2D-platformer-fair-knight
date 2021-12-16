//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "update.h"

#define CALLER "update"

void updateFrame(int state) {

  // Refresh display at 60FPS

  // Call update() again in 'interval' milliseconds
  glutTimerFunc(1000/FRAMERATE, updateFrame, 0);

  // Redisplay frame
  glutPostRedisplay();
}

void updatePhase(int state) {

  // Capture game input layout
  keysGame(&game);
  // Check if context is valid
  if (game.state == GAME && !game.context->isSuspended && isValidContext(game.context)) {
    // update scrolling values according to player position
    updateScrolling(game.context);
    // Process game phases
    playerPhase(game.context);
    bulletPhase(game.context);
    monsterPhase(game.context);
  } else {
    logger(TRACE,CALLER,"unable to process status update!");
  }

  // Call update() again in 'interval' milliseconds
  glutTimerFunc(1000/MOVERATE, updatePhase, 0);

  // Redisplay frame
  glutPostRedisplay();
}

void updateFlashing(int state) {

  // Not Used, would enable sprites to flash (e.g. when hurt or with special status)

  // Call update() again in 'interval' milliseconds
  glutTimerFunc(1000/FLASHINGRATE, updateFlashing, 0);

  // Redisplay frame
  glutPostRedisplay();
}

void updateAnim(int state) {

  logger(TRACE,CALLER,"updating animation...");

  // Check if game has context and should be updated
  if (game.state == GAME && !game.context->isSuspended && isValidContext(game.context)) {
    // Make shortcut
    Sprite* s      = NULL;
    Coordinates* c = NULL;

    // Update player sprites
    Player* player = game.context->player;
    if (isValidPlayer(player)) {
      c = player->coord;
      s = player->sprites;
      player->sprites = s->next;
      c->w = (c->x + (float)s->width);
      c->h = (c->y + (float)s->height*.6);
    } else {
      logger(TRACE,CALLER,"unable to update player sprite!");
    }

    // Update monster sprites
    Monster* monsters = game.context->monsters;
    if (isValidMonsters(monsters)) {
      while(monsters != NULL) {
        s = monsters->sprites;
        monsters->sprites = s->next;
        c = monsters->coord;
        c->w = (c->x + (float)s->width);
        c->h = (c->y + (float)s->height*.75);
        monsters = monsters->next;
        }
    } else {
        logger(TRACE,CALLER,"Unable to ipdate monster sprite!");
    }

    // Update items sprites
    Item* items = game.context->items;
    if (isValidItems(items)) {
      while(items != NULL) {
        s = items->sprites;
        items->sprites = s->next;
        c = items->coord;
        c->w = (c->x + (float)s->width);
        c->h = (c->y + (float)s->height);
        items = items->next;
      }
    } else {
      logger(TRACE,CALLER,"unable to udpate item sprite");
    }
  }

  // Call update() again in 'interval' milliseconds
  glutTimerFunc(1000/ANIMSPEED, updateAnim, 0);

  // Redisplay frame
  glutPostRedisplay();

  logger(TRACE,CALLER,"animation updated.");
}
