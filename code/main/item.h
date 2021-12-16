//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _item_
#define _item_ "item.h"

#include "lib.h"
#include "coord.h"
#include "sprites.h"
#include "assets.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

structdef(Item){
  Coordinates* coord;            // Item position on field
         char  name[MAXSTRING];  // item name
          obj  type;             // item enum type
       effect  effect;           // item enum effect
          int  value;            // item score value
       Sprite* sprites;          // item sprites chain
         Item* next;             // item next chained item
};

// INITIALISATION

Item* initItem(void);
  /* Create new item structure */

// FILES

Item* readItems(void);
  /* Read item definition file */

// FUNCTIONS

Item* appendItem(Item* item,Item* items);
  /**/
bool setItemSprites(Item* obj,type type, int name, anim anim, Asset* assets);
  /**/
int collectItems(Coordinates* coord, Item** items);
  /* Read through item chain to determine and collect items in collision with coord */
int collectItem(Item* item, Item** items);
  /* Remove item from chain and return score value */

// TESTS
bool isValidItems(Item* items);
  /**/
bool isValidItem(Item* item);
  /**/

// DESTROY

void destroyItems(Item* items);
  /* Destroy chain of item */
void destroyItem(Item* item);
  /* Free item struc from memory */

// UTILITY
void printItems(Item* items);
  /* Print chain of items to screen */
void printItem(Item* item);
  /* Print item information on screen */


#endif
