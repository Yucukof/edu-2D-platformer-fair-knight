//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _debug_
#define _debug_ "debug.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <GL/glut.h>

// STRUCTURES

typedef enum errorLevel {
  TRACE,      // Technical feedback
  INFO,       // Standard feedback
  WARN,       // Important feedback
  ERR,        // Significant error
  FAIL        // Non-recoverable error
} errorLevel;

typedef struct Log {
        char* time;       // TIme of occurence
  errorLevel  level;      // Severity level
        char* location;   // Location in Code
        char* msg;        // Message
} Log;

// FUNCTIONS

void activateLog(bool active);
  /* Start/stop logger utility */
void logger(errorLevel level, char location[], char msg[]);
  /* Accept log input, convert it to struct and call feedback functions */

// UTILITY

void echo(Log entry);
  /* print log content on screen */
void print(Log entry);
  /* print log content to debug.log file */

// FILES

void openLog(void);
  /* Create/open debug.log file in wrtie mode */
void closeLog(void);
  /* Close debug.log file */

#endif
