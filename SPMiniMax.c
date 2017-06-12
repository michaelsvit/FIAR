#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPMiniMaxNode.h"
#include "SPFIARGame.h"

/* the function return the best move according to the current player*/
int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
    int suggestedMove = -1;
    spScoreCurrentMove(currentGame, maxDepth, &suggestedMove);
    return suggestedMove;
}

