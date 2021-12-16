//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "item.h"

#define CALLER "item"

char* item_value[] = {
  "LOOT",
  "INVULNERABILITY",
  "DAMAGEUP",
  "SPEEDUP",
  "JUMPUP",
  "LIFEUP"
};

// INITIALISATION

Item* initItem(void) {
  // Initialise item
  Item* item = malloc(sizeof(Item));
  // Check if instantiation was successful
  if (item != NULL) {
    // Set initial values
    item->name[0] = '\0';
    item->type    = MAXOBJ;
    item->value   = 0;
    item->sprites = NULL;
    item->next    = NULL;
    item->coord   = initCoord();
    // Feedback
  } else {
    //ELSE, print error
    logger(FAIL,CALLER,"failed to load item!");
  }
  // Return struct
  return item;
}

// FILES

Item* readItems(void) {
  char buffer[MAXSTRING];

  logger(INFO,CALLER,"loading items spec...");
  // opening items file
  File* f = openFile("items.csv","text","r");
  //
  Item* items = NULL;
  // Check if file was successfully opened
  if (f->ptr != NULL) {
    Item* previous = NULL;

    int i = 0,j = 0; // i = item counter, j = row counter
    while (!feof(f->ptr)) {
      // Allocate space for new item
      Item* next = initItem();
      // Read new row in items file
      fscanf(f->ptr,"%s ; %u ; %u\n"
            , next->name
            ,&next->type
            ,&next->value);

      // IF item values are valid
      if (isValidItem(next)) {
        // THEN, add new item
        // IF previous item exists
        if (previous != NULL) {
          // THEN, add curent item as next
          previous->next = next;
        } else {
          // ELSE, set first item of array
          items = next;
        }
        // Switching to next item
        previous  = next;
        i++;
      } else {
        // ELSE output error location
        sprintf(buffer,"invalid item entry found at: %i",j);
        // and free allocated space
        destroyItem(next);
        logger(WARN,CALLER,buffer);
      }
      j++;
    }
    // Set final item next to NULL 'end of array)'
    previous->next = NULL;
    // Feedback: number of items loaded
    sprintf(buffer,"%i items loaded.",i);
    logger(INFO,CALLER,buffer);
    // Release items file
    closeFile(f);
  } else {
    // THEN report failure and exit
    logger(FAIL,CALLER,"failed load items!");
  }
  return items;
}

// FUNCTIONS

Item* appendItem(Item* item,Item* items) {
  // Allocate memory space for output pointer
  Item* output = NULL;
  // check if input is valid
  if (isValidItem(item)) {
    // Check if chain exists
    if (items == NULL) {
      // If not, start chain
      output = item;
    } else {
      // Else check if chain is valid
      if (isValidItems(items)) {
        // Store chain start pointer
        output = items;
        // Read through chain to find append position
        while (items->next != NULL) {
          items = items->next;
        }
        // Append item to chain
        items->next = item;
        logger(TRACE,CALLER,"item appended.");
      }
    }
  } else {
    // Output failure
    logger(ERR,CALLER,"unable to append item");
  }
  // Return chain start
  return output;
}

bool setItemSprites(Item* obj,type type, int name, anim anim, Asset* assets) {
  if (obj != NULL && obj->coord !=  NULL) {
    obj->sprites = loadAssetSprites(type,name,anim,assets);
    // Check if sprites were successfully loaded
    if (obj->sprites != NULL) {
      // Set obj hitbox size
      obj->coord->w   = obj->coord->x+(float)obj->sprites->width;
      obj->coord->h   = obj->coord->y+(float)obj->sprites->height;
      // Return success
      return true;
    } else {
      // If anim or coord failed to load, output error and return failure
      logger(ERR,CALLER,"failed to load obj sprites!");
      return false;
    }
  }
  // Output error
  logger(ERR,CALLER,"unable to set obj sprites!");
  // Return failure
  return false;
}

int collectItems(Coordinates* coord, Item** items) {
  // Allocate memory space for and initialise output
  int output = 0;
  // Check if input is valid
  if (isValidCoord(coord) && ((*items) == NULL || isValidItems(*items))) {
    // Make cursor to read through chain
    Item* cursor = *items;
    // Read through chain for collectable item
    while (cursor != NULL){
      Item* next = next = cursor->next;
      // Check if item can be collected
      if (isColliding(coord,cursor->coord)) {
        // Collect item
        output += collectItem(cursor,items);
      }
      // Move through chain
      cursor = next;
    }
  } else {
    // IF input is not valid, display error message
    logger(ERR,CALLER,"unable to collect items.");
  }
  // Return score value as output (0 if not found or invalid)
  return output;
}

int collectItem(Item* item, Item** items) {
  // Allocate memory space for and initialise output
  int output = 0;
  // Check if input is valid
  if (isValidItem(item) && isValidItems(*items)) {
    // Allocate memory for cursor variables
    Item* previous = NULL;
    Item* cursor   = *items;
    // Read through chain to find target item
    while (cursor != item && cursor != NULL) {
      // Save previous item address
      previous = cursor;
      // Switch to next
      cursor = cursor->next;
    }
    // check if item was successfully found
    if (cursor != NULL) {
      // Check if is start of chain
      if (previous != NULL) {
        // If not, then remove item simply
        previous->next = cursor->next;
      } else {
        // Else, change start of chain
        *items = cursor->next;
      }
      // Store its score value in output variable
      output += cursor->value;
      // free memory
      destroyItem(cursor);
    } else {
      // If not found, ouput error message
      logger(WARN,CALLER,"item not found in chain.");
    }
  } else {
    // IF input is not valid, display error message
    logger(ERR,CALLER,"unable to collect item.");
  }
  // Return score value as output (0 if not found or invalid)
  return output;
}

// TESTS

bool isValidItems(Item* items) {
  // Check if passed parameteris valid
  if (items != NULL) {
    // Allocate memory space for test result
    bool isValid = true;
    // Validate all items in chain
    while (items != NULL) {
      if (!isValidItem(items)) {
        isValid = false;
      }
    items = items->next;
    }
    // Evaluate test result
    if (!isValid) {
      logger(ERR,CALLER,"invalid items");
    }
    // Return test results
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"could not validate items!");
    return false;
  }
}
bool isValidItem(Item* item) {
  // Check if passed parameteris valid
  if (item != NULL) {
    // Allocate memory space for test result
    bool isValid = true;
    // Perform checks
      if (item->coord == NULL && !isValidCoord(item->coord)) {
        isValid = false;
      }
      if (item->sprites != NULL && !isValidSprites(item->sprites)) {
        isValid = false;
      }
    // Evaluate test results
    if (!isValid) {
      logger(ERR,CALLER,"invalid item");
    }
    // Return test result
    return isValid;
  } else {
    logger(ERR,CALLER,"could not validate item!");
    return false;
  }
}

// DESTROY

void destroyItems(Item* items){
  char buffer[MAXSTRING];

  logger(WARN,CALLER,"detroying items...");
  // Set item to be destroyed
  Item* toDestroy = items;
  int i = 0; // Detroyed items counter
  while (toDestroy != NULL) {
    // Save next item in buffer
    Item* next = toDestroy->next;
    // Free current item
    destroyItem(toDestroy);
    // Set next item as current
    toDestroy = next;
    // Increment destroyed items counter
    i++;
  }
  // Feedback: number of items destroyed
  sprintf(buffer,"%i items destroyed.",i);
  logger(WARN,CALLER,buffer);
}

void destroyItem(Item* item){
  // Destroy associated coordinates
  destroyCoordinates(item->coord);
  // Free item from memory
  free(item);
  logger(TRACE,CALLER,"item destroyed.");
}

// UTILITY

void printItems(Item* items) {
  // Print array of items
  while (items != NULL) {
    printItem(items);
    items = items->next;
  }
}

void printItem(Item* item) {
  // check if passed parameter is valid
  if (isValidItem(item)) {
    char buffer[MAXSTRING*2];
    sprintf(buffer,"%s\n\t~ Value: %i"
                  ,item->name,item->value);
    printCoord(item->coord);
  } else {
    logger(ERR,CALLER,"unable to print item");
  }
}
