//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "level.h"

#define CALLER "level"

// INITIALISATION

Level* initLevel(void) {
  // Allocate memory
  Level* level = malloc(sizeof(Level));
  // If score is not instantiated, log error and exit
  if (level != NULL) {
    // Else set initial values
    level->name[0]  = '\0';
    level->width    = 0;
    level->height   = 0;
    level->scrollx  = 0;
    level->scrolly  = 0;
    level->index    = 0;
    level->map      = NULL;
    level->next     = NULL;
  }
  logger(TRACE,CALLER,"score initialised.\n");
  return level;
}

// FILES

Level* readLevels(void) {
  char buffer[MAXSTRING];
  logger(INFO,CALLER,"loading levels...");
  // Allocating space in memory
  Level* first = NULL;
  // Open levels file
  File* f = openFile("levels",CALLER, "r");
  // Check if file was successfully opened
  if (f->ptr != NULL) {
    int i = 0; // Level counter
    Level* previous = NULL;
    // Read levels file and add entries
    while(!feof(f->ptr)) {
      // Allocate space for new level and read next row
      Level* next = initLevel();
      fscanf(f->ptr,"%s %i %i %i %i\n"
            ,next->name
            ,&next->width
            ,&next->height
            ,&next->scrollx
            ,&next->scrolly);

      // Compute and store level index
      next->index = i+1;
      // if previous level exists
      if (previous != NULL) {
        // Then, add current level as next
        previous->next = next;
      } else {
        // Else, register first level
        first = next;
      }
      // Switch to next and increment counter
      previous = next;
      i++;
    }
    // Set final level next to NULL (end of array)
    previous->next = NULL;
    // Feedback: number of levels read
    sprintf(buffer,"%i levels read.",i-1);
    logger(INFO,CALLER,buffer);
    // Release levels file
    closeFile(f);
  }
  // Return struct
  return first;
}

// FUNCTIONS

bool loadMap(Level* level) {
  char buffer[MAXSTRING];
  // Check if pointers ar valid
  if (level != NULL || level->name != NULL) {
    logger(INFO,CALLER,"loading map...");
    // Specify actual file path to map and open it in read mode
    concat(buffer,level->name,".map");
    File* f = openFile(buffer,CALLER,"r");
    // Check if file was successfully opened
    if (f->ptr != NULL) {
      // generate height of bidimensional char table
      level->map = malloc(level->height*sizeof(char*));
      // allocate temporary space to read line in file
      char* buf = malloc((level->width+3)*sizeof(char));
      // set number of lines to read
      int h = level->height // number of lines to read
        , i = 0;            // number of lines read
      // As long as file has rows and height is not reached
      while (!feof(f->ptr) && h > 0) {
        h--;
        // allocate space for row in memory
        level->map[h]=malloc(level->width+1*sizeof(char));
        // load row (including newline char \n, carriage return \r, etc.)
        fgets(buf, level->width+3, f->ptr);
        // transfer correct length of char to table
        memcpy(level->map[h],buf,level->width);
        // increment lines read counter
        i++;
      }
      // release row buffer
      free(buf);
      // Feedback: number of rows read
      sprintf(buffer,"%i rows loaded",i);
      logger(INFO,CALLER,buffer);
      // release file map
      closeFile(f);
      // return success
      return true;
    } else {
      // if file could not be opened, return error
      logger(FAIL,CALLER,"could not load map (file is null)");
    }
  } else {
    // If level could not be found or path is null, return error
    logger(FAIL,CALLER,"could not load map (level is null)!");
  }
  return false;
}

// TESTS
bool isValidLevel(Level* level) {
  logger(FAIL,CALLER,"isValidLevel not implemented");
  return false;
}

// DESTROY

void destroyLevels(Level* levels) {
  char buffer[MAXSTRING];
  logger(WARN,CALLER,"Destroying levels...");
  // Setting first level from array to destroy from resources
  Level* toDestroy = levels;
  // Destroy array of levels from resources
  int i = 0; // Detroyed levels counter
  while (toDestroy != NULL) {
    // Store next level before destruction
    Level* next = toDestroy->next;
    // Free level from memory
    destroyLevel(toDestroy);
    // Switch to next
    toDestroy = next;
    // Increment coutner
    i++;
  }
  // Feedback: number of level destroyed
  sprintf(buffer,"%i levels destroyed.",i);
  logger(WARN,CALLER,buffer);
}

void destroyLevel(Level* level) {
  char buffer[MAXSTRING];
  // Output level being freed
  sprintf(buffer,"destroying level %i...",level->index);
  logger(WARN,CALLER,buffer);
  // Free level map
  destroyMap(level);
  level->map = NULL;
  // Free level
  free(level);
  logger(WARN,CALLER,"level destroyed.");
}

void destroyMap(Level* level) {
  logger(WARN,CALLER,"destroying map...");
  // Free each row of map from memory
  if (level->map != NULL) {
    for (int i = 0; i < level->height; i++) {
      free(level->map[i]);
    }
    // Free main pointer
    free(level->map);
  }
  logger(WARN,CALLER,"map destroyed.");
}

// UTILITY

void printLevels(Level* levels){
  while (levels != NULL){
    printLevel(levels);
    levels = levels->next;
  }
}

void printLevel(Level* level) {
  // Check if passed parameter is valid
  if (level != NULL) {
    // double the size of buffer to accommodate with level->name already being MAXSTRING
    char buffer[MAXSTRING*2];
    // Put level info in buffer
    sprintf(buffer,"%s (%i)\n\t\t~ [%i x %i]"
    ,level->name
    ,level->index
    ,level->width
    ,level->height);
    // Output buffer to logger
    logger(INFO,CALLER,buffer);

    printMap(level);
  } else {
    logger(WARN,CALLER,"unable to display level.");
  }
}

void printMap(Level* level) {
  // check if passed parameter is valid
  if (level != NULL) {
    // If map is valid
    if (level->map != NULL) {
      logger(INFO,CALLER,"displaying map...");
      // Output level map to screen (will not be in log file)
      for (int h = level->height-1; h >= 0; h--) {
        // Print each row with row num
        printf("%i\t %.*s\n",h,level->width,level->map[h]);
      }
    } else {
      logger(ERR,CALLER,"unable to display map");
    }
  } else {
    logger(ERR,CALLER,"unable to display level");
  }
}
