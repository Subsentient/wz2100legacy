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
 * clParse.c
 *
 * Parse command line arguments
 *
 */
 
/**This file is inferior to the previous implementation that provided popt, but it will suffice.**/

#include "lib/framework/frame.h"
#include "lib/netplay/netplay.h"

#include "clparse.h"
#include "display3d.h"
#include "frontend.h"
#include "keybind.h"
#include "loadsave.h"
#include "main.h"
#include "multiplay.h"
#include "version.h"
#include "warzoneconfig.h"
#include "wrappers.h"

//! Let the end user into debug mode....
BOOL	bAllowDebugMode = false;

typedef enum
{
	// We don't want to use zero, so start at one (1)
	CLI_CHEAT = 1,
	CLI_CONFIGDIR,
	CLI_DATADIR,
	CLI_DEBUG,
	CLI_DEBUGFILE,
	CLI_FLUSHDEBUGSTDERR,
	CLI_FULLSCREEN,
	CLI_GAME,
	CLI_HELP,
	CLI_MOD_GLOB,
	CLI_MOD_CA,
	CLI_MOD_MP,
	CLI_SAVEGAME,
	CLI_WINDOW,
	CLI_MASTERSERVER,
	CLI_MASTERSERVER_PORT,
	CLI_VERSION,
	CLI_RESOLUTION,
	CLI_SHADOWS,
	CLI_NOSHADOWS,
	CLI_SOUND,
	CLI_NOSOUND,
	CLI_SELFTEST,
	CLI_CONNECTTOIP,
	CLI_HOSTLAUNCH,
	CLI_NOASSERT,
	CLI_CRASH,
	CLI_TEXTURECOMPRESSION,
	CLI_NOTEXTURECOMPRESSION
} CLI_OPTIONS;

static CLISPEC optionsTable[] =
{
	{ "--cheat",      LARG_NONE,   CLI_CHEAT,      N_("Run in cheat mode"),                 NULL },
	{ "--datadir",    LARG_STRING, CLI_DATADIR,    N_("Set default data directory"),        N_("data directory") },
	{ "--configdir",  LARG_STRING, CLI_CONFIGDIR,  N_("Set configuration directory"),       N_("configuration directory") },
	{ "--debug",      LARG_STRING, CLI_DEBUG,      N_("Show debug for given level"),        N_("debug level") },
	{ "--debugfile",  LARG_STRING, CLI_DEBUGFILE,  N_("Log debug output to file"),          N_("file") },
	{ "--flush-debug-stderr", LARG_NONE, CLI_FLUSHDEBUGSTDERR, N_("Flush all debug output written to stderr"), NULL },
	{ "--fullscreen", LARG_NONE,   CLI_FULLSCREEN, N_("Play in fullscreen mode"),           NULL },
	{ "--game",       LARG_STRING, CLI_GAME,       N_("Load a specific game"),              N_("game-name") },
	{ "--help",       LARG_NONE,   CLI_HELP,       N_("Show this help message and exit"),   NULL },
	{ "--mod",        LARG_STRING, CLI_MOD_GLOB,   N_("Enable a global mod"),               N_("mod") },
	{ "--mod_ca",     LARG_STRING, CLI_MOD_CA,     N_("Enable a campaign only mod"),        N_("mod") },
	{ "--mod_mp",     LARG_STRING, CLI_MOD_MP,     N_("Enable a multiplay only mod"),       N_("mod") },
	{ "--noassert",	LARG_NONE,   CLI_NOASSERT,   N_("Disable asserts"),                   NULL },
	{ "--crash",		LARG_NONE,   CLI_CRASH,      N_("Causes a crash to test the crash handler"), NULL },
	{ "--savegame",   LARG_STRING, CLI_SAVEGAME,   N_("Load a saved game"),                 N_("savegame") },
	{ "--window",     LARG_NONE,   CLI_WINDOW,     N_("Play in windowed mode"),             NULL },
	{ "--masterserver",LARG_STRING, CLI_MASTERSERVER, N_("Set IP or domain to use for the lobby server."), N_("masterserver") },
	{ "--masterserver-port",LARG_STRING, CLI_MASTERSERVER_PORT, N_("Set port number for the lobby server. Default is 9990."), N_("e.g. 4444") },
	{ "--version",    LARG_NONE,   CLI_VERSION,    N_("Show version information and exit"), NULL },
	{ "--resolution", LARG_STRING, CLI_RESOLUTION, N_("Set the resolution to use"),         N_("WIDTHxHEIGHT") },
	{ "--shadows",    LARG_NONE,   CLI_SHADOWS,    N_("Enable shadows"),                    NULL },
	{ "--noshadows",  LARG_NONE,   CLI_NOSHADOWS,  N_("Disable shadows"),                   NULL },
	{ "--sound",      LARG_NONE,   CLI_SOUND,      N_("Enable sound"),                      NULL },
	{ "--nosound",    LARG_NONE,   CLI_NOSOUND,    N_("Disable sound"),                     NULL },
	{ "--selftest",   LARG_NONE,   CLI_SELFTEST,   N_("Activate self-test"),                NULL },
	{ "--join",       LARG_STRING, CLI_CONNECTTOIP,N_("connect directly to IP/hostname"),   N_("host") },
	{ "--host",       LARG_NONE,   CLI_HOSTLAUNCH, N_("go directly to host screen"),        NULL },
	{ "--texturecompression", LARG_NONE, CLI_TEXTURECOMPRESSION, N_("Enable texture compression"), NULL },
	{ "--notexturecompression", LARG_NONE, CLI_NOTEXTURECOMPRESSION, N_("Disable texture compression"), NULL },
	// Terminating entry
	{ NULL }
};

static void DumpHelpToConsole(void)
{
	unsigned long Inc = 0;
	
	
	printf("%s\n\n", GetVersionInfo());
	
	for (; optionsTable[Inc].ArgName; ++Inc)
	{
		if (optionsTable[Inc].ArgArg)
		{
			printf("%s=%s:\n\t%s\n", optionsTable[Inc].ArgName, optionsTable[Inc].Example, optionsTable[Inc].Help);
		}
		else
		{
			printf("%s:\n\t%s\n", optionsTable[Inc].ArgName, optionsTable[Inc].Help);
		}
	}
}

static CLISPEC *lookupArgument(const char *InStream)
{ /*Subsentient's replacement for libpopt.*/
	unsigned long Inc = 0;
	char ArgStream[2048] = { '\0' }; // Should be more than enough.
	
	/*Don't include the data past the equals.*/
	for (; InStream[Inc] != '\0' && InStream[Inc] != '='; ++Inc)
	{
		ArgStream[Inc] = InStream[Inc];
	}
	ArgStream[Inc] = '\0';
	
	for (Inc = 0; optionsTable[Inc].ArgName != NULL; ++Inc)
	{
		if (!strcmp(ArgStream, optionsTable[Inc].ArgName))
		{
			return &optionsTable[Inc]; //Return a pointer.
										/*I'd sure hope so, past me!*/
		}
	}
	
	return NULL;
}

static BOOL getArgumentParam(CLISPEC *ArgStruct, const char *CurArgv, void *OutPtr)
{ /*Writes the desired argument type to OutPtr.*/
	char *LookupTable = (char*)CurArgv;
	char Delim[8192];
	
	snprintf(Delim, 8192, "%s=", ArgStruct->ArgName);
	
	if (!(LookupTable = strstr(LookupTable, Delim)))
	{
		/*Use this to skip to next argument, so we don't need to pass all of argv[].
		 * I imagine this isn't perfectly safe for some implementations, but it's sure convenient.*/
		LookupTable = (char*)&CurArgv[strlen(CurArgv) + 1];
	}
	else
	{
		LookupTable += strlen(Delim); //Go past the equals.
	}
	
	switch (ArgStruct->ArgArg)
	{
		case LARG_STRING:
		{
			strcpy((char*)OutPtr, LookupTable);
			return true;
			break;
		}
		case LARG_NUMBER:
		{
			*(int*)OutPtr = atoi(LookupTable);
			return true;
			break;
		}
		case LARG_FLOAT:
		{
			*(float*)OutPtr = atof(LookupTable);
			return true;
			break;
		}
		case LARG_BOOLEAN:
		{
			if (!strcmp(LookupTable, "true"))
			{
				*(BOOL*)OutPtr = true;
				return true;
			}
			else if (!strcmp(LookupTable, "false"))
			{
				*(BOOL*)OutPtr = false;
				return true;
			}
			else
			{
				return false;
			}
			break;
		}
		case LARG_NONE:
		default:
		{
			return false;
		}
	}
	
	return false;
}

/**Command line parser. Handles everything.**/
bool ParseCommandLine(int argc, const char **argv, bool Early)
{
	short Inc = 1;
	CLISPEC *CurArg;
	char ArgRet[8192];
	
	for (; Inc < argc; ++Inc)
	{
		if (!(CurArg = lookupArgument(argv[Inc])))
		{
			if (!strncmp(argv[Inc], "--", strlen("--")))
			{ /*Lots of arguments won't have this if we omit the '='.*/
				printf("%s: invalid command line argument.\n", argv[Inc]);
				return false;
			}
			else
			{
				continue;
			}
		}
		
		if (CurArg->ArgArg != LARG_NONE)
		{
			if (argv[Inc][strlen(CurArg->ArgName)] != '=' && Inc == argc - 1)
			{
				printf("Missing option for argument \"%s\"\n", CurArg->ArgName);
				return false;
			}
			else if (argv[Inc][strlen(CurArg->ArgName)] == '=' && strlen(argv[Inc]) <= strlen(CurArg->ArgName) + 1)
			{
				printf("Empty equals sign for argument \"%s\"\n", CurArg->ArgName);
				return false;
			}
		}

		if (!Early)
		{
			switch (CurArg->ArgID)
			{
				case CLI_DEBUG:
				case CLI_DEBUGFILE:
				case CLI_FLUSHDEBUGSTDERR:
				case CLI_CONFIGDIR:
				case CLI_HELP:
				case CLI_VERSION:
					// These options are parsed in ParseCommandLineEarly() already, so ignore them
					break;
	
				case CLI_NOASSERT:
					kf_NoAssert();
					break;
	
					// NOTE: The sole purpose of this is to test the crash handler.
				case CLI_CRASH:
					CauseCrash = true;
					NetPlay.bComms = false;
					sstrcpy(aLevelName, "CAM_3A");
					SetGameMode(GS_NORMAL);
					break;
	
				case CLI_CHEAT:
					//printf("  ** DEBUG MODE UNLOCKED! **\n");
					bAllowDebugMode = true;
					break;
	
				case CLI_DATADIR:
					// retrieve the quoted path name
					if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
					{
						debug(LOG_FATAL, "Unrecognised datadir");
						
						return false;
					}
					sstrcpy(datadir, ArgRet);
					break;
	
				case CLI_FULLSCREEN:
					war_setFullscreen(true);
					break;
				case CLI_CONNECTTOIP:
					//get the ip we want to connect with, and go directly to join screen.
					if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
					{
						debug(LOG_FATAL, "No IP/hostname given");
						
						return false;
					}
					sstrcpy(iptoconnect, ArgRet);
					break;
				case CLI_HOSTLAUNCH:
					// go directly to host screen, bypass all others.
					hostlaunch = true;
					break;
				case CLI_GAME:
					// retrieve the game name
					if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
					{
						debug(LOG_POPUP, "No game name");
						
						return false;
					}
					if (strcmp(ArgRet, "CAM_1A") && strcmp(ArgRet, "CAM_2A") && strcmp(ArgRet, "CAM_3A")
							&& strcmp(ArgRet, "TUTORIAL3") && strcmp(ArgRet, "FASTPLAY"))
					{
						debug(LOG_FATAL, "The game parameter requires one of the following keywords:");
						debug(LOG_FATAL, "CAM_1A, CAM_2A, CAM_3A, TUTORIAL3, or FASTPLAY");
						return false;
					}
					NetPlay.bComms = false;
					bMultiPlayer = false;
					bMultiMessages = false;
					NetPlay.players[0].allocated = true;
					if (strcmp(ArgRet, "CAM_1A") && strcmp(ArgRet, "CAM_2A") && strcmp(ArgRet, "CAM_3A"))
					{
						game.type = CAMPAIGN;
					}
					else
					{
						game.type = SKIRMISH; // tutorial is skirmish for some reason
					}
					sstrcpy(aLevelName, ArgRet);
					SetGameMode(GS_NORMAL);
					break;
				case CLI_MOD_GLOB:
					{
						unsigned int i;
	
						// retrieve the file name
						if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
						{
							debug(LOG_FATAL, "Missing mod name?");
							
							return false;
						}
	
						// Find an empty place in the global_mods list
						for (i = 0; i < 100 && global_mods[i] != NULL; ++i);
						if (i >= 100 || global_mods[i] != NULL)
						{
							debug(LOG_FATAL, "Too many mods registered! Aborting!");
							
							return false;
						}
						global_mods[i] = strdup(ArgRet);
						break;
					}
				case CLI_MOD_CA:
					{
						unsigned int i;
	
						// retrieve the file name
						if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
						{
							debug(LOG_FATAL, "Missing mod name?");
							
							return false;
						}
	
						// Find an empty place in the campaign_mods list
						for (i = 0; i < 100 && campaign_mods[i] != NULL; ++i);
						if (i >= 100 || campaign_mods[i] != NULL)
						{
							debug(LOG_FATAL, "Too many mods registered! Aborting!");
							
							return false;
						}
						campaign_mods[i] = strdup(ArgRet);
						break;
					}
				case CLI_MOD_MP:
					{
						unsigned int i;
	
						// retrieve the file name
						if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
						{
							debug(LOG_FATAL, "Missing mod name?");
							
							return false;
						}
	
						for (i = 0; i < 100 && multiplay_mods[i] != NULL; ++i);
						if (i >= 100 || multiplay_mods[i] != NULL)
						{
							debug(LOG_FATAL, "Too many mods registered! Aborting!");
							
							return false;
						}
						multiplay_mods[i] = strdup(ArgRet);
						break;
					}
				case CLI_RESOLUTION:
					{
						unsigned int width, height;
	
						if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
						{
							debug(LOG_FATAL, "You must specify a proper resolution.");
							return false;
						}
						
						if (sscanf(ArgRet, "%ix%i", &width, &height ) != 2 )
						{
							debug(LOG_FATAL, "Invalid parameter specified (format is WIDTHxHEIGHT, e.g. 800x600)");
							return false;
						}
						if (width < 640)
						{
							debug(LOG_POPUP, "Screen width < 640 unsupported, using 640");
							width = 640;
						}
						if (height < 480)
						{
							debug(LOG_POPUP, "Screen height < 480 unsupported, using 480");
							height = 480;
						}
						// tell the display system of the desired resolution
						pie_SetVideoBufferWidth(width);
						pie_SetVideoBufferHeight(height);
						// and update the configuration
						war_SetWidth(width);
						war_SetHeight(height);
						break;
					}
				case CLI_SAVEGAME:
					// retrieve the game name
					if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
					{
						debug(LOG_POPUP, "Unrecognised savegame name");
						
						return false;
					}
					snprintf(saveGameName, sizeof(saveGameName), "%s/%s", SaveGamePath, ArgRet);
					SetGameMode(GS_SAVEGAMELOAD);
					break;
	
				case CLI_WINDOW:
					war_setFullscreen(false);
					break;
	
				case CLI_SHADOWS:
					setDrawShadows(true);
					break;
	
				case CLI_NOSHADOWS:
					setDrawShadows(false);
					break;
	
				case CLI_SOUND:
					war_setSoundEnabled(true);
					break;
	
				case CLI_NOSOUND:
					war_setSoundEnabled(false);
					break;
					
				case CLI_TEXTURECOMPRESSION:
					debug(LOG_INFO, "Enabling texture compression.");
					war_SetTextureCompression(true);
					break;
	
				case CLI_NOTEXTURECOMPRESSION:
					debug(LOG_INFO, "Disabling texture compression.");
					war_SetTextureCompression(false);
					break;
	
				case CLI_SELFTEST:
					selfTest = true;
					break;
				case CLI_MASTERSERVER:
					{
						//Allow us to set the lobby server from the CLI.
						const char *ServerVictory = "Using %s as masterserver.\n";
						char OutMsg[512];
	
						if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
						{
							puts("Bad lobby server IP or domain provided.");
							return false;
						}
	
						snprintf(OutMsg, 512, ServerVictory, ArgRet);
						debug(LOG_INFO, OutMsg);
	
						NETsetMasterserverName(ArgRet);
						break;
					}
				case CLI_MASTERSERVER_PORT:
					{
						const char *PortVictory = "Using %s as masterserver port number.\n";
						char OutMsg[512];
						/* ^ This is OK since it's at the beginning of a block. ^
						 * -Subsentient*/
	
						//Allow us to set the lobby server port as well.
						if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
						{
							puts("Bad lobby server port provided.");
							return false;
						}
	
						snprintf(OutMsg, 512, PortVictory, ArgRet);
						debug(LOG_INFO, OutMsg);
	
						NETsetMasterserverPort(atoi(ArgRet));
						break;
					}
			}
		}
		else
		{	
			switch (CurArg->ArgID)
			{
				case CLI_DEBUG:
					// retrieve the debug section name
					
					if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
					{
						debug(LOG_FATAL, "Usage: --debug <flag>");
						
						return false;
					}
	
					// Attempt to enable the given debug section
					if (!debug_enable_switch(ArgRet))
					{
						debug(LOG_FATAL, "Debug flag \"%s\" not found!", ArgRet);
						
						return false;
					}
					break;
	
				case CLI_DEBUGFILE:
					// find the file name
					if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
					{
						debug(LOG_FATAL, "Missing debugfile filename?");
						
						return false;
					}
					debug_register_callback( debug_callback_file, debug_callback_file_init, debug_callback_file_exit, (void *)ArgRet );
					customDebugfile = true;
					break;
	
				case CLI_FLUSHDEBUGSTDERR:
					// Tell the debug stderr output callback to always flush its output
					debugFlushStderr();
					break;
	
				case CLI_CONFIGDIR:
					// retrieve the configuration directory
					if (!getArgumentParam(CurArg, argv[Inc], ArgRet))
					{
						debug(LOG_FATAL, "Unrecognised configuration directory");
						
						return false;
					}
					sstrcpy(configdir, ArgRet);
					break;
	
				case CLI_HELP:
					DumpHelpToConsole();
					return false;
					break;
				case CLI_VERSION:
					printf("%s\n", GetVersionInfo());
					
					return false;
					break;
	
				default:
					break;
			}
		}	
	}

	return true;
}
