//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "credits.h"

#define CALLER "credits"

// INITIALISATION

char** initCredits(void) {
  char** credits = malloc(sizeof(char*)*MAXSTRING);
  credits[0]     = malloc(sizeof(char)*MAXSTRING);
  return credits;
}

// FILES

char** readCredits(void) {
  char buffer[MAXSTRING];

  logger(INFO,CALLER,"loading credits...");
  // Set target address
  char** credits = NULL;
  // Opening credits file
  File* f = openFile("credits","text","r");

  // check if file was successfully opened
	if (f->ptr != NULL) {
    // ALlocate memory space for credits struct
    credits = initCredits();
    int i = 0   // counter for rows
      , j = 0;  // counter for characters
    while (!feof(f->ptr)) {
      // Read next character
      char c = fgetc(f->ptr);
      // IF char is newline
      if (c == '\n') {
        // THEN allocate memory space for newline and reset char counter
        i++;
        j=0;
        credits[i] = malloc(sizeof(char)*MAXSTRING);
      }
      // Store new character
      credits[i][j] = c;
      j++;
    }
    credits[i]=NULL;
    // Feedback: number of rows read
    sprintf(buffer,"%i credits read.",i);
    logger(INFO,CALLER,buffer);
    // Release credits file
		closeFile(f);
  }
  // return pointer
  return credits;
}

// TESTS

bool isValidCredits(char** credits) {
  // Check if credits are valid
  if (credits != NULL){
    if (credits[0] != NULL) {
      return true;
    }
  } else {
    logger(WARN,CALLER,"invalid credits");
  }
  // if not, return failure
  return false;
}

// DESTROY

void destroyCredits(char** credits) {
  // Check if credits sould be deleted
  if (credits != NULL) {
    int i = 0;
    while (credits[i] != NULL) {
      free(credits[i]);
      i++;
    }
    // Release initial pointer
    free(credits);
  }
}
