// rules.js v0.10
// Skirmish Base Script.
//
// contains the rules for starting and ending a game.
//
// /////////////////////////////////////////////////////////////////

/* Version history

Based on and inspired by Shadow Wolf TJC 's Enahnced SitRep mod I decided to
rewrite rules.js from scratch, then incorporate SitRep features.

v0.1  - Original rules.js from Warzone 3.1 Beta 10 [WZ Devs]
v0.2  - Refactored player init code, split out in to rules.init.js [Aubergine]
v0.3  - Added situation reporting (SitRep) features [Aubergine based on Shadow Wolf's SitRep mod]
v0.4  - Refactored victory condition checks [Aubergine]
v0.5  - Added throttled audioAlert() function to help with SitRep sounds [Aubergine]
v0.6  - Refactored SitRep code, split out in to rules.report.js [Aubergine]
v0.7  - Lots of SitRep enhancements, eg. demolish/recycle, object transfer, etc. [Aubergine]
v0.8  - Released to http://forums.wz2100.net/viewtopic.php?f=10&t=9392 [Aubergine]
v0.9  - Fixed bug with HQ events and pcv634.ogg
v0.10 - Complete rewrite of rules.report.js, added new rules.report.msg.js, lots of code cleaning
v0.11 - Split in to includes for easier modding, now works with Warzone 3.1 RC 3

*/

// /////////////////////////////////////////////////////////////////
// STATIC INCLUDES (DO NOT MOD THESE)

// Situation reporting API: (triggered by various events in rules.js)
include("multiplay/skirmish/rules.report.js"); // report.*
// Player initialisation API: (triggered by 'eventGameInit()' in rules.js)
// (you can override rules.init.js, but I don't recommend it)
include("multiplay/skirmish/rules.init.js"); // init.*

// /////////////////////////////////////////////////////////////////
// DYNAMIC INCLUDES (MODDABLE)

// Define which players on the map are spectators
include("multiplay/skirmish/rules.spectators.js");

// Structrues to enable for all players
include("multiplay/skirmish/rules.init.enableStructures.js");
// Components to enable for all players
include("multiplay/skirmish/rules.init.enableComponents.js");
// Technologies to enable for all players
include("multiplay/skirmish/rules.init.enableTechnologies.js");
// Structure limits for all players
include("multiplay/skirmish/rules.init.structureLimits.js");
// Effect of baseType setting for all players
include("multiplay/skirmish/rules.init.baseType.js");
// Initialise localhost human player (if not a spectator)
include("multiplay/skirmish/rules.init.humanPlayer.js");
// Initialise localhost spectator player (if applicable)
include("multiplay/skirmish/rules.init.spectatorPlayer.js");

// Situation reporting taxonomy (report definitions):
include("multiplay/skirmish/rules.report.msg.js"); // report.msg.*
// Note: For minor mods to taxonomy, see note at end of rules.victory.js

// Victory conditions: (triggered by 'checkPlayers()' in rules.js)
include("multiplay/skirmish/rules.victory.js"); // isPlayerAlive() and checkGameOutcome()


// /////////////////////////////////////////////////////////////////
// CONSTANTS

// Make team-related code a bit easier to read:
const teams = (alliancesType == ALLIANCES_TEAMS);
// Avoid strangeness if human switches to another player (via debug menu):
const humanPlayer = selectedPlayer;

// /////////////////////////////////////////////////////////////////
// PLAYER INITIALISATION

function eventGameInit() {
	hackNetOff();

    // init.* functions (and associated data) are defined in rules.init.js
    // isSpectator() is from rules.spectators.js

	// Initialisation common to all players:
	var player = maxPlayers;
	while (-1<--player) {
		if (!isSpectator(player)) {
			init.enableStructuresFor(player);
			init.enableComponentsFor(player);
			init.enableTechnologiesFor(player);
			init.setLimitsFor(player);
		}
		init.applyBaseTypeFor(player);
	}
	applyLimitSet();

	// Initialisation specific to the local human player:
	if (isSpectator(me)) {
		init.spectatorPlayer();
	} else {
		init.humanPlayer();
	}
	
	hackNetOn();
}

// The game is starting...
function eventStartLevel() {
	setTimer("checkPlayers", 2501);
	report(isSpectator(humanPlayer) ? "startLevel.spectator" : "startLevel.player");
}

// /////////////////////////////////////////////////////////////////
// PLAYER STATES & GAME WIN/LOSE CONDITIONS

// isPlayerAlive() and hasGameEnded() are defined in rules.victory.js

// Keep track of players entering/leaving warzone, for situation reporting purposes
var playerLiving = [];
(function() { // initialise playerAlive array
	var player = maxPlayers;
	while (-1<--player) playerLiving[player] = isPlayerAlive(player);
})();

// Make sure human only sees GameOverMessage once, prevent spectators from ever seeing it
var gameOverShown = isSpectator(me);

// Check through each player to see if they are alive or not, then check game victory conditions
function checkPlayers() {
	var player = maxPlayers;
	var ally;
	// initialise counters
	playerLiving.ally = playerLiving.team = playerLiving.enemy = 0;

	// check status players
	while (-1<--player) {
		if (isSpectator(player)) continue; // ignore spectator players
		ally = (player == me || allianceExistsBetween(me,player));
		if (isPlayerAlive(player)) { // player alive
			if (ally) {
				playerLiving[(teams) ? "team" : "ally"] += 1;
				// If ally is low on power, and I have lots, suggest a power transfer
				// POWER_THRESHOLD is defined in rules.victory.js for easier modding
				if (POWER_THRESHOLD.enabled && player!=me && playerPower(player) < POWER_THRESHOLD.ally && playerPower(me) > POWER_THRESHOLD.me) {
					report("hint.power.low",{player:player});
				}
			} else {
				playerLiving.enemy += 1;
			}
			if (!playerLiving[player]) { // player was dead, so they only just got revived
				playerLiving[player] = true;
				report.playerAlive(player, true); // announce ressurection of the player
			}
		} else { // player dead
			if (playerLiving[player]) { // player was alive, so they only just got killed
				playerLiving[player] = false;
				report.playerAlive(player, false); // announce demise if the player
			}
		}		
	}
	
	// console("DEBUG: Alive? Me: "+playerAlive[me]+", Allies: "+playerAlive.ally+", Team: "+playerAlive.team+", Enemies: "+playerAlive.enemy);

    var victory = checkGameOutcome(playerLiving[me], playerLiving.ally, playerLiving.team, playerLiving.enemy, teams);

	if (victory == null) {
		return; // game still in progress
	} else if (!gameOverShown) { // game finished
		gameOverShown = true;
		gameOverMessage(victory);
	}
}

// /////////////////////////////////////////////////////////////////
// SITUATION REPORTING (IN-GAME EVENTS)

function eventResearched(research,obj) {
	report.researched(research,obj);
}

// Sensors detect something...
function eventObjectSeen(by,obj) {
	report.objectSeen(by,obj);
}

// Structure built...
function eventStructureBuilt(obj,droid) {
	// handle HQ construction
	if (obj.player == humanPlayer && obj.type == STRUCTURE && obj.stattype == HQ) {
		setMiniMap(true); // show minimap
		setDesign(true); // permit designs
	}
	// report on situation
	report.structureBuilt(obj,droid);
}

// Structure is ready for use...
function eventStructureReady(obj) {
	report.structureReady(obj);
}

// Droid built...
function eventDroidBuilt(obj,struct) {
// disabled because of http://forums.wz2100.net/viewtopic.php?f=32&t=9419
	// report.droidBuilt(obj,struct);
}

// We're under attack...
function eventAttacked(obj,attacker) {
	report.attacked(obj,attacker);
}

// Something destroyed...
function eventDestroyed(obj) {
	// handle HQ destruction
	if (obj.player == humanPlayer && obj.type == STRUCTURE && obj.stattype == HQ) {
		setMiniMap(false); // hide minimap
		setDesign(false);  // prevent designs
	}
	// report on situation
	report.destroyed(obj);
}

function eventObjectTransfer(obj,from) {
	report.transfer(obj,from);
}