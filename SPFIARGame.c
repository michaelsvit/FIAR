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
        /* Put disc in given column and update top index */
        src->gameBoard[game->tops[col]][col] = src->currentPlayer;
        src->tops[col]++;
        return SP_FIAR_GAME_SUCCESS;
    }
    return SP_FIAR_GAME_INVALID_MOVE;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col){
    return src->tops[col] != SP_FIAR_GAME_N_COLUMNS;
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
    if(src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL){
        src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
    } else{
        src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
    }
    return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
    if(!src){
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    }

    /* Print gameBoard */
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
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