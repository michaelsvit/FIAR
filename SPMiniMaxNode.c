#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#define SPAN_SIZE 4

void scoreScanRows(SPFiarGame* game, int* scoring) {
    for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - SPAN_SIZE + 1; j++) {
            int totalSum = 0;
            for (int col = 0; col < SPAN_SIZE; col++) {
                if (game->gameBoard[i][j+col] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
                    totalSum++;
                }
                else if (game->gameBoard[i][j+col] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
                    totalSum--;
                }
            }
            scoring[totalSum+4]++;
        }
    }
}

void scoreScanCols(SPFiarGame* game, int* scoring) {
    for (int i = 0; i < SP_FIAR_GAME_N_ROWS - SPAN_SIZE + 1; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
            int totalSum = 0;
            for (int row = 0; row < SPAN_SIZE; row++) {
                if (game->gameBoard[i+row][j] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
                    totalSum++;
                }
                else if (game->gameBoard[i+row][j] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
                    totalSum--;
                }
            }
            scoring[totalSum+4]++;
        }
    }
}

void scoreScanDownDiag(SPFiarGame* game, int* scoring) {
    /* from left upper corner to bottom right */
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
            scoring[totalSum+4]++;
        }
    }
}


void scoreScanUpDiag(SPFiarGame* game, int* scoring) {
    /* from bottom left corner to upper right */
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

    scoreScanRows(game, scoring);
    scoreScanCols(game, scoring);
    scoreScanDownDiag(game, scoring);
    scoreScanUpDiag(game, scoring);

    /* if [-4] > 0 or [4] > 0 */
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

