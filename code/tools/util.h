//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _util_
#define _util_ "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "debug.h"

// STRUCTURES

typedef unsigned int uint;

typedef struct File {
  FILE* ptr;      // Pointer to file
  char* name;     // File name
  char* path;     // File path
  char* mode;     // File mode
} File;
  // Utility file structure

// FUNCTIONS

void concat(char *output, char *str1, char *str2);
  // Concatenate two strings in a third string withouth affecting the input strings
void timerControl(unsigned int* timer,bool* active);
  // Update timer status based on timer value

// FILES

File* openFile(char* filename, char* filepath,char* filemode);
  // Use file name, path and mode to open file, generate File structure and return pointer to it
bool closeFile(File* f);
  // Receive File structure, log and close file at pointer

  // DESTROY

void destroyFile(File* f);
  // Free File structure contents in memory

#endif
