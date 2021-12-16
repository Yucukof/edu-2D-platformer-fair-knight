//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _CREDITS_
#define _CREDITS_ "credits.h"

#include "lib.h"

// INITIALISATION

char** credits(void);

// FILES

char** readCredits(void);

// TESTS

bool isValidCredits(char** credits);

// DESTROY

void destroyCredits(char** credits);

#endif
