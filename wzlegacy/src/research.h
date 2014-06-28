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
/** @file
 *  structures required for research stats
 */

#ifndef __INCLUDED_SRC_RESEARCH_H__
#define __INCLUDED_SRC_RESEARCH_H__

#include "objectdef.h"

#define NO_RESEARCH_ICON 0
//max 'research complete' console message length
#define MAX_RESEARCH_MSG_SIZE 200


//used for loading in the research stats into the appropriate list
enum
{
	REQ_LIST,
	RED_LIST,
	RES_LIST
};


enum
{
	RID_ROCKET,
	RID_CANNON,
	RID_HOVERCRAFT,
	RID_ECM,
	RID_PLASCRETE,
	RID_TRACKS,
	RID_DROIDTECH,
	RID_WEAPONTECH,
	RID_COMPUTERTECH,
	RID_POWERTECH,
	RID_SYSTEMTECH,
	RID_STRUCTURETECH,
	RID_CYBORGTECH,
	RID_DEFENCE,
	RID_QUESTIONMARK,
	RID_GRPACC,
	RID_GRPUPG,
	RID_GRPREP,
	RID_GRPROF,
	RID_GRPDAM,
	RID_MAXRID
};


/* The store for the research stats */
extern		RESEARCH				*asResearch;
extern		uint32_t					numResearch;

//List of pointers to arrays of PLAYER_RESEARCH[numResearch] for each player
extern PLAYER_RESEARCH		*asPlayerResList[MAX_PLAYERS];

//used for Callbacks to say which topic was last researched
extern RESEARCH				*psCBLastResearch;
extern STRUCTURE			*psCBLastResStructure;
extern int32_t				CBResFacilityOwner;

/* Default level of sensor, repair and ECM */
extern uint32_t	aDefaultSensor[MAX_PLAYERS];
extern uint32_t	aDefaultECM[MAX_PLAYERS];
extern uint32_t	aDefaultRepair[MAX_PLAYERS];

//extern BOOL loadResearch(void);
extern BOOL loadResearch(const char *pResearchData, uint32_t bufferSize);
//Load the pre-requisites for a research list
extern BOOL loadResearchPR(const char *pPRData, uint32_t bufferSize);
//Load the artefacts for a research list
extern BOOL loadResearchArtefacts(const char *pArteData, uint32_t bufferSize, uint32_t listNumber);
//Load the pre-requisites for a research list
extern BOOL loadResearchFunctions(const char *pFunctionData, uint32_t bufferSize);
//Load the Structures for a research list
extern BOOL loadResearchStructures(const char *pStructData, uint32_t bufferSize, uint32_t listNumber);

/*function to check what can be researched for a particular player at any one
  instant. Returns the number to research*/
//extern uint8_t fillResearchList(uint8_t *plist, uint32_t playerID, uint16_t topic,
//							   uint16_t limit);
//needs to be uint16_t sized for Patches
extern uint16_t fillResearchList(uint16_t *plist, uint32_t playerID, uint16_t topic,
								 uint16_t limit);

/* process the results of a completed research topic */
extern void researchResult(uint32_t researchIndex, uint8_t player, BOOL bDisplay, STRUCTURE *psResearchFacility);

//this just inits all the research arrays
extern BOOL ResearchShutDown(void);
//this free the memory used for the research
extern void ResearchRelease(void);

/* For a given view data get the research this is related to */
extern RESEARCH *getResearch(const char *pName, BOOL resName);

/* sets the status of the topic to cancelled and stores the current research
   points accquired */
extern void cancelResearch(STRUCTURE *psBuilding);

/* For a given view data get the research this is related to */
extern RESEARCH *getResearchForMsg(struct _viewdata *pViewData);

/* Sets the 'possible' flag for a player's research so the topic will appear in
the research list next time the Research Facilty is selected */
extern BOOL enableResearch(RESEARCH *psResearch, uint32_t player);

/*find the last research topic of importance that the losing player did and
'give' the results to the reward player*/
extern void researchReward(uint8_t losingPlayer, uint8_t rewardPlayer);

/*check to see if any research has been completed that enables self repair*/
extern BOOL selfRepairEnabled(uint8_t player);

extern int32_t	mapRIDToIcon( uint32_t rid );
extern int32_t	mapIconToRID(uint32_t iconID);
extern BOOL checkResearchStats(void);

/*puts research facility on hold*/
extern void holdResearch(STRUCTURE *psBuilding);
/*release a research facility from hold*/
extern void releaseResearch(STRUCTURE *psBuilding);

/*checks the stat to see if its of type wall or defence*/
extern BOOL wallDefenceStruct(STRUCTURE_STATS *psStats);

extern void enableSelfRepair(uint8_t player);

void CancelAllResearch(uint32_t pl);

extern BOOL researchInitVars(void);

#endif // __INCLUDED_SRC_RESEARCH_H__
