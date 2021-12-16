//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "fonts.h"

#define CALLER "fonts"

FT_Library ft; // FreeType Global library variable

// INITIALISATION

Font* initFont(void) {
  // Allocate memory space for Font struct
  Font* font = malloc(sizeof(Font));
  // Set initial values
  font->name[0] = '\0';
  font->face    = NULL;
  font->index   = 0;
  font->next    = NULL;
  // Return pointer
  return font;
}

// FILES

Font* readFonts(void) {
  char buffer[MAXSTRING];

  logger(INFO,CALLER,"reading fonts...");
  // Set fonts target
  Font* fonts = NULL;

  // Open fonts fle
  File* f = openFile("fonts",CALLER, "r");

  // Check if file was successfully opened
  if (f->ptr != NULL) {

    int i = 0   // counter of fonts
      , j = 0;  // counter of rows
    Font* previous = NULL;
    // Read font file
    while(!feof(f->ptr)) {
      // Allocate space for new font
      Font* next = initFont();
      // Read next row into new font
      fscanf(f->ptr,"%s\n",next->name);

      if (next->name[0] != '\0') {
        next->index = i;

        // IF previous font exists
        if (previous != NULL) {
          // THEN set current font as next
          previous->next = next;
        } else {
          // ELSE set first font
          fonts = next;
        }
        // Switch to next font
        previous = next;
        i++;
      } else {
        sprintf(buffer,"invalid font found at row %i",j);
        logger(ERR,CALLER,buffer);
      }
      j++;
    }
    // Set final font next to NULL (end of array)
    previous->next = NULL;

    // Feedback: number of font sread
    sprintf(buffer,"%i fonts read.",i);
    logger(INFO,CALLER,buffer);

    // Release fonts file
    closeFile(f);
  }
  loadFonts(fonts);
  // Return pointer
  return fonts;
}

bool loadFonts(Font* fonts) {
  char buffer[MAXSTRING];

  logger(INFO,CALLER,"initialising FreeType...");
  // Initialise FreeType and validate loading
  if (FT_Init_FreeType(&ft)) {
    logger(ERR,CALLER,"failed to init FreeType!");
  } else {
    logger(INFO,CALLER,"FreeType successfully loaded.");

    int i = 0  // font counter
      , j = 0; // row cointer
    Font* font = fonts; // fonts array

    // while there is a valid font entry
    while (font != NULL) {
      // load corresponding ttf file
      if (loadFont(font,ft)) {
        // increment valid fonts counter if loading is successful
        i++;
      }
      // go to next font
      font = font->next;
      // increment row counter
      j++;
    }
    // Feedback: nubmer of valid fonts loaded:
    sprintf(buffer,"%i/%i fonts loaded.\n",i,j);
    logger(INFO,CALLER,buffer);

    // If no font was loaded, return error
    return (i > 0);
  }
  // if FreeType could not be loaded, return error
  return false;
}

bool loadFont(Font* font,FT_Library fontLibrary) {
  char buffer[MAXSTRING];
  char path[MAXSTRING];

  // put  actual font path in buffer
  concat(path,"../assets/fonts/",font->name);
  concat(path,path,".ttf");

  concat(buffer,"loading font ",font->name);
  concat(buffer,buffer,"...");

  logger(INFO,CALLER,buffer);

  FT_Face face; // FreeType Font object
  // Load font in memory based on path computed above (parameters: FT_Library, filepath, sub-font item eg. bold, target face variable)
  int error = FT_New_Face(fontLibrary,path,0,&face);

  // Check if error and of what type
  if (error == FT_Err_Unknown_File_Format) {
    // return error
    logger(ERR,CALLER,"ttf file format not recognised");
    return false;

  } else if (error) {
    // return error
    logger(ERR,CALLER,"unknown error raised");
    return false;

  } else {
    // If no error, save font, return success
    font->face = face;
    logger(TRACE,CALLER,"font successfully loaded.");
    return true;
  }
}

Font* findFont(Font* fonts,int index) {
  char buffer[MAXSTRING];

  // Store default font
  Font* defaultFont = fonts;

  // Search font in array of fonts starting from pointer
  while (fonts->index != index && fonts->next != NULL) {
    fonts = fonts->next;
  }

  // IF target font is not found
  if (fonts == NULL || fonts->index != index) {
    sprintf(buffer,"font %i not found",index);
    logger(ERR,CALLER,buffer);
    // THEN, attempt to use default
    fonts = defaultFont;
    // Check if default font is available
    if (defaultFont == NULL) {
      // IF not, show feedback
      logger(ERR,CALLER,"default font not found!");
    }
  }
  // Return font pointer
  return fonts;
}

// DESTROY

void destroyFonts(Font* fonts) {
  char buffer[MAXSTRING];
  logger(WARN,CALLER,"destroying fonts...");
  int i = 0; // Destroyed fonts counter
  // Free all fonts beyond pointer
  while (fonts != NULL) {
    // Store next font to destroy
    Font* next = fonts->next;
    // Free current font
    destroyFont(fonts);
    // Switch to next
    fonts = next;
    // Increment coutner
    i++;
  }
  sprintf(buffer,"%i fonts detroyed.\n",i);
  logger(WARN,CALLER,buffer);
}

void destroyFont(Font* font) {
  // Free font struct from memory
  free(font);
}

// UTILITY

void printFonts(Font* fonts){
  // Check if passed parameter is valid
  if (fonts != NULL) {
    while (fonts != NULL){
      printFont(fonts);
      fonts = fonts->next;
    }
  } else {
    logger(ERR,CALLER,"unable to print fonts!");
  }
}
void printFont(Font* font){
  char buffer[MAXSTRING*2];
  // Check if passed parameter is valid
  if (font != NULL){
    sprintf(buffer,"%i. %s"
           ,font->index
           ,font->name);
    logger(INFO,CALLER,buffer);
  } else {
    logger(ERR,CALLER,"unable to print font!");
  }
}
