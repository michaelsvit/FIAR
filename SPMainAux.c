#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPMainAux.h"
#include "SPFIARParser.h"
#include "SPFIARGame.h"
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
            if(difficultyLevel >=0 && difficultyLevel < 8) {
                return difficultyLevel;
            }
            else {
                printf("Error: invalid level (should be between 1 to 7\n");
            }
        }
        else {
            printf("Error: invalid level (should be between 1 to 7\n");
        }
    }
}

void undoHandler(SPFiarGame* currentGame) {
    int firstColNumber = undoColNumber(currentGame) + 1;
    /* check if error*/
    if (firstColNumber == -1) {
        printf("Error: cannot undo previous move!\n");
        return;
    }
    if (spFiarCheckWinner(currentGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        spFiarGameUndoPrevMove(currentGame);
        printf("Remove disc: remove user’s disc at column %d\n", firstColNumber);
        return;
    }

    spFiarGameUndoPrevMove(currentGame);
    printf("Remove disc: remove computer’s disc at column %d\n", firstColNumber);

    int secondColNumber = undoColNumber(currentGame) + 1;
    /* check if error*/
    if (secondColNumber == -1) {
        printf("Error: cannot undo previous move!\n");
        return;
    }
    spFiarGameUndoPrevMove(currentGame);
    printf("Remove disc: remove user’s disc at column %d\n", secondColNumber);

    spFiarGamePrintBoard(currentGame);
}

void addDiscHandler(SPFiarGame* currentGame, SPCommand command, int difficultyLevel) {
    if (command.validArg == false || command.arg-1 < 0 || command.arg-1 > 7) {
        printf("Error: column number must be in range 1-7\n");
        return;
    }
    if (!spFiarGameIsValidMove(currentGame, command.arg - 1)) {
        printf("Error: column %d is full\n", command.arg - 1);
        return;
    }
    if (spFiarCheckWinner(currentGame) != 0) {
        printf("Error: the game is over\n");
        return;
    }
    /* if no problem set the move*/
    spFiarGameSetMove(currentGame, command.arg-1);

    /* if the user wins */
    if (spFiarCheckWinner(currentGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        spFiarGamePrintBoard(currentGame);
        printf("Game over: you win\nPlease enter ‘quit’ to exit or ‘restart’ to start a new game!\n");
        return;
    }
    int computerCol = spMinimaxSuggestMove(currentGame, difficultyLevel);
    spFiarGameSetMove(currentGame, computerCol);

    /* if the compuer wins */
    if (spFiarCheckWinner(currentGame) == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
        spFiarGamePrintBoard(currentGame);
        printf("Game over: computer wins\nPlease enter ‘quit’ to exit or ‘restart’ to start a new game!\n");
        return;
    }
    /* if no winner */
    spFiarGamePrintBoard(currentGame);
}

void suggestMoveHandler(SPFiarGame* currentGame, int difficultyLevel) {
    if (spFiarCheckWinner(currentGame) != 0) {
        printf("Error: the game is over\n");
        return;
    }
    int suggestedMove = spMinimaxSuggestMove(currentGame, difficultyLevel);
    printf("Suggested move: drop a disc to column %d\n", suggestedMove);
}

