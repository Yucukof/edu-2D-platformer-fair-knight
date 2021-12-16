//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _support_
#define _support_ "support.h"

#include "lib.h"

#include "scores.h"
#include "credits.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Accessory items
structdef(Support){
    Score*  scores;
     char** credits;
};

// INITIALISATION
Support* initSupport(void);
  /* Initialise support with empty substructures */

// FILES
Support* readSupport(void);
  /* Build support by reading files for sub-structs */

// TESTS
bool isValidSupport(Support* support);
  /* Check if support were correctly loaded */

// DESTROY
void destroySupport(Support* support);
  /* Free support and sub-structs from memory */

#endif
