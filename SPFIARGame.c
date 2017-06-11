#include <stdlib.h>
#include <stdio.h>
#include "SPFIARGame.h"

SPFiarGame* spFiarGameCreate(int historySize){
    if(historySize <= 0){
        return NULL;
    }
    SPFiarGame *game = (SPFiarGame *)malloc(sizeof(SPFiarGame));
    if(!game){
        return NULL;
    }

    /* Initialize gameBoard with empty entries */
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
            game->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
        }
    }

    /* Initialize tops with 0 values */
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        game->tops[i] = 0;
    }

    /* Initialize currentPlayer to user */
    game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

    /* Initialize history ArrayList */
    game->history = spArrayListCreate(historySize);
    if(!game->history){
        free(game);
        return NULL;
    }

    return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
    if(!src){
        return NULL;
    }
    SPFiarGame *copy = (SPFiarGame *)malloc(sizeof(SPFiarGame));
    if(!copy){
        return NULL;
    }

    /* Copy gameBoard */
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
            copy->gameBoard[i][j] = src->gameBoard[i][j];
        }
    }

    /* Copy tops */
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        copy->tops[i] = src->tops[i];
    }

    /* Copy current player */
    copy->currentPlayer = src->currentPlayer;

    /* Copy history */
    copy->history = spArrayListCopy(src->history);
    if(!copy->history){
        free(copy);
        return NULL;
    }

    return copy;
}

void spFiarGameDestroy(SPFiarGame* src){
    if(!src){
        return;
    }

    spArrayListDestroy(src->history);
    free(src);
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
    if(!src || col < 0 || col >= SP_FIAR_GAME_N_COLUMNS){
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    }
    if(spFiarGameIsValidMove(src, col)){
        /* Add move to history */
        if(spArrayListIsFull(src->history)){
            SP_ARRAY_LIST_MESSAGE msg = spArrayListRemoveLast(src->history);
            if(msg == SP_ARRAY_LIST_INVALID_ARGUMENT){
                return SP_FIAR_GAME_INVALID_ARGUMENT;
            }
        }
        SP_ARRAY_LIST_MESSAGE msg = spArrayListAddFirst(src->history, col);
        if(msg == SP_ARRAY_LIST_INVALID_ARGUMENT){
            return SP_FIAR_GAME_INVALID_ARGUMENT;
        }
        /* Put disc in given column, update top index and change player */
        src->gameBoard[src->tops[col]][col] = src->currentPlayer;
        src->tops[col]++;
        changePlayer(src);
        return SP_FIAR_GAME_SUCCESS;
    }
    return SP_FIAR_GAME_INVALID_MOVE;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col){
    return src->tops[col] != SP_FIAR_GAME_N_ROWS;
}

void changePlayer(SPFiarGame *src){
    if(src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL){
        src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
    } else{
        src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
    }
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
    if(!src){
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    }
    if(spArrayListIsEmpty(src->history)){
        return SP_FIAR_GAME_NO_HISTORY;
    }

    /* Undo last move */
    int lastMoveCol = spArrayListGetFirst(src->history);
    spArrayListRemoveFirst(src->history);  /* impossible to get an error here */

    src->tops[lastMoveCol]--;
    /* Set gameBoard entry of last added disc to be empty */
    src->gameBoard[src->tops[lastMoveCol]][lastMoveCol] = SP_FIAR_GAME_EMPTY_ENTRY;
    /* Change current player */
    changePlayer(src);
    return SP_FIAR_GAME_SUCCESS;
}

int undoColNumber(SPFiarGame* src) {
    if(!src) {
        return -1;
    }
    if(spArrayListIsEmpty(src->history)) {
        return -1;
    }
    else {
        int lastMoveCol = spArrayListGetFirst(src->history);
        return lastMoveCol;
    }
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
    if(!src){
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    }

    /* Print gameBoard */
    for(int i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--){
        /* Print single row */
        printf("| ");
        for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
            printf("%c ", src->gameBoard[i][j]);
        }
        printf("|\n");
    }

    /* Print footer */
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS*2 + 3; i++){
        putchar('-');
    }
    printf("\n  ");
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        printf("%d ", i+1);
    }
    putchar('\n');

    return SP_FIAR_GAME_SUCCESS;
}

char spFiarGameGetCurrentPlayer(SPFiarGame* src){
    if(!src){
        return SP_FIAR_GAME_EMPTY_ENTRY;
    } else{
        return src->currentPlayer;
    }
}

char spFiarCheckWinner(SPFiarGame* src){
    if(!src){
        return 0;
    }

    /* Check for a tie */
    if(checkTie(src)){
        return SP_FIAR_GAME_TIE_SYMBOL;
    }

    /* Check if there's a winner */
    /* Scan rows */
    char winner = scanRows(src);
    if(winner){
        return winner;
    }

    /* Scan columns */
    winner = scanCols(src);
    if(winner){
        return winner;
    }

    /* Scan downward diagonals */
    winner = scanDownwardDiag(src);
    if(winner){
        return winner;
    }

    /* Scan upward diagonals */
    winner = scanUpwardDiag(src);
    if(winner){
        return winner;
    }

    /* No winner was found */
    return 0;
}

bool checkTie(SPFiarGame* src){
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        if(src->tops[i] < SP_FIAR_GAME_N_ROWS){
            return false;
        }
    }
    return true;
}

void updateVariables(char boardSymbol, int *seqCount, char *seqSymbol){
    if(boardSymbol == *seqSymbol){
        (*seqCount)++;
    } else{
        if(boardSymbol == SP_FIAR_GAME_EMPTY_ENTRY){
            *seqCount = 0;
        } else{
            *seqSymbol = boardSymbol;
            *seqCount = 1;
        }
    }
}

char scanRows(SPFiarGame* src){
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        /* Scan single row */
        int seqCount = 0;
        char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
        for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
            char boardSymbol = src->gameBoard[i][j];
            updateVariables(boardSymbol, &seqCount, &seqSymbol);
            if(seqCount == SP_FIAR_GAME_SPAN){
                /* Found winner */
                return seqSymbol;
            }
        }
    }

    /* No winner found in rows */
    return 0;
}

char scanCols(SPFiarGame* src){
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        /* Scan single column */
        int seqCount = 0;
        char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
        for(int j = 0; j < SP_FIAR_GAME_N_ROWS; j++){
            char boardSymbol = src->gameBoard[j][i];
            updateVariables(boardSymbol, &seqCount, &seqSymbol);
            if(seqCount == SP_FIAR_GAME_SPAN){
                /* Found winner */
                return seqSymbol;
            }
        }
    }

    /* No winner found in columns */
    return 0;
}

char scanDownwardDiag(SPFiarGame* src){
    int maxDiagLength = SP_FIAR_GAME_N_ROWS < SP_FIAR_GAME_N_COLUMNS ?
                        SP_FIAR_GAME_N_ROWS : SP_FIAR_GAME_N_COLUMNS;
    /* Scan from bottom left corner in increasing diagonal length until maxDiagLength reached */
    char winner = scanDownDiagIncreasing(src, maxDiagLength);
    if(winner){
        return winner;
    }

    /* Scan all diagonals with length maxDiagLength */
    winner = scanDownMaxLengthDiag(src, maxDiagLength);
    if(winner){
        return winner;
    }

    /* Scan rest of the diagonals in decreasing diagonal length */
    winner = scanDownDiagDecreasing(src, maxDiagLength);
    if(winner){
        return winner;
    }

    /* No winner found in downward diagonals */
    return 0;
}

char scanDownDiagIncreasing(SPFiarGame *src, int maxDiagLength){
    for(int diagLength = 1; diagLength < maxDiagLength; diagLength++){
        /* Scan single diagonal */
        int seqCount = 0;
        char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
        for(int i = diagLength-1, j = 0; (i<j?j-i:i-j) < diagLength; i--, j++){
            char boardSymbol = src->gameBoard[i][j];
            updateVariables(boardSymbol, &seqCount, &seqSymbol);
            if(seqCount == SP_FIAR_GAME_SPAN){
                /* Found winner */
                return seqSymbol;
            }
        }
    }
    
    /* No winner found */
    return 0;
}

char scanDownMaxLengthDiag(SPFiarGame *src, int maxDiagLength){
    if(SP_FIAR_GAME_N_ROWS <= SP_FIAR_GAME_N_COLUMNS){
        for(int diagStartCol = 0; diagStartCol + maxDiagLength <= SP_FIAR_GAME_N_COLUMNS; diagStartCol++){
            /* Scan single diagonal */
            int seqCount = 0;
            char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
            for(int i = maxDiagLength-1, j = diagStartCol; (i<j?j-i:i-j) < maxDiagLength; i--, j++){
                char boardSymbol = src->gameBoard[i][j];
                updateVariables(boardSymbol, &seqCount, &seqSymbol);
                if(seqCount == SP_FIAR_GAME_SPAN){
                    /* Found winner */
                    return seqSymbol;
                }
            }
        }
    } else{
        for(int diagStartRow = maxDiagLength-1; diagStartRow < SP_FIAR_GAME_N_ROWS; diagStartRow++){
            /* Scan single diagonal */
            int seqCount = 0;
            char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
            for(int i = diagStartRow, j = 0; (i<j?j-i:i-j) < maxDiagLength; i--, j++){
                char boardSymbol = src->gameBoard[i][j];
                updateVariables(boardSymbol, &seqCount, &seqSymbol);
                if(seqCount == SP_FIAR_GAME_SPAN){
                    /* Found winner */
                    return seqSymbol;
                }
            }
        }
    }

    /* No winner found */
    return 0;
}

char scanDownDiagDecreasing(SPFiarGame *src, int maxDiagLength){
    for(int diagLength = maxDiagLength-1; diagLength > 0; diagLength--){
        /* Scan single diagonal */
        int seqCount = 0;
        char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
        for(int i = SP_FIAR_GAME_N_ROWS-1, j = SP_FIAR_GAME_N_COLUMNS - diagLength; (i<j?j-i:i-j) < diagLength; i--, j++){
            char boardSymbol = src->gameBoard[i][j];
            updateVariables(boardSymbol, &seqCount, &seqSymbol);
            if(seqCount == SP_FIAR_GAME_SPAN){
                /* Found winner */
                return seqSymbol;
            }
        }
    }

    /* No winner found */
    return 0;
}

char scanUpwardDiag(SPFiarGame* src){
    int maxDiagLength = SP_FIAR_GAME_N_ROWS < SP_FIAR_GAME_N_COLUMNS ?
                        SP_FIAR_GAME_N_ROWS : SP_FIAR_GAME_N_COLUMNS;
    /* Scan from top left corner in increasing diagonal length until maxDiagLength reached */
    char winner = scanUpDiagIncreasing(src, maxDiagLength);
    if(winner){
        return winner;
    }

    /* Scan all diagonals with length maxDiagLength */
    winner = scanUpMaxLengthDiag(src, maxDiagLength);
    if(winner){
        return winner;
    }

    /* Scan rest of the diagonals in decreasing diagonal length */
    winner = scanUpDiagDecreasing(src, maxDiagLength);
    if(winner){
        return winner;
    }

    /* No winner found in upward diagonals */
    return 0;
}

char scanUpDiagIncreasing(SPFiarGame *src, int maxDiagLength){
    for(int diagLength = 1; diagLength < maxDiagLength; diagLength++){
        /* Scan single diagonal */
        int seqCount = 0;
        char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
        for(int i = SP_FIAR_GAME_N_ROWS - diagLength, j = diagLength-1; i < SP_FIAR_GAME_N_ROWS && j < SP_FIAR_GAME_N_COLUMNS; i++, j++){
            char boardSymbol = src->gameBoard[i][j];
            updateVariables(boardSymbol, &seqCount, &seqSymbol);
            if(seqCount == SP_FIAR_GAME_SPAN){
                /* Found winner */
                return seqSymbol;
            }
        }
    }
    
    /* No winner found */
    return 0;
}

char scanUpMaxLengthDiag(SPFiarGame *src, int maxDiagLength){
    if(SP_FIAR_GAME_N_ROWS <= SP_FIAR_GAME_N_COLUMNS){
        for(int diagStartCol = 0; diagStartCol + maxDiagLength <= SP_FIAR_GAME_N_COLUMNS; diagStartCol++){
            /* Scan single diagonal */
            int seqCount = 0;
            char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
            for(int i = 0, j = diagStartCol; i < SP_FIAR_GAME_N_ROWS && j < SP_FIAR_GAME_N_COLUMNS; i++, j++){
                char boardSymbol = src->gameBoard[i][j];
                updateVariables(boardSymbol, &seqCount, &seqSymbol);
                if(seqCount == SP_FIAR_GAME_SPAN){
                    /* Found winner */
                    return seqSymbol;
                }
            }
        }
    } else{
        for(int diagStartRow = SP_FIAR_GAME_N_ROWS - maxDiagLength; diagStartRow >= 0; diagStartRow--){
            /* Scan single diagonal */
            int seqCount = 0;
            char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
            for(int i = diagStartRow, j = 0; i < SP_FIAR_GAME_N_ROWS && j < SP_FIAR_GAME_N_COLUMNS; i++, j++){
                char boardSymbol = src->gameBoard[i][j];
                updateVariables(boardSymbol, &seqCount, &seqSymbol);
                if(seqCount == SP_FIAR_GAME_SPAN){
                    /* Found winner */
                    return seqSymbol;
                }
            }
        }
    }

    /* No winner found */
    return 0;
}

char scanUpDiagDecreasing(SPFiarGame *src, int maxDiagLength){
    for(int diagLength = maxDiagLength-1; diagLength > 0; diagLength--){
        /* Scan single diagonal */
        int seqCount = 0;
        char seqSymbol = SP_FIAR_GAME_PLAYER_1_SYMBOL;
        for(int i = 0, j = SP_FIAR_GAME_N_COLUMNS - diagLength; i < SP_FIAR_GAME_N_ROWS && j < SP_FIAR_GAME_N_COLUMNS; i++, j++){
            char boardSymbol = src->gameBoard[i][j];
            updateVariables(boardSymbol, &seqCount, &seqSymbol);
            if(seqCount == SP_FIAR_GAME_SPAN){
                /* Found winner */
                return seqSymbol;
            }
        }
    }

    /* No winner found */
    return 0;
}
