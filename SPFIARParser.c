#include <stdlib.h>
#include <string.h>

#include "SPFIARParser.h"

bool spParserIsInt(const char* str){
	if(!(*str)){
		/* Empty string */
		return false;
	}
	bool isInt = true;
	for(int i = 0; str[i] != '\0'; i++){
		if(!((str[i] >= '0' && str[i] <= '9') || (i == 0 && (str[i] == '-' || str[i] == '+')))){
			isInt = false;
		}
	}
	return isInt;
}

SPCommand spParserPraseLine(const char* str){
	SPCommand cmd;
	cmd.validArg = false;
	if(!str){
		cmd.cmd = SP_INVALID_LINE;
		return cmd;
	}
	/* Create copy of given string to prevent segfault when running tests on string literal */
	char *strcopy = (char *)malloc(strlen(str)+1);
	strcpy(strcopy, str);
	
	char *cmdName, *cmdArg;
	const char *delimeter = " \t\r\n";

	/* by default - invalid */
	cmd.cmd = SP_INVALID_LINE;

	cmdName = strtok(strcopy, delimeter);
	if(cmdName){
		if(strcmp(cmdName, "undo_move")==0){
			cmd.cmd = SP_UNDO_MOVE;
		} else if(strcmp(cmdName, "add_disc")==0) {
			cmdArg = strtok(NULL, delimeter);
			if (cmdArg) {
				cmd.cmd = SP_ADD_DISC;
				if (spParserIsInt(cmdArg)) {
					cmd.arg = atoi(cmdArg);
					cmd.validArg = true;
				}
			}
		} else if(strcmp(cmdName, "suggest_move")==0){
			cmd.cmd = SP_SUGGEST_MOVE;
		} else if(strcmp(cmdName, "quit")==0){
			cmd.cmd = SP_QUIT;
		} else if(strcmp(cmdName, "restart")==0){
			cmd.cmd = SP_RESTART;
		}

		/* check no extra arguments are given */
		cmdArg = strtok(NULL, delimeter);
		if (cmdArg != NULL) {
			cmd.cmd = SP_INVALID_LINE;
		}
	}

	free(strcopy);
	return cmd;
}

