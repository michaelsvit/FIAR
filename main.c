#include <stdio.h>
#include <stdlib.h>
#include "SPMainAux.h"
#include "SPFIARGame.h"
#include "SPFIARParser.h"

#define USER_INPUT_LEN          1024
#define HISTORY_SIZE            20
#define NEXT_MOVE_COMMAND       "Please make the next move:\n"
#define INVALID_COMMAND_ERROR   "Error: invalid command\n"

int main()
{
    char userInput[USER_INPUT_LEN];
    bool error = false;
    int difficultyLevel;
    SPFiarGame* currentGame = NULL;

    difficultyLevel = getDifficultyLevel();
    if (difficultyLevel == 0) {
        printf("Exiting...\n");
        return 0;
    }

    currentGame = spFiarGameCreate(HISTORY_SIZE);
    /* if spFiarGameCreate did not work*/
    if (!currentGame) {
        printMallocError();
        return 0;
    }
    
    spFiarGamePrintBoard(currentGame);
    
    while (true) {
        if (!error) {
            getLineFromUser(NEXT_MOVE_COMMAND, userInput, USER_INPUT_LEN);
        } else {
            getLineFromUser("", userInput, USER_INPUT_LEN);
        }

        SPCommand command = spParserPraseLine(userInput);
        SP_COMMAND currentCommand = command.cmd;

        if (currentCommand == SP_UNDO_MOVE) {
            error = !undoHandler(currentGame);
        }
        else if (currentCommand == SP_ADD_DISC) {
            error = !addDiscHandler(currentGame, command, difficultyLevel);
        }
        else if (currentCommand == SP_SUGGEST_MOVE) {
            suggestMoveHandler(&currentGame, difficultyLevel);
            if(currentGame == NULL){
                /* Memory allocation error occurred */
                return 0;
            }
            /* force request for another input without printing prompt */
            error = true;
        }
        else if (currentCommand == SP_RESTART) {
            printf("Game restarted!\n");
            spFiarGameDestroy(currentGame);

            difficultyLevel = getDifficultyLevel();
            if (difficultyLevel == 0) {
                printf("Exiting...\n");
                break;
            }
            
            currentGame = spFiarGameCreate(HISTORY_SIZE);
            /* if spFiarGameCreate did not work*/
            if (!currentGame) {
                printMallocError();
                break;
            }
            
            else {
                spFiarGamePrintBoard(currentGame);
                error = false;
            }
        }
        else if (currentCommand == SP_QUIT) {
            printf("Exiting...\n");
            spFiarGameDestroy(currentGame);
            break;
        } else {
            printf("%s", INVALID_COMMAND_ERROR);
            error = true;
        }
    }
    return 0;
}

