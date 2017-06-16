#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include "SPFIARGame.h"

/**
* Scan game board rows and count number of spans of size 4
* of each value according to the MiniMax algorithm.
* @param game          game instance to be scanned
* @param scoring  array of counters for each span value
*/
void scoreScanRows(SPFiarGame* game, int* scoring);

/**
* Scan game board columns and count number spans of size 4
* of each value according to the MiniMax algorithm.
* @param game          game instance to be scanned
* @param scoring  array of counters for each span value
*/

void scoreScanCols(SPFiarGame* game, int* scoring);

/**
* Scan game board downward diagonals and count number of spans of size 4
* of each value according to the MiniMax algorithm.
* @param game           game instance to be scanned
* @param scoring  array of counters for each span value
*/
void scoreScanDownDiag(SPFiarGame* game, int* scoring);

/**
* Scan game board upward diagonals and count number of spans with size 4
* of each value according to the MiniMax algorithm.
* @param game           game instance to be scanned
* @param scoring  array of counters for each span value
*/
void scoreScanUpDiag(SPFiarGame* game, int* scoring);

/**
 * calculates and return the score of a spesific gameboard
 * by scaning all spans of 4, and with a specific weight vector
 * @param game           game instance to be scanned
 * @return - the score for specific board
 */
int score(SPFiarGame* game);

/**
 * A recursive function that calculates a MiniMax tree and returns the score
 * at it's root node, and the first move through which this score was calculated.
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the miniMax algorithm
 * @param suggestedMove - wil contain column number that will give the best score for the current player
 * @return - the score for the suggested move, found by minimax algorithm
 */
int spScoreCurrentMove(SPFiarGame* currentGame, unsigned int maxDepth, int* suggestedMove);

#endif
