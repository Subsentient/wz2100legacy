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
/**This file handles spectating in-game.**/

#include "lib/framework/frame.h"
#include "lib/netplay/netplay.h"
#include "console.h"
#include "droid.h"
#include "display3d.h"
#include "structure.h"
#include "objects.h"
#include "display.h"
#include "multiplay.h"
#include "multiint.h"

#include "spectate.h"

/*Globals.*/
bool AllowSpectating = true;


bool PlayerSpectating(int Player)
{ /*Makes one wonder if we really need this.*/
	return NetPlay.players[Player].spectating;
}

void SendSpectateRequest(void)
{ /*Turns us into a spectator and notifies everyone else, if allowed.*/
	uint8_t OurP = (uint8_t)selectedPlayer;
	
	if (!AllowSpectating)
	{
		addConsoleMessage("Spectating is disabled for this game.", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
		return;
	}
	else if (NetPlay.players[selectedPlayer].spectating)
	{
		addConsoleMessage(_("You are already a spectator."), DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
		return;
	}
	
	if (NetPlay.bComms)
	{ /*Don't send if in skirmish.*/
		NETbeginEncode(NET_SPECTATE, NET_ALL_PLAYERS);
		NETuint8_t(&OurP);
		NETend();
	}
	
	MakeToSpectator(OurP);
}

void recvSpectateRequest(void)
{
	uint8_t inPlayer;
	char Buf[MAX_CONSOLE_STRING_LENGTH];
	
	NETbeginDecode(NET_SPECTATE);
	NETuint8_t(&inPlayer);
	NETend();
	
	if (!AllowSpectating)
	{
		debug(LOG_ERROR, "%s (player %d) attempted to spectate themselves with spectator mode disabled.", NetPlay.players[inPlayer].name, inPlayer);
		
		if (NetPlay.isHost)
		{
			snprintf(Buf, MAX_CONSOLE_STRING_LENGTH, _("Auto-kicking %s (%d) for cheating by trying to use spectator mode when disabled."), NetPlay.players[inPlayer].name, inPlayer);
			
			debug(LOG_ERROR, Buf);
			sendTextMessage(Buf, true);
			kickPlayer(inPlayer, _("Cheater!"), ERROR_KICKED);
		}
		
		return;
	}
	
	if (NETgetSource() != inPlayer)
	{ /*Someone is trying to cheat.*/
		int BadPlayer = NETgetSource();
		
		debug(LOG_WARNING, "%s (player %d) tried to cheat by trying to turn another player into a spectator.",
																	NetPlay.players[BadPlayer].name, BadPlayer);
		
		if (NetPlay.isHost)
		{
			snprintf(Buf, MAX_CONSOLE_STRING_LENGTH,
				_("Auto-kicking %s (%d) for cheating by trying to turn another player into a spectator."),
				NetPlay.players[inPlayer].name, inPlayer);
			
			debug(LOG_ERROR, Buf);
			sendTextMessage(Buf, true);
			kickPlayer(inPlayer, _("Cheater!"), ERROR_KICKED);
		}
		else if (NetPlay.hostPlayer == BadPlayer)
		{ /*It was the host. We want out.*/
			debug(LOG_WARNING, "The scoundril is the host! I'm outta here.");
			
			snprintf(Buf, MAX_CONSOLE_STRING_LENGTH, 
				_("%s: The host is cheating by sending rouge spectator signals. Beware."),
				NetPlay.players[selectedPlayer].name);
			
			sendTextMessage(Buf, true);
			
			NETclose();
		}
		
		return;
	}
	
	MakeToSpectator(inPlayer);
}

void MakeToSpectator(uint8_t inPlayer)
{ /*actually does the work on turning players into a spectator.*/
	char Buf[MAX_CONSOLE_STRING_LENGTH];
	DROID *psCDroid, *psNDroid;
	STRUCTURE *psCStruct, *psNStruct;
	
	if (NetPlay.players[inPlayer].spectating)
	{
		debug(LOG_ERROR, "Received spectate request from player %d, but they are already spectating!", inPlayer);
		return;
	}
	
	/*Destroy all their droids.*/	
	for (psCDroid = apsDroidLists[inPlayer]; psCDroid; psCDroid = psNDroid)
	{
		psNDroid = psCDroid->psNext;
		destroyDroid(psCDroid);
	}
	
	/*Destroy all their structures.*/
	for(psCStruct = apsStructLists[inPlayer]; psCStruct; psCStruct = psNStruct)
	{
		psNStruct = psCStruct->psNext;
		destroyStruct(psCStruct);
	}
	
	NetPlay.players[inPlayer].spectating = true; //Set the bit that they are now a spectator.
	
	if (inPlayer == selectedPlayer)
	{ /*Oh, cool, it's us! Give us all the goodies!*/
		radarOnScreen = true;
		godMode = true; /*Let us see everything now.*/
		revealAll(inPlayer);
		
		strncpy(Buf, _("You are now a spectator."), MAX_CONSOLE_STRING_LENGTH);
		addConsoleMessage(Buf, DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
	}
	else
	{
		snprintf(Buf, MAX_CONSOLE_STRING_LENGTH, _("%s has become a spectator."), NetPlay.players[inPlayer].name);
		debug(LOG_INFO, Buf);
		
		addConsoleMessage(Buf, DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
	}
}
