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
 * Loop.c
 *
 * The main game loop
 *
 */
#include "lib/framework/frame.h"
#include "lib/framework/input.h"
#include "lib/framework/strres.h"

#include "lib/ivis_common/rendmode.h"
#include "lib/ivis_common/piestate.h" //ivis render code
#include "lib/ivis_common/piemode.h"
// FIXME Direct iVis implementation include!
#include "lib/ivis_common/rendmode.h" //ivis render code
#include "lib/ivis_opengl/screen.h"

#include "lib/gamelib/gtime.h"
#include "lib/gamelib/animobj.h"
#include "lib/script/script.h"
#include "lib/sound/audio.h"
#include "lib/sound/cdaudio.h"
#include "lib/sound/mixer.h"
#include "lib/netplay/netplay.h"

#include "loop.h"
#include "objects.h"
#include "display.h"
#include "map.h"
#include "hci.h"
#include "ingameop.h"
#include "miscimd.h"
#include "effects.h"
#include "radar.h"
#include "projectile.h"
#include "console.h"
#include "power.h"
#include "message.h"
#include "bucket3d.h"
#include "display3d.h"
#include "warzoneconfig.h"

#include "multiplay.h" //ajl
#include "scripttabs.h"
#include "levels.h"
#include "visibility.h"
#include "multimenu.h"
#include "intelmap.h"
#include "loadsave.h"
#include "game.h"
#include "multijoin.h"
#include "lighting.h"
#include "intimage.h"
#include "lib/framework/cursors.h"
#include "seqdisp.h"
#include "mission.h"
#include "warcam.h"
#include "lighting.h"
#include "mapgrid.h"
#include "edit3d.h"
#include "drive.h"
#include "target.h"
#include "fpath.h"
#include "scriptextern.h"
#include "cluster.h"
#include "cmddroid.h"
#include "keybind.h"
#include "wrappers.h"

#include "warzoneconfig.h"

#ifdef DEBUG
#include "objmem.h"
#endif

static void fireWaitingCallbacks(void);

/*
 * Global variables
 */
unsigned int loopPieCount;
unsigned int loopTileCount;
unsigned int loopPolyCount;
unsigned int loopStateChanges;

/*
 * local variables
 */
static BOOL paused = false;
static BOOL video = false;
static BOOL bQuitVideo = false;

//holds which pause is valid at any one time
typedef struct _pause_state
{
	unsigned gameUpdatePause	: 1;
	unsigned audioPause			: 1;
	unsigned scriptPause		: 1;
	unsigned scrollPause		: 1;
	unsigned consolePause		: 1;
	unsigned editPause		: 1;
} PAUSE_STATE;

static PAUSE_STATE	pauseState;
static	uint32_t	numDroids[MAX_PLAYERS];
static	uint32_t	numMissionDroids[MAX_PLAYERS];
static	uint32_t	numTransporterDroids[MAX_PLAYERS];
static	uint32_t	numCommandDroids[MAX_PLAYERS];
static	uint32_t	numConstructorDroids[MAX_PLAYERS];

static int32_t videoMode = 0;

LOOP_MISSION_STATE		loopMissionState = LMS_NORMAL;

// this is set by scrStartMission to say what type of new level is to be started
int32_t	nextMissionType = LDS_NONE;//MISSION_NONE;

/* Force 3D display */
uint32_t	mcTime;

/* The main game loop */
GAMECODE gameLoop(void)
{
	DROID		*psCurr, *psNext;
	STRUCTURE	*psCBuilding, *psNBuilding;
	FEATURE		*psCFeat, *psNFeat;
	uint32_t		i, widgval;
	BOOL		quitting = false;
	INT_RETVAL	intRetVal;
	int			clearMode = 0;

	if (bMultiPlayer && !NetPlay.isHostAlive && NetPlay.bComms && !NetPlay.isHost)
	{
		intAddInGamePopup();
	}

	if (!war_GetFog())
	{
		PIELIGHT black;

		// set the fog color to black (RGB)
		// the fogbox will get this color
		black.rgba = 0;
		black.byte.a = 255;
		pie_SetFogColour(black);
	}
	if(getDrawShadows())
	{
		clearMode |= CLEAR_SHADOW;
	}
	if (loopMissionState == LMS_SAVECONTINUE)
	{
		pie_SetFogStatus(false);
		clearMode = CLEAR_BLACK;
	}
	pie_ScreenFlip(clearMode);//gameloopflip

	HandleClosingWindows();	// Needs to be done outside the pause case.

	audio_Update();

	pie_ShowMouse(true);

	if (!paused)
	{
		if (!scriptPaused() && !editPaused())
		{
			/* Update the event system */
			if (!bInTutorial)
			{
				eventProcessTriggers(gameTime / SCR_TICKRATE);
			}
			else
			{
				eventProcessTriggers(gameTime2 / SCR_TICKRATE);
			}
		}

		/* Run the in game interface and see if it grabbed any mouse clicks */
		if (!rotActive
				&& getWidgetsStatus()
				&& dragBox3D.status != DRAG_DRAGGING
				&& wallDrag.status != DRAG_DRAGGING)
		{
			intRetVal = intRunWidgets();
		}
		else
		{
			intRetVal = INT_NONE;
		}

		//don't process the object lists if paused or about to quit to the front end
		if (!(gameUpdatePaused() || intRetVal == INT_QUIT))
		{
			if( dragBox3D.status != DRAG_DRAGGING
					&& wallDrag.status != DRAG_DRAGGING
					&& ( intRetVal == INT_INTERCEPT
						 || ( radarOnScreen
							  && CoordInRadar(mouseX(), mouseY())
							  && getHQExists(selectedPlayer) ) ) )
			{
				// Using software cursors (when on) for these menus due to a bug in SDL's SDL_ShowCursor()
				pie_SetMouse(CURSOR_DEFAULT, war_GetColouredCursor());

				intRetVal = INT_INTERCEPT;
			}

#ifdef DEBUG
			// check all flag positions for duplicate delivery points
			checkFactoryFlags();
#endif
			if (!editPaused())
			{
				// Update abandoned structures
				handleAbandonedStructures();
			}

			//handles callbacks for positioning of DP's
			process3DBuilding();

			// Update the base movement stuff
			moveUpdateBaseSpeed();

			// Update the visibility change stuff
			visUpdateLevel();

			// do the grid garbage collection
			gridGarbageCollect();

			if (!editPaused())
			{
				//update the findpath system
				fpathUpdate();
			}

			// update the cluster system
			clusterUpdate();

			if (!editPaused())
			{
				// update the command droids
				cmdDroidUpdate();
				if(getDrivingStatus())
				{
					driveUpdate();
				}
			}

			//ajl. get the incoming netgame messages and process them.
			if (bMultiPlayer)
			{
				multiPlayerLoop();
			}

			if (!editPaused())
			{

				fireWaitingCallbacks(); //Now is the good time to fire waiting callbacks (since interpreter is off now)

				for(i = 0; i < MAX_PLAYERS; i++)
				{
					//update the current power available for a player
					updatePlayerPower(i);

					//set the flag for each player
					setHQExists(false, i);
					setSatUplinkExists(false, i);

					numCommandDroids[i] = 0;
					numConstructorDroids[i] = 0;
					numDroids[i] = 0;
					numTransporterDroids[i] = 0;

					for(psCurr = apsDroidLists[i]; psCurr; psCurr = psNext)
					{
						/* Copy the next pointer - not 100% sure if the droid could get destroyed
						but this covers us anyway */
						psNext = psCurr->psNext;
						droidUpdate(psCurr);

						// update the droid counts
						numDroids[i]++;
						switch (psCurr->droidType)
						{
							case DROID_COMMAND:
								numCommandDroids[i] += 1;
								break;
							case DROID_CONSTRUCT:
							case DROID_CYBORG_CONSTRUCT:
								numConstructorDroids[i] += 1;
								break;
							case DROID_TRANSPORTER:
								if( (psCurr->psGroup != NULL) )
								{
									DROID *psDroid = NULL;

									numTransporterDroids[i] += psCurr->psGroup->refCount - 1;
									// and count the units inside it...
									for (psDroid = psCurr->psGroup->psList; psDroid != NULL && psDroid != psCurr; psDroid = psDroid->psGrpNext)
									{
										if (psDroid->droidType == DROID_CYBORG_CONSTRUCT || psDroid->droidType == DROID_CONSTRUCT)
										{
											numConstructorDroids[i] += 1;
										}
										if (psDroid->droidType == DROID_COMMAND)
										{
											numCommandDroids[i] += 1;
										}
									}
								}
								break;
							default:
								break;
						}
					}

					numMissionDroids[i] = 0;
					for(psCurr = mission.apsDroidLists[i]; psCurr; psCurr = psNext)
					{
						/* Copy the next pointer - not 100% sure if the droid could
						get destroyed but this covers us anyway */
						psNext = psCurr->psNext;
						missionDroidUpdate(psCurr);
						numMissionDroids[i]++;
						switch (psCurr->droidType)
						{
							case DROID_COMMAND:
								numCommandDroids[i] += 1;
								break;
							case DROID_CONSTRUCT:
							case DROID_CYBORG_CONSTRUCT:
								numConstructorDroids[i] += 1;
								break;
							case DROID_TRANSPORTER:
								if( (psCurr->psGroup != NULL) )
								{
									numTransporterDroids[i] += psCurr->psGroup->refCount - 1;
								}
								break;
							default:
								break;
						}
					}
					for(psCurr = apsLimboDroids[i]; psCurr; psCurr = psNext)
					{
						/* Copy the next pointer - not 100% sure if the droid could
						get destroyed but this covers us anyway */
						psNext = psCurr->psNext;

						// count the type of units
						switch (psCurr->droidType)
						{
							case DROID_COMMAND:
								numCommandDroids[i] += 1;
								break;
							case DROID_CONSTRUCT:
							case DROID_CYBORG_CONSTRUCT:
								numConstructorDroids[i] += 1;
								break;
							default:
								break;
						}
					}

					// FIXME: These for-loops are code duplicationo
					/*set this up AFTER droidUpdate so that if trying to building a
					new one, we know whether one exists already*/
					setLasSatExists(false, i);
					for (psCBuilding = apsStructLists[i]; psCBuilding; psCBuilding = psNBuilding)
					{
						/* Copy the next pointer - not 100% sure if the structure could get destroyed but this covers us anyway */
						psNBuilding = psCBuilding->psNext;
						structureUpdate(psCBuilding);
						//set animation flag
						if (psCBuilding->pStructureType->type == REF_HQ &&
								psCBuilding->status == SS_BUILT)
						{
							setHQExists(true, i);
						}
						if (psCBuilding->pStructureType->type == REF_SAT_UPLINK &&
								psCBuilding->status == SS_BUILT)
						{
							setSatUplinkExists(true, i);
						}
						//don't wait for the Las Sat to be built - can't build another if one is partially built
						if (asWeaponStats[psCBuilding->asWeaps[0].nStat].
								weaponSubClass == WSC_LAS_SAT)
						{
							setLasSatExists(true, i);
						}
					}
					for (psCBuilding = mission.apsStructLists[i]; psCBuilding;
							psCBuilding = psNBuilding)
					{
						/* Copy the next pointer - not 100% sure if the structure could get destroyed but this covers us anyway. It shouldn't do since its not even on the map!*/
						psNBuilding = psCBuilding->psNext;
						missionStructureUpdate(psCBuilding);
						if (psCBuilding->pStructureType->type == REF_HQ &&
								psCBuilding->status == SS_BUILT)
						{
							setHQExists(true, i);
						}
						if (psCBuilding->pStructureType->type == REF_SAT_UPLINK &&
								psCBuilding->status == SS_BUILT)
						{
							setSatUplinkExists(true, i);
						}
						//don't wait for the Las Sat to be built - can't build another if one is partially built
						if (asWeaponStats[psCBuilding->asWeaps[0].nStat].
								weaponSubClass == WSC_LAS_SAT)
						{
							setLasSatExists(true, i);
						}
					}
				}

				missionTimerUpdate();

				proj_UpdateAll();

				for(psCFeat = apsFeatureLists[0]; psCFeat; psCFeat = psNFeat)
				{
					psNFeat = psCFeat->psNext;
					featureUpdate(psCFeat);
				}

			}
			else // if editPaused()
			{
				for (i = 0; i < MAX_PLAYERS; i++)
				{
					for(psCurr = apsDroidLists[i]; psCurr; psCurr = psNext)
					{
						/* Copy the next pointer - not 100% sure if the droid could get destroyed
						but this covers us anyway */
						psNext = psCurr->psNext;
						processVisibility((BASE_OBJECT *)psCurr);
						calcDroidIllumination(psCurr);
					}
					for (psCBuilding = apsStructLists[i]; psCBuilding; psCBuilding = psNBuilding)
					{
						/* Copy the next pointer - not 100% sure if the structure could get destroyed but this covers us anyway */
						psNBuilding = psCBuilding->psNext;
						processVisibility((BASE_OBJECT *)psCBuilding);
					}
				}
			}

			/* update animations */
			animObj_Update();

			objmemUpdate();
		}
		if (!consolePaused())
		{
			/* Process all the console messages */
			updateConsoleMessages();
		}
		if (!scrollPaused() && !getWarCamStatus() && dragBox3D.status != DRAG_DRAGGING && intMode != INT_INGAMEOP )
		{
			scroll();
		}
	}
	else // paused
	{
		// Using software cursors (when on) for these menus due to a bug in SDL's SDL_ShowCursor()
		pie_SetMouse(CURSOR_DEFAULT, war_GetColouredCursor());

		intRetVal = INT_NONE;
		if (loop_GetVideoStatus())
		{
			bQuitVideo = !seq_UpdateFullScreenVideo(NULL);
		}

		if(dragBox3D.status != DRAG_DRAGGING)
		{
			scroll();
		}

		if(InGameOpUp || isInGamePopupUp)		// ingame options menu up, run it!
		{
			widgval = widgRunScreen(psWScreen);
			intProcessInGameOptions(widgval);
			if(widgval == INTINGAMEOP_QUIT_CONFIRM || widgval == INTINGAMEOP_POPUP_QUIT)
			{
				if(gamePaused())
				{
					kf_TogglePauseMode();
				}
				intRetVal = INT_QUIT;
			}
		}

		if(bLoadSaveUp && runLoadSave(true) && strlen(sRequestResult))
		{
			debug( LOG_NEVER, "Returned %s", sRequestResult );
			if(bRequestLoad)
			{
				loopMissionState = LMS_LOADGAME;
				NET_InitPlayers();			// otherwise alliances were not cleared
				sstrcpy(saveGameName, sRequestResult);
			}
			else
			{
				char msgbuffer[256] = {'\0'};

				if (saveInMissionRes())
				{
					if (saveGame(sRequestResult, GTYPE_SAVE_START))
					{
						sstrcpy(msgbuffer, _("GAME SAVED :"));
						sstrcat(msgbuffer, sRequestResult);
						addConsoleMessage( msgbuffer, LEFT_JUSTIFY, NOTIFY_MESSAGE);
					}
					else
					{
						ASSERT( false, "Mission Results: saveGame Failed" );
						sstrcpy(msgbuffer, _("Could not save game!"));
						addConsoleMessage( msgbuffer, LEFT_JUSTIFY, NOTIFY_MESSAGE);
						deleteSaveGame(sRequestResult);
					}
				}
				else if (bMultiPlayer || saveMidMission())
				{
					if (saveGame(sRequestResult, GTYPE_SAVE_MIDMISSION))//mid mission from [esc] menu
					{
						sstrcpy(msgbuffer, _("GAME SAVED :"));
						sstrcat(msgbuffer, sRequestResult);
						addConsoleMessage( msgbuffer, LEFT_JUSTIFY, NOTIFY_MESSAGE);
					}
					else
					{
						ASSERT(!"saveGame(sRequestResult, GTYPE_SAVE_MIDMISSION) failed", "Mid Mission: saveGame Failed" );
						sstrcpy(msgbuffer, _("Could not save game!"));
						addConsoleMessage( msgbuffer, LEFT_JUSTIFY, NOTIFY_MESSAGE);
						deleteSaveGame(sRequestResult);
					}
				}
				else
				{
					ASSERT( false, "Attempt to save game with incorrect load/save mode" );
				}
			}
		}
	}

	/* Check for quit */
	if (intRetVal == INT_QUIT)
	{
		if (!loop_GetVideoStatus())
		{
			//quitting from the game to the front end
			//so get a new backdrop
			quitting = true;

			pie_LoadBackDrop(SCREEN_RANDOMBDROP);
		}
		else //if in video mode esc kill video
		{
			bQuitVideo = true;
		}
	}
	if (!loop_GetVideoStatus() && !quitting)
	{
		if (!gameUpdatePaused())
		{
			if (dragBox3D.status != DRAG_DRAGGING
					&& wallDrag.status != DRAG_DRAGGING)
			{
				ProcessRadarInput();
			}
			processInput();

			//no key clicks or in Intelligence Screen
			if (intRetVal == INT_NONE && !InGameOpUp && !isInGamePopupUp)
			{
				processMouseClickInput();
			}
			displayWorld();
		}
		/* Display the in game interface */
		pie_SetDepthBufferStatus(DEPTH_CMP_ALWAYS_WRT_ON);
		pie_SetFogStatus(false);

		if(bMultiPlayer && bDisplayMultiJoiningStatus)
		{
			intDisplayMultiJoiningStatus(bDisplayMultiJoiningStatus);
			setWidgetsStatus(false);
		}

		if(getWidgetsStatus())
		{
			intDisplayWidgets();
		}
		pie_SetDepthBufferStatus(DEPTH_CMP_LEQ_WRT_ON);
		pie_SetFogStatus(true);

		pie_DrawMouse(mouseX(), mouseY());
	}

	/* Check for toggling video playbackmode */
	if (bQuitVideo && loop_GetVideoStatus())
	{
		seq_StopFullScreenVideo();
		bQuitVideo = false;
	}

	pie_GetResetCounts(&loopPieCount, &loopTileCount, &loopPolyCount, &loopStateChanges);

	if (fogStatus & FOG_BACKGROUND)
	{
		if (loopMissionState == LMS_SAVECONTINUE)
		{
			pie_SetFogStatus(false);
			clearMode = CLEAR_BLACK;
		}
	}
	else
	{
		clearMode = CLEAR_BLACK;//force to black 3DFX
	}

	if (!quitting)
	{
		/* Check for toggling display mode */
		if ((keyDown(KEY_LALT) || keyDown(KEY_RALT)) && keyPressed(KEY_RETURN))
		{
			screenToggleMode();
		}
	}

	// deal with the mission state
	switch (loopMissionState)
	{
		case LMS_CLEAROBJECTS:
			missionDestroyObjects();
			setScriptPause(true);
			loopMissionState = LMS_SETUPMISSION;
			break;

		case LMS_NORMAL:
			// default
			break;
		case LMS_SETUPMISSION:
			setScriptPause(false);
			if (!setUpMission(nextMissionType))
			{
				return GAMECODE_QUITGAME;
			}
			break;
		case LMS_SAVECONTINUE:
			// just wait for this to be changed when the new mission starts
			clearMode = CLEAR_BLACK;
			break;
		case LMS_NEWLEVEL:
			//nextMissionType = MISSION_NONE;
			nextMissionType = LDS_NONE;
			return GAMECODE_NEWLEVEL;
			break;
		case LMS_LOADGAME:
			return GAMECODE_LOADGAME;
			break;
		default:
			ASSERT( false, "unknown loopMissionState" );
			break;
	}

	if (quitting)
	{
		pie_SetFogStatus(false);
		pie_ScreenFlip(CLEAR_BLACK);//gameloopflip
		/* Check for toggling display mode */
		if ((keyDown(KEY_LALT) || keyDown(KEY_RALT)) && keyPressed(KEY_RETURN))
		{
			screenToggleMode();
		}
		return GAMECODE_QUITGAME;
	}
	else if (loop_GetVideoStatus())
	{
		audio_StopAll();
		return GAMECODE_PLAYVIDEO;
	}

	return GAMECODE_CONTINUE;
}

/* The video playback loop */
void videoLoop(void)
{
	static BOOL bActiveBackDrop = false;
	const BOOL userSkip = keyPressed(KEY_ESC) || mouseReleased(MOUSE_LMB);

	if (screen_GetBackDrop())
	{
		bActiveBackDrop = true;
		screen_StopBackDrop();
	}
	else
	{
		bActiveBackDrop = false;
	}

	if (loop_GetVideoStatus())
	{
		bQuitVideo = !seq_UpdateFullScreenVideo(NULL);
	}

	pie_ScreenFlip(CLEAR_BLACK); // videoloopflip

	// if the video finished or user is skipping the video
	if (bQuitVideo || userSkip)
	{
		seq_StopFullScreenVideo();
		bQuitVideo = false;

		// set the next video off - if any - if the user isn't skipping
		if (seq_AnySeqLeft() && !userSkip)
		{
			seq_StartNextFullScreenVideo();
		}
		else
		{
			if (bActiveBackDrop)
			{
				screen_RestartBackDrop();
			}
			// remove the intelligence screen if necessary
			if (messageIsImmediate())
			{
				intResetScreen(true);
				setMessageImmediate(false);
			}
			//don't do the callback if we're playing the win/lose video
			if (!getScriptWinLoseVideo())
			{
				eventFireCallbackTrigger((TRIGGER_TYPE)CALL_VIDEO_QUIT);
			}
			else
			{
				displayGameOver(getScriptWinLoseVideo() == PLAY_WIN);
			}
		}
	}
}


void loop_SetVideoPlaybackMode(void)
{
	videoMode += 1;
	paused = true;
	video = true;
	gameTimeStop();
	pie_SetFogStatus(false);
	audio_StopAll();
	pie_ShowMouse(false);
}


void loop_ClearVideoPlaybackMode(void)
{
	videoMode -= 1;
	paused = false;
	video = false;
	gameTimeStart();
	pie_SetFogStatus(true);
	cdAudio_Resume();
	pie_ShowMouse(true);
	ASSERT( videoMode == 0, "loop_ClearVideoPlaybackMode: out of sync." );
}


int32_t loop_GetVideoMode(void)
{
	return videoMode;
}

BOOL loop_GetVideoStatus(void)
{
	return video;
}

BOOL editPaused(void)
{
	return pauseState.editPause;
}

void setEditPause(bool state)
{
	pauseState.editPause = state;
}

BOOL gamePaused( void )
{
	return paused;
}

void setGamePauseStatus( BOOL val )
{
	paused = val;
}

BOOL gameUpdatePaused(void)
{
	return pauseState.gameUpdatePause;
}
BOOL audioPaused(void)
{
	return pauseState.audioPause;
}
BOOL scriptPaused(void)
{
	return pauseState.scriptPause;
}
BOOL scrollPaused(void)
{
	return pauseState.scrollPause;
}
BOOL consolePaused(void)
{
	return pauseState.consolePause;
}

void setGameUpdatePause(BOOL state)
{
	pauseState.gameUpdatePause = state;
	if (state)
	{
		screen_RestartBackDrop();
	}
	else
	{
		screen_StopBackDrop();
	}
}
void setAudioPause(BOOL state)
{
	pauseState.audioPause = state;
}
void setScriptPause(BOOL state)
{
	pauseState.scriptPause = state;
}
void setScrollPause(BOOL state)
{
	pauseState.scrollPause = state;
}
void setConsolePause(BOOL state)
{
	pauseState.consolePause = state;
}

//set all the pause states to the state value
void setAllPauseStates(BOOL state)
{
	setGameUpdatePause(state);
	setAudioPause(state);
	setScriptPause(state);
	setScrollPause(state);
	setConsolePause(state);
}

uint32_t	getNumDroids(uint32_t player)
{
	return(numDroids[player]);
}

uint32_t	getNumTransporterDroids(uint32_t player)
{
	return(numTransporterDroids[player]);
}

uint32_t	getNumMissionDroids(uint32_t player)
{
	return(numMissionDroids[player]);
}

uint32_t	getNumCommandDroids(uint32_t player)
{
	return numCommandDroids[player];
}

uint32_t	getNumConstructorDroids(uint32_t player)
{
	return numConstructorDroids[player];
}


// increase the droid counts - used by update factory to keep the counts in sync
void incNumDroids(uint32_t player)
{
	numDroids[player] += 1;
}
void incNumCommandDroids(uint32_t player)
{
	numCommandDroids[player] += 1;
}
void incNumConstructorDroids(uint32_t player)
{
	numConstructorDroids[player] += 1;
}

/* Fire waiting beacon messages which we couldn't run before */
static void fireWaitingCallbacks(void)
{
	BOOL bOK = true;

	while(!isMsgStackEmpty() && bOK)
	{
		bOK = msgStackFireTop();
		if(!bOK)
		{
			ASSERT(false, "fireWaitingCallbacks: msgStackFireTop() failed (stack count: %d)", msgStackGetCount());
		}
	}
}
