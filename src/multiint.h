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
 *  Interface defines/externs for warzone frontend.
 */

#ifndef __INCLUDED_SRC_MULTIINT_H__
#define __INCLUDED_SRC_MULTIINT_H__

#include "lib/netplay/netplay.h"
#include "lib/widget/widgbase.h"

extern LOBBY_ERROR_TYPES getLobbyError(void);
extern void setLobbyError(LOBBY_ERROR_TYPES error_type);

extern	void	runConnectionScreen		(void);
extern	BOOL	startConnectionScreen	(void);
extern	void	intProcessConnection	(uint32_t id);

extern	void	runGameFind				(void);
extern	void	startGameFind			(void);

void updateLimitFlags(void);

extern	void	runMultiOptions			(void);
extern	BOOL	startMultiOptions		(BOOL bReenter);
extern	void	frontendMultiMessages	(void);

extern BOOL addMultiBut(W_SCREEN *screen, uint32_t formid, uint32_t id, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *tipres, uint32_t norm, uint32_t down, uint32_t hi);

extern	char	sPlayer[128];

void	kickPlayer(uint32_t player_id, const char *reason, LOBBY_ERROR_TYPES type);
uint32_t	addPlayerBox(BOOL);			// players (mid) box
void loadMapPreview(bool hideInterface);


// ////////////////////////////////////////////////////////////////
// CONNECTION SCREEN

#define CON_CONTYPES		10103
#define CON_CONTYPESWIDTH	290
#define CON_CONTYPES_FORM	10104
#define CON_TYPESID_START	10105
#define CON_TYPESID_END		10128

#define CON_SETTINGS		10130
#define CON_SETTINGS_LABEL	10131
#define CON_SETTINGSX		220
#define	CON_SETTINGSY		190
#define CON_SETTINGSWIDTH	200
#define CON_SETTINGSHEIGHT	100

#define CON_PASSWORD_LABEL	10132

#define CON_OK				10101
#define CON_OKX				CON_SETTINGSWIDTH-MULTIOP_OKW*2-13
#define CON_OKY				CON_SETTINGSHEIGHT-MULTIOP_OKH-3

#define CON_CANCEL			10102

#define CON_PHONE			10132
#define CON_PHONEX			20
#define CON_PHONEY			45

#define CON_IP				10133
#define CON_IPX				20
#define CON_IPY				45

#define CON_IP_CANCEL		10134

//for clients
#define CON_PASSWORD		10139
#define CON_PASSWORDX		20
#define CON_PASSWORDY		110
#define CON_PASSWORDYES		10141
#define CON_PASSWORDNO		10142
// for hosts
#define CON_H_PASSWORD		10140
#define CON_H_PASSWORDX		MCOL2
#define CON_H_PASSWORDY		MROW10 +31



// ////////////////////////////////////////////////////////////////
// GAME FIND SCREEN

#define GAMES_GAMESTART		10201
#define GAMES_GAMEEND		GAMES_GAMESTART+20
#define GAMES_GAMEWIDTH		490
#define GAMES_GAMEHEIGHT	25
#define GAMES_GAME_X 45
#define GAMES_GAME_Y (40+((5+GAMES_GAMEHEIGHT)*i)) /*Not to be used elsewhere than the one place in multiint.c.*/
#define GAMES_NOGAME_X		70
#define GAMES_NOGAME_Y		40

#define GAMES_GAMELEGEND_X GAMES_GAME_X + xOffset /* << Warning: Don't use these elsewhere.*/
#define GAMES_GAMELEGEND_Y 20 + yOffset /* << */
#define GAMES_GAMELEGEND_WIDTH GAMES_GAMEWIDTH
#define GAMES_GAMELEGEND_HEIGHT 14

#define GAMES_GAME_PINGTXT_OFFSET 2
#define GAMES_GAME_PLAYERSTXT_OFFSET 25
#define GAMES_GAME_STATUSTXT_OFFSET 60
#define GAMES_GAME_GAMENAMETXT_OFFSET 100
#define GAMES_GAME_MAPNAMETXT_OFFSET 250
#define GAMES_GAME_HOSTEDBYTXT_OFFSET 375

// ////////////////////////////////////////////////////////////////
// GAME OPTIONS SCREEN

#define MULTIOP_PLAYERS			10231
#define MULTIOP_PLAYERSX		360
#define MULTIOP_PLAYERSY		4
#define MULTIOP_PLAYER_START		10232		//list of players
#define MULTIOP_PLAYER_END		10249
#define MULTIOP_PLAYERSW		263
#define MULTIOP_PLAYERSH		356

#define MULTIOP_ROW_WIDTH		246

//Team chooser
#define MULTIOP_TEAMS_START		102310			//List of teams
#define MULTIOP_TEAMS_END		102317
#define MULTIOP_TEAMSWIDTH		29
#define	MULTIOP_TEAMSHEIGHT		36

#define MULTIOP_TEAMCHOOSER_FORM	102800
#define MULTIOP_TEAMCHOOSER			102810
#define MULTIOP_TEAMCHOOSER_END		102817

// 'Ready' button
#define MULTIOP_READY_FORM_ID		102900
#define MULTIOP_READY_START			(MULTIOP_READY_FORM_ID + MAX_PLAYERS + 1)
#define	MULTIOP_READY_END			(MULTIOP_READY_START + 7)
#define MULTIOP_READY_WIDTH			41
#define MULTIOP_READY_HEIGHT		36
#define MULTIOP_READY_IMG_OFFSET_X	3
#define MULTIOP_READY_IMG_OFFSET_Y	6

#define MULTIOP_PLAYERWIDTH		245
#define	MULTIOP_PLAYERHEIGHT		36

#define MULTIOP_OPTIONS			10250
#define MULTIOP_OPTIONSX		40
#define MULTIOP_OPTIONSY		4
#define MULTIOP_OPTIONSW		284
#define MULTIOP_OPTIONSH		356

#define MULTIOP_EDITBOXW		196
#define	MULTIOP_EDITBOXH		30

#define MULTIOP_SLOT_FORM 11113

#define MULTIOP_SLOTOFFSET_X 5
#define MULTIOP_SLOTOFFSET_Y 3

#define MULTIOP_ADDSLOT 11111
#define MULTIOP_REMSLOT 11112

#define MULTIOP_SLOT_FORMX MULTIOP_CANCELX + 10
#define MULTIOP_SLOT_FORMY MROW10

#define MULTIOP_SPEC_FORM 11116

#define MULTIOP_SPECON 11114
#define MULTIOP_SPECOFF 11115

#define	MULTIOP_BLUEFORMW		226

#define	MROW1					4
#define	MROW2					MROW1+MULTIOP_EDITBOXH+4
#define	MROW3					MROW2+MULTIOP_EDITBOXH+4
#define	MROW4					MROW3+MULTIOP_EDITBOXH+4
#define MROW5					MROW4+36
#define	MROW6					MROW5+31
#define	MROW7					MROW6+31
#define	MROW8					MROW7+31
#define	MROW9					MROW8+31
#define	MROW10					MROW9+31
#define	MROW11					MROW10+31

#define MCOL0					50
#define MCOL1					(MCOL0+26+10)	// rem 10 for 4 lines.
#define MCOL2					(MCOL1+38)
#define MCOL3					(MCOL2+38)
#define MCOL4					(MCOL3+38)

#define MULTIOP_PNAME_ICON		10252
#define MULTIOP_PNAME			10253
#define MULTIOP_GNAME_ICON		10254
#define MULTIOP_GNAME			10255
#define MULTIOP_FNAME_ICON		10256
#define MULTIOP_FNAME			10257
#define MULTIOP_MAP_ICON		10258
#define MULTIOP_MAP				10259

//#define MULTIOP_ARENA			10260
#define MULTIOP_CAMPAIGN		10261
//#define MULTIOP_TEAMPLAY		10262
#define MULTIOP_SKIRMISH		10263


#define MULTIOP_TECH_LOW		10264
#define MULTIOP_TECH_MED		10265
#define MULTIOP_TECH_HI			10266

#define MULTIOP_CLEAN			10267
#define MULTIOP_BASE			10268
#define MULTIOP_DEFENCE			10269

#define MULTIOP_ALLIANCE_N		10270
#define MULTIOP_ALLIANCE_Y		10271
#define MULTIOP_ALLIANCE_TEAMS	102710		//locked teams

#define MULTIOP_POWLEV_LOW		10272
#define MULTIOP_POWLEV_MED		10273
#define MULTIOP_POWLEV_HI		10274

#define MULTIOP_REFRESH			10275
#define MULTIOP_REFRESHX		75
#define MULTIOP_REFRESHY		453

#define MULTIOP_HOST			10276
#define MULTIOP_HOSTX			5
#define MULTIOP_HOSTY			MROW3+3

#define MULTIOP_STRUCTLIMITS	10277
#define MULTIOP_STRUCTLIMITSX	5
#define MULTIOP_STRUCTLIMITSY	MROW2+5

#define MULTIOP_OKX				MULTIOP_HOSTX
#define MULTIOP_OKY				MULTIOP_HOSTY
#define MULTIOP_CANCELX			6
#define MULTIOP_CANCELY			6

#define MULTIOP_CHATBOX			10278
#define MULTIOP_CHATBOXX		MULTIOP_OPTIONSX
#define MULTIOP_CHATBOXY		362
#define MULTIOP_CHATBOXW		((MULTIOP_PLAYERSX+MULTIOP_PLAYERSW) - MULTIOP_OPTIONSX)
#define MULTIOP_CHATBOXH		115

#define MULTIOP_CHATEDIT		10279
#define MULTIOP_CHATEDITX		4
#define	MULTIOP_CHATEDITY		MULTIOP_CHATBOXH-14
#define	MULTIOP_CHATEDITW		MULTIOP_CHATBOXW-8
#define MULTIOP_CHATEDITH		9

#define MULTIOP_COLCHOOSER_FORM	10280
#define MULTIOP_COLCHOOSER		10281
#define MULTIOP_COLCHOOSER_END	10288
#define MULTIOP_COLCHOOSER_KICK	10289

#define MULTIOP_LIMIT			10292	// 2 for this (+label)
#define MULTIOP_GAMETYPE		10294
#define MULTIOP_POWER			10296
#define MULTIOP_ALLIANCES		10298
#define MULTIOP_BASETYPE		10300
#define MULTIOP_TECHLEVEL		10302
#define MULTIOP_COMPUTER		10304
#define	MULTIOP_FOG				10306

#define MULTIOP_COMPUTER_Y		10308
#define MULTIOP_COMPUTER_N		10309

#define	MULTIOP_FOG_ON			10310
#define	MULTIOP_FOG_OFF			10311

#define MULTIOP_SKSLIDE			10313
#define MULTIOP_SKSLIDE_END		10320

#define MULTIOP_PLAYCHOOSER		10321
#define MULTIOP_PLAYCHOOSER_END	10330

#define MULTIOP_MAP_PREVIEW 920000
#define MULTIOP_MAP_BUT		920002

#define MULTIOP_PASSWORD	920010
#define MULTIOP_PASSWORD_BUT 920012
#define MULTIOP_PASSWORD_EDIT 920013

#define MULTIOP_NO_SOMETHING            10331  // Up to 10340 reserved for future use.
#define MULTIOP_NO_SOMETHINGX           3
#define MULTIOP_NO_SOMETHINGY           MROW5

// ///////////////////////////////
// Many Button Variations..

#define CON_BUTWIDTH			60
#define CON_BUTHEIGHT			46

#define CON_CONBUTW			CON_CONTYPESWIDTH-15
#define CON_CONBUTH			46

#define	CON_NAMEBOXWIDTH		CON_SETTINGSWIDTH-CON_PHONEX
#define	CON_NAMEBOXHEIGHT		15

#define CON_COMBUTWIDTH			37
#define CON_COMBUTHEIGHT		24

#define MULTIOP_OKW			37
#define MULTIOP_OKH			24

#define MULTIOP_BUTW			35
#define MULTIOP_BUTH			24

#endif // __INCLUDED_SRC_MULTIINT_H__
