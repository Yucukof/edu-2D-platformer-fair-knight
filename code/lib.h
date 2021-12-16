//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _lib_
#define _lib_ "lib.h"

// Standard libraries

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Included libraries

#include <GL/glew.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include "ft2build.h"
#include FT_FREETYPE_H

// Custom libraries

#include "debug.h"
#include "util.h"

// Constants

#define MAXSTRING    255 // Maximum length for String

#define FONTSIZE    24.0
#define FRAMERATE     60 // Number of frames per second
#define ANIMSPEED      4 // Number of animations per second
#define FLASHINGRATE   1 // Number of flash per second
#define MOVERATE      15 // Number of steps per second
#define MOVETIMER     10   // Lenght of jump

#define DISPLAYHEIGHT 300 // Screen height
#define DISPLAYWIDTH  500 // Screen width
#define UNIT           16 // Default game unit
#define BOTTOMBANNER  DISPLAYHEIGHT/8
#define MAXWIDTH       30 // Map maximum width
#define MAXHEIGHT      30 // Map maximum height

#define MAXSCORE       10 // Maximum number of high scores
#define SCORESTRING    10 // Max length for score names

#define DEFAULTTIMER   10 // Default duration for status timers

#define MAXMONSTERS     6 // Number of monsters active on board

// Enumeration

#define enumdef(NAME) typedef enum NAME NAME; enum NAME

// Sprites and characters directions
enumdef(direction){
  LEFT,
  RIGHT,
  UP,
  DOWN,
  INVERT,
  MAXDIRECTION
};

// Game state
enumdef(state) {
  MENU,
  GAME,
  MAXSTATE
};

// Possible sprite and font colour
enumdef(colour) {
  WHITE,
  RED,
  BLUE,
  GREEN,
  ORANGE,
  GREY,
  YELLOW,
  BLACK,
  BLUR,
  UNCHANGED,
  MAXCOLOUR
};

// Sprite types
enumdef(type){
  HERO,
  MOB,
  OBJ,
  TILE,
  BACK,
  BUTTON,
  MAXTYPE
};

// Hero sprite variants
enumdef(hero){
  KNIGHT,
  MAXHERO
};

// Monster sprites and type variants
enumdef(mob){
  BIGDEMON,
  BIGZOMBIE,
  CHORT,
  GOBLIN,
  ICEZOMBIE,
  ORC,
  CHAMAN,
  OGRE,
  MUDDY,
  NECROMANCER,
  WARRIOR,
  SKELETON,
  SWAMPY,
  TINYZOMBIE,
  WIZARD,
  WOGOL,
  ZOMBIE,
  MAXMOB
};

// item effects
enumdef(effect) {
  LOOT,
  INVULNERABILITY,
  DAMAGEUP,
  SPEEDUP,
  JUMPUP,
  LIFEUP,
  MAXSTATUS
};

// Object sprites and type
enumdef(obj){
  COIN,
  BLUE_FLASK,
  BULLET,
  RED_FLASK,
  HEART_FULL,
  HEART_EMPTY,
  SKULL,
  MAXOBJ
};

// TIle sprites and type
enumdef(tile){
  WALL,
  BANNER_BLUE,
  BANNER_RED,
  DOOR,
  MAXTILE
};

// Background Sprites and type
enumdef(back){
  CHURCH,
  MAXBACK
};

// Button sprites
enumdef(button){
  WOODEN,
  ARROW_LEFT,
  ARROW_RIGHT,
  ARROW_TOP,
  ARROW_DOWN,
  BUTTON_EMPTY,
  UNAMUR,
  OPENGL,
  PAUSE,
  MAXBUTTON
};

// Animation type (not used)
enumdef(anim) {
  IDLE,
  WALKING,
  ATTACKING,
  FALLING,
  INVULNERABLE,
  STUNNED,
  DYING,
  OPEN,
  MAXANIM
};

// Menu list
enumdef(menu) {
  HOMESCREEN,
  PLAY,
  RULES,
  SCORES,
  CREDITS,
  EXIT,
  MAXMENU
};

#endif
