#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPMainAux.h"
#include "SPMiniMax.h"


#define USER_INPUT_LEN 1024


void getLineFromUser(const char* prompt, char* buf, int len) {
    printf("%s", prompt);
    fgets(buf, len, stdin);
    if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
    }
}

int getDifficultyLevel() {
    char userInput[USER_INPUT_LEN];
    int difficultyLevel = -1;
    while (true) {
        getLineFromUser("Please enter the difficulty level between [1-7]:\n", userInput, USER_INPUT_LEN);
        if(spParserIsInt(userInput)) {
            difficultyLevel = atoi(userInput);
            if(difficultyLevel >= 1 && difficultyLevel <= 7) {
                return difficultyLevel;
            }
            else {
                printf("Error: invalid level (should be between 1 to 7)\n");
            }
        }
        else {
            SPCommand cmd = spParserPraseLine(userInput);
            if (cmd.cmd == SP_QUIT) {
                return 0;
            }
            printf("Error: invalid level (should be between 1 to 7)\n");
        }
    }
}

bool undoHandler(SPFiarGame* currentGame) {
    int firstColNumber = undoColNumber(currentGame) + 1;
    /* check if error*/
    if (firstColNumber == 0) {
        printf("Error: cannot undo previous move!\n");
        return false;
    }
    if (spFiarCheckWinner(currentGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        spFiarGameUndoPrevMove(currentGame);
        printf("Remove disc: remove user's disc at column %d\n", firstColNumber);
        return false;
    }

    spFiarGameUndoPrevMove(currentGame);
    printf("Remove disc: remove computer's disc at column %d\n", firstColNumber);

    int secondColNumber = undoColNumber(currentGame) + 1;
    /* check if error*/
    if (secondColNumber == 0) {
        printf("Error: cannot undo previous move!\n");
        return false;
    }
    spFiarGameUndoPrevMove(currentGame);
    printf("Remove disc: remove user's disc at column %d\n", secondColNumber);

    spFiarGamePrintBoard(currentGame);
    return true;
}

bool addDiscHandler(SPFiarGame** currentGame, SPCommand command, int difficultyLevel) {
    int userCol = command.arg-1;
    if (command.validArg == false || userCol < 0 || userCol >= SP_FIAR_GAME_N_COLUMNS) {
        printf("Error: column number must be in range 1-7\n");
        return false;
    }
    if (!spFiarGameIsValidMove(*currentGame, userCol)) {
        printf("Error: column %d is full\n", command.arg);
        return false;
    }
    if (spFiarCheckWinner(*currentGame) != 0) {
        printf("Error: the game is over\n");
        return false;
    }
    /* if no problem set the move*/
    spFiarGameSetMove(*currentGame, userCol);

    /* if the user wins */
    if (spFiarCheckWinner(*currentGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        spFiarGamePrintBoard(*currentGame);
        printf("Game over: you win\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
        return false;
    }
    int computerCol = spMinimaxSuggestMove(*currentGame, difficultyLevel);
    if(computerCol == -1){
        handleMallocError(currentGame);
    }
    printf("Computer move: add disc to column %d\n", computerCol+1);
    spFiarGameSetMove(*currentGame, computerCol);

    /* if the compuer wins */
    if (spFiarCheckWinner(*currentGame) == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
        spFiarGamePrintBoard(*currentGame);
        printf("Game over: computer wins\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
        return false;
    }
    /* if no winner */
    spFiarGamePrintBoard(*currentGame);
    return true;
}

void suggestMoveHandler(SPFiarGame** currentGame, int difficultyLevel) {
    if (spFiarCheckWinner(*currentGame) != 0) {
        printf("Error: the game is over\n");
    }
    int suggestedMove = spMinimaxSuggestMove(*currentGame, difficultyLevel);
    if(suggestedMove != -1){
        printf("Suggested move: drop a disc to column %d\n", suggestedMove+1);
    } else {
        handleMallocError(currentGame);
    }
}

void handleMallocError(SPFiarGame **currentGame){
    spFiarGameDestroy(*currentGame);
    /* Set currentGame to NULL in main loop to inform about malloc error */
    *currentGame = NULL;
    printMallocError();
}

void printMallocError(){
    printf("Error: malloc has failed");
}

