/*
	This file is part of Warzone 2100.
	Copyright (C) 2006-2011  Warzone 2100 Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef __INCLUDED_SRC_AIEXPERIENCE_H__
#define __INCLUDED_SRC_AIEXPERIENCE_H__

#define	SAVE_FORMAT_VERSION			2

#define MAX_OIL_ENTRIES				600		//(Max number of derricks or oil resources) / 2

#define	MAX_OIL_DEFEND_LOCATIONS	100		//max number of attack locations to store
#define	MAX_OIL_LOCATIONS			300		//max number of oil locations to store
#define	MAX_BASE_DEFEND_LOCATIONS	30		//max number of base locations to store
#define SAME_LOC_RANGE				8		//if within this range, consider it the same loc

extern int32_t baseLocation[MAX_PLAYERS][MAX_PLAYERS][2];
extern int32_t baseDefendLocation[MAX_PLAYERS][MAX_BASE_DEFEND_LOCATIONS][2];
extern int32_t oilDefendLocation[MAX_PLAYERS][MAX_OIL_DEFEND_LOCATIONS][2];

extern int32_t baseDefendLocPrior[MAX_PLAYERS][MAX_BASE_DEFEND_LOCATIONS];
extern int32_t oilDefendLocPrior[MAX_PLAYERS][MAX_OIL_DEFEND_LOCATIONS];

extern	BOOL SavePlayerAIExperience(int32_t nPlayer, BOOL bNotify);
extern	int32_t LoadPlayerAIExperience(int32_t nPlayer);

extern	void LoadAIExperience(BOOL bNotify);
extern	BOOL SaveAIExperience(BOOL bNotify);


extern	BOOL ExperienceRecallOil(int32_t nPlayer);
extern	void InitializeAIExperience(void);
extern	BOOL OilResourceAt(uint32_t OilX, uint32_t OilY, int32_t VisibleToPlayer);

extern	int32_t ReadAISaveData(int32_t nPlayer);
extern	BOOL WriteAISaveData(int32_t nPlayer);

extern	BOOL SetUpInputFile(int32_t nPlayer);
extern	BOOL SetUpOutputFile(int32_t nPlayer);

extern	BOOL StoreBaseDefendLoc(int32_t x, int32_t y, int32_t nPlayer);
extern	BOOL StoreOilDefendLoc(int32_t x, int32_t y, int32_t nPlayer);

extern	BOOL SortBaseDefendLoc(int32_t nPlayer);
extern	BOOL SortOilDefendLoc(int32_t nPlayer);

extern	int32_t GetOilDefendLocIndex(int32_t x, int32_t y, int32_t nPlayer);
extern	int32_t GetBaseDefendLocIndex(int32_t x, int32_t y, int32_t nPlayer);

extern BOOL CanRememberPlayerBaseLoc(int32_t lookingPlayer, int32_t enemyPlayer);
extern BOOL CanRememberPlayerBaseDefenseLoc(int32_t player, int32_t index);
extern BOOL CanRememberPlayerOilDefenseLoc(int32_t player, int32_t index);

extern	void BaseExperienceDebug(int32_t nPlayer);
extern	void OilExperienceDebug(int32_t nPlayer);

extern BOOL canRecallOilAt(int32_t nPlayer, int32_t x, int32_t y);

//Return values of experience-loading routine
#define EXPERIENCE_LOAD_OK			0			//no problemens encountered
#define EXPERIENCE_LOAD_ERROR		1			//error while loading experience
#define EXPERIENCE_LOAD_NOSAVE		(-1)		//no experience exists

#endif // __INCLUDED_SRC_AIEXPERIENCE_H__
