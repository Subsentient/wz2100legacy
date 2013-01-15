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

//This file contains functions for spectator support.

#include "lib/framework/frame.h"
#include "main.h"
#include "display3d.h"
#include "cheat.h"
#include "keymap.h"
#include "advvis.h"
#include "droid.h"
#include "objmem.h"
#include "spectate.h"
#include "multiplay.h"
#include "multigifts.h"
#include "multiint.h"
#include "lib/netplay/netplay.h"
#include "hci.h"
#include "lib/widget/widget.h"
#include "lib/gamelib/gtime.h"
#include "lib/framework/wzapp.h"

#include "console.h"

//Define variables.
bool allowSpectating = true; //If spectating is allowed in the game. Overridden if disabled in MP game or config.
bool isSpectating = false; //Are we spectating?
bool blockDebug = false; //Block debug mode? Used only for spectators.

/*Functions start here.*/

//Thread used by spectator support. -Subsentient

int specThread(void *)
//Tiny function needed for delaying the minimap display for spectators, so if we have an HQ destroyed we still get a minimap. -Subsentient
//It needs to be in a thread so that it's actually useful, otherwise it hard freezes the game until it's time is up and the stucts are not destroyed yet.
//This also enables uplink-ish-ness and stuff.
{

	for(int tempgt = wzGetTicks(); apsStructLists[selectedPlayer] != NULL && apsDroidLists[selectedPlayer] != NULL; tempgt = tempgt + 1000) //Just increment tempgt to be efficient.
	{
		while (wzGetTicks() < tempgt + 1000) //We just keep waiting until everything is dead so we don't
		{					//spawn our minimap too soon and have it turned off when our HQ dies. -Subsentient
			wzYieldCurrentThread();
		}
	}

	widgDelete(psWScreen, IDPOW_POWERBAR_T); //Deletes the power bar. -Subsentient
	godMode = true;
	revealAll(selectedPlayer);
	setRevealStatus(true);
	radarPermitted = true;

	if (getDebugMappingStatus()) //Disable debug mode if we are already in it. -Subsentient
	{
		sendProcessDebugMappings(false);
	}

	return 0; 
}


//Sends spectator signal to other clients, if everything is in order.

void sendSpecSignal()
{
	if (!isSpectating && (allowSpectating || !NetPlay.bComms)) //Don't let us spam spectator. -Subsentient
	{
		if (bMultiPlayer)
		{
			isSpectating = true;

			NETbeginEncode(NETgameQueue(selectedPlayer), GAME_SPECMODE); //Send the universal signal, "Make me a spectator, everyone." -Subsentient
			{
				NETuint32_t(&selectedPlayer);
			}
			NETend();
		}
	
		else
		{
			addConsoleMessage("You are not in a multiplayer game.", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
		}

	}

	else if (!isSpectating && !allowSpectating)
	{
		addConsoleMessage("Spectating is not enabled for this game.", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
	}
	else
	{
		addConsoleMessage("You are already a spectator.", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
	}
}


//Subsentient's way of handling the GAME_SPECMODE network signal.

void recvSpecSignal(NETQUEUE queue) {

	DROID *psCDroid, *psNDroid;
	STRUCTURE *psCStruct, *psNStruct;
	WZ_THREAD *minimapThread = NULL;
	int otherGuy;
	uint32_t newSpec;
	char specmsg[256];

	NETbeginDecode(queue, GAME_SPECMODE); //Figure out who dun' it. -Subsentient
	{
		NETuint32_t(&newSpec);
	}
	NETend();

	if (newSpec != queue.index) //Someone is pulling a funky, sender and new spectator don't match.
	{
		debug(LOG_ERROR, "Player %d (%s) has attempted to spectate player %d! This is probably cheating.", queue.index, getPlayerName(queue.index), newSpec);
		if (NetPlay.isHost)
		{
			debug(LOG_INFO, "Kicking player %d for attempting to cheat with spectator calls.", queue.index);
			kickPlayer(queue.index, "Trying to cheat and remove people from the game isn't polite.", ERROR_KICKED);
		}
		return; //Now that we did what we should, we exit the function and don't carry out the order. -Subsentient
	}

	if (NetPlay.isHost && !allowSpectating && NetPlay.bComms) //Someone is trying to spectate in a game that doesn't permit it.
	{
		debug(LOG_INFO, "Player %d has sent a request to spectate, but spectating is disabled. That can only mean trying to cheat. Kicking.", queue.index);
		kickPlayer(queue.index, "You modified the game to send a spectator signal, so you can probably do other nasty things. Bye.", ERROR_KICKED);
		return;
	}

	NetPlay.players[newSpec].spectating = true; //The player is now spectating. Tell the netcode that. -Subsentient

	for(psCDroid=apsDroidLists[newSpec]; psCDroid; psCDroid=psNDroid) //Destroy all droids for the new spectator. -Subsentient
	{
		psNDroid = psCDroid->psNext;
		destroyDroid(psCDroid, gameTime);
	}

	for(psCStruct=apsStructLists[newSpec]; psCStruct; psCStruct=psNStruct) //Now for structures. -Subsentient
	{
		psNStruct = psCStruct->psNext;
		turnOffMultiMsg(true);
		destroyStruct(psCStruct, gameTime);
		turnOffMultiMsg(false);
	}

	for (otherGuy = 0; otherGuy < MAX_PLAYERS; otherGuy++) //Breaks alliances with everyone. -Subsentient
	{
		if (otherGuy != newSpec)
		{
			breakAlliance(newSpec, otherGuy, true, false);
		}
	}

	if (isSpectating && newSpec == selectedPlayer) //If we are the target, start our thread to initialize the spectator features. -Subsentient
	{
		minimapThread = wzThreadCreate(specThread, NULL);
		wzThreadStart(minimapThread);
		addConsoleMessage("You have entered spectator mode.", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
	}

	else 
	{
		blockDebug = true;
		strcpy(specmsg, getPlayerName(newSpec));
		strcat(specmsg, " is now a spectator.");
		addConsoleMessage(specmsg, DEFAULT_JUSTIFY, SYSTEM_MESSAGE); 
	}

}

