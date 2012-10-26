// rules.victory.js v0.1
// Determines if a player is alive, checks victory conditions
// Also allows modding of POWER_THRESHOLD constant
//
// This stuff is used by 'checkPlayers()' in rules.js
//
// /////////////////////////////////////////////////////////////////



// /////////////////////////////////////////////////////////////////
// POWER_THRESHOLD constant:
// Suggest human transfers power to an ally if certain thresholds are met
// 
// Properties:
//   enabled: Is this feature enabled?
//   ally:    If an ally drops below this power level...
//   me:      ...make the suggestion if I have more than this power level

const POWER_THRESHOLD = {
        enabled: true,
		ally:    500,
		me:      1000
	  }


// /////////////////////////////////////////////////////////////////
// isPlayerAlive() function:
// Determine if a specified player is dead or alive
// DO NOT try and revive a player in this function!
// 
// Parameters:
//   player:    ID of the player to check
//   spectator: Is this player a spectator?
//
// Return values:
//   true:  Player is ALIVE
//   false: Player is DEAD
//
// Standard game rules: (for reference)
// * Alive if they have at least one of these: FACTORY, CYBORG_FACTORY, any droid 
// * Otherwise they are dead
//
// Useful resources:
// * Droid types:     https://warzone.atlassian.net/wiki/display/jsapi/.droidType
// * Structure types: https://warzone.atlassian.net/wiki/display/jsapi/.stattype
// * enumStruct():    https://warzone.atlassian.net/wiki/pages/viewpage.action?pageId=360565
// * enumDroid():     https://warzone.atlassian.net/wiki/pages/viewpage.action?pageId=360698

function isPlayerAlive(player) {

	return (enumStruct(player, FACTORY).length || enumStruct(player, CYBORG_FACTORY).length || enumDroid(player).length);

}


// /////////////////////////////////////////////////////////////////
// checkGameOutcome() function:
// Determine if game is won or lost, or still in progress
// (from perspective of local human player)
//
// Parameters:
//   meAlive:    Is 'me' alive? true/false   ('me' == humanPlayer as defined in rules.js)
//   allyAlive:  Number of allies alive      (will be 0 if fixedTeams == true)
//   teamAlive:  Number of team mates alive  (will be 0 if fixedTeams == false)
//   enemyAlive: Number of enemies alive     (does not include Scavengers at present)
//   fixedTeams: Fixed team game? true/false (alliancesType == ALLIANCES_TEAMS)
//
// Return values:
//   true:  Player has WON the game
//   false: Player has LOST the game
//   null:	Game is still in progress (not won/lost yet)
//
// Standard game rules: (for reference)
// * In fixedTeams mode, the game is only lost if you and all your team mates are dead
//   (!meAlive && !teamAlive)
// * Game is won when all enemies are dead, ingoring scavengers
//   (!enemiesAlive)

function checkGameOutcome(meAlive,allyAlive,teamAlive,enemyAlive,fixedTeams) {

	// GAME WON:
	// My enemies are dead
	if (!enemyAlive) return true;

    // GAME OVER:
	// Me and my team are dead
	if (!meAlive && !teamAlive) return false;

	// Otherwise game still in progress:
	return null;
	
}


/* Want to make tweaks to the situation reporting taxonomy?

Do something like this:

report.msg["startLevel.welcome"] = {ogg:"pcv476", msg:"Entering Warzone!"}; // entering warzone

...that would override the existing startLevel.welcome message, but you can easily add your own, eg:

report.msg["my.custom.message"] = {ogg:"pcv476", msg:"Abandon hope all ye who enter here!"};

For more details on the object properties, etc., see rules.report.msg.js

If you add any custom audio you'll need to override audio.cfg and audio.wrf as well:
* audio.cfg: https://warzone.atlassian.net/wiki/display/mod/audio.cfg
* audio.wrf: https://warzone.atlassian.net/wiki/display/mod/audio.wrf

*/
