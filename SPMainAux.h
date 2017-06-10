#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include "SPFIARParser.h"
#include "SPFIARGame.h"

void getLineFromUser(const char* prompt, char* buf, int len);
int getDifficultyLevel();
void undoHandler(SPFiarGame* currentGame);
void addDiscHandler(SPFiarGame* currentGame, SPCommand command, int difficultyLevel);
void suggestMoveHandler(SPFiarGame* currentGame, int difficultyLevel);

#endif
