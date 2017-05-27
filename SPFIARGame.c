#include "SPFIARGame.h"

SPFiarGame* spFiarGameCreate(int historySize){
    if(historySize <= 0){
        return NULL;
    }
    SPFiarGame *game = malloc(sizeof(SPFiarGame));
    if(!game){
        return NULL;
    }

    // Initialize gameBoard with empty entries
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
            game->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
        }
    }

    // Initialize tops with 0 values
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        game->tops[i] = 0;
    }

    // Initialize currentPlayer to user
    game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

    // Initialize history ArrayList
    game->history = spArrayListCreate(historySize);
    if(!game->history){
        return NULL;
    }

    return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
    if(!src){
        return NULL;
    }
    SPFiarGame *copy = malloc(sizeof(SPFiarGame));
    if(!copy){
        return NULL;
    }

    // Copy gameBoard
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
            copy->gameBoard[i][j] = src->gameBoard[i][j];
        }
    }

    // Copy tops
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        copy->tops[i] = src->tops[i];
    }

    // Copy current player
    copy->currentPlayer = src->currentPlayer;

    // Copy history
    copy->history = spArrayListCopy(src->history);
    if(!copy->history){
        return NULL;
    }

    return copy;
}