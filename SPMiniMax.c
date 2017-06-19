#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPMiniMaxNode.h"

/* the function return the best move according to the current player*/
int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
    if(currentGame == NULL || maxDepth == 0){
        return -1;
    }
    int suggestedMove = 0;
    int errorOccured = 0;
    spScoreCurrentMove(currentGame, maxDepth, &suggestedMove, &errorOccured);
    if(errorOccured){
        return -1;
    }
    return suggestedMove;
}

