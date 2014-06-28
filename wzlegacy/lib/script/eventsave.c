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
 * EvntSave.c
 *
 * Save the state of the event system.
 *
 */
#include <string.h>

#include "lib/framework/frame.h"
#include "lib/framework/frameresource.h"
#include "lib/framework/endian_hack.h"
#include "lib/framework/string_ext.h"
#include "script.h"
#include "eventsave.h"



// the event save file header
typedef struct _event_save_header
{
	char		aFileType[4];
	uint32_t		version;
} EVENT_SAVE_HDR;


// save the context information for the script system
static BOOL eventSaveContext(char *pBuffer, uint32_t *pSize)
{
	uint32_t				size, valSize;
	int32_t				numVars, i, numContext;
	SCRIPT_CONTEXT		*psCCont;
	VAL_CHUNK			*psCVals;
	INTERP_VAL			*psVal;
	SCR_VAL_SAVE		saveFunc;
	char				*pPos;
//not hashed	char				*pScriptID;
	uint32_t				hashedName;
	uint16_t				*pValSize = NULL;


	size = 0;
	numContext = 0;
	pPos = pBuffer;

	// reserve space to store how many contexts are saved
	if (pBuffer != NULL)
	{
		pPos += sizeof(int16_t);
	}
	size += sizeof(int16_t);

	// go through the context list
	for(psCCont = psContList; psCCont != NULL; psCCont = psCCont->psNext)
	{
		numContext += 1;

		// save the context info
//nothashed if (!resGetIDfromData("SCRIPT", psCCont->psCode, &hashedName))
		if (!resGetHashfromData("SCRIPT", psCCont->psCode, &hashedName))
		{
			debug( LOG_FATAL, "eventSaveContext: couldn't find script resource id" );
			abort();
			return false;
		}
		numVars = psCCont->psCode->numGlobals + psCCont->psCode->arraySize;

		if (pBuffer != NULL)
		{
//not hashed			strcpy(pPos, pScriptID);
//not hashed			pPos += strlen(pScriptID) + 1;
			*((uint32_t *)pPos) = (uint32_t)hashedName;
			endian_udword((uint32_t *)pPos);
			pPos += sizeof(uint32_t);

			*((int16_t *)pPos) = (int16_t)numVars;
			endian_sword((int16_t *)pPos);
			pPos += sizeof(int16_t);

			*pPos = (uint8_t)psCCont->release;
			pPos += sizeof(uint8_t);
		}

//not hashed		size += strlen(pScriptID) + 1 + sizeof(int16_t) + sizeof(uint8_t);
		size += sizeof(uint32_t) + sizeof(int16_t) + sizeof(uint8_t);

		// save the context variables
		for(psCVals = psCCont->psGlobals; psCVals != NULL; psCVals = psCVals->psNext)
		{
			for(i = 0; i < CONTEXT_VALS; i += 1)
			{
				psVal = psCVals->asVals + i;

				// store the variable type
				if (pBuffer != NULL)
				{
					ASSERT( psVal->type < int16_t_MAX,
							"eventSaveContext: variable type number too big" );

					*((int16_t *)pPos) = (int16_t)psVal->type;
					endian_sword((int16_t *)pPos);

					pPos += sizeof(int16_t);
				}
				size += sizeof(int16_t);

				// store the variable value
				if (psVal->type == VAL_STRING)
				{
					uint32_t stringLen = 0;

					if(psVal->v.sval != NULL && strlen(psVal->v.sval) > 0)
					{
						stringLen = strlen(psVal->v.sval) + 1;
					}

					if (pBuffer != NULL)
					{
						*((uint32_t *)pPos) = stringLen;
						endian_udword((uint32_t *)pPos);
						pPos += sizeof(uint32_t);

						if(stringLen > 0)
						{
							strcpy((char *)pPos, psVal->v.sval);
						}
						pPos += stringLen;
					}

					size += sizeof(uint32_t) + stringLen;
				}
				else if (psVal->type < VAL_USERTYPESTART)
				{
					// internal type
					if (pBuffer != NULL)
					{
						/* FIXME: this does not work for VAL_OBJ_GETSET, VAL_FUNC_EXTERN */
						*((uint32_t *)pPos) = (uint32_t)psVal->v.ival;
						endian_udword((uint32_t *)pPos);

						pPos += sizeof(uint32_t);
					}

					size += sizeof(uint32_t);
				}
				else
				{
					// user defined type
					saveFunc = asScrTypeTab[psVal->type - VAL_USERTYPESTART].saveFunc;

					ASSERT( saveFunc != NULL,
							"eventSaveContext: no save function for type %d\n", psVal->type );

					// reserve some space to store how many bytes the value uses
					if (pBuffer != NULL)
					{
						pValSize = (uint16_t *)pPos;
						pPos += sizeof(uint16_t);
					}
					size += sizeof(uint16_t);

					if (!saveFunc(psVal, pPos, &valSize))
					{
						debug( LOG_FATAL, "eventSaveContext: couldn't get variable value size" );
						abort();
						return false;
					}

					if (pBuffer != NULL)
					{
						*pValSize = (uint16_t)valSize;
						endian_uword((uint16_t *)pValSize);

						pPos += valSize;
					}
					size += valSize;
				}

				numVars -= 1;
				if (numVars <= 0)
				{
					// done all the variables
					ASSERT( psCVals->psNext == NULL,
							"eventSaveContext: number of context variables does not match the script code" );
					break;
				}
			}
		}
		ASSERT( numVars == 0,
				"eventSaveContext: number of context variables does not match the script code" );
	}

	// actually store how many contexts have been saved
	if (pBuffer != NULL)
	{
		*((int16_t *)pBuffer) = (int16_t)numContext;
		endian_sword((int16_t *)pBuffer);
	}
	*pSize = size;

	return true;
}

// load the context information for the script system
static BOOL eventLoadContext(const int32_t version, char *pBuffer, uint32_t *pSize, BOOL bHashed)
{
	uint32_t				size, valSize, stringLen;
	int32_t				numVars, i, numContext, context;
	SCRIPT_CONTEXT		*psCCont;
	INTERP_TYPE			type;
	SCR_VAL_LOAD		loadFunc;
	char				*pPos;
	char				*pScriptID = NULL;
	uint32_t				hashedName;
	SCRIPT_CODE			*psCode;
	CONTEXT_RELEASE			release;
	INTERP_VAL			*psVal, data;

	size = 0;
	pPos = pBuffer;

	// get the number of contexts in the save file
	endian_sword((int16_t *)pPos);
	numContext = *((int16_t *)pPos);
	pPos += sizeof(int16_t);
	size += sizeof(int16_t);

	// go through the contexts
	for(context = 0; context < numContext; context += 1)
	{
		if(bHashed)
		{
			endian_udword((uint32_t *)pPos);
			hashedName = *((uint32_t *)pPos);
			psCode = (SCRIPT_CODE *)resGetDataFromHash("SCRIPT", hashedName);
			pPos += sizeof(uint32_t);
		}
		else
		{
			// get the script code
			pScriptID = (char *)pPos;
			psCode = (SCRIPT_CODE *)resGetData("SCRIPT", pScriptID);
			pPos += strlen(pScriptID) + 1;
		}
		// check the number of variables
		endian_sword((int16_t *)pPos);
		numVars = psCode->numGlobals + psCode->arraySize;

		if (numVars != *((int16_t *)pPos))
		{
			ASSERT(false, "Context %d of %d: Number of context variables (%d) does not match the script code (%d)",
				   context, numContext, numVars, *((int16_t *)pPos));
			return false;
		}
		pPos += sizeof(int16_t);

		release = (CONTEXT_RELEASE) * pPos;
		pPos += sizeof(uint8_t);

		// create the context
		if (!eventNewContext(psCode, release, &psCCont))
		{
			return false;
		}

		// bit of a hack this - note the id of the context to link it to the triggers
		psContList->id = (int16_t)context;

		if(bHashed)
		{
			size += sizeof(uint32_t) + sizeof(int16_t) + sizeof(uint8_t);
		}
		else
		{
			size += strlen(pScriptID) + 1 + sizeof(int16_t) + sizeof(uint8_t);
		}

		// set the context variables
		for(i = 0; i < numVars; i += 1)
		{
			// get the variable type
			endian_sword((int16_t *)pPos);
			type = (INTERP_TYPE) * ((int16_t *)pPos);
			pPos += sizeof(int16_t);
			size += sizeof(int16_t);

			// get the variable value
			if (type < VAL_USERTYPESTART)
			{
				data.type = type;

				endian_udword((uint32_t *)pPos);

				switch (type)
				{
					case VAL_BOOL:
						data.v.bval = *((BOOL *)pPos);
						pPos += sizeof(BOOL);
						size += sizeof(BOOL);
						break;
					case VAL_FLOAT:
						data.v.fval = *((float *)pPos);
						pPos += sizeof(float);
						size += sizeof(float);
						break;
					case VAL_INT:
					case VAL_TRIGGER:
					case VAL_EVENT:
					case VAL_VOID:
					case VAL_OPCODE:
					case VAL_PKOPCODE:
						data.v.ival = *((uint32_t *)pPos);
						pPos += sizeof(uint32_t);
						size += sizeof(uint32_t);
						break;
					case VAL_STRING:
						data.v.sval = (char *)malloc(MAXSTRLEN);
						strcpy(data.v.sval, "\0");

						stringLen = *((uint32_t *)pPos);	//read string length

						pPos += sizeof(uint32_t);
						size += sizeof(uint32_t);

						//load string
						if(stringLen > 0)
						{
							strlcpy(data.v.sval, (char *)pPos, MIN(stringLen + 1, MAXSTRLEN));
							pPos += stringLen;
							size += stringLen;
						}
						break;
					case VAL_OBJ_GETSET:
						/* FIXME: saving pointer on disk! */
						data.v.pObjGetSet = *((SCRIPT_VARFUNC *)pPos);
						pPos += sizeof(SCRIPT_VARFUNC);
						size += sizeof(SCRIPT_VARFUNC);
						break;
					case VAL_FUNC_EXTERN:
						/* FIXME: saving pointer on disk! */
						data.v.pFuncExtern = *((SCRIPT_FUNC *)pPos);
						pPos += sizeof(SCRIPT_FUNC);
						size += sizeof(SCRIPT_FUNC);
						break;
					default:
						ASSERT( false, "eventLoadContext: invalid internal type" );
				}

				// set the value in the context
				if (!eventSetContextVar(psCCont, (uint32_t)i, &data))
				{
					debug( LOG_FATAL, "eventLoadContext: couldn't set variable value" );
					abort();
					return false;
				}
			}
			else
			{
				// user defined type
				loadFunc = asScrTypeTab[type - VAL_USERTYPESTART].loadFunc;

				ASSERT( loadFunc != NULL,
						"eventLoadContext: no load function for type %d\n", type );

				endian_uword((uint16_t *)pPos);
				valSize = *((uint16_t *)pPos);

				pPos += sizeof(uint16_t);
				size += sizeof(uint16_t);

				// get the value pointer so that the loadFunc can write directly
				// into the variables data space.
				if (!eventGetContextVal(psCCont, (uint32_t)i, &psVal))
				{
					debug( LOG_FATAL, "eventLoadContext: couldn't find variable in context" );
					abort();
					return false;
				}

				if (!loadFunc(version, psVal, pPos, valSize))
				{
					debug( LOG_FATAL, "eventLoadContext: couldn't get variable value" );
					abort();
					return false;
				}

				pPos += valSize;
				size += valSize;
			}
		}
	}

	*pSize = size;

	return true;
}

// return the index of a context
static BOOL eventGetContextIndex(SCRIPT_CONTEXT *psContext, int32_t *pIndex)
{
	SCRIPT_CONTEXT	*psCurr;
	int32_t			index;

	index = 0;
	for(psCurr = psContList; psCurr != NULL; psCurr = psCurr->psNext)
	{
		if (psCurr == psContext)
		{
			*pIndex = index;
			return true;
		}
		index += 1;
	}

	return false;
}

// find a context from it's id number
static BOOL eventFindContext(int32_t id, SCRIPT_CONTEXT **ppsContext)
{
	SCRIPT_CONTEXT	*psCurr;

	for(psCurr = psContList; psCurr != NULL; psCurr = psCurr->psNext)
	{
		if (psCurr->id == id)
		{
			*ppsContext = psCurr;
			return true;
		}
	}

	return false;
}

// save a list of triggers
static BOOL eventSaveTriggerList(ACTIVE_TRIGGER *psList, char *pBuffer, uint32_t *pSize)
{
	ACTIVE_TRIGGER		*psCurr;
	uint32_t				size;
	char				*pPos;
	int32_t				numTriggers, context;

	size = 0;
	pPos = pBuffer;

	// reserve some space for the number of triggers
	if (pBuffer != NULL)
	{
		pPos += sizeof(int32_t);
	}
	size += sizeof(int32_t);

	numTriggers = 0;
	for(psCurr = psList; psCurr != NULL; psCurr = psCurr->psNext)
	{
		numTriggers += 1;

		if (pBuffer != NULL)
		{
			*((uint32_t *)pPos) = psCurr->testTime;
			endian_udword((uint32_t *)pPos);

			pPos += sizeof(uint32_t);
			if (!eventGetContextIndex(psCurr->psContext, &context))
			{
				debug( LOG_FATAL, "eventSaveTriggerList: couldn't find context" );
				abort();
				return false;
			}
			*((int16_t *)pPos) = (int16_t)context;
			endian_sword((int16_t *)pPos);
			pPos += sizeof(int16_t);
			*((int16_t *)pPos) = psCurr->type;
			endian_sword((int16_t *)pPos);
			pPos += sizeof(int16_t);
			*((int16_t *)pPos) = psCurr->trigger;
			endian_sword((int16_t *)pPos);
			pPos += sizeof(int16_t);
			*((uint16_t *)pPos) = psCurr->event;
			endian_uword((uint16_t *)pPos);
			pPos += sizeof(uint16_t);
			*((uint16_t *)pPos) = psCurr->offset;
			endian_uword((uint16_t *)pPos);
			pPos += sizeof(uint16_t);
		}
		size += sizeof(uint32_t) + sizeof(int16_t) * 3 + sizeof(uint16_t) * 2;
	}
	if (pBuffer != NULL)
	{
		*((int32_t *)pBuffer) = numTriggers;
		endian_sdword((int32_t *)pBuffer);
	}

	*pSize = size;

	return true;
}


// load a list of triggers
static BOOL eventLoadTriggerList(WZ_DECL_UNUSED const int32_t version, char *pBuffer, uint32_t *pSize)
{
	uint32_t				size, event, offset, time;
	char				*pPos;
	int32_t				numTriggers, context, type, trigger, i;
	SCRIPT_CONTEXT		*psContext;

	size = 0;
	pPos = pBuffer;

	// get the number of triggers
	endian_sdword((int32_t *)pPos);
	numTriggers = *((int32_t *)pPos);
	pPos += sizeof(int32_t);
	size += sizeof(int32_t);

	for(i = 0; i < numTriggers; i += 1)
	{
		endian_udword((uint32_t *)pPos);
		time = *((uint32_t *)pPos);
		pPos += sizeof(uint32_t);

		endian_sword((int16_t *)pPos);
		context = *((int16_t *)pPos);
		pPos += sizeof(int16_t);
		if (!eventFindContext(context, &psContext))
		{
			debug( LOG_FATAL, "eventLoadTriggerList: couldn't find context" );
			abort();
			return false;
		}

		endian_sword((int16_t *)pPos);
		type = *((int16_t *)pPos);
		pPos += sizeof(int16_t);

		endian_sword((int16_t *)pPos);
		trigger = *((int16_t *)pPos);
		pPos += sizeof(int16_t);

		endian_uword((uint16_t *)pPos);
		event = *((uint16_t *)pPos);
		pPos += sizeof(uint16_t);

		endian_uword((uint16_t *)pPos);
		offset = *((uint16_t *)pPos);
		pPos += sizeof(uint16_t);

		size += sizeof(uint32_t) + sizeof(int16_t) * 3 + sizeof(uint16_t) * 2;

		if (!eventLoadTrigger(time, psContext, type, trigger, event, offset))
		{
			return false;
		}
	}

	*pSize = size;

	return true;
}


// Save the state of the event system
BOOL eventSaveState(int32_t version, char **ppBuffer, uint32_t *pFileSize)
{
	uint32_t			size, totalSize;
	char			*pBuffer, *pPos;
	EVENT_SAVE_HDR	*psHdr;

	totalSize = sizeof(EVENT_SAVE_HDR);

	// find the size of the context save
	if (!eventSaveContext(NULL, &size))
	{
		return false;
	}
	totalSize += size;

	// find the size of the trigger save
	if (!eventSaveTriggerList(psTrigList, NULL, &size))
	{
		return false;
	}
	totalSize += size;

	// find the size of the callback trigger save
	if (!eventSaveTriggerList(psCallbackList, NULL, &size))
	{
		return false;
	}
	totalSize += size;



	// Allocate the buffer to save to
	pBuffer = (char *)malloc(totalSize);
	if (pBuffer == NULL)
	{
		debug( LOG_FATAL, "eventSaveState: out of memory" );
		abort();
		return false;
	}
	pPos = pBuffer;


	// set the header
	psHdr = (EVENT_SAVE_HDR *)pPos;
	psHdr->aFileType[0] = 'e';
	psHdr->aFileType[1] = 'v';
	psHdr->aFileType[2] = 'n';
	psHdr->aFileType[3] = 't';
	psHdr->version = version;
	endian_udword(&psHdr->version);

	pPos += sizeof(EVENT_SAVE_HDR);


	// save the contexts
	if (!eventSaveContext(pPos, &size))
	{
		return false;
	}
	pPos += size;

	// save the triggers
	if (!eventSaveTriggerList(psTrigList, pPos, &size))
	{
		return false;
	}
	pPos += size;

	// save the callback triggers
	if (!eventSaveTriggerList(psCallbackList, pPos, &size))
	{
		return false;
	}
	pPos += size;

	*ppBuffer = pBuffer;
	*pFileSize = totalSize;

	return true;
}


// Load the state of the event system
BOOL eventLoadState(char *pBuffer, uint32_t fileSize, BOOL bHashed)
{
	uint32_t			size, totalSize, version;
	char			*pPos;
	EVENT_SAVE_HDR	*psHdr;


	pPos = pBuffer;
	totalSize = 0;

	// Get the header
	psHdr = (EVENT_SAVE_HDR *)pPos;
	endian_udword(&psHdr->version);
	if (strncmp(psHdr->aFileType, "evnt", 4) != 0)
	{
		debug( LOG_FATAL, "eventLoadState: invalid file header" );
		abort();
		return false;
	}
	/*	if ((psHdr->version != 1) &&
			(psHdr->version != 2))
		{
			DBERROR(("eventLoadState: invalid file version"));
			return false;
		}*/
	version = psHdr->version;
	pPos += sizeof(EVENT_SAVE_HDR);
	totalSize += sizeof(EVENT_SAVE_HDR);


	// load the event contexts
	if (!eventLoadContext(version, pPos, &size, bHashed))
	{
		return false;
	}

	pPos += size;
	totalSize += size;

	// load the normal triggers
	if (!eventLoadTriggerList(version, pPos, &size))
	{
		return false;
	}
	pPos += size;
	totalSize += size;

	// load the callback triggers
	if (!eventLoadTriggerList(version, pPos, &size))
	{
		return false;
	}
	pPos += size;
	totalSize += size;

	if (totalSize != fileSize)
	{
		debug( LOG_FATAL, "eventLoadState: corrupt save file" );
		abort();
		return false;
	}

	return true;
}
