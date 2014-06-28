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
 *  Definitions for multi player statistics and scores for league tables.
 *  Also Definitions for saved Arena Forces to enable teams to be saved to disk
 */

#ifndef __INCLUDED_SRC_MULTISTATS_H__
#define __INCLUDED_SRC_MULTISTATS_H__

typedef struct
{
	uint32_t played;						/// propogated stats.
	uint32_t wins;
	uint32_t losses;
	uint32_t totalKills;
	uint32_t totalScore;

	uint32_t recentKills;				// score/kills in last game.
	uint32_t recentScore;

	uint32_t killsToAdd;					// things to add next time score is updated.
	uint32_t scoreToAdd;
} PLAYERSTATS;

BOOL saveMultiStats(const char *sFName, const char *sPlayerName, const PLAYERSTATS *playerStats);	// to disk
BOOL loadMultiStats(char *sPlayerName, PLAYERSTATS *playerStats);					// form disk
PLAYERSTATS getMultiStats(uint32_t player);									// get from net
BOOL setMultiStats(int32_t player, PLAYERSTATS plStats, BOOL bLocal);			// send to net.
void updateMultiStatsDamage(uint32_t attacker, uint32_t defender, uint32_t inflicted);
void updateMultiStatsGames(void);
void updateMultiStatsWins(void);
void updateMultiStatsLoses(void);
void updateMultiStatsKills(BASE_OBJECT *psKilled, uint32_t player);
void recvMultiStats(void);

#endif // __INCLUDED_SRC_MULTISTATS_H__
