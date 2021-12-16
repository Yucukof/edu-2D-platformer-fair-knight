//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _level_
#define _level_ "level.h"

#include "lib.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

structdef(Level){
   char   name[MAXSTRING];  // Level name
    int   index;            // Index number
    int   width;            // width
    int   height;           // Height
    int   scrollx;          // Horizontal scroll position
    int   scrolly;          // Vertical scroll position
   char** map;              // Address of map char representation
  Level*  next;             // Link to next level
};

// INITIALISATION

Level* initLevel(void);
  /* Initialise new level */

// FILES

Level* readLevels (void);
  /* Read levels file and return array of levels */

// FUNCTIONS

bool loadMap(Level* level);
  /* Load bidimensional char map into level structure */

// TEST

bool isValidLevel(Level* level);
  /* Check if level is valid */

// DESTROY

void destroyLevels(Level* levels);
  /* Free array of levels beyond pointer from memory */
void destroyLevel(Level* level);
  /* Free level and attached map from memory */
void destroyMap(Level* level);
  /* Free map from memory */

// UTILITY

void printLevels(Level* levels);
  /* Print array of levels to screen */
void printLevel(Level* level);
  /* print level info to screen */
void printMap(Level* level);
  /* Print map to screen */

#endif
