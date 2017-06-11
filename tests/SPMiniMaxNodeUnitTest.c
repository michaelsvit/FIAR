#include "unit_test_util.h"
#include "SPMiniMaxNode.h"
#include <stdbool.h>
#include <stdlib.h>

#define HISTORY_SIZE 20

static bool scanUpwardDiagTest(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);

    /*
    * Create the following game board:
    * |o      |
    * |xo     |
    * |oxo    |
    * |xoxo   |
    * |oxox  x|
    * |xoxo  x|
    * ---------
    *  1234567
    */
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        ASSERT_TRUE(spFiarGameSetMove(game, 0) == SP_FIAR_GAME_SUCCESS);
    }
    ASSERT_TRUE(spFiarGameSetMove(game, SP_FIAR_GAME_N_COLUMNS-1) == SP_FIAR_GAME_SUCCESS);
    for(int j = 1; j <= 2; j++){
        for(int i = 0; i < SP_FIAR_GAME_N_ROWS-j; i++){
            ASSERT_TRUE(spFiarGameSetMove(game, j) == SP_FIAR_GAME_SUCCESS);
        }
    }
    ASSERT_TRUE(spFiarGameSetMove(game, SP_FIAR_GAME_N_COLUMNS-1) == SP_FIAR_GAME_SUCCESS);
    for(int i = 0; i < SP_FIAR_GAME_N_ROWS-3; i++){
        ASSERT_TRUE(spFiarGameSetMove(game, 3) == SP_FIAR_GAME_SUCCESS);
    }
    spFiarGamePrintBoard(game);

    int *spanCounters = calloc(9, sizeof(int));
    scoreScanUpDiag(game, spanCounters);

    /* Check correctness of results */
    ASSERT_TRUE(spanCounters[0] == 0);
    ASSERT_TRUE(spanCounters[1] == 1);
    ASSERT_TRUE(spanCounters[2] == 3);
    ASSERT_TRUE(spanCounters[3] == 2);
    ASSERT_TRUE(spanCounters[5] == 2);
    ASSERT_TRUE(spanCounters[6] == 2);
    ASSERT_TRUE(spanCounters[7] == 2);
    ASSERT_TRUE(spanCounters[8] == 0);

    free(spanCounters);
    spFiarGameDestroy(game);
    return true;
}

int main() {
    RUN_TEST(scanUpwardDiagTest);
    return 0;
}