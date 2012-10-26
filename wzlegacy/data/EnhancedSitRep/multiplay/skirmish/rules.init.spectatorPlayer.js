// rules.init.spectatorPlayer.js v0.1
//
// These settings are applied to the localhost human player only if they ARE a spectator
//
// Useful references:
// * https://warzone.atlassian.net/wiki/display/mod/Player+Initialisation
//
// Triggered by "eventGameInit()" in rules.js
//
// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
// SPECTATOR SET-UP
//
// Basic requirements:
// * Give them a SAT_UPLINK
// * Remove any other buildings/droids
// * Disable as much as possible
// * Ensure minimap is on

init.spectatorPlayer = function() {

	// Ensure they have a SAT_UPLINK
	var hasUplink = (enumStruct(me, SAT_UPLINK).length);
	if (!hasUplink) { // need to place one!
		enableStructure(SAT_UPLINK, me);
		var truck = enumDroid(me,DROID_CONSTRUCT);
		if (truck.length) {
		    truck = truck[0];
			var pos = pickStructLocation(truck, SAT_UPLINK, truck.x, truck.y);
			addStructure(SAT_UPLINK, me, pos.x, pos.y);
		}
	}

	// Kill any starting units
	var droids = enumDroid();
	var i;
	while (-1<--i) { // destroy each droid
		// v3.1 branch
		orderDroidLoc(droids[i], 12, droids[i].x, droids[i].y);
		// v3.2 branch
		// removeObject(droids[i]);
	}
	
	// Ensure the minimap is on, and designer is off
	setMiniMap(true);
	setDesign(false);
    
}