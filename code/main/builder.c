//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "builder.h"

#define CALLER "builder"

// FUNCTIONS

Context* buildContext(Resources* resources, int levelIndex) {
  // Reference for ASCII character indexes: http://www.asciitable.com
  char buffer[MAXSTRING];
  // Allocate memory space for builder summary
  int monsters = 0
    , items    = 0;

  // make shortcut to context
  Context* context = initContext();

  logger(INFO,CALLER,"building context...\n");
  // Load level and map
  context->level = loadLevel(resources,levelIndex);
  // Initialise test variables
  bool playerLoaded  = false;
  bool exitAvailable = false;
  // Read through map from context
  for (int h = 0; h < context->level->height; h++) {
    for (int w = 0; w < context->level->width; w++) {
      // Get current character to check its value
      char token = context->level->map[h][w];
      // If is a player token
      if (token == '@') {
        // and player has not yet been initialised
        if (!playerLoaded) {
          // Set player assets
          // THEN update player's location on map
          logger(INFO,CALLER,"locating player...");
          Coordinates* coord = context->player->coord;
          coord->x    = w*UNIT;
          coord->y    = h*UNIT;
          coord->look = RIGHT;
          setPlayerSprites(context->player,HERO,KNIGHT,IDLE,resources->assets);
          // confirm player is initialised on map
          playerLoaded = true;
          // Feedback: player's location
          sprintf(buffer,"player coordinates: [%i,%i].",w+1,h+1);
          logger(INFO,CALLER,buffer);
        } else {
        // Signal that player was already located
        logger(WARN,CALLER,"duplicate player found.");
        // Clear duplicate player from map
        context->level->map[h][w] = '*';
      }
    // If not a player token
    } else
      // but is an exit token (to change level)
      if (token == '!' ) {
        logger(INFO,CALLER,"map exit set.");
        exitAvailable = true;
      } else if (token == '?' ) {
        logger(INFO,CALLER,"secret exit located.");
      } else
      // but a monster token
      if (token >= 65 && token <= 90 ) {
        // THEN generate new monster on map
        context->monsters = appendMonster(spawnMonster((int)token-65,w,h,resources),context->monsters);
        monsters++;
      } else
      // but a item token
      if (token >= 48 && token <= 57 ) {
        // Make item
        context->items = appendItem(spawnItem((int)token-48,w,h,resources),context->items);
        items++;
      }
    }
  }
  // IF no player at all is found, THEN context is not complete
  if (!playerLoaded) {
    logger(ERR,CALLER,"player not found in map!");
    context->isValid = false;
    // return context nevertheless
    return context;
  }
  // IF no exit is available, THEN context is incomplete
  if (!exitAvailable) {
    logger(ERR,CALLER,"exit not found in map!");
    context->isValid = false;
    // return context nevertheless
    return context;
  }
  // If all texts are passed, then context is said valid
  sprintf(buffer,"context built.\n\t\t~ spawned %i monsters\n\t\t~ placed %i items",monsters,items);
  logger(INFO,CALLER,buffer);
  context->isValid = true;
  // Return context
  return context;
}

void reloadContext(Context** context, Resources* resources) {
  logger(WARN,CALLER,"reloading context...");
  // Retrieve context address
  Context* toDestroy = *context;
  // Set context off
  toDestroy->isValid = false;
  // Save level index
  int index = toDestroy->level->index;
  // Rebuild context
  Context* newContext = buildContext(resources,index);
  // validate new context
  newContext->isValid = true;
  // activate new context
  *context = newContext;
  logger(INFO,CALLER,"context reloaded.\n");
  // Destroy previous instance
  destroyContext(toDestroy);
}

void updateContext(Context** context, Resources* resources) {
  logger(WARN,CALLER,"reloading context...");
  // Retrieve context address
  Context* toDestroy = *context;
  Player* player = (*context)->player;
  // Set context off
  toDestroy->isValid = false;
  // Get next level index
  int index = (*toDestroy->level->next).index;
  // Rebuild context
  Context* newContext = buildContext(resources,index);
  // transfer scores and player from old to new context
  setCoord(player->coord,newContext->player->coord->x,newContext->player->coord->y,RIGHT);
  destroyPlayer(newContext->player);
  newContext->player = player;
  toDestroy->player = NULL;
  destroyScore(newContext->score);
  newContext->score = toDestroy->score;
  toDestroy->score = NULL;
  // validate new context
  newContext->isValid = true;
  // activate new context
  *context = newContext;
  logger(INFO,CALLER,"context reloaded.\n");
  // Destroy previous instance
  destroyContext(toDestroy);
}

Monster* spawnMonster(mob type, float x, float y, Resources* resources) {
  // Allocate memory space for return object
  Monster* m = NULL;
  // Check if monster is valid (cf. enum)
  if (type >=0 && type < MAXMOB) {
    // Find relevant monster in assets
    Monster* mob = resources->monsters;
    while (mob != NULL && mob->type != type) {
        mob = mob->next;
    }
    // IF matching monster is found
    if (mob != NULL) {
      // THEN copy its value onto new monster
      m = initMonster();
      strcpy(m->name,     mob->name);
      m->type           = mob->type;
      m->baseHealth     = mob->baseHealth;
      m->currentHealth  = mob->baseHealth;
      m->baseAttack     = mob->baseAttack;
      m->currentAttack  = mob->baseAttack;
      m->baseSpeed      = mob->baseSpeed;
      m->currentSpeed   = mob->baseSpeed;
      m->sight          = mob->sight;
      m->reach          = mob->reach;
      m->scoreValue     = mob->scoreValue;
      m->coord->x       = x*UNIT;
      m->coord->y       = y*UNIT;
      m->coord->look    = RIGHT;
      // Load sprites and check if sucessful
      if(!setMonsterSprites(m,MOB,mob->type,IDLE,resources->assets)){
        logger(ERR,CALLER,"failed to load sprites");
      }
    } else {
      // Output error
      logger(ERR,CALLER,"Invalid monster spawn detected!");
    }
  }
  return m;
}

Item* spawnItem(obj type, float x, float y, Resources* resources) {
  // ALlocate memory space for return object
  Item* item = NULL;
  // Check if item is valid (cf. enum)
  if (type >=0 && type < MAXOBJ) {
    // Make cursor to read through resources
    Item* cursor = resources->items;
    // Cycle through chain
    while (cursor != NULL && cursor->type != type) {
        cursor = cursor->next;
    }
    // Check if matching item was found
    if (cursor != NULL) {
      // THEN copy its value onto new item
      item = initItem();
      strcpy(item->name,   cursor->name);
      item->type         = cursor->type;
      item->effect       = cursor->effect;
      item->value        = cursor->value;
      item->coord->x     = x*UNIT;
      item->coord->y     = y*UNIT;
      item->coord->look  = RIGHT;
      // Load sprites and check if sucessful
      if(!setItemSprites(item,OBJ,cursor->type,IDLE,resources->assets)){
        logger(ERR,CALLER,"failed to load sprites");
      }
    }
  } else {
    logger(ERR,CALLER,"invalid spawn detected!");
  }
  return item;
}
