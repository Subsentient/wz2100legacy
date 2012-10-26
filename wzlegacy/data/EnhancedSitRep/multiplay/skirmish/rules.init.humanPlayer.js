// rules.init.humanPlayer.js v0.1
//
// These settings are applied to the localhost human player if they are NOT a spectator
//
// Useful references:
// * https://warzone.atlassian.net/wiki/display/mod/Player+Initialisation
//
// Triggered by "eventGameInit()" in rules.js
//
// /////////////////////////////////////////////////////////////////


init.humanPlayer = function() {
	// Set minimap and desgin based on presence of a HQ
	var state = (!!enumStruct(me, HQ).length);
	setMiniMap(state);
	setDesign(state);

	// This is the only template that should be enabled before design is allowed
	enableTemplate("ConstructionDroid");
}
