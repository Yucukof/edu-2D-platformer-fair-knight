//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "sprites.h"

#define CALLER "sprites"

// INITIALISATION

Sprite* initSprite(void) {
  // Allocate memory space for new Sprite struct
  Sprite* s = malloc(sizeof(Sprite));
  // Check if instantiation was successful
  if (s != NULL) {
    // Set initial values
    s->path[0]  = '\0';
    s->frame    = 0;
    s->width    = 0;
    s->height   = 0;
    s->texture  = 0;
    s->next     = NULL;
  }
  // return pointer
  return s;
}

// FUNCTIONS

Sprite* buildSprites(char* path, int frames) {
  char buffer[MAXSTRING];
  Sprite* first = NULL;  // First sprite of chain
  // check if asset contains more than 1 frame
  if (frames > 1) {
    // IF yes, THEN Initialise loop
    Sprite* previous = NULL;  // Previous sprite in chain
    char frameNumber[3];      // Temporary buffer to insert frame number in path
    int i = 0;                // frame counter
    do {
      // Allocate frame space
      Sprite* next = initSprite();
      // Dynmically determine frame path and store it
      strcpy(next->path,path);
      sprintf(frameNumber,"_%i",i);
      strcat(next->path,frameNumber);
      // Store frame number
      next->frame = i;
      // Check if previous frame exists
      if (previous != NULL) {
        // chain previous frame with current
        previous->next = next;
      } else {
        // ELSE, set first frame
        first = next;
      }
      // switch to next frame
      previous = next;
      // increment counter
      i++;
    } while (i < frames);
    // POint  back to firt frame to make loop
    previous->next = first;
    // Feedback: Number of frames loaded
    sprintf(buffer,"-> %i frames loaded",i);
    logger(INFO,CALLER,buffer);
  } else {
    // ELSE, save single sprite in asset (with self chaining)
    first = initSprite();
    strcpy(first->path,path);
    first->frame = 0;
    first->next  = first;
    // No feeback
  }
  return first;
}

void loadSprites(Sprite* sprites, int frames) {
  char buffer[MAXSTRING];
  // check if passed parameter is valid
  if (isValidSprites(sprites)) {
    // Make cursor
    Sprite* cursor = sprites;
    logger(INFO,CALLER,"loading animation...");
    // Load each frame
    for (int i = 0; i < frames; i++) {
      loadTexture(cursor);
      // load GLUT texture and check if successful
      if (isValidTexture(cursor)) {
        // if sucessfull, check if next frame is available
        if (cursor->next == NULL) {
          // Output error
          concat(buffer,"incomplete animation detected: ",cursor->path);
          logger(ERR,CALLER,buffer);
        }
        // Switch to next
        cursor = cursor->next;
      }
    }
  } else {
    // Output error
    logger(ERR,CALLER,"unable to load sprites!");
  }
}

void loadTexture(Sprite* s) {
  // Check if passed input is valid
  if (isValidSprite(s)) {
    char buffer[MAXSTRING*2];
    // check if texture was not already loaded
    if (!glIsTexture(s->texture)) {
      // Log start on screen
      sprintf(buffer,"loading from %s...",s->path);
      logger(INFO,CALLER,buffer);
      // Enable bi-dimensional image representation (bitmap)
      glEnable(GL_TEXTURE_2D);
      // Allocate space for texture and default it to false
      GLuint texture = 0;
      // Ask GLUT to generate unilayer texture (for it is 2D, no need for more)
      glGenTextures(1, &texture);
      // Activate texturing
      glBindTexture(GL_TEXTURE_2D, texture);
      // Check if texture was successfully generated
      if (glIsTexture(texture)) {
        // Set texture parameter (clamping,aliasing)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int c; // channel variable, used to store output of SOIL but will not be used
        // Set actual path
        strncpy(buffer,s->path,MAXSTRING);
        concat(buffer,"../assets/sprites/",buffer);
        concat(buffer,buffer,".png");
        // Load image as texturable bitmap using SOIL library
        unsigned char* image = SOIL_load_image(buffer,&s->width,&s->height,&c,SOIL_LOAD_RGBA);
        // Check if loading was successful
        if (image != NULL) {
          // Output success
          logger(TRACE,CALLER,"image read.");
          // Copy image data to texture
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->width, s->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
          // Release image data
          SOIL_free_image_data(image);
          // Check if texture was loaded successfully
          if (glIsTexture(texture)) {
            // Output success
            logger(TRACE,CALLER,"texture loaded.");
            // Save texture for later use
            s->texture = texture;
          } else {
            // else output failure
            logger(ERR,CALLER,"failed to load sprite.");
          }
        } else {
          // If not, output error and return failure
          concat(buffer,"could not load sprite at ",buffer);
          logger(ERR,CALLER,buffer);
        }
      } else {
        // Output GLUT error
        sprintf(buffer,"failed to instantiate texture! (%i)",glGetError());
        logger(ERR,CALLER,buffer);
      }
    } else {
    }
  } else {
    logger(ERR,CALLER,"unable to load asset");
  }
}

// TESTS

bool isValidSprites(Sprite* sprites) {
  // Check if passed parameter is valid
  if (sprites != NULL) {
    // Allocate memory space for test response
    bool isValid = true;
    // Make cursor to cycle through chain
    Sprite* cursor = sprites;
    // Perform checks on sprites chain
    while (cursor){
      // Check if sprite is valid
      if (!isValidSprite(cursor)) {
        isValid = false;
      } else {
        // Check if animation loop was fully read (back to start)
        if (cursor == sprites) {
          // IF so, exit test
          break;
        }
      }
      // ELSE switch to next sprite
      cursor = cursor->next;
    }
    // Evaluate test result
    if (!isValid) {
      logger(ERR,CALLER,"invalid sprite found!");
    }
    // Return test result
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate sprite!");
    return false;
  }
}

bool isValidSprite(Sprite* sprite) {
  // Check if passed parameter is valid
  if (sprite != NULL) {
    // Allocate memory space for test response
    bool isValid = true;
    // Check if core values are valid
    if (sprite->path == NULL || strlen(sprite->path) == 0) {
      logger(ERR,CALLER,"invalid path!");
      isValid = false;
    } else {
      // If texture is said to be loaded, validate texture
      if (sprite->texture != 0 && !glIsTexture(sprite->texture)) {
        logger(ERR,CALLER,"invalid texture!");
        isValid = false;
      }
       // else {
       //  // Perform additional checks
       //  if (sprite->width == 0) {
       //    logger(ERR,CALLER,"invalid width!");
       //    isValid = false;
       //  }
       //  if (sprite->height == 0) {
       //    logger(ERR,CALLER,"invalid height!");
       //    isValid = false;
        // }
      // }
    }
    // Evaluate test result
    if (!isValid) {
      logger(ERR,CALLER,"invalid sprite found!");
    }
    // Return test result
    return isValid;
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate sprite!");
    return false;
  }
}

bool isValidTexture(Sprite* s) {
  // Allocate memory space for test result
  bool isValid = false;
  // Check if passed input is valid
  if (isValidSprite(s)) {
    // Check if texture was previously loaded
    if (s->texture != 0) {
      // Check if texture was generated by GLUT
      if (glIsTexture(s->texture)) {
        // If yes, then texture is valid
        isValid = true;
      } else {
        // Else output error
        logger(ERR,CALLER,"texture is not GLUT-valid");
      }
    } else{
      // Otherwise, output that texture is not loaded
      logger(TRACE,CALLER,"texture is not loaded");
    }
  }
  // Return test result
  return isValid;
}

// DESTROY

void destroySprites(Sprite* sprites) {
  char buffer[MAXSTRING];
  // Check if passed input is valid
  if (isValidSprites(sprites)){
    logger(TRACE,CALLER,"destroying sprites...");
    // Set first sprite of chain (to avoid looping destruction)
    Sprite* first = sprites;
    int i = 0; // destroyed sprites counter
    while (sprites != NULL && sprites != first) {
      // Store next Sprite to destroy
      Sprite* next = sprites->next;
      // Free current sprite
      destroySprite(sprites);
      // Switch to next
      sprites = next;
      i++;
    }
    sprintf(buffer,"%i sprites destroyed.",i);
    logger(TRACE,CALLER,buffer);
  }
}

void destroySprite(Sprite* sprite) {
  logger(TRACE,CALLER,"destroying sprite...");
  free(sprite);
}

// UTILITY

void printSprite(Sprite* sprite) {
  // Check if sprite is valid
  if (sprite != NULL) {
    char buffer[MAXSTRING*2];
    // Put info to buffer
    sprintf(buffer,"%s-%i (%ix%i)"
           ,sprite->path
           ,sprite->frame
           ,sprite->width
           ,sprite->height);
    // Output info to screen
    logger(INFO,CALLER,buffer);
  } else {
    // Output error to screen
    logger(ERR,CALLER,"unable to print sprite");
  }
}
