//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "resources.h"

#define CALLER "resources"

// INITIALISATION

Resources* initResources(void) {
  // Allocate memory space for resources
  Resources* resources = malloc(sizeof(Resources));
  // Check if instantiation was successful
  if (resources != NULL) {
    // Default substructures pointers to NULL
    resources->fonts     = NULL;
    resources->assets    = NULL;
    resources->levels    = NULL;
    resources->monsters  = NULL;
    resources->items     = NULL;
  }
  // Return struct
  return resources;
}

// FILES

Resources* readResources(void) {
  logger(INFO,CALLER,"loading resources...\n");
  // Initialisation of Resources structure
  Resources* resources = malloc(sizeof(Resources));
  // Check if instantiation was successful
  if (resources != NULL) {
    // Read files for values
    resources->fonts     = readFonts();
    resources->assets    = readAssets();
    resources->monsters  = readMonsters();
    resources->items     = readItems();
    resources->levels    = readLevels();
    // Check if successful
    if (isValidResources(resources)) {
      logger(INFO,CALLER,"resources loaded.\n");
      return resources;
    } else {
      logger(FAIL,CALLER,"failed to load resources!\n");
    }
  } else {
    logger(FAIL,CALLER,"could not initialise resources!\n");
  }
  return NULL;
}

// FUNCTIONS

Level* loadLevel(Resources* resources, int index) {
  char buffer[MAXSTRING];
  logger(INFO,CALLER,"loading level...");
  // Reload first level as starting point
  Level* current = resources->levels;
  // Read through level resources to find matching level
  while (current != NULL && current->index != index) {
    current = current->next;
  }
  // IF level is found
  if (current != NULL && current-> index == index) {
    // THEN, set level as current level and load corresponding map
    sprintf(buffer,"Level %i loaded.",index);
    logger(INFO,CALLER,buffer);
    // Load its char map
    loadMap(current);
    // return struct
    return current;
  } else {
    // ELSE, Output failure, return NULL
    sprintf(buffer,"Failed to load level %i!",index);
    logger(FAIL,CALLER,buffer);
    return NULL;
  }
}

// TESTS

bool isValidResources(Resources* resources) {
  // Check if passed resources are available
  if (resources != NULL) {
    // Initialise test return condition;
    bool isValid = true;
    // Check if all resources were loaded
    if (resources->levels == NULL) {
      logger(ERR,CALLER,"levels not initalised!");
      isValid = false;
    }
    if (resources->assets == NULL) {
      logger(ERR,CALLER,"assets not initalised!");
    //  isValid = false;
    }
    if (resources->fonts == NULL) {
      logger(ERR,CALLER,"fonts not initalised!");
      isValid = false;
    }
    if (resources->monsters == NULL) {
      logger(ERR,CALLER,"monsters not initalised!");
      isValid = false;
    }
    if (resources->items == NULL) {
      logger(ERR,CALLER,"items not initalised!");
      isValid = false;
    }
    // Check validity condition
    if (!isValid) {
      logger(ERR,CALLER,"invalid resources found!");
    }
    // Return test value
    return isValid;
  } else {
    logger(ERR,CALLER,"could not validate resources");
    return false;
  }
}

// DESTROY

void destroyResources(Resources* resources) {
  logger(WARN,CALLER,"destroying resources...\n");
  // Free substructures
  destroyLevels(resources->levels);
  destroyMonsters(resources->monsters);
  destroyAssets(resources->assets);
  destroyFonts(resources->fonts);
  // Free resources structs
  free(resources);
  // Output to screen
  logger(WARN,CALLER,"resources destroyed.\n");
}
