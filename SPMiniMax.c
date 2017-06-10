#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPMiniMaxNode.h"
#include "SPFIARGame.h"


int spScoreCurrentMove(SPFiarGame* currentGame, unsigned int maxDepth, int* suggestedMove) {
    char currentPlayer = spFiarGameGetCurrentPlayer(currentGame);
    int currentScore = (currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) ? INT_MIN : INT_MAX;
    int currentSuggestedMove = 0;
    int sonScore;
    
    if (maxDepth == 0) {
        return score(currentGame);
    }

    char sy = spFiarCheckWinner(currentGame);
    if (sy == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        return INT_MAX;
    } 
    if (sy == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
        return INT_MIN;
    } 
    if (sy == SP_FIAR_GAME_TIE_SYMBOL) {
        return 0;
    }

    for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
        if (spFiarGameIsValidMove(currentGame, i)) {
            SPFiarGame* cpyGame = spFiarGameCopy(currentGame);
            spFiarGameSetMove(cpyGame, i);
            sonScore = spScoreCurrentMove(cpyGame, maxDepth-1, NULL);
            if ((currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL && sonScore > currentScore) ||
                    (currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL && sonScore < currentScore)) {
                currentScore = sonScore;
                currentSuggestedMove = i;
            }
            spFiarGameDestroy(cpyGame);
        }
    }

    if (suggestedMove != NULL) {
        *suggestedMove = currentSuggestedMove;
    }

    return currentScore;
}

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
    int suggestedMove = -1;
    spScoreCurrentMove(currentGame, maxDepth, &suggestedMove);
    return suggestedMove;
}

