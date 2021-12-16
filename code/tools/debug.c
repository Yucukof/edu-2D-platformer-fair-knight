//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "debug.h"

#define CALLER "debug" // Log location

// Global variables for debugger
FILE       *f;        // Debug.log file
bool       isEnabled; // Debugger activation status
errorLevel level;     // Debugger activation level

// Text values of errorLevel enum
char* errorLevel_value[] = {
  "TRAC",
  "INFO",
  "WARN",
  "ERR ",
  "FAIL",
  "FATAL"
};

// FUNCTIONS

void activateLog(bool active) {
  // Actiave or deactivate logging depending on input value
  if (active) {
    isEnabled = true;
    level = INFO;
    openLog();
    logger(INFO,"debug","Debugger enabled.\n");
  } else {
    logger(INFO,"debug","Debugger disabled.\n");
    isEnabled = false;
    closeLog();
  }
}

void logger(errorLevel level, char location[], char msg[]) {

  // Get current datetime
  time_t timer;
  time(&timer);
  struct tm* t = localtime(&timer);

  // Extract time value from struct and store it into buffer
  char time[10];
  sprintf(time,"%02i:%02i:%02i",t->tm_hour,t->tm_min,t->tm_sec);

  // Create new log entry with all input and time
  Log entry = {
     .time     = time,
     .level    = level,
     .location = location,
     .msg      = msg,
  };

  // Send log feedback
  echo(entry);
  print(entry);

  // If level is failure, exit program abrupty
  if (level == FAIL) {
    entry.location = "FATAL";
    entry.msg = "EXITING PROGRAM!\n";
    echo(entry);
    print(entry);
    closeLog();
    exit(EXIT_FAILURE);
  }
}

// UTILITY

void echo(Log entry) {
  // if logging is enabled, then print log value to screen
  if (isEnabled && entry.level >= level) {
    printf("%s [%s] %s: %s\n",entry.time,errorLevel_value[entry.level],entry.location,entry.msg);
  }
}

void print(Log entry) {
  // If logging is enabled, check if debug file is available then print log to file.
  if (isEnabled && entry.level >= level) {
    if (f != NULL) {
      fprintf(f,"%s [%s] %s: %s\n",entry.time,errorLevel_value[entry.level],entry.location,entry.msg);
    } else {
      logger(ERR,CALLER,"could not write to file!");
    }
  }
}

// FILES

void openLog(void) {
  // If logging is enabled, then attempt to open static log file and print first logs
  if (isEnabled) {
    f = fopen("../assets/debug.log","w");
    if (f == NULL) {
      logger(FAIL,CALLER,"File could not be opened!\n");
    }
    logger(INFO,CALLER,"Log file created.");
  }
}

void closeLog(void) {
  // If log file is available, print last logs and close file
  if (f != NULL) {
    logger(INFO,"debug","Debugger disabled.");
    logger(INFO,"debug","Log file closed.\n");
    fclose(f);
  }
}
