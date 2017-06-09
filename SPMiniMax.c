#define SPAN_SIZE 4

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
    int suggestedMove;
    spScoreCurrentMove(currentGame, maxDepth, &suggestedMove);
    return suggestedMove;
}

int spScoreCurrentMove(SPFiarGame* currentGame, unsigned int maxDepth, int* suggestedMove) {
    char currentPlayer = spFiarGameGetCurrentPlayer(currentGame);
    int currentScore = (current_player == SP_FIAR_GAME_PLAYER_1_SYMBOL) ? MIN_INT : MAX_INT;
    int currentSuggestedMove = -1;
    int sonScore;
    
    if (maxDepth == 0) {
        return score(currentGame);
    }

    char sy = spFiarCheckWinner(currentGame);
    if (sy == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        return MAX_INT;
    } 
    if (sy == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
        return MIN_INT;
    } 
    if (sy == SP_FIAR_GAME_TIE_SYMBOL) {
        return 0;
    } 

    for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        if (spFiarGameIsValidMove(currentGame, i)) {
            SPFiarGame* cpyGame = spFiarGameCopy(currentGame);
            spFiarGameSetMove(cpyGame, i);
            sonScore = spScoreCurrentMove(currentGame, maxDepth-1, NULL);
            if ((current_player == SP_FIAR_GAME_PLAYER_1_SYMBOL && sonScore > currentScore) ||
                    (current_player == SP_FIAR_GAME_PLAYER_2_SYMBOL && sonScore < currentScore)) {
                currentScore = sonScore;
                currentSuggestedMove = i;
            }
            spFiarGameDestroy(cpyGame);
    }

    if (suggestedMove != NULL) {
        *suggestedMove = currentSuggestedMove;
    }

    return currentScore;
}

int score(SPFiarGame* game) {
    if (game == NULL) {
        return 0;
    }
    int weightVector[] = {0, -5, -2, -1, 0, 1, 2, 5, 0};
    int totalScore = 0;

    /* *scoring = [-4, -3, -2, -1, 0, 1, 2, 3, 4] */
    int scoring[9] = {0, };

    scanRows(game, scoring);
    scanCols(game, scoring);
    scanDownDiag(game, scoring);
    scanUpDiag(game, scoring);

    /* if [-4] > 0 or [4] > 0 */
    if (scoring[0] > 0) {
        return MIN_INT;
    }
    if (scoring[8] > 0) {
        return MAX_INT;
    }

    for (int i = 0; i < 9; i++) {
        totalScore += weightVector[i] * scoring[i];
    }
    return totalScore;
}

void scanRows(SPFiarGame* game, int* scoring) {
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

void scanCols(SPFiarGame* game, int* scoring) {
    for (int i = 0; i < SP_FIAR_GAME_N_ROWS - SPAN_SIZE + 1; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
            int totalSum = 0;
            for (int row = 0; row < 4; row++) {
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

void scanDownDiag(SPFiarGame* game, int* scoring) {
    /* from left upper corner to bottom right */
    for (int i = 0; i < SP_FIAR_GAME_N_ROWS - SPAN_SIZE + 1; i++) {
        for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - SPAN_SIZE + 1; j++) {
            int totalSum = 0;
            for (int diag = 0; diag < spanSize; diag++) {
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


void scanUpDiag(SPFiarGame* game, int* scoring) {
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

