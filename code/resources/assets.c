//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "assets.h"

#define CALLER "assets"

// INITIALISATION

Asset* initAsset(void) {
  // Allocate memory space for new Asset struct
  Asset* asset = malloc(sizeof(Asset));
  // Check if instantiation was successful
  if (asset != NULL) {
    // Set initial values
    asset->path[0]  ='\0';
    asset->type     = 0;
    asset->name     = 0;
    asset->anim     = 0;
    asset->frames   = 0;
    asset->sprites  = NULL;
    asset->next     = NULL;
  }
  // return pointer
  return asset;
}

// FILES

Asset* readAssets(void) {
  char buffer[MAXSTRING];

  logger(INFO,CALLER,"loading assets...");
  // Declare return variable
  Asset* assets = NULL;

  // Open Sprites file
  File* f = openFile("sprites","sprites","r");

  // CHeck if file was successfully opened
  if (f->ptr != NULL) {
    int i = 0   // Asset counter
      , j = 0;  // row counter
    logger(INFO,CALLER,"loading sprites list...");

    Asset* previous = NULL;

    while (!feof(f->ptr)) {
      // Allocate asset space in memory
      Asset* next = initAsset();
      // Read row from file into asset
      fscanf(f->ptr,"[%u-%u-%u] %s (%i)\n"
            ,&next->type
            ,&next->name
            ,&next->anim
            , next->path
            ,&next->frames);

      // Check if asset is valid
      if (isValidAsset(next)&&!isDuplicateAsset(next,assets)) {
        concat(buffer,"- ",next->path);
        logger(INFO,CALLER,buffer);
        // Dynamically set actual file path (based on asset type)
        setAssetPath(next);
        // Append additional frames if asset is animated
        next->sprites = buildSprites(next->path, next->frames);
        // Check if previous asset exists
        if(previous != NULL) {
          // THEN, Chain current asset with previous
          previous->next = next;
        } else {
          // ELSE, set current asset as first asset
          assets = next;
        }
        // Switch to next asset
        previous = next;
        // Increment asset counter
        i++;
      } else {
        // Output error
        sprintf(buffer,"invalid asset found at: %i",j);
        logger(WARN,CALLER,buffer);
        // free invalid asset from heap
        destroyAsset(next);
      }
      // Increment row counter
      j++;
    }
    // End asset chain
    previous->next = NULL;
    // Feedback: number of valid assets read
    sprintf(buffer,"%i/%i assets loaded",i,j);
    logger(INFO,CALLER,buffer);
    // Release sprites file
    closeFile(f);
  }
  return assets;
}

// FUNCTIONS

void setAssetPath(Asset* asset) {
  char path[MAXSTRING];
  // Specify target directory of assets (based on asset type)
  switch (asset->type) {
    case HERO:
      strcpy(path,"player/");
      break;
    case MOB:
      strcpy(path,"monster/");
      break;
    case OBJ:
      strcpy(path,"item/");
      break;
    case TILE:
      strcpy(path,"tile/");
      break;
    case BACK:
      strcpy(path,"background/");
      break;
    case BUTTON:
      strcpy(path,"button/");
      break;
    case MAXTYPE:
      break;
  }
  concat(path,path,asset->path);
  strcpy(asset->path,path);
}

Sprite* loadAssetSprites(type type, int name, anim anim, Asset* assets) {
  // Initialise return pointer variable
  Sprite* s = NULL;
  // Build target asset as struct
  Asset target = {
    .type = type,
    .name = name,
    .anim = anim
  };
  // Browse resources for target asset
  while (assets != NULL && !isMatchingAssets(assets,&target)) {
    assets = assets->next;
  }
  // IF target asset was found
  if (assets != NULL) {
    // Check if sprites is available
    if (assets->sprites != NULL) {
      // Set its address in return variable
      s = assets->sprites;
    } else {
      logger(ERR,CALLER,"sprites not found!");
    }
  } else {
    // ELSE, output error
    logger(ERR,CALLER,"asset not found!");
  }
  // return pointer
  return s;
}

// TESTS

bool isValidAsset(Asset* a) {
  // Check if input is valid
  if (a == NULL) {
    // Otherwise output failure
    return false;
  }
  // Make sure assets match enums
  // TYPE enum
  if (a->type < 0 || a->type >= MAXTYPE) {
    logger(ERR,CALLER,"invalid type found!");
    return false;
  }
  // ANIM enum
  if (a->sprites <0 || a->anim >= MAXANIM) {
    logger(ERR,CALLER,"invalid anim found!");
    return false;
  }
  // Depending on type, check on relevant enum
  // HERO/MOB/OBJECT/TILE/BACKGROUND/BUTTON
  switch (a->type) {
    case HERO:
      if (a->name <0 || a->name >= MAXHERO) {
        return false;
      }
      break;
    case MOB:
      if (a->name <0 || a->name >= MAXMOB) {
        return false;
      }
      break;
    case OBJ:
      if (a->name <0 || a->name >= MAXOBJ) {
        return false;
      }
      break;
    case TILE:
      if (a->name <0 || a->name >= MAXTILE) {
        return false;
      }
      break;
    case BACK:
      if (a->name <0 || a->name >= MAXBACK) {
        return false;
      }
      break;
    case BUTTON:
      if (a->name <0 || a->name >= MAXBUTTON) {
        return false;
      }
      break;
    case MAXTYPE:
      logger(ERR,CALLER,"invalid name found");
      return false;
      break;
  }
  // If all tests passed, then return true
  return true;
}

bool isDuplicateAsset(Asset* a, Asset* assets) {
  //Check in array of assets if a similar asset is already present
  while (assets != NULL) {
    if (isMatchingAssets(a,assets)) {
      // if so, return true and exit
      return true;
    }
    // switch to next
    assets = assets->next;
  }
  // if no similar asset is found, then return false
  return false;
}

bool isMatchingAssets(Asset* a1, Asset* a2) {
  // Check if passed parameters are valid
  if (a1 != NULL && a2 != NULL) {
    // Compare assets values
    if ( a1->type == a2->type
      && a1->name == a2->name
      && a1->anim == a2->anim ) {
        // If all are equivalent, then assets are the same
        return true;
    }
  } else {
    // Output error
    logger(ERR,CALLER,"unable to match assets!");
  }
  // In all other cases, assets cannot be the same
  return false;
}

// DESTROY

void destroyAssets(Asset* assets) {
  char buffer[MAXSTRING];
  logger(WARN,CALLER,"destroying assets...");
  int i = 0; // Destroyed assets counter
  // Free all assets in chain
  while(assets != NULL) {
    // Store next assets before destruction
    Asset* next = assets->next;
    // Free current asset
    destroyAsset(assets);
    // Switch to next
    assets = next;
    // Increment counter
    i++;
  }
  // Output confirmation to screen
  sprintf(buffer,"%i assets destroyed.",i);
  logger(WARN,CALLER,buffer);
}

void destroyAsset(Asset* asset) {
  // free assets sprites
  destroySprites(asset->sprites);
  free(asset);
}

// UTILITY

void printAssets(Asset* assets) {
  // Check if asset is valid
  if (isValidAsset(assets)) {
    while (isValidAsset(assets)) {
      printAsset(assets);
      assets = assets->next;
    }
  } else {
    // Output error to screen
    logger(ERR,CALLER,"unable to print assets");
  }
}

void printAsset(Asset* asset) {
  // Check if asset is valid
  if (isValidAsset(asset)) {
    char buffer[MAXSTRING*2];
    // Put info to buffer
    sprintf(buffer,"%s\n\t\t~ Type %i\n\t\t~ Index %i\n\t\t~ %i frames"
           ,asset->path
           ,asset->type
           ,asset->name
           ,asset->frames);
    // Output info to screen
    logger(INFO,CALLER,buffer);
    // Output first sprite if available
    printSprite(asset->sprites);
  } else {
    // Output error to screen
    logger(ERR,CALLER,"unable to print asset");
  }
}
