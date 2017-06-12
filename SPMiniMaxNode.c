#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
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

/* the function gets a game and returns the score for the spesific board */
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

/**
 * A recursive function that calculates and return the best column to put a disc on by calculating the
 * score for the current user and the curretn suggested move. use minimax algorithm.
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the miniMax algorithm
 * @param suggestedMove - pointer to the column number that will give the best score for the current player
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 wins
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 wins
 * SP_FIAR_GAME_TIE_SYMBOL - if there is a tie
 * if maxDapth is 0 - return the score of the current move
 */
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

