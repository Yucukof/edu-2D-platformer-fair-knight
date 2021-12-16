//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _text_
#define _text_ "text.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "lib.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Font storing entry
structdef(Font){
     char  name[MAXSTRING];
  FT_Face  face;
      int  index;
     Font* next;
};

// INITIALISATION

Font* initFont(void);

// FILES

Font* readFonts(void);
  /* Read fonts file and load corresponding font files */
bool loadFonts(Font* fonts);
  /* Execute loadFont for fonts array */
bool loadFont(Font* font, FT_Library fontLibrary);
  /* Load ttf file for specified font */

// FUNCTIONS

Font* findFont(Font* fonts, int index);
  /* Find font in resources for drawing */

// DESTROY

void destroyFonts(Font* fonts);
  /* Free array of fonts from memory from pointer*/
void destroyFont(Font* font);
  /* Free font and face object from memory */

// UTILITY

void printFonts(Font* fonts);
  /* Print array of font to screen */
void printFont(Font* font);
  /* Print font info to screen */

#endif
