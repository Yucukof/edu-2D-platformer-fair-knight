//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "power.h"

#define CALLER "power"

// INITIALISATION

Power* initPower(void){
  // Allocate memory space for new Power struct
  Power* power = malloc(sizeof(Power));
  // Check if allocation was successful
  if (power != NULL){
    power->type   = 0;
    power->active = false;
    power->timer  = 0;
    power->next   = NULL;
  }
  // Return new power struct
  return power;
}

// FILES

// FUNCTIONS



// TESTS

bool isValidPowers(Power* powers) {
  // Check if passed parameter is valid
  if (powers != NULL) {
    // Allocate memory space for test response
    bool isValid = true;
    // Read through power chain
    while (powers != NULL) {
      // Check single power
      if (!isValidPower(powers)) {
        isValid = false;
      }
      // switch to next power in chain
      powers = powers->next;
    }
    // check if successful
    if (!isValid) {
      // If not, then output error
      logger(ERR,CALLER,"invalid power chain!");
    }
    // Return test value
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate powers!");
    return false;
  }
}

bool isValidPower(Power* power) {
  // Check if passed parameter is valid
  if (power != NULL) {
    // Allocate memory space for test response
    bool isValid = true;

    // TODO: perform checks

    // Return test value
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate power");
    return false;
  }
}

// DESTROY

void destroyPowers(Power* powers) {
char buffer[MAXSTRING];

logger(WARN,CALLER,"destroying powers...");
// Set power to destroy
Power* toDestroy = powers;

int i = 0; // Destroyed powers count
while (toDestroy != NULL) {
  // Put next power in buffer
  Power* next = toDestroy->next;
  // Free current power
  destroyPower(toDestroy);
  // Set next power as current
  toDestroy = next;
  // Increment destroyed powers counter
  i++;
  }
  // Feedback: number of powers effectively destroyed
  sprintf(buffer,"%i powers destroyed.",i);
  logger(WARN,CALLER,buffer);
}

void destroyPower(Power* power){
  // Free power struct from memory
  free(power);
}

// UTILITY
void printPowers(Power* powers) {
  // Read array of powers and print out each entry
  while (powers!= NULL) {
    printPower(powers);
    powers = powers->next;
  }
}

void printPower(Power* power) {
  // char buffer[MAXSTRING];
  // // Put power info in buffer
  // sprintf(buffer,"%s\n\t\t~HP: %i/%i\n\t\t~AP: %i/%i\n\t\t~SP: %i/%i\n\t\t~Sight: %i\n\t\t~Reach: %i"
  //        ,mobType_value[mob->type]
  //        ,mob->currentHealth,mob->baseHealth
  //        ,mob->currentAttack,mob->baseAttack
  //        ,mob->currentSpeed,mob->baseSpeed
  //        ,mob->sight
  //        ,mob->reach);
  // // Output buffer to logger
  // logger(INFO,CALLER,buffer);
  // // Print power coordinates
  // printCoord(mob->coord);
  // printSprite(mob->sprites);
}
