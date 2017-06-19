#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include "SPFIARParser.h"
#include "SPFIARGame.h"

/**
* Get input from user through stdin.
* @param prompt   prompt to be printed to user
* @param buf      buffer for user input
* @param len      size of buffer
*/
void getLineFromUser(const char* prompt, char* buf, int len);

/**
* Get game difficulty level from user through stdin.
* @return     difficulty level
*/
int getDifficultyLevel();

/**
* Handle undo command from user.
* @param currentGame    currently running game instance
* @return               true iff undo was successful
*/
bool undoHandler(SPFiarGame* currentGame);

/**
* Handle add_disc command from user.
* @param currentGame        currently running game instance
* @param command            command input from user
* @param difficultyLevel    difficulty level of given game instance
* @return                   true iff add_disc was successful
*/
bool addDiscHandler(SPFiarGame* currentGame, SPCommand command, int difficultyLevel);

/**
* Handle suggest_move command from user.
* @param currentGame        currently running game instance
* @param difficultyLevel    difficulty level of given game instance
* @return                   true iff suggest_move was successful
*/
bool suggestMoveHandler(SPFiarGame* currentGame, int difficultyLevel);

/**
* Print error message about malloc failure.
*/
void printMallocError();

#endif
