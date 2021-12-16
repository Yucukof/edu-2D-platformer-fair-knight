//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "controls.h"

#define CALLER "controls"

// FUNCTIONS

void keysGame(Game* game) {
  // Check if input is valid
  if (isValidGame(game)) {
    // Check current state to choose appropriate keys handler
    if (game->state == GAME) {
      // Use game handler
      keysContext(game->context);
    } else {
      // Otherwise, default to menu handler
      keysMenu(game->menu);
    }
    // In any way, allow resources handler
    keysResources(game->resources);
  } else {
    logger(FAIL,CALLER,"unable to process game key input!!");
  }
}

void keysMenu(Menu* menu) {

  if (isValidMenu(menu)) {

    if (game.menu->id == HOMESCREEN) {
      if (isActiveKey('z')) {
        if (isValidFocus(menu->focus-1)) {
          setFocus(menu,menu->focus-1);
        } else {
          setFocus(menu,MAXMENU-1);
        }
        deactivateKey('z');
      } else if (isActiveKey('s') || isActiveKey('\t')) {
        if (isValidFocus(menu->focus+1)) {
          setFocus(menu,menu->focus+1);
        } else {
          setFocus(menu,1);
        }
        deactivateKey('s'); deactivateKey('\t');
      }

      if (isActiveKey('\r')) {
        if (game.menu->focus == PLAY) {
          startGame(&game);
        } else
        if (game.menu->focus == EXIT) {
          exitGame(&game);
        } else
        if (!isMenuFocused(menu)) {
          chooseMenu(menu);
          deactivateKey(' ');
        }
      }
    }

    if (isActiveKey('m')) {
      printMenu(menu);
      deactivateKey('m');
    }

    if (isActiveKey(27)) {
      if (game.menu->id != HOMESCREEN) {
        setMenu(game.menu,0);
        setFocus(game.menu,0);
      } else {
        exitGame(&game);
      }
      deactivateKey(27);
    }
  } else {
    logger(ERR,CALLER,"unable to process key menu!");
  }
}

void keysResources(Resources* resources) {
  if (isValidResources(resources)) {
    if (isActiveKey('a')) {
      printAssets(game.resources->assets);
      deactivateKey('a');
    }
  } else {
    logger(ERR,CALLER,"unable to process key resources!");
  }
}

void keysContext(Context* context){
  /* Check if context is valid */
  if (isValidContext(context)) {

    // Performing action irrelevant to pause

    // CONTEXT

      if (isActiveKey('p')) {
        // Checking if game should be paused/unpaused.
        if (!context->isSuspended) {
          logger(INFO,CALLER,"pausing game.");
          context->isSuspended = true;
        } else
        if (game.context->isSuspended) {
          logger(INFO,CALLER,"unpausing game.");
          context->isSuspended = false;
        }
        deactivateKey('p');
      }

      if (isActiveKey('r')) {
        // Reload context and level
        reloadContext(&game.context,game.resources);
        deactivateKey('r');
      }

      if (isActiveKey(27)) {
        // 27 is ESCAPE character
        stopGame(&game);
        deactivateKey(27);
      }

    // INFO - List and display information from structures belonging to context
      if (isActiveKey('h')) {
        printPlayer(context->player);
        deactivateKey('h');
      }

      if (isActiveKey('m')) {
        printMonsters(context->monsters);
        deactivateKey('m');
      }

      if (isActiveKey('i')) {
        printItems(context->items);
        deactivateKey('i');
      }

      if (isActiveKey('b')) {
        printBullets(context->bullets);
        deactivateKey('b');
      }

      if (isActiveKey('l')) {
        printMap(context->level);
        deactivateKey('l');
      }

      if (isActiveKey('k')) {
        printScore(context->score);
        deactivateKey('k');
      }

    // performing actions when game is running
    if (!context->isSuspended) {

      // PLAYER - all action relevant to game: movement, bullet firing,...
        Coordinates* coord = game.context->player->coord;

        if (isActiveKey('z')) {
          setMoving(coord,UP);
        } else {
          stopMoving(coord,UP);
        }

        if (isActiveKey('d')) {
          setMoving(coord,RIGHT);
        } else {
          stopMoving(coord,RIGHT);
        }

        if (isActiveKey('q')) {
          setMoving(coord,LEFT);
        } else {
          stopMoving(coord,LEFT);
        }

        if (isActiveKey('s')) {
          setMoving(coord,DOWN);
        }
        if (isActiveKey(' ')) {
          float x = 0;
          if (coord->look == LEFT) {
            x = coord->x;
          } else {
            x = coord->w;
          }
          float y = (coord->y+UNIT/4);
          Bullet* bullet = newBullet(x,y,coord->look);
          setBulletSprites(bullet, game.resources->assets);
          appendBullet(bullet,&game.context->bullets);
          deactivateKey(' ');
        }
      }
  } else {
    logger(ERR,CALLER,"unable to process key context!");
  }
}
