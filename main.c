#include <stdio.h>
#include <stdlib.h>
#include "SPMainAux.h"
#include "SPFIARGame.h"
#include "SPFIARParser.h"

#define USER_INPUT_LEN 1024


int main()
{
    char userInput[USER_INPUT_LEN];
    int difficultyLevel = getDifficultyLevel();
    SPFiarGame* currentGame = spFiarGameCreate(20);

    spFiarGamePrintBoard(currentGame);

    while(true) {
        getLineFromUser("Please enter a command:\n", userInput, USER_INPUT_LEN);
        SPCommand command = spParserPraseLine(userInput);
        SP_COMMAND currentCommand = command.cmd;

        if (currentCommand == SP_UNDO_MOVE) {
            undoHandler(currentGame);
        }

        if (currentCommand == SP_ADD_DISC) {
            addDiscHandler(currentGame, command, difficultyLevel);
        }

        if (currentCommand == SP_SUGGEST_MOVE) {
           suggestMoveHandler(currentGame, difficultyLevel);
        }

        if (currentCommand == SP_RESTART) {
            spFiarGameDestroy(currentGame);
            difficultyLevel = getDifficultyLevel();
            SPFiarGame* currentGame = spFiarGameCreate(20);
            spFiarGamePrintBoard(currentGame);
        }

        if (currentCommand == SP_QUIT) {
            spFiarGameDestroy(currentGame);
            break;
        }
    }
    return 0;
}

