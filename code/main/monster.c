//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "monster.h"

#define CALLER "monster"

char* monsterType_value[] = {
  "big_demon",
  "big_zombie",
  "chort",
  "goblin",
  "ice_zombie",
  "orc",
  "chaman",
  "ogre",
  "muddy",
  "necromancer",
  "warrior",
  "skeleton",
  "swampy",
  "tiny_zombie",
  "wizard",
  "wogol",
  "zombie",
};

// INITIALISATION

Monster* initMonster(void) {
  // Allocate space for Monster struct in  memory
  Monster* monster = malloc(sizeof(Monster));
  // Check if instantiation is successful
  if (monster != NULL) {
    // Set initial values
    monster->name[0]        = '\0';
    monster->type           = 0;
    monster->baseHealth     = 0;
    monster->currentHealth  = 0;
    monster->baseSpeed      = 0;
    monster->currentSpeed   = 0;
    monster->baseAttack     = 0;
    monster->currentAttack  = 0;
    monster->sight          = 0;
    monster->reach          = 0;
    monster->isStunned      = false;
    monster->isAttacking    = false;
    monster->timer          = 0;
    monster->scoreValue     = 0;
    monster->sprites        = NULL;
    monster->next           = NULL;
    monster->coord       = initCoord();
  } else {
    // ELSE, output error
    logger(ERR,CALLER,"failed to initialise monster");
  }
  // Return struct pointer
  return monster;
}

// FILES

Monster* readMonsters(void) {
  char buffer[MAXSTRING];

  logger(INFO,CALLER,"loading monsters spec...");
  // opening monsters file
  File* f = openFile("monsters.csv","text","r");
  //
  Monster* monsters = NULL;

  // Check if file was successfully opened
  if (f->ptr != NULL) {
    Monster* previous = NULL;

    int i = 0,j = 0; // i = monster counter, j = row counter
    while (!feof(f->ptr)) {
      // Allocate space for new monster
      Monster* next = initMonster();
      // Read new row in monsters file
      fscanf(f->ptr,"%s ; %u ; %u ; %u ; %u ; %u ; %u ;\n"
            , next->name
            ,&next->baseHealth
            ,&next->baseSpeed
            ,&next->baseAttack
            ,&next->sight
            ,&next->reach
            ,&next->scoreValue);
      // IF monster values are valid
      if (isValidMonster(next)) {
        // THEN, add new monster
        // IF previous monster exists
        if (previous != NULL) {
          // THEN, add curent monster as next
          previous->next = next;
        } else {
          // ELSE, set first monster of array
          monsters = next;
        }
        // Switching to next monster
        previous  = next;
        i++;
      } else {
        // ELSE output error location
        sprintf(buffer,"invalid monster entry found at: %i",j);
        logger(WARN,CALLER,buffer);
        // and free allocated space
        destroyMonster(next);
      }
      j++;
    }
    // Set final monster next to NULL 'end of array)'
    previous->next = NULL;

    // Feedback: number of monsters loaded
    sprintf(buffer,"%i monsters loaded.",i);
    logger(INFO,CALLER,buffer);

    // Release monsters file
    closeFile(f);

  } else {
    // THEN report failure and exit
    logger(FAIL,CALLER,"failed load monsters!");
  }
  return monsters;
}

// FUNCTIONS

Monster* appendMonster(Monster* monster,Monster* monsters) {
  // Allocate memory space for output pointer
  Monster* output = NULL;
  // check if input is valid
  if (isValidMonster(monster)) {
    // Check if chain exists
    if (monsters == NULL) {
      // If not, start chain
      output = monster;
    } else {
      // Else check if chain is valid
      if (isValidMonsters(monsters)) {
        // Store chain start pointer
        output = monsters;
        // Read through chain to find append position
        while (monsters->next != NULL) {
          monsters = monsters->next;
        }
        // Append monster to chain
        monsters->next = monster;
      }
    }
  } else {
    // Output failure
    logger(ERR,CALLER,"unable to append monster");
  }
  // Return chain start
  return output;
}

void removeMonster(Monster* monster, Monster** monsters) {
  // check if monster chain exists
  if (*monsters != NULL) {
    // Check if input is valid
    if (isValidMonster(monster) && isValidMonsters(*monsters)) {
      // Create cursor to read through chain
      Monster* cursor = *monsters;
      // Reserve space for previous monster if needed
      Monster* previous = NULL;
      // Read through chain
      while (cursor != NULL && cursor != monster) {
        // Store previous monster address
        previous = cursor;
        // Switch to next cursor
        cursor = cursor->next;
      }
      // Check if monster was found
      if (cursor != NULL) {
        // Check if is first monster of chain
        if (previous != NULL) {
          previous->next = cursor->next;
        } else {
          *monsters = cursor->next;
        }
      }
      // Free memory
      destroyMonster(cursor);
      logger(WARN,CALLER,"monster killed.");
    } else {
      logger(ERR,CALLER,"unable to kill monster!");
    }
  }
}

void hitMonster(Monster* monster, int hitpoint) {
  if (isValidMonster(monster)) {
    monster->currentHealth -= hitpoint;
    logger(INFO,CALLER,"monster was hurt.");
  } else {
    logger(ERR,CALLER,"monster could not be hurted");
  }
}

Monster* getFirstHitMonster(Coordinates* coord, Monster* monsters) {
  // Allocate memory space for return Monster
  Monster* output = NULL;
  // check if monster chain exists
  if (monsters != NULL) {
    // Check if input is valid
    if (isValidCoord(coord) && isValidMonsters(monsters)) {
      Monster* cursor = monsters;
      while (cursor != NULL) {
        if (isColliding(coord,cursor->coord)) {
          logger(INFO,CALLER,"hit landed.");
          output = cursor;
          break;
        } else {
          cursor = cursor->next;
        }
      }
    } else {
      // Otherwise, output error
      logger(ERR,CALLER,"unable to hit monsters.");
    }
  }
  // Return monster address if applicable
  return output;
}

bool setMonsterSprites(Monster* monster,type type, int name, anim anim, Asset* assets) {
  if (monster != NULL && monster->coord !=  NULL) {
    monster->sprites = loadAssetSprites(type,name,anim,assets);
    // Check if sprites were successfully loaded
    if (isValidSprite(monster->sprites)) {
      // Set monster hitbox size
      monster->coord->w   = monster->coord->x+monster->sprites->width;
      monster->coord->h   = monster->coord->y+monster->sprites->height;
      // Return success
      return true;
    } else {
      // If anim or coord failed to load, output error and return failure
      logger(ERR,CALLER,"failed to load monster sprites!");
      return false;
    }
  }
  // Output error
  logger(ERR,CALLER,"unable to set monster sprites");
  // Return failure
  return false;
}

void setAttacking(Monster* monster, bool activate) {
  // Check if monster is not stunned and able to attack
  if (!monster->isStunned) {
    // Check if not already attacking
    if (!monster->isAttacking && activate) {
      monster->currentSpeed *= 2;
      logger(INFO,CALLER,"monster is attacking.");
    } else if (monster->isAttacking && !activate) {
      monster->currentSpeed = monster->baseSpeed;
      logger(INFO,CALLER,"monster ceased attack.");
    }
    // Turn attack mode ON or OFF according to activate
    monster->isAttacking = activate;
  } else {
    // Turn mode OFF
    monster->isAttacking = false;
  }
}

void setStunned(Monster* monster, bool activate) {
  // Check if input is valid
  if (isValidMonster(monster)) {
    // Set attack mode OFF
    setAttacking(monster,false);
    // Set stun status ON and start timer
    monster->isStunned = true;
    monster->timer = DEFAULTTIMER;
    // Output success
    logger(INFO,CALLER,"monster stunned.");
  } else {
    // Output error
    logger(ERR,CALLER,"unable to stun monster");
  }
}

// TESTS

bool isValidMonsters(Monster* monsters) {
  // Allocate memory space for test response
  bool isValid = false;
  // Check if passed parameter is valid
  if (isValidMonster(monsters)) {
    isValid = true;
    // Read through power chain
    while (monsters != NULL) {
      // Check single power
      if (!isValidMonster(monsters)) {
        isValid = false;
        break;
      }
      // switch to next monster in chain
      monsters = monsters->next;
    }
    // check if successful
    if (!isValid) {
      // If not, then output error
      logger(ERR,CALLER,"invalid monsters chain!");
    }
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate monsters!");
  }
  return isValid;
}

bool isValidMonster(Monster* monster) {
  // Allocate memory space for test response
  bool isValid = false;
  // Check if passed parameter is valid
  if (monster != NULL) {
    // Check if monster is of valid type
    for (int i = 0 ; i < MAXMOB; i++) {
      if (strcmp(monster->name,monsterType_value[i]) == 0) {
        monster->type = i;
        isValid = true;
      }
    }
    if (monster->sprites != NULL && !isValidSprites(monster->sprites)) {
      isValid = false;
    }
    if (monster->coord != NULL && !isValidCoord(monster->coord)) {
      isValid = false;
    }
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate monster");
  }
  // Return test value
  return isValid;
}

bool isStunnedMonster(Monster* monster) {
  // Allocate memory space for test value
  bool output = false;
  // Check if input is valid
  if (isValidMonster(monster)) {
    // Check if monster has stunned status
    if (monster->isStunned) {
      // if not, then monster is stunned
      output = true;
    }
  } else {
    // Otherwise, output error
    logger(ERR,CALLER,"cannot check for stunned monster!");
  }
  // return test value in output
  return output;;
}

bool isDeadMonster(Monster* monster) {
  // Allocate memory space for test value
  bool output = false;
  // Check if input is valid
  if (isValidMonster(monster)) {
    // Check if monster has HP left
    if (monster->currentHealth < 0) {
      // if not, then monster is said dead
      output = true;
    }
  } else {
    // Otherwise, output error
    logger(ERR,CALLER,"cannot check for dead monster!");
  }
  // return test value in output
  return output;;
}

// DESTROY

void destroyMonsters(Monster* monsters) {
  char buffer[MAXSTRING];

  logger(WARN,CALLER,"destroying monsters...");
  // Set monster to destroy
  Monster* toDestroy = monsters;

  int i = 0; // Destroyed monster count
  while (toDestroy != NULL) {
    // Put next monster in buffer
    Monster* next = toDestroy->next;
    // Free current monster
    destroyMonster(toDestroy);
    // Set next monster as current
    toDestroy = next;
    // Increment destroyed monsters counter
    i++;
  }
  // Feedback: number of monsters effectively destroyed
  sprintf(buffer,"%i monsters destroyed.",i);
  logger(WARN,CALLER,buffer);
}

void destroyMonster(Monster* monster) {
  // Free monster coordinates and info from memory
  destroyCoordinates(monster->coord);
  free(monster);
  logger(TRACE,CALLER,"monster destroyed.");
}

// UTILITY

void printMonsters(Monster* monsters) {
  // Read array of monsters and print out each entry
  while (monsters!= NULL) {
    printMonster(monsters);
    monsters = monsters->next;
  }
}

void printMonster(Monster* monster) {
  char buffer[MAXSTRING*2];
  // Put monster info in buffer
  sprintf(buffer,"%s\n\t\t~HP: %i/%i\
          \n\t\t~AP: %i/%i \
          \n\t\t~SP: %i/%i \
          \n\t\t~Sight: %i \
          \n\t\t~Reach: %i \
          \n\t\t~Attack: %i \
          \n\t\t~Stun: %i \
          \n\t\t~Value: %i"
         ,monsterType_value[monster->type]
         ,monster->currentHealth,monster->baseHealth
         ,monster->currentAttack,monster->baseAttack
         ,monster->currentSpeed,monster->baseSpeed
         ,monster->sight
         ,monster->reach
         ,monster->isAttacking
         ,monster->isStunned
         ,monster->scoreValue);
  // Output buffer to logger
  logger(INFO,CALLER,buffer);
  // Print Monster monster coordinates
  printCoord(monster->coord);
  printSprite(monster->sprites);
}
