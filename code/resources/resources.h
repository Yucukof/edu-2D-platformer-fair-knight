//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _resources_
#define _resources_ "resources.h"


#include "lib.h"
#include "assets.h"
#include "fonts.h"
#include "level.h"
#include "monster.h"
#include "item.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Resources keeper
structdef(Resources){
    Level* levels;
    Asset* assets;
  Monster* monsters;
     Item* items;
     Font* fonts;
};

// INITIALISATION
Resources* initResources(void);
  /* Initialise resources with empty substructures */

// FILES
Resources* readResources(void);
  /* Build resources by reading files for sub-structs */

// FUNCTIONS

Level* loadLevel(Resources* resources, int number);
  /*Load level information from resources  */

// TESTS
bool isValidResources(Resources* resources);
  /* Check if resources were correctly loaded */

// DESTROY
void destroyResources(Resources* resources);
  /* Free resources and sub-structs from memory */

#endif
