//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "support.h"

#define CALLER "support"

// INITIALISATION

Support* initSupport(void) {
  // Allocate memory space for support
  Support* support = malloc(sizeof(Support));
  // Check if instantiation was successful
  if (support != NULL) {
    // Default substructures pointers to NULL
    support->credits   = NULL;
    support->scores    = NULL;
  }
  // Return struct
  return support;
}

// FILES

Support* readSupport(void) {
  logger(INFO,CALLER,"loading support...\n");
  // Initialisation of Support structure
  Support* support = malloc(sizeof(Support));
  // Check if instantiation was successful
  if (support != NULL) {
    // Read files for values
    support->scores    = readScores();
    support->credits   = readCredits();
    // Check if successful
    if (isValidSupport(support)) {
      logger(INFO,CALLER,"support loaded.\n");
      return support;
    } else {
      logger(ERR,CALLER,"failed to load support!\n");
    }
  } else {
    logger(ERR,CALLER,"could not initialise support!\n");
  }
  return NULL;
}

// TESTS

bool isValidSupport(Support* support) {
  // Check if passed support are available
  if (support != NULL) {
    // Initialise test return condition;
    bool isValid = true;
    // Check if all support were loaded
    if (support->scores != NULL && !isValidScores(support->scores)) {
      // Score is facultative, so no error
      logger(WARN,CALLER,"scores is not initalised.");
      isValid = false;
    }
    if (!isValidCredits(support->credits)) {
      logger(ERR,CALLER,"credits not initalised!");
      isValid = false;
    }
    // Return test value
    return isValid;
  } else {
    logger(ERR,CALLER,"could not validate support");
    return false;
  }
}

// DESTROY

void destroySupport(Support* support) {
  logger(WARN,CALLER,"destroying support...\n");
  // Free substructures
  destroyScores(support->scores);
  destroyCredits(support->credits);
  // Free support structs
  free(support);
  // Output to screen
  logger(WARN,CALLER,"support destroyed.\n");
}
