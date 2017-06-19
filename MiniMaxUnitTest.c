#include <stdbool.h>
#include <stdlib.h>
#include "unit_test_util.h"
#include "SPMiniMax.h"

#define HISTORY_SIZE 20
#define MAX_DEPTH 1

static bool spMiniMaxSuggestMoveTest1(){
    ASSERT_TRUE(spMinimaxSuggestMove(NULL, MAX_DEPTH) == -1);
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(spMinimaxSuggestMove(game, 0) == -1);

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
    ASSERT_TRUE(spMinimaxSuggestMove(game, MAX_DEPTH) == 3);
    
    spFiarGameDestroy(game);
    return true;
}

static bool spMiniMaxSuggestMoveTest2(){
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
    ASSERT_TRUE(spMinimaxSuggestMove(game, MAX_DEPTH) == 0);
    
    spFiarGameDestroy(game);
    return true;
}

static bool spMiniMaxSuggestMoveTest3(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);

    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 0);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 1);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 2);
    ASSERT_TRUE(spMinimaxSuggestMove(game, MAX_DEPTH) == 6);
    
    spFiarGameDestroy(game);
    return true;
}

static bool spMiniMaxSuggestMoveTest4(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);

    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 0);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 1);
    spFiarGameSetMove(game, 6);
    spFiarGameSetMove(game, 2);
    spFiarGameSetMove(game, 5);
    ASSERT_TRUE(spMinimaxSuggestMove(game, MAX_DEPTH) == 3);
    
    spFiarGameDestroy(game);
    return true;
}

int main(){
    RUN_TEST(spMiniMaxSuggestMoveTest1);
    RUN_TEST(spMiniMaxSuggestMoveTest2);
    RUN_TEST(spMiniMaxSuggestMoveTest3);
    RUN_TEST(spMiniMaxSuggestMoveTest4);

    return 0;
}