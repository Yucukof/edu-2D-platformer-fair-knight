//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "motion.h"

#define CALLER "motion"

// FUNCTION

void motion(Coordinates* coord, int speed, Context* context) {
  // do movement
  move(coord,speed,context);
  // If is falling, do fall movement
  if (isFalling(coord,context)) {
    fall(coord,context);
  }
}

void setMoveRandom(Coordinates* origin, Context* context) {
  Moving* m = origin->moving;
  // Check if falling
  if (!isFalling(origin, context)) {
    // If not, then check if timer ran out
    if (m->timer <= 0) {
      // If yes, then reset random timer
      m->timer = MOVETIMER*(rand()%15);
      // Define random direction (left, right or not moving)
      origin->look = rand()%2;
      // Depending on direction
      if (origin->look == LEFT || origin->look == RIGHT) {
        // Start moving
        setMoving(origin,origin->look);
      } else {
        // Stop moving completely
        resetMoving(origin);
      }
    } else
    // If timer is running, decrement and check if 0
    if (--m->timer <= 0) {
      resetMoving(origin);
    }
  }
}

void move(Coordinates* origin, int speed, Context* context) {
  // Make shortcut
  Moving* m = origin->moving;
  // check if can jump or descend
  if(!isFalling(origin,context)) {
    // jump
    if (m->up) {
      jump(origin,speed,context);
    } else {
      // Fall
      if (m->down) {
        steps(origin,speed,DOWN,context);
      }
    }
  }
  // Move left or right if needed (regardless of jump/fall)
  if (m->right) {
    steps(origin,speed,RIGHT,context);
  } else {
    if (m->left) {
      steps(origin,speed,LEFT,context);
    }
  }
}

void jump(Coordinates* origin, int speed, Context* context) {
  Moving* m = origin->moving;
  // check if jump timer is not set
  if (m->timer == 0) {
    // If not, set it to allow jump
    m->timer = MOVETIMER;
  } else {
    // else, decrement it and if zero
    if (--m->timer <= 0) {
      // Stop jump
      fall(origin,context);
    }
  }
  // make jump
  steps(origin,speed + m->timer/1.25,UP, context);
}

void fall(Coordinates* origin, Context* context) {
  // stop jumping if needed
  stopMoving(origin,UP);
  // start falling
  setMoving(origin,DOWN);
  // do fall movement
  steps(origin,UNIT/4,DOWN,context);
}

void steps(Coordinates* origin, int speed, direction orientation, Context* context) {
  // Repeat step according to speed
  for (size_t i = 0; i < speed; i++) {
    step(origin,orientation,context);
  }
}

void step(Coordinates* origin,direction orientation,Context* context) {
  Coordinates* next = nextCoord(origin, orientation,context);
  // Check if next step is available
  if (!isBlocked(next,orientation,context)
   && !isCollidingWithMonsters(origin,next,context->monsters)
   && !isCollidingWithPlayer(origin,next,context->player)) {
    // Update look
    setLook(next,orientation);
    // switch to next coordinate
    swapCoord(origin,next,context);
  } else {
    // stop movement because of block
    stopMoving(origin,orientation);
  }
  // Free next coordinates (not useful anymore)
  destroyCoordinates(next);
}

Coordinates* nextCoord(Coordinates* origin,direction orientation, Context* context) {
  // Check if passed parameters are valid
  if (origin != NULL && context != NULL) {
    // Check if level is null
    if (context->level != NULL) {
      // Initialise next coordinates and copy origin values
      Coordinates* next = initCoord();
      next->x      = origin->x;
      next->y      = origin->y;
      next->w      = origin->w;
      next->h      = origin->h;
      // Set next coordinates estimate
      switch (orientation) {
        case LEFT:
          next->x-=1;
          next->w-=1;
          break;
        case RIGHT:
          next->x+=1;
          next->w+=1;
          break;
        case UP:
          next->y+=1;
          next->h+=1;
          break;
        case DOWN:
          next->y-=1;
          next->h-=1;
          break;
        default:{
          logger(WARN,CALLER,"invalid direction received.");
          return next;
        }
      }
      // Adapt estimate to level boundaries
      amendCoord(next,context);
      // Return next coordinates
      return next;
    } else {
      // Output error
      logger(ERR,CALLER,"Invalid swap attempted:\n\t\t-> level not found");
      return NULL;
    }
  } else {
    // Output detailed error
    char buffer[MAXSTRING];
    strcpy(buffer,"invalid next attempted: ");
    if (origin == NULL) {
      concat(buffer,buffer,"\n\t\t~> null origin coord");
    }
    if (context == NULL) {
      concat(buffer,buffer,"\n\t\t~> invalid context");
    }
    logger(ERR,CALLER,buffer);
    // Return null to cascade error
    return NULL;
  }
}

void amendCoord(Coordinates* coord, Context* context){
  Level* level = context->level;
  // Check X origin
  if (coord->x < 0) {
    coord->x = ((float)level->width-1)*UNIT;
  } else {
    if ((int)coord->x/UNIT >= level->width) {
      coord->x = 0;
    }
  }
  // Check X width
  if (coord->w < 0) {
    coord->w = ((float)level->width-1)*UNIT;
  } else {
    if ((int)coord->w/UNIT >= level->width) {
      coord->w = 0;
    }
  }
  // Check Y origin
  if (coord->y < 0) {
    coord->y = ((float)level->height-1)*UNIT;
  } else {
    if ((int)coord->y/UNIT >= level->height) {
      coord->y = 0;
    }
  }
  // Check Y height
  if (coord->h < 0) {
    coord->h = ((float)level->height-1)*UNIT;
  } else {
    if ((int)coord->h/UNIT >= level->height) {
      coord->h = 0;
    }
  }
}

void swapCoord(Coordinates* origin,Coordinates* next,Context* context) {
  // Check if input is valid
  if (isValidCoord(origin) && isValidCoord(next) && context != NULL && context->level != NULL) {
    // Update origin with next values
    origin->x      = next->x;
    origin->y      = next->y;
    origin->w      = next->w;
    origin->h      = next->h;
  } else {
    // Output detailed error
    char buffer[MAXSTRING];
    strcpy(buffer,"invalid swap attempted: ");
    if (context == NULL) {
      concat(buffer,buffer,"\n\t\t~> invalid context");
    }
    if (context->level == NULL) {
      concat(buffer,buffer,"\n\t\t~> invalid level");
    }
    if (origin == NULL) {
      concat(buffer,buffer,"\n\t\t~> null origin coord");
    }
    if (next == NULL) {
      concat(buffer,buffer,"\n\t\t~> null next coord");
    }
    logger(ERR,CALLER,buffer);
  }
}

// TESTS

bool isFalling(Coordinates* origin, Context* context) {
  // Put below tile in memory
  Coordinates* ground = nextCoord(origin,DOWN,context);
  // determine if above ground and not jumping
  if (!isBlocked(ground,DOWN,context) && !origin->moving->up) {
    // IF not, allow fall
    return true;
  } else {
    // Otherwise keep going
    return false;
  }
}

bool isBlocked(Coordinates* next, direction orientation, Context* context) {
  // Convert coord coordinates to map coordinates
  int x = (int)next->x / UNIT
    , y = (int)next->y / UNIT
    , w = (int)next->w / UNIT
    , h = (int)next->h / UNIT;
  // Get map char value for testing
  char x1 = context->level->map[y][x];
  char x2 = context->level->map[y][w];
  char y1 = context->level->map[h][x];
  char y2 = context->level->map[h][w];
  // Testing value depending on direction
  if (orientation == DOWN && (int)next->y % UNIT == UNIT-1) {
    // check if one-way plateform
    if (x1 == '=' || x2 == '=') {
      return true;
    }
  }
  // Otherwise, check for basic block
  if (x1 == '#' || x2 == '#' || y1 == '#' || y2 == '#'){
    return true;
  }
  return false;
}

bool isCollidingWithPlayer(Coordinates* origin, Coordinates* next, Player* player) {
  // Allocate memory space for test
  bool collision = false;
    // check if not colliding with itself
  if (player->coord != origin) {
    if (isColliding(next,player->coord)) {
      collision = true;
    }
  }
  // If no collision found, return false
  return collision;
}

bool isCollidingWithMonsters(Coordinates* origin, Coordinates* next, Monster* monsters) {
  // Allocate memory space for test
  bool collision = false;
  // Loop through monsters chain
  while (monsters != NULL) {
    // check if not colliding with itself
    if (monsters->coord != origin) {
      if (isColliding(next,monsters->coord)) {
        collision = true;
      }
    }
    // switch to next monster
    monsters = monsters->next;
  }
  // If no collision   found, return false
  return collision;
}
