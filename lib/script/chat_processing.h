/*This code copyrighted (2013) for the Warzone 2100 Legacy Project under the GPLv2.

Warzone 2100 Legacy is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Warzone 2100 Legacy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Warzone 2100 Legacy; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA*/
/*
 * chat_processing.h
 *
 * Misc definitions for chat parser
 */
#ifndef _chat_processing_h
#define _chat_processing_h

#include "lib/framework/frame.h"
#include "script.h"

#ifndef MAXSTRLEN
#define MAXSTRLEN 255
#endif

/* Max number of commands in a player chat message */
#define	MAX_CHAT_COMMANDS 10

/* Max number of parameters allowed in a single chat message command  */
#define MAX_CHAT_CMD_PARAMS 10


/* Holds information for each recognized
* command in a chat message */
typedef struct _chat_command_data
{
	const char	*pCmdDescription;				/* String representing a certain command */
	BOOL		bPlayerAddressed[MAX_PLAYERS];	/* Flag to indicate whether a command was addressed to a certain player */
	int32_t		numCmdParams;					/* Number of extracted parameters associated with each command */
	INTERP_VAL	parameter[MAX_CHAT_CMD_PARAMS];	/* Parameters extracted from text - to be used with scripts */
} CHAT_CMD_DATA;

typedef struct _chat_command
{
	char			lastMessage[MAXSTRLEN];			/* Parse the same mesage only once - in case more than one player is trying to parse */
	int32_t			numCommands;					/* Total number of commands in chat message */
	CHAT_CMD_DATA	cmdData[MAX_CHAT_COMMANDS];		/* Holds information for each recognized command */
} CHAT_MSG;

extern CHAT_MSG chat_msg;

/* Store parameter extracted from the message - for scripts */
//extern BOOL chat_store_parameter(INTERP_VAL *parameter);

extern void chatGetErrorData(int *pLine, char **ppText);

/* Set the current input buffer for the lexer */
extern void chatSetInputBuffer(char *pBuffer, uint32_t size);

// Load message
extern BOOL chatLoad(char *pData, uint32_t size);

#endif
