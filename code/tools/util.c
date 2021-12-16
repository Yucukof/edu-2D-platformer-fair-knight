//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "util.h"

#define MAXSTRING 255
#define CALLER "util" // Debugger location

// FUNCTIONS

void concat(char* output, char* str1, char* str2) {

  char buf1[MAXSTRING];
  char buf2[MAXSTRING];

  // Make copies of input strings
  strncpy(buf1,str1,MAXSTRING);
  strncpy(buf2,str2,MAXSTRING);

  // Concatenate input strings
  strncat (buf1,buf2,MAXSTRING-strlen(buf1));

  // Store concatenation in output string
  strncpy(output,buf1,MAXSTRING);
}

void timerControl(unsigned int* timer, bool* active) {
  if (*timer <= 0) {
    *active = false;
  } else {
    *active = true;
  }
}

// FILES

File* openFile(char* filename, char* filepath, char* filemode) {

  // Create new File structure
  File* f = malloc(sizeof(File));
  // Copy input values into File struct
  f->name = malloc(sizeof(char)*MAXSTRING);
  strncpy(f->name,filename,MAXSTRING);
  f->path = malloc(sizeof(char)*MAXSTRING);
  strncpy(f->path,filepath,MAXSTRING);
  f->mode = malloc(sizeof(char)*MAXSTRING);
  strncpy(f->mode,filemode,MAXSTRING);

  char buffer[MAXSTRING];

  // Make log entry
  concat(buffer,"opening file ",filename);
  concat(buffer,buffer,"...");
  logger(INFO,filepath,buffer);

  // Generate actual/relative fiepath
  concat(buffer,"./../assets/",filepath);
  concat(buffer,buffer,"/");
  concat(buffer,buffer,filename);

  // Open file
  f->ptr = fopen(buffer,filemode);

  // Check file status and generate relevant log entry
  if (f->ptr == NULL) {
    concat(buffer,filename," could not be opened!");
    logger(ERR,filepath,buffer);
  } else {
    concat(buffer,filename," successfully opened.");
    logger(INFO,filepath,buffer);
  }

  // Return File struct
  return f;
}

bool closeFile(File* f) {
  char buffer[MAXSTRING];

  // Check if File struct has pointer
  if (f != NULL) {
    if (f->ptr != NULL) {
      fclose(f->ptr);
      concat(buffer,f->name," file closed.\n");
      logger(INFO,f->path,buffer);
      destroyFile(f);
      return true;
    } else {
      concat(buffer,f->name," not available for closing!");
      logger(WARN,f->path,buffer);
      destroyFile(f);
      return false;
    }
  } else {
    logger(ERR,CALLER,"invalid file close requested");
    return false;
  }
}

// DESTROY

void destroyFile(File* f) {
  // Free char values of File structure
  free(f->name);
  f->name = NULL;
  free(f->path);
  f->path = NULL;
  free(f->mode);
  f->mode = NULL;
  free(f);
}
