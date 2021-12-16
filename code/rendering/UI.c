//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "UI.h"

#define CALLER "UI"

void draw(void) {
  // clear buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Reset to black
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glLoadIdentity();
  // Check current game state to choose drawing
  if (game.state == MENU) {
    // Draw current menu
    drawMenu(&game);
  } else
  if ((game.state == GAME) && isValidContext(game.context)) {
    // draw game only if context is valid
    scrollx = game.context->level->scrollx;
    scrolly = game.context->level->scrolly;
    drawUI(&game);
  } else {
    // Otherwise output erro to screen
    logger(FAIL,CALLER,"invalid state");
  }
  // swap buffers
  glutSwapBuffers();
}

// MENU

void drawMenu(Game *game) {
  // Check if passed input is valid
  if (isValidGame(game)) {
    // Check if menu is valid
    if (isValidMenu(game->menu)) {
      // Draw appropriate menu
      switch (game->menu->id) {

        case HOMESCREEN:
        drawHome(game);
        break;

        case SCORES:
        drawScores(game);
        break;

        case CREDITS:
        drawCredits(game);
        break;

        case RULES:
        drawRules(game);
        break;

        // Else, default to home
        default:
        drawHome(game);
      }
    } else {
      // Otherwise, draw home as defautlt and reset menu
      logger(ERR,CALLER,"Unable to draw menu!");
      setMenu(game->menu,1);
      drawHome(game);
    }
  } else {
    // Output error and exit
    logger(FAIL, CALLER,"unable to draw game menu!");
  }
}

void drawHome(Game *game) {

  drawCenteredText("Fair Knight Demo"
                  ,DISPLAYHEIGHT*5/8,40,6,RED
                  ,game->resources->fonts);

  colour overlay = WHITE;

  if (game->menu->focus == 1) {
    overlay = RED;
  }
  drawCenteredText("Play",DISPLAYHEIGHT*.49,18,5,overlay,game->resources->fonts);
  overlay = WHITE;

  if (game->menu->focus == 2) {
    overlay = RED;
  }
  drawCenteredText("Rules",DISPLAYHEIGHT*.42,18,5,overlay,game->resources->fonts);
  overlay = WHITE;

  if (game->menu->focus == 3) {
    overlay = RED;
  }
  drawCenteredText("Scores",DISPLAYHEIGHT*.35,18,5,overlay,game->resources->fonts);
  overlay = WHITE;

  if (game->menu->focus == 4) {
    overlay = RED;
  }
  drawCenteredText("Credits",DISPLAYHEIGHT*.28,18,5,overlay,game->resources->fonts);
  overlay = WHITE;

  if (game->menu->focus == 5) {
    overlay = RED;
  }
  drawCenteredText("Exit",DISPLAYHEIGHT*.21,18,5,overlay,game->resources->fonts);
  overlay = WHITE;

  Sprite* s = NULL;

  s = loadAssetSprites(BUTTON,UNAMUR,IDLE,game->resources->assets);
  drawSprite(s,DISPLAYWIDTH*2/10,DISPLAYHEIGHT/10,UNIT*2*1.16,UNIT*2,WHITE,RIGHT);

  s = loadAssetSprites(BUTTON,OPENGL,IDLE,game->resources->assets);
  drawSprite(s,DISPLAYWIDTH*7/10,DISPLAYHEIGHT/10,UNIT*2*2.4,UNIT*2,WHITE,RIGHT);

}

void drawCredits(Game *game) {

  drawCanevas(game);

  drawCenteredText("Credits"
                  ,DISPLAYHEIGHT*9/16,32,5,GREY
                  ,game->resources->fonts);

  int i = 0;
  while (game->support->credits[i] != NULL) {
    drawText(game->support->credits[i]
                    ,DISPLAYWIDTH/10
                    ,(DISPLAYHEIGHT*4/8)-i*16,10,5,WHITE
                    ,game->resources->fonts);
    i++;
  }
}

void drawScores(Game *game) {

  drawCanevas(game);

  drawCenteredText("Scores"
                  ,DISPLAYHEIGHT*9/16,32,5,GREY
                  ,game->resources->fonts);

  Score* scores = game->support->scores;
  int i = 0; // Score counter
  // Display scores on screen
  drawText("POINTS",DISPLAYWIDTH/10,(DISPLAYHEIGHT*.5)-16,18,5,GREY,game->resources->fonts);
  drawText("KILLS",DISPLAYWIDTH*3/10,(DISPLAYHEIGHT*.5)-16,18,5,GREY,game->resources->fonts);
  drawText("PLAYER",DISPLAYWIDTH*5/10,(DISPLAYHEIGHT*.5)-16,18,5,GREY,game->resources->fonts);

  if (scores != NULL && isValidScores(scores)) {
    Score* cursor = scores;
    while (cursor) {
      char buffer[MAXSTRING];
      sprintf(buffer,"%i",cursor->points);
      drawText(buffer,DISPLAYWIDTH/10,(DISPLAYHEIGHT*.4)-i*16,18,5,GREY,game->resources->fonts);
      sprintf(buffer,"%i",cursor->kills);
      drawText(buffer,DISPLAYWIDTH*3/10,(DISPLAYHEIGHT*.4)-i*16,18,5,GREY,game->resources->fonts);
      sprintf(buffer,"%s",cursor->pseudo);
      drawText(buffer,DISPLAYWIDTH*5/10,(DISPLAYHEIGHT*.4)-i*16,18,5,GREY,game->resources->fonts);
      cursor = cursor->next;
      i++;
    }
  } else {
    logger(TRACE,CALLER,"invalid scores");
  }
}

void drawRules(Game *game) {

  drawCanevas(game);

  drawCenteredText("Rules"
                  ,DISPLAYHEIGHT*9/16,32,5,GREY
                  ,game->resources->fonts);

  Sprite* s = loadAssetSprites(BUTTON,BUTTON_EMPTY,IDLE,game->resources->assets);

  float x = DISPLAYWIDTH*.3
      , y = DISPLAYHEIGHT*.4;
  drawSprite(s,x,y,UNIT*2,UNIT*2,WHITE,RIGHT);
  drawText("Z",x+UNIT/2,y+UNIT/2,28,5,BLACK,game->resources->fonts);
  y = DISPLAYHEIGHT*.275;
  drawSprite(s,x,y,UNIT*2,UNIT*2,WHITE,RIGHT);
  drawText("S",x+UNIT/2,y+UNIT/2,28,5,BLACK,game->resources->fonts);
  x = DISPLAYWIDTH*.225;
  drawSprite(s,x,y,UNIT*2,UNIT*2,WHITE,RIGHT);
  drawText("Q",x+UNIT/2,y+UNIT/2,28,5,BLACK,game->resources->fonts);
  x = DISPLAYWIDTH*.375;
  drawSprite(s,x,y,UNIT*2,UNIT*2,WHITE,RIGHT);
  drawText("D",x+UNIT/2,y+UNIT/2,28,5,BLACK,game->resources->fonts);
  x = DISPLAYWIDTH*.6;
  drawSprite(s,x,y,UNIT*6,UNIT*2,WHITE,RIGHT);
  drawText("Space",x+UNIT/2,y+UNIT/2,28,5,BLACK,game->resources->fonts);

  drawText("Move",DISPLAYWIDTH*.3,y-UNIT,12,5,BLACK,game->resources->fonts);
  drawText("Throw knives to hurt ennemies",x-UNIT*2,y-UNIT,12,5,BLACK,game->resources->fonts);

}

void drawCanevas(Game *game) {

  drawCenteredText("Fair Knight Demo"
                  ,DISPLAYHEIGHT*6/8,40,6,WHITE
                  ,game->resources->fonts);

  Sprite* s = loadAssetSprites(BUTTON,ARROW_LEFT,IDLE,game->resources->assets);
  drawSprite(s,10,DISPLAYHEIGHT*7/8,16,16,WHITE,RIGHT);
  drawText("[ESC]"
           ,32
           ,(DISPLAYHEIGHT*7/8),12,5,WHITE
           ,game->resources->fonts);

   s = loadAssetSprites(BUTTON,WOODEN,IDLE,game->resources->assets);
   drawSprite(s,DISPLAYWIDTH*1/20,DISPLAYHEIGHT*1/20,DISPLAYWIDTH*.9,DISPLAYHEIGHT*.65,WHITE,RIGHT);
}

// GAME

void drawUI(Game *game) {
  // Check if game is valid
  if (isValidGame(game)) {
    drawBack(game);
    drawField(game);
    drawParty(game);
    drawFront(game);

    if (game->context->isSuspended) {
      drawPause(game);
    }
    if (isDeadPlayer(game->context->player)) {
      game->context->isSuspended = true;
      drawGameOver(game);
    }
    int x = game->context->player->coord->x/UNIT;
    int y = game->context->player->coord->y/UNIT;
    if (game->context->level->map[y][x] == '!') {
      game->context->isSuspended = true;
      if (game->context->level->next != NULL) {
        updateContext(&game->context,game->resources);
      } else {
        drawVictory(game);
      }
    }
  }
}

void drawBack(Game *game) {

  Sprite* s = initSprite();
  s = loadAssetSprites(BACK,CHURCH,IDLE,game->resources->assets);
  drawBackground(s,WHITE);
}

void drawField(Game *game) {
  // Allocate space for asset sprite reference
  Sprite* s = NULL;
  // Allocate space for tile info
  char token;
  // Set map boundaries
  int x = 0
    , y = 0
    , width  = game->context->level->width
    , height = game->context->level->height;

  // Read through map
  while (y < height) {
    while (x < width) {
      // Get map tile token
      token = game->context->level->map[y][x];
      // Load specific sprites depending on token
      if (token == '#' || token == '=') {
        s = loadAssetSprites(TILE,WALL,IDLE,game->resources->assets);
      }
      if (token == '<') {
        s = loadAssetSprites(TILE,BANNER_BLUE,IDLE,game->resources->assets);
      }
      if (token == '>') {
        s = loadAssetSprites(TILE,BANNER_RED,IDLE,game->resources->assets);
      }
      if (token == '!') {
        s = loadAssetSprites(TILE,DOOR,IDLE,game->resources->assets);
      }
      // Draw corresponding tile with banner offset
      if (s && isValidSprite(s)) {
        drawTile(s,(float)x,(float)y);
      }
      // move left on map
      x++;
      // reset sprite
      s = NULL;
    }
    // move up on map and restart row
    x=0;
    y++;
  }
}

void drawParty(Game *game) {
  // Check if passed input is valid
  if (isValidGame(game)) {
    // Draw all characters
    drawPlayer(game->context->player,WHITE);

    Bullet* bullets = game->context->bullets;
    while(bullets != NULL) {
      drawBullet(bullets);
      bullets = bullets->next;
    }

    Monster* mobs = game->context->monsters;
    while(mobs != NULL) {
      drawMonster(mobs,WHITE);
      mobs = mobs->next;
    }

    Item* items = game->context->items;
    while(items != NULL) {
      drawItem(items);
      items = items->next;
    }
  } else {
    // Output error
    logger(ERR,CALLER,"unable to draw party!");
  }
}

void drawFront(Game *game) {
  char buffer[MAXSTRING];
  Sprite* s;

  // LEFT PANEL
  s = loadAssetSprites(BUTTON,WOODEN,IDLE,game->resources->assets);
  drawSprite(s,DISPLAYWIDTH/20  ,DISPLAYHEIGHT/20
              ,DISPLAYWIDTH/3   ,DISPLAYHEIGHT/10
              ,WHITE,RIGHT);

  s = loadAssetSprites(OBJ,COIN,IDLE,game->resources->assets);
  drawSprite(s,DISPLAYWIDTH*8/100  ,DISPLAYHEIGHT*8/100
              ,s->width*1.5        ,s->height*1.5
              ,WHITE,RIGHT);

  s = loadAssetSprites(OBJ,SKULL,IDLE,game->resources->assets);
  drawSprite(s,DISPLAYWIDTH*28/100  ,DISPLAYHEIGHT*7/100
              ,s->width*1.5         ,s->height*1.5
              ,WHITE,RIGHT);
  sprintf(buffer,"%i",game->context->score->points);
  drawText(buffer,DISPLAYWIDTH*12/100,DISPLAYHEIGHT/15
                 ,18,2,WHITE,game->resources->fonts);

  sprintf(buffer,"%i",game->context->score->kills);
  drawText(buffer,DISPLAYWIDTH*32/100,DISPLAYHEIGHT/15
                 ,18,2,WHITE,game->resources->fonts);

  // RIGHT PANEL
  s = loadAssetSprites(BUTTON,WOODEN,IDLE,game->resources->assets);
  drawSprite(s,(DISPLAYWIDTH*19/20)-(DISPLAYWIDTH/3),DISPLAYHEIGHT/20
              ,DISPLAYWIDTH/3                       ,DISPLAYHEIGHT/10
              ,WHITE,RIGHT);

  // Draw player's potential and actual health points
  // draw consumed health points
  s = loadAssetSprites(OBJ,HEART_EMPTY,IDLE,game->resources->assets);
  for (size_t i = 0; i < game->context->player->baseHealth; i++) {
    if (i >= game->context->player->baseHealth - game->context->player->currentHealth) {
      // Draw remaining health points
      s = loadAssetSprites(OBJ,HEART_FULL,IDLE,game->resources->assets);
    }
    drawSprite(s,(DISPLAYWIDTH*39/40)-(DISPLAYWIDTH/3)+(s->width*i*2),DISPLAYHEIGHT*3/50
                ,s->width*1.5                                        ,s->height*1.5
                ,WHITE,RIGHT);
  }

  // Button helper
  s = loadAssetSprites(BUTTON,ARROW_LEFT,IDLE,game->resources->assets);
  drawSprite(s,10,DISPLAYHEIGHT*7/8,16,16,WHITE,RIGHT);
  drawText("[ESC]"
           ,32
           ,(DISPLAYHEIGHT*7/8),12,5,WHITE
           ,game->resources->fonts);
  s = loadAssetSprites(BUTTON,PAUSE,IDLE,game->resources->assets);
  drawSprite(s,DISPLAYWIDTH-10-UNIT,DISPLAYHEIGHT*7/8,16,16,WHITE,RIGHT);
  drawText("[P]"
           ,DISPLAYWIDTH-32-UNIT
           ,(DISPLAYHEIGHT*7/8),12,5,WHITE
           ,game->resources->fonts);
}

void drawPause(Game *game) {

  drawRectangle(0,0,DISPLAYWIDTH,DISPLAYHEIGHT,BLUR);
  drawCenteredText("PAUSE"
                  ,DISPLAYHEIGHT/2,80,0,WHITE
                  ,game->resources->fonts);
}

void drawGameOver(Game *game) {

  drawRectangle(0,0,DISPLAYWIDTH,DISPLAYHEIGHT,BLACK);
  drawCenteredText("GAME OVER"
                  ,DISPLAYHEIGHT/2,60,0,RED
                  ,game->resources->fonts);
  drawCenteredText("Press escape to quit."
                  ,DISPLAYHEIGHT/3,30,0,RED
                  ,game->resources->fonts);
}

void drawVictory(Game *game) {

  drawRectangle(0,0,DISPLAYWIDTH,DISPLAYHEIGHT,BLACK);
  drawCenteredText("Victory"
                  ,DISPLAYHEIGHT/2,60,0,YELLOW
                  ,game->resources->fonts);
  drawCenteredText("Press escape to quit."
                  ,DISPLAYHEIGHT/3,30,0,WHITE
                  ,game->resources->fonts);
}
