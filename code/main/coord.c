//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "coord.h"

#define CALLER "move"

char* direction_value[] = {
  "LEFT",
  "RIGHT",
  "UP",
  "DOWN"
};

// INITIALISATION

Moving* initMoving(void) {
  // Allocate memory space for moving
  Moving* mov = malloc(sizeof(Moving));
  // Check if instantiation is successful
  if (mov != NULL) {
    // Set initial values
    mov->left  = false;
    mov->right = false;
    mov->up    = false;
    mov->down  = false;
    mov->timer = 0;
  }
  // return new moving struct
  return mov;
}

Coordinates* initCoord(void) {
  // Allocate memory space for coordinate
  Coordinates* coord = malloc(sizeof(Coordinates));
  // Check if instantiation is successful
  if (coord != NULL) {
    // Set initial values
    coord->x      = 0;
    coord->y      = 0;
    coord->w      = UNIT;
    coord->h      = UNIT;
    coord->look   = RIGHT;
    coord->moving = initMoving();
  }
  // return new coordinate
  return coord;
}

// FUNCTIONS

  /* See motion.h */

void setCoord(Coordinates* coord,float x, float y, direction orientation) {
  // Check if input is valid
  if (isValidCoord(coord)) {
    coord->x    = x;
    coord->y    = y;
    coord->look = orientation;
  } else {
    // Output failure
    logger(ERR,CALLER,"unable to set coordinates!");
  }
}

void setCoordSize(Coordinates* coord,float w, float h) {
  // Check if input is valid
  if (isValidCoord(coord)) {
    coord->w    = w;
    coord->h    = h;
  } else {
    // Output failure
    logger(ERR,CALLER,"unable to set coord size!");
  }
}

void setLook(Coordinates* coord,direction orientation) {
  // Check if orientation can lead to look update
  if (orientation == LEFT || orientation == RIGHT) {
    // Update look if needed
    coord->look = orientation;
  }
}

void setMoving(Coordinates* coord,direction orientation) {
  // Check if coord is valid
  if (coord != NULL) {
    // Update the way character is facing
    setLook(coord,orientation);

    // Check orientation to update moving status
    switch (orientation) {

      case RIGHT:
        coord->moving->right = true;
        coord->moving->left  = false;
        break;

      case LEFT:
        coord->moving->left  = true;
        coord->moving->right = false;
        break;

      case UP:
        // do jump only if not already moving down
        if (!coord->moving->down) {
          coord->moving->up    = true;
        }
        break;

      case DOWN:
        coord->moving->up    = false;
        coord->moving->down  = true;
        break;

      case INVERT:
        resetMoving(coord);
        break;

      default:
        // if invalid move direction, log warning
        logger(WARN,CALLER,"Invalid move recorded.");
        printCoord(coord);
        break;
    }
  } else {
    // output error
    logger(ERR,CALLER,"invalid move recorded!");
  }
}

void stopMoving(Coordinates* coord,direction orientation) {
  // Check if input is valid
  if (coord != NULL) {
    // depending on direction, turn option off
    switch (orientation) {
      case LEFT:
        coord->moving->left  = false;
        break;
      case RIGHT:
        coord->moving->right = false;
        break;
      case UP:
        coord->moving->up    = false;
        break;
      case DOWN:
        coord->moving->down  = false;
        break;
      default:
        // if direction is not valid, log warning
        logger(WARN,CALLER,"unable to stop moving.");
        break;
    }
  } else {
    // output error
    logger(ERR,CALLER,"unable to stop moving!");
  }
}

void resetMoving(Coordinates* coord) {
  // Check if input is valid
  if (coord != NULL && coord->moving != NULL) {
    // Reset all values
    coord->moving->timer  = 0;
    coord->moving->up     = false;
    coord->moving->left   = false;
    coord->moving->right  = false;
    coord->moving->down   = false;
  } else {
    // log error
    logger(ERR,CALLER,"unable to reset moving!");
  }
}

void getDistance(Coordinates* coord1, Coordinates* coord2, float* distx, float* disty) {
  // return absolute horizontal and vertical distances between the two coordinates
  *distx = fminf(fabs((coord1->x) - (coord2->w)),fabs((coord1->w) - (coord2->x)));
  *disty = fabs((coord1->y) - (coord2->y)),fabs((coord1->y) - (coord2->h));
}

void closeIn(Coordinates* marching, Coordinates* target){
  if (marching->x < target->x) {
    setMoving(marching,RIGHT);
  } else if (marching->x > target->x) {
    setMoving(marching,LEFT);
  }
}

// TESTS

bool isValidMoving(Moving* moving) {
  // Check if passed parameter is valid
  if (moving != NULL) {
    // Allocate memory space for test results
    bool isValid = true;
    // Perform checks
      /* No test yet */
    // Evaluate test result
    if (!isValid) {
      logger(ERR,CALLER,"invalid moving!");
    }
    // Return test value
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate moving!");
    return false;
  }
}

bool isValidCoord(Coordinates* coord) {
  // Check if passed parameter is valid
  if (coord != NULL) {
    // Allocate memory space for test results
    bool isValid = true;
    // Perform checks
      /* TODO */
    // Validate substructure
    if (!isValidMoving(coord->moving)) {
      isValid = false;
    }
    // Evaluate test result
    if (!isValid) {
      logger(ERR,CALLER,"invalid coordinates!");
    }
    // Return test value
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate coordinates!");
    return false;
  }
}

bool isMoving(Coordinates* coord) {
  // Check if input is correct
  if (isValidCoord(coord)) {
    // Check if any of the direction is active
    if (coord->moving->left || coord->moving->right || coord->moving->up || coord->moving->down) {
      // if yes, coord is moving
      return true;
    } else {
      // Otherwise coord is immobile.
      return false;
    }
  } else {
    logger(ERR,CALLER,"unable to check if moving!");
    return false;
  }
}

bool isInRange(Coordinates* watcher, Coordinates* subject,int sight) {
  // Check if subject is in range for watcher
  float distx, disty;
  getDistance(watcher,subject,&distx,&disty);
  // is in sight?
  if (disty < UNIT*3) {
    if (distx < UNIT*sight) {
      return true;
    }
  }
  // Otherwise, return not visible
  return false;
}

bool isColliding(Coordinates* origin, Coordinates* next) {
  // check if input is valid
  if (isValidCoord(origin) && isValidCoord(next)) {
    // Make sure that origin is not also next
    if (origin != next) {
      // Set collision initial conditions to false
      bool xCollision = false;
      bool yCollision = false;
      // Check for 1-axis collision
        // horizontal collision
      if (origin->x >= next->x && origin->x <= next->w) {
        xCollision = true;
      } else {
        if (origin->w >= next->x && origin->w <= next->w) {
          xCollision = true;
        } else {
          if (origin->x <= next->x && origin->w >= next->w) {
            xCollision = true;
          }
        }
      }
        // vertical collision
      if (origin->y >= next->y && origin->y <= next->h) {
        yCollision = true;
      } else {
        if (origin->h >= next->y && origin->h <= next->h) {
          yCollision = true;
        } else {
          if (origin->y <= next->y && origin->h >= next->h) {
            yCollision = true;
          }
        }
      }
      // Check for 2-axis collision
      if (xCollision && yCollision) {
        // if check met, return true
        return true;
      }
    }
  } else {
    // Output error and return false
    logger(ERR,CALLER,"unable to check collision!");
  }
  // if origin is next or not colliding or error
  return false;
}

bool atArmReach(Coordinates* watcher, Coordinates* subject,int reach) {
  // Check if input is valid
  if (isValidCoord(watcher) && isValidCoord(subject)){
    // Get horizonal and vertical distance
    float distx,disty;
    getDistance(watcher,subject,&distx,&disty);
    // Check if distance closer or equal to reach
    if (distx <= reach && disty <= reach) {
      // confirm reach
      logger(INFO,CALLER,"at arms reach.");
      return true;
    }
  } else {
    // Output error
    logger(ERR,CALLER,"could not check reach!");
  }
  // Return failure or false
  return false;
}

bool isVisible(Coordinates* watcher, Coordinates* subject, int sight) {
  // Check if input is valid
  if (isValidCoord(watcher) && isValidCoord(subject)) {
    // Check if watcher is looking in direction of subject
    if (subject->x < watcher->x) {
      if (watcher->look == LEFT) {
        return true;
      }
    } else {
      if (watcher->look == RIGHT){
        return true;
      }
    }
  } else {
    // Otherwise, output error
    logger(ERR,CALLER,"unable to check if visible!");
  }
  // Return failure or not visible
  return false;
}

// DESTROY

void destroyMoving(Moving* moving) {
  // Free moving struct from memory
  free(moving);
}

void destroyCoordinates(Coordinates* coord) {
  // Free sub-struct moving
  destroyMoving(coord->moving);
  // Free coordinates struct from memory
  free(coord);
}

// UTILITY

void printCoord(Coordinates* coord) {
  char buffer[MAXSTRING];

  sprintf(buffer,"\n\t\t~Position: [%.0f-%.0f][%.0f-%.0f]\n\t\t~Direction: %s (%i)\n\t\t~Movement:"
         ,coord->x+1
         ,coord->y+1
         ,coord->w+1
         ,coord->h+1
         ,direction_value[coord->look]
         ,coord->moving->timer);

  if (coord->moving->left) {
    concat(buffer,buffer," <");
  }
  if (coord->moving->right) {
    concat(buffer,buffer," >");
  }
  if (coord->moving->up) {
    concat(buffer,buffer," ^");
  }
  if (coord->moving->down) {
    concat(buffer,buffer," v");
  }

  logger(INFO,CALLER,buffer);
}
