#include "SPFIARGame.h"

#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

void scoreScanRows(SPFiarGame* game, int* scoring);
void scoreScanCols(SPFiarGame* game, int* scoring);
void scoreScanDownDiag(SPFiarGame* game, int* scoring);
void scoreScanUpDiag(SPFiarGame* game, int* scoring);
int score(SPFiarGame* game);

#endif
