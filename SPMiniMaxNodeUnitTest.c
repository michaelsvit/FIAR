/**
* SPMiniMaxNode unit tests module.
* Computer symbols are counted as negative, player symbols are counted as positive.
* spanCounters = {-4, -3, -2, -1, 0, 1, 2, 3, 4}
*
* Test 1 creates board 1 and then checks individual scanning functions against
* expected results.
* Test 2 creates board 2 and then checks the value return by the utility function
* against the value provided in the exercise instructions.
*/

#include "unit_test_util.h"
#include "SPMiniMaxNode.h"
#include <stdbool.h>
#include <stdlib.h>

#define HISTORY_SIZE 20
#define SPAN_SIZE 4

/**
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
static bool createBoard1(SPFiarGame *game){
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
    return true;
}

/**
* Create the following game board (same as example in instructions):
* |       |
* | x     |
* | x     |
* | oox   |
* | xoo   |
* | xxoxoo|
* ---------
*  1234567
*/
static bool createBoard2(SPFiarGame *game){
    ASSERT_TRUE(spFiarGameSetMove(game, 1) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 3) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 2) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 5) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 1) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 6) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 4) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 1) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 1) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 3) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 1) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 2) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 3) == SP_FIAR_GAME_SUCCESS);
    ASSERT_TRUE(spFiarGameSetMove(game, 2) == SP_FIAR_GAME_SUCCESS);
    return true;
}

static bool checkResults(int *spanCounters, int *expectedResults){
    for(int i = 0; i < SPAN_SIZE*2 + 1; i++){
        if(i == SPAN_SIZE){
            continue;
        }
        ASSERT_TRUE(spanCounters[i] == expectedResults[i]);
    }
    return true;
}

static bool scoreScanUpwardDiagTest(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);
    ASSERT_TRUE(createBoard1(game));

    int *spanCounters = calloc(9, sizeof(int));
    scoreScanDownDiag(game, spanCounters);

    /* Check correctness of results */
    ASSERT_TRUE(checkResults(spanCounters, (int[]){0,2,2,2,0,2,3,1,0}));

    free(spanCounters);
    spFiarGameDestroy(game);
    return true;
}

static bool scoreScanDownwardDiagTest(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);
    ASSERT_TRUE(createBoard1(game));

    int *spanCounters = calloc(9, sizeof(int));
    scoreScanUpDiag(game, spanCounters);

    /* Check correctness of results */
    ASSERT_TRUE(checkResults(spanCounters, (int[]){2,1,1,0,0,2,0,1,1}));

    free(spanCounters);
    spFiarGameDestroy(game);
    return true;
}

static bool scoreScanRowsTest(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);
    ASSERT_TRUE(createBoard1(game));

    int *spanCounters = calloc(9, sizeof(int));
    scoreScanRows(game, spanCounters);

    /* Check correctness of results */
    ASSERT_TRUE(checkResults(spanCounters, (int[]){0,0,0,7,0,1,1,0,0}));

    free(spanCounters);
    spFiarGameDestroy(game);
    return true;
}

static bool scoreScanColsTest(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);
    ASSERT_TRUE(createBoard1(game));

    int *spanCounters = calloc(9, sizeof(int));
    scoreScanCols(game, spanCounters);

    /* Check correctness of results */
    ASSERT_TRUE(checkResults(spanCounters, (int[]){0,0,0,4,0,1,1,0,0}));

    free(spanCounters);
    spFiarGameDestroy(game);
    return true;
}

static bool scoreTest(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);
    ASSERT_TRUE(createBoard2(game));

    int utilityValue = score(game);

    /* Check correctness of results */
    ASSERT_TRUE(utilityValue == -6);

    spFiarGameDestroy(game);
    return true;
}

static bool printTest1(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);
    ASSERT_TRUE(createBoard1(game));
    printf("********************TEST 1********************\n");
    spFiarGamePrintBoard(game);
    return true;
}

static bool printTest2(){
    SPFiarGame *game = spFiarGameCreate(HISTORY_SIZE);
    ASSERT_TRUE(game != NULL);
    ASSERT_TRUE(createBoard2(game));
    printf("********************TEST 2********************\n");
    spFiarGamePrintBoard(game);
    return true;
}

int main() {
    RUN_TEST(printTest1);
    RUN_TEST(scoreScanUpwardDiagTest);
    RUN_TEST(scoreScanDownwardDiagTest);
    RUN_TEST(scoreScanRowsTest);
    RUN_TEST(scoreScanColsTest);

    putchar('\n');

    RUN_TEST(printTest2);
    RUN_TEST(scoreTest);
    return 0;
}