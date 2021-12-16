//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//
#ifndef _monster_
#define _monster_ "monster.h"

#include "lib.h"
#include "coord.h"
#include "sprites.h"
#include "assets.h"

#define structdef(NAME) typedef struct NAME NAME; struct NAME

// Monster characters info
structdef(Monster){
  Coordinates*  coord;            // Position on field
         char   name[MAXSTRING];  // Monster name
          mob   type;             // Type of monster
          int   baseHealth;       // Default health
          int   currentHealth;    // Current health level
          int   baseSpeed;        // Default speed
          int   currentSpeed;     // Modified speed
          int   baseAttack;       // Defautl attack
          int   currentAttack;    // Current attack
          int   sight;            // Lenght of sight to detect player
          int   reach;            // Max distance for hitting player
         bool   isStunned;        // Stunning status of monster
         bool   isAttacking;      // Attacking status of monster
          int   timer;            // Stun/random movement timer
          int   scoreValue;       // Kill value
       Sprite*  sprites;          // Link to sprite object
      Monster*  next;             // Link to chained monsters
};

// INITIALISATION

Monster* initMonster(void);
  /* Initialise new monster */

// FILES

Monster* readMonsters  (void);
  /* Read monster definition file */

// FUNCTIONS

Monster* appendMonster(Monster* monster, Monster* monsters);
  /* Append monster to chain and return first items of chain*/
void removeMonster(Monster* monster, Monster** monsters);
  /* Remove monster from chain and retart it if needed */
void hitMonster(Monster* monster, int hitpoint);
  /* Inflict damage to monster currentHealth following hitpoint */

Monster* getFirstHitMonster(Coordinates* coord, Monster* monsters);
  /* Get first monster that is hit by coordinates */

bool setMonsterSprites(Monster* mob,type type, int name, anim anim, Asset* assets);
  /* Find and attached monster sprites */
void setAttacking(Monster* monster, bool activate);
  /* Set or reset isAttacking flag */
void setStunned(Monster* monster, bool activate);
  /* Set or reset isStunned flag */

// TESTS

bool isValidMonsters(Monster* mobs);
  /* Check if all monsters in chain are vaild */
bool isValidMonster(Monster* mob);
  /* Check if monster struct is valid */
bool isStunnedMonster(Monster* monster);
  /* Check if monster is stunned */
bool isDeadMonster(Monster* monster);
  /* Check if monster has HP left */

// DESTROY

void destroyMonsters(Monster* mobs);
  /* Free array of monster beyond pointer from memory */
void destroyMonster(Monster* mob);
  /* Free Monster from memory*/

// UTILITY

void printMonsters(Monster* mob);
  /* Print array of monsters beyond pointer*/
void printMonster(Monster* mob);
  /* Print monster info and coordinates to screen*/

#endif
