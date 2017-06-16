#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include "SPFIARParser.h"
#include "SPFIARGame.h"

/**
Get input from user through stdin.

*/
void getLineFromUser(const char* prompt, char* buf, int len);
int getDifficultyLevel();
bool undoHandler(SPFiarGame* currentGame);
bool addDiscHandler(SPFiarGame* currentGame, SPCommand command, int difficultyLevel);
bool suggestMoveHandler(SPFiarGame* currentGame, int difficultyLevel);

#endif
