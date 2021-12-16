//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _move_
#define _move_ "coord.h"

#include "lib.h"
#include <math.h>

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Moving state
structdef(Moving){
  bool   left;    // Left movmeents
  bool   right;   // Right movements
  bool   up;      // Upwards movements (incl. jump)
  bool   down;    // Downwards movements (incl. fall)
   int   timer;   // Fall timer
};

// Field position
structdef(Coordinates){
       float   x,y,w,h; // Position on field and hitbox values
   direction   look;    // Orientation
      Moving*  moving;  // Link to movement condition
};

// INITIALISATION

Moving* initMoving(void);
  /* Create new moving struct */
Coordinates* initCoord(void);
  /* Create new coord and moving struct */

// FUNCTIONS

void setCoord(Coordinates* coord,float x, float y, direction orientation);
  /* Set coordinates position and direction */
void setCoordSize(Coordinates* coord,float w, float h);
  /* Set coordinates box size */

void setLook(Coordinates* coord,direction orientation);
  /* Set coordinates horizontal direction */
void setMoving(Coordinates* coord,direction orientation);
  /* Set coordinates movement status to true */
void stopMoving(Coordinates* coord,direction orientation);
  /* Set coordinates movement status to false */
void resetMoving(Coordinates* coord);
  /* Set all coordinates movements to false */

void getDistance(Coordinates* coord1, Coordinates* coord2, float* distx, float* disty);
  /* Compute distance between two coordinates */
void closeIn(Coordinates* marching, Coordinates* target);
  /* Set horizontal moving ON of marching in direction of target */

// TESTS

bool isValidMoving(Moving* moving);
  /* Check if moving struct is valid */
bool isValidCoord(Coordinates* coord);
  /* Check if coord struct is valid */

bool isMoving(Coordinates* coord);
  /* check if the coordinates is moving */
bool isColliding(Coordinates* origin, Coordinates* target);
  /* Check if origin and target coordinates are colliding */
bool isInRange(Coordinates* watcher, Coordinates* subject,int sight);
  /* Check if subject is within range of watcher */
bool atArmReach(Coordinates* watcher, Coordinates* subject,int reach);
  /* Check if subject is close to watcher */
bool isVisible(Coordinates* watcher, Coordinates* subject, int sight);
  /* check if subject position is visible from watcher position */

// DESTROY

void destroyMoving(Moving* moving);
  /* Free moving struct from memory */
void destroyCoordinates(Coordinates* coord);
  /* Free coordinates and moving struct from memory */

// UTILITY

void printCoord(Coordinates* coord);
  /* Output coordinates info to screen */

#endif
