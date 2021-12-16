//
// Copyright (c) 2019 by Hadrien BAILLY. All Rights Reserved.
//

#include "scores.h"

#define CALLER "score"

// INITIALISATION

Score* initScore(void) {

  logger(INFO,CALLER,"initialising score...");
  Score* score = malloc(sizeof(Score));

  // IF score was successful instantiated
  if (score != NULL) {
    // THEN, set initial values
    strncpy(score->pseudo,"PSEUDO",MAXPSEUDO);
    score->points = 0;
    score->kills  = 0;
    score->next   = NULL;
    logger(INFO,CALLER,"score initialised.\n");
  } else {
    // ELSE, log error and exit
    logger(FAIL,CALLER,"failed to initialise score!");
  }
  return score;
}

// FILES

Score* readScores(void) {
  char buffer[MAXSTRING];
  logger(INFO,CALLER,"loading high scores...");
  Score* scores = NULL;
  // Open Score file
  File* f = openFile("score","text","r");
  // Check if score file was successfully opened
  if (f->ptr != NULL) {
    int i = 0; // Score counter
    Score* previous = NULL;
    // Read score file and add entries up to MAXSCORE
    while (!feof(f->ptr) && i < MAXSCORE) {
      // Allocate space for new entry and read next row
      Score* next = malloc(sizeof(Score));
      fread(next,sizeof(Score),1,f->ptr);
      // IF previous score exists
      if (previous != NULL) {
        // THEN, add current score as next
        previous->next = next;
      } else {
        // ELSE, register first level
        scores = next;
      }
      // Switch to next and increment counter
      previous = next;
      i++;
      // Check if was last scores
      if (previous->next == NULL) {
        // If yes, then break
        break;
      }
    }
    // Set final score next to NULL (end of array)
    previous->next = NULL;
    // Feedback: number of score read
    sprintf(buffer," %i scores loaded.",i);
    logger(INFO,CALLER,buffer);
    // Release score file
    closeFile(f);
  } else {
    logger(WARN,CALLER,"0 scores loaded.\n");
  }
  return scores;
}

void writeScores (Score* scores) {
  char buffer[MAXSTRING];
    // Check if passed input is valids
    if (isValidScores(scores)) {
    logger(INFO,CALLER,"writing score to file...");

    File* f = openFile("score","text","w");
    if (f->ptr) {
      int i = 0; // counter of written score
      // Make cursor to read through scores
      Score* cursor = scores;
      while (cursor != NULL && i < MAXSCORE) {
        // Write to file
        fwrite(scores,sizeof(Score),1,f->ptr);
        cursor = cursor->next;
        i++;
      }
      // Feedback: number of scores writtent
      sprintf(buffer,"%i",i);
      concat(buffer,buffer," scores written to file.");
      logger(INFO,CALLER,buffer);
      closeFile(f);
    } else {
      logger(ERR,CALLER,"Unable to write score file!");
    }
  } else {
    logger(ERR,CALLER,"unable to write scores");
  }
}

// FUNCTIONS

void updateScore(Score* score, int point,int kill) {
  char buffer[MAXSTRING];
  // Check if valid input
  if (isValidScore(score)) {
    if (point < 0 || kill < 0 || (!point && !kill)) {
      if (point <= 0) {
        sprintf(buffer,"invalid points: %i",point);
        logger(WARN,CALLER,buffer);
        point = 0;
      }
      if (kill < 0) {
        sprintf(buffer,"invalid kill: %i",kill);
        logger(WARN,CALLER,buffer);
        kill = 0;
      }
    }
    // Echo update
    sprintf(buffer,"update\n\t~points: %i+%i\n\t~kills: %i+%i",score->points,point,score->kills,kill);
    logger(INFO,CALLER,buffer);
    // update score value
    score->points += point;
    score->kills  += kill;
  } else {
    // Otherwise, output error to screen
    logger(ERR,CALLER,"unable to update score");
  }
}

void resetScore (Score* score) {
  // Check if passed input is valid
  if (isValidScore(score)) {
    // Reset score value to zero
    score->points = 0;
    score->kills  = 0;
    logger(WARN,CALLER,"score reset.");
  } else {
    // Otherwise, output error to screen
    logger(ERR,CALLER,"Unable to reset score");
  }
}

void insertScore(Score** score, Score** scores, int rank) {
  char buffer[MAXSTRING];
  // Check if new score is valid
  if (isValidScore(*score)) {
    // Check if chain exists
    if (*scores != NULL) {
      // Check if scores are valid
      if (isValidScores(*scores)) {
        // Set variable to find rank position
        int i = 0;
        rank--; // adapt rank to machine counting
        // Make cursor to read through chain
        Score* cursor   = *scores;
        Score* previous = NULL;
        // Reach position
        while (i < rank && cursor != NULL) {
          previous = cursor;
          cursor = cursor->next;
          i++;
        }
        (*score)->next = cursor;
        if (previous == NULL) {
          *scores = *score;
        } else {
          previous->next = *score;
        }
        sprintf(buffer,"score inserted (%i rank)",rank+1);
        logger(INFO,CALLER,buffer);
        // Remove excessive scores from chain
        while (cursor != NULL && i < MAXSCORE) {
          previous = cursor;
          cursor = cursor->next;
          i++;
        }
        if (previous != NULL) {
          previous->next = NULL;
        }
        destroyScores(cursor);
      } else {
        logger(ERR,CALLER,"unable to insert in list.");
      }
    } else {
      // If not, then start chain with new score
      *scores = *score;
      logger(WARN,CALLER,"starting score list.");
    }
  } else {
    logger(ERR,CALLER,"Unable to insert new score.");
  }
}

int getScoreRank(Score* score, Score* scores) {
  // Allocate and initialise memory for output
  int rank = 1;
  // Check if passed input is valid
  if (isValidScore(score)) {
    // Check if chain to compare with exists
    if (scores != NULL) {
      // CHeck if chain is valid
      if (isValidScores(scores)) {
        // Make cursor to cycle in chain
        Score* cursor = scores;
        // Read trough chain
        while (cursor != NULL) {
          // Check if new score is better than current score in cursor
          if (isBetterScore(score,cursor)) {
            // if yes, break to return current ranking
            break;
          }
          // Else, increment ranking order
          rank++;
          cursor = cursor->next;
        }
      } else {
        logger(ERR,CALLER,"unable to compute ranking");
      }
    } else {
      // if not scores, then new score is first in chain and should have rank 1
      rank = 1;
    }
  } else {
    // Otherwise output error and return flag 0 ranking
    logger(ERR,CALLER,"unable to compute ranking (invalid score)");
    rank = 0;
  }
  // Output rank value
  return rank;
}

void registerScore(Score** score, Score** scores) {
  // Check if input is valid
  if (isValidScore(*score)) {
    // Check if score can be registered (empty or valid chain)
    if (*scores == NULL || isValidScores(*scores)) {
      // Get score ranking compared to chain
      int rank = getScoreRank(*score,*scores);
      // check if is effectively ranked
      if (rank > 0 && rank <= MAXSCORE) {
        // If so, then insert it in chain and remove last entry
        insertScore(score,scores,rank);
        // break source link to score for preservation
        *score = NULL;
      } else {
        // Else, do nothing
        logger(INFO,CALLER,"score not ranked.");
      }
    } else {
      // if there is an issue with scores, output error attempt to fix it
      logger(WARN,CALLER,"overriding scores");
      // If so, then insert it in chain and remove last entry
      insertScore(score,scores,1);
      // break source link to score for preservation
      *score = NULL;
    }
  } else {
    // Otherwise, output error to screen
    logger(ERR,CALLER,"unable to register new score!");
  }
}

void registerPseudo(char* pseudo, Score* score) {
  // Check if passed input is valid
  if (pseudo != NULL && strlen(pseudo) > 0) {
    // Check if passed score is valid
    if (isValidScore(score)) {
      // If yes, then pseudo can be copied into score
      strncpy(score->pseudo,pseudo,MAXPSEUDO);
    } else {
      // Otherwise output error
      logger(ERR,CALLER,"unable to register pseudo");
    }
  } else {
    // Output invalid pseudo
    logger(ERR,CALLER,"unable to register invalid pseudo");
  }
}

// TESTS

bool isValidScores(Score* scores) {
  // Allocate memory for output
  bool isValid = false;
  // Check if passed input is valid
  if (isValidScore(scores)) {
    // Make cursor to read through chain
    Score* cursor = scores;
    isValid = true;
    // Cycle through chain
    while (cursor) {
      // Check if single score is valid
      if(!isValidScore(cursor)){
        // If not change whole test value.
        isValid = false;
        // Exit test
        break;
      }
      // Otherwise switch to next
      cursor = cursor->next;
    }
  } else {
    // Otherwise, output error to screen
    logger(ERR,CALLER,"unable to validate scores!");
  }
  // output test result
  return isValid;
}

bool isValidScore(Score* score) {
  // Allocate memory space for test response
  bool isValid = false;
  // Check if passed parameter is valid
  if (score != NULL) {
    // Check if score values are correct
    if (score->pseudo != NULL && score->points >= 0 && score->kills >= 0) {
      isValid = true;
    }
  } else {
    // Output error and return failure
    logger(ERR,CALLER,"unable to validate score!");
  }
  return isValid;
}

bool isBetterScore(Score* new,Score* old) {
  // Check if input is correct
  if (isValidScore(new)) {
    if (isValidScore(old)) {
      if(old->points < new->points) {
      return true;
      } else
      if (old->points == new->points
       && old->kills  <  new->kills) {
        return true;
      } else {
        return false;
      }
    } else {
      logger(WARN,CALLER,"no old score.");
      return true;
    }
  } else {
    logger(WARN,CALLER,"no new score.");
    return false;
  }
}

// DESTROY

void destroyScores(Score* scores) {
  char buffer[MAXSTRING];
  int i = 0; // Deleted scores counter
  // Free all chained scores
  while (scores != NULL) {
    // free score
    destroyScore(scores);
    // Switch to next score
    scores = scores->next;
    // Increment counter
    i++;
  }
  // Feedback: number of scores effectively destroyed
  sprintf(buffer,"%i scores destroyed.",i);
  logger(WARN,CALLER,buffer);
}

void destroyScore(Score* score) {
  // Free score from memory
  free(score);
}

// UTILITY

void printScore(Score* score) {
  char buffer[MAXSTRING];
  // Output score value to buffer and send to buffer
  sprintf(buffer,"\n\t\t~player: %s\n\t\t~points: %i\n\t\t~kills %i"
         ,score->pseudo
         ,score->points
         ,score->kills);
  logger(INFO,CALLER,buffer);
}
