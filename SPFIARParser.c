#include <stdlib.h>
#include <string.h>

#include "SPFIARParser.h"

bool spParserIsInt(const char* str){
	bool isInt = true;
	for(int i = 0; str[i] != '\0'; i++){
		if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '-')){
			isInt = false;
		}
	}
	return isInt;
}

SPCommand spParserPraseLine(char* str){
	SPCommand cmd;
	cmd.validArg = false;
	if(!str){
		cmd.cmd = SP_INVALID_LINE;
		return cmd;
	}
	// Create copy of given string to prevent segfault when running tests on string literal
	char *strcopy = malloc(strlen(str)+1);
	strcpy(strcopy, str);
	
	char *cmdName, *cmdArg;
	const char *delimeter = " \t\r\n";

	cmdName = strtok(strcopy, delimeter);
	if(cmdName){
		if(strcmp(cmdName, "undo_move")==0){
			cmd.cmd = SP_UNDO_MOVE;
		} else if(strcmp(cmdName, "add_disc")==0){
			cmd.cmd = SP_ADD_DISC;
			cmdArg = strtok(NULL, delimeter);
			if(cmdArg && spParserIsInt(cmdArg)){
				cmd.arg = atoi(cmdArg);
				cmd.validArg = true;
			}
		} else if(strcmp(cmdName, "suggest_move")==0){
			cmd.cmd = SP_SUGGEST_MOVE;
		} else if(strcmp(cmdName, "quit")==0){
			cmd.cmd = SP_QUIT;
		} else if(strcmp(cmdName, "restart")==0){
			cmd.cmd = SP_RESTART;
		} else{
			cmd.cmd = SP_INVALID_LINE;
		}
	}

	free(strcopy);
	return cmd;
}