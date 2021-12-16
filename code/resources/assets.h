//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _assets_
#define _assets_ "assets.h"

#include "lib.h"
#include "sprites.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Asset storing entry
structdef(Asset){
    char  path[MAXSTRING];
    type  type;
     int  name;
    anim  anim;
     int  frames;
  Sprite* sprites;
   Asset* next;
};

// INITIALISATION

Asset* initAsset(void);
  /* Create new asset struct */

// FILES
Asset* readAssets(void);
  /* Read assets file to load assets */

// FUNCTIONS

void setAssetPath(Asset* a);
  /* Resolve actual path to file from asset type */
Sprite* loadAssetSprites(type type, int name, anim anim, Asset* assets);
  /* Find matching assets and return its sprites */

// TESTS

bool isValidAsset(Asset* a);
  /* Validate asset content */
bool isDuplicateAsset(Asset* a, Asset* assets);
  /* Check if similar assets exists in assets chain */
bool isMatchingAssets (Asset* a1, Asset* a2);
  /* Compare two assets for equivalence */

// DESTROY

void destroyAssets(Asset* assets);
  /* Free array of assets beyond pointer */
void destroyAsset(Asset* asset);
  /* Free asset and related sprites */

// UTILITY
void printAssets(Asset* assets);
  /* Output array of asset to screen */
void printAsset(Asset* asset);
  /* Output asset info to screen */

#endif
