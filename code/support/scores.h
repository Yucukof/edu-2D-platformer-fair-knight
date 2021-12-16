//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#ifndef _score_
#define _score_ "scores.h"

#include "lib.h"
#define MAXPSEUDO 10

#define structdef(NAME) typedef struct NAME NAME; struct NAME
// Score board
structdef(Score){
   char  pseudo[MAXPSEUDO];  // Player pseudonym
    int  points;             // Values of items collected/monsters killed
    int  kills;              // Number of monsters killed
  Score* next;               // Next score in array (for score keeping)
};

// INITIALISATION

Score* initScore(void);
  /* Initialise new score structure */

// FILES

Score* readScores(void);
 /* Read score file and return first pointer to array of scores */
void writeScores(Score* score);
  /* Write array of scores beyond pointer to file */

// FUNCTIONS

void updateScore (Score* score, int point,int kill);
  /* Update score values with passed input */
void resetScore  (Score* score);
  /* Reset score values to 0 */
void insertScore(Score** score, Score** scores, int rank);
  /* Insert score in scores chain at indicated rank */

void registerScore(Score** score, Score** scores);
  /*Check if new score is ranked and save it to scores support resources */
void registerPseudo(char* pseudo, Score* score);
  /* Save pseudo into score struc */

int getScoreRank(Score* score, Score* scores);
  /* Compute score ranking compared to passed chain */

// TESTS

bool isValidScores(Score* scores);
  /* Validate array of scores */
bool isValidScore(Score* score);
  /* Check if score is valid */
bool isBetterScore(Score* new, Score* old);
  /*  Check if new score is better than old score */

// DESTROY

void destroyScores(Score* scores);
  /* Free entire array of scores beyond pointer from memory*/
void destroyScore(Score* score);
  /* Free single score from memory*/

// UTILITY

void printScore(Score* score);
/* Print score value to screen */

#endif
