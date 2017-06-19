#include <stdbool.h>
#include "unit_test_util.h"
#include "SPFIARGame.h"

#define HISTORY_SIZE 20

static bool spFiarGameCreateTest(){
    ASSERT_FALSE(spFiarGameCreate(0));
    ASSERT_FALSE(spFiarGameCreate(-1));

    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game);
    ASSERT_TRUE(spArrayListMaxCapacity(game->history) == HISTORY_SIZE);
    spFiarGameDestroy(game);
    return true;
}

static bool spFiarGameCopyTest(){
    ASSERT_FALSE(spFiarGameCopy(NULL));

    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    SPFiarGame *cp = spFiarGameCopy(game);
    ASSERT_TRUE(cp);
    ASSERT_TRUE(cp->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL);
    for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
        ASSERT_TRUE(cp->tops[i] == 0);
    }
    ASSERT_TRUE(spArrayListSize(cp->history) == 0);
    spFiarGameDestroy(cp);

    spFiarGameSetMove(game, 0);
    spFiarGameSetMove(game, 3);
    spFiarGameSetMove(game, SP_FIAR_GAME_N_COLUMNS-1);

    cp = spFiarGameCopy(game);
    ASSERT_TRUE(cp);
    ASSERT_TRUE(cp->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL);
    ASSERT_TRUE(cp->tops[0] == 1);
    ASSERT_TRUE(cp->tops[3] == 1);
    ASSERT_TRUE(cp->tops[SP_FIAR_GAME_N_COLUMNS-1] == 1);
    ASSERT_TRUE(spArrayListSize(cp->history) == 3);
    ASSERT_TRUE(spArrayListGetAt(cp->history, 1) == 3);

    spFiarGameDestroy(cp);
    spFiarGameDestroy(game);
    return true;
}

static bool spFiarGameSetMoveTest(){
    ASSERT_TRUE(spFiarGameSetMove(NULL, 3) == SP_FIAR_GAME_INVALID_ARGUMENT);
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(spFiarGameSetMove(game, 7) == SP_FIAR_GAME_INVALID_ARGUMENT);
    ASSERT_TRUE(spFiarGameSetMove(game, -1) == SP_FIAR_GAME_INVALID_ARGUMENT);
    
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        ASSERT_TRUE(spFiarGameSetMove(game, 0) == SP_FIAR_GAME_SUCCESS);
    }
    ASSERT_TRUE(spFiarGameSetMove(game, 0) == SP_FIAR_GAME_INVALID_MOVE);
    spFiarGameDestroy(game);
    return true;
}

static bool spFiarGameUndoPrevMoveTest(){
    ASSERT_TRUE(spFiarGameUndoPrevMove(NULL) == SP_FIAR_GAME_INVALID_ARGUMENT);

    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(spFiarGameUndoPrevMove(game) == SP_FIAR_GAME_NO_HISTORY);
    spFiarGameSetMove(game, 6);
    ASSERT_TRUE(spFiarGameUndoPrevMove(game) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(game->tops[6] == 0);
    ASSERT_TRUE(spArrayListIsEmpty(game->history));
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        spFiarGameSetMove(game, 3);
    }
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        ASSERT_TRUE(spFiarGameUndoPrevMove(game) == SP_FIAR_GAME_SUCCESS);
    }
    ASSERT_TRUE(game->tops[3] == 0);
    ASSERT_TRUE(spArrayListIsEmpty(game->history));
    spFiarGameDestroy(game);
    return true;
}

/**
* Create the following game board:
* |       |
* |       |
* |o      |
* |xo     |
* |oxo   x|
* |xoxo  x|
* ---------
*  1234567
*/
static bool spFiarCheckWinnerTest1(){
    ASSERT_FALSE(spFiarCheckWinner(NULL));

    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    for(int i = 0; i < 4; i++){
        spFiarGameSetMove(game, 0);
    }
    spFiarGameSetMove(game, SP_FIAR_GAME_N_COLUMNS-1);
    for(int j = 1; j <= 2; j++){
        for(int i = 0; i < 4-j; i++){
            spFiarGameSetMove(game, j);
        }
    }
    spFiarGameSetMove(game, SP_FIAR_GAME_N_COLUMNS-1);
    spFiarGameSetMove(game, 3);
    spFiarGamePrintBoard(game);
    ASSERT_TRUE(spFiarCheckWinner(game) == SP_FIAR_GAME_PLAYER_2_SYMBOL);

    spFiarGameDestroy(game);    
    return true;
}

/**
* Create the following game board:
* |       |
* |       |
* |   o   |
* |  ox   |
* | oxo  x|
* |oxox  x|
* ---------
*  1234567
*/
static bool spFiarCheckWinnerTest2(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    for(int i = 0; i < 4; i++){
        spFiarGameSetMove(game, 3);
    }
    spFiarGameSetMove(game, SP_FIAR_GAME_N_COLUMNS-1);
    for(int j = 1; j <= 2; j++){
        for(int i = 0; i < 4-j; i++){
            spFiarGameSetMove(game, 3-j);
        }
    }
    spFiarGameSetMove(game, SP_FIAR_GAME_N_COLUMNS-1);
    spFiarGameSetMove(game, 0);
    spFiarGamePrintBoard(game); 
    ASSERT_TRUE(spFiarCheckWinner(game) == SP_FIAR_GAME_PLAYER_2_SYMBOL);
    
    spFiarGameDestroy(game);    
    return true;
}

/**
* Create the following game board:
* |       |
* |       |
* |      x|
* |      x|
* |      x|
* |ooo   x|
* ---------
*  1234567
*/
static bool spFiarCheckWinnerTest3(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 0);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 1);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 2);
    spFiarGameSetMove(game, 6);
    
    spFiarGamePrintBoard(game);    
    ASSERT_TRUE(spFiarCheckWinner(game) == SP_FIAR_GAME_PLAYER_1_SYMBOL);
    
    spFiarGameDestroy(game);    
    return true;
}

/**
* Create the following game board:
* |       |
* |       |
* |       |
* |      x|
* |      x|
* |oooo xx|
* ---------
*  1234567
*/
static bool spFiarCheckWinnerTest4(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 0);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 1);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 2);
    spFiarGameSetMove(game, 5);
    spFiarGameSetMove(game, 3);    
    
    spFiarGamePrintBoard(game);    
    ASSERT_TRUE(spFiarCheckWinner(game) == SP_FIAR_GAME_PLAYER_2_SYMBOL);
    
    spFiarGameDestroy(game);    
    return true;
}

int main(){
    RUN_TEST(spFiarGameCreateTest);
    RUN_TEST(spFiarGameCopyTest);
    RUN_TEST(spFiarGameSetMoveTest);
    RUN_TEST(spFiarGameUndoPrevMoveTest);
    RUN_TEST(spFiarCheckWinnerTest1);
    RUN_TEST(spFiarCheckWinnerTest2);
    RUN_TEST(spFiarCheckWinnerTest3);
    RUN_TEST(spFiarCheckWinnerTest4);

    return 0;
}