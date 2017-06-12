#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include "SPFIARParser.h"
#include "SPFIARGame.h"

void getLineFromUser(const char* prompt, char* buf, int len);
int getDifficultyLevel();
bool undoHandler(SPFiarGame* currentGame);
bool addDiscHandler(SPFiarGame* currentGame, SPCommand command, int difficultyLevel);
bool suggestMoveHandler(SPFiarGame* currentGame, int difficultyLevel);

#endif
