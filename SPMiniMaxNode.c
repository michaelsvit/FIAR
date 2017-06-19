#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPMiniMaxNode.h"

#define SPAN_SIZE 4

void scoreScanRows(SPFiarGame* game, int* scoring) {
    /* calculates the total sum of spans of four in rows, where player 1 symbol gets +1 and player 2 symbol gets -1 */
    for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - SPAN_SIZE + 1; j++) {
            int totalSum = 0;
            for (int col = 0; col < SPAN_SIZE; col++) {
            /* for every span of four in rows*/
                if (game->gameBoard[i][j+col] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
                    totalSum++;
                }
                else if (game->gameBoard[i][j+col] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
                    totalSum--;
                }
            }
            /* totalSum+4 because the way scoring[] is build*/
            scoring[totalSum+4]++;
        }
    }
}

void scoreScanCols(SPFiarGame* game, int* scoring) {
    /* calculates the total sum of spans of four in columns, where player 1 symbol gets +1 and player 2 symbol gets -1 */
    for (int i = 0; i < SP_FIAR_GAME_N_ROWS - SPAN_SIZE + 1; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
            int totalSum = 0;
            for (int row = 0; row < SPAN_SIZE; row++) {
                /* for every span of four in columns*/
                if (game->gameBoard[i+row][j] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
                    totalSum++;
                }
                else if (game->gameBoard[i+row][j] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
                    totalSum--;
                }
            }
            /* totalSum+4 because the way scoring[] is build*/
            scoring[totalSum+4]++;
        }
    }
}

void scoreScanUpDiag(SPFiarGame* game, int* scoring) {
    /* from bottom left corner to upper right */
    for (int i = 0; i < SP_FIAR_GAME_N_ROWS - SPAN_SIZE + 1; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - SPAN_SIZE + 1; j++) {
            int totalSum = 0;
            for (int diag = 0; diag < SPAN_SIZE; diag++) {
                if (game->gameBoard[i+diag][j+diag] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
                    totalSum++;
                }
                else if (game->gameBoard[i+diag][j+diag] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
                    totalSum--;
                }
            }
            /* totalSum+4 because the way scoring[] is build*/
            scoring[totalSum+4]++;
        }
    }
}


void scoreScanDownDiag(SPFiarGame* game, int* scoring) {
    /* from left upper corner to bottom right */
    for (int i = SPAN_SIZE-1; i < SP_FIAR_GAME_N_ROWS; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - SPAN_SIZE + 1; j++) {
            int totalSum = 0;
            for (int diag = 0; diag < SPAN_SIZE; diag++) {
                if (game->gameBoard[i-diag][j+diag] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
                    totalSum++;
                } else if (game->gameBoard[i-diag][j+diag] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
                    totalSum--;
                }
            }
            /* totalSum+4 because the way scoring[] is build*/
            scoring[totalSum+4]++;
        }
    }
}

int score(SPFiarGame* game) {
    if (game == NULL) {
        return 0;
    }
    int weightVector[] = {0, -5, -2, -1, 0, 1, 2, 5, 0};
    int totalScore = 0;

    /* *scoring = [-4, -3, -2, -1, 0, 1, 2, 3, 4] */
    int scoring[9] = {0, };

    /* update *scoring due to all spans of four*/
    scoreScanRows(game, scoring);
    scoreScanCols(game, scoring);
    scoreScanDownDiag(game, scoring);
    scoreScanUpDiag(game, scoring);

    /* if [-4] > 0 or [4] > 0 it means we found a winner*/
    if (scoring[0] > 0) {
        return INT_MIN;
    }
    if (scoring[8] > 0) {
        return INT_MAX;
    }

    for (int i = 0; i < 9; i++) {
        totalScore += weightVector[i] * scoring[i];
    }

    return totalScore;
}


int spScoreCurrentMove(SPFiarGame* currentGame, unsigned int maxDepth, int* suggestedMove) {
    char currentPlayer = spFiarGameGetCurrentPlayer(currentGame);
    int currentScore = (currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) ? INT_MIN : INT_MAX;
    int currentSuggestedMove = 0;
    int sonScore;

    /* We reached a leaf node */
    if (maxDepth == 0) {
        return score(currentGame);
    }

    /* If current board configuration has a winner then stop recursion */
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

    /* Make a recursive call for each possible move from current game state */
    for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
        if (spFiarGameIsValidMove(currentGame, i)) {
            SPFiarGame* cpyGame = spFiarGameCopy(currentGame);
            if(!cpyGame){
                *suggestedMove = -1;
                return 0;
            }
            spFiarGameSetMove(cpyGame, i);
            sonScore = spScoreCurrentMove(cpyGame, maxDepth-1, NULL);
            /* If a memory allocation error occured anywhere along the recursion
             * then free memory and return immediately
             */
            if(*suggestedMove == -1){
                spFiarGameDestroy(cpyGame);
                return 0;
            }
            /* If this is a min-node and child score is lower than current score
             *  or this is a max-node and child score is higher than current score
             *  then update corresponding variables
             */
            if ((currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL && sonScore > currentScore) ||
                    (currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL && sonScore < currentScore)) {
                currentScore = sonScore;
                currentSuggestedMove = i;
            }
            spFiarGameDestroy(cpyGame);
        }
    }

    /* If this is the root node then update suggested move */
    if (suggestedMove != NULL && *suggestedMove != -1) {
        *suggestedMove = currentSuggestedMove;
    }

    return currentScore;
}

