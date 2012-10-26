// rules.init.js v0.10
// Player initialisation data and functions
//
// The functions are called from eventGameInit() in rules.js
//
// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// INIT FUNCTIONS
// Called from eventGameInit() in rules.js

function init(){};

// This object specifies what effect the baseType setting has on players
init.baseTypes = {}; // populated by rules.init.baseType.js

// Enable structures for player
init.enableStructuresFor = function(player) {
	init.enableStructures.forEach(function(struct) {
		enableStructure(struct, player);
	});
}
// Enable components for player
init.enableComponentsFor = function(player) {
	init.enableComponents.forEach(function(component) {
		makeComponentAvailable(component, player);
	});
}
// Enable technologies for player
init.enableTechnologiesFor = function(player) {
	init.enableTechnologies.forEach(function(technology) {
		enableResearch(technology, player);
	});
}
// Set structure limits for player
init.setLimitsFor = function(player) {
	for (structure in init.structureLimits) {
		setStructureLimits(structure, init.structureLimits[structure], player);
	}
}
// Apply base type for player
init.applyBaseTypeFor = function(player) {
	// starting power:
	setPower(init.baseTypes[baseType].power, player);
	// research technologies:
	var technology = init.baseTypes[baseType].numTechs;
	while (-1<--technology) {
		completeResearch(init.researchTechnologies[technology], player);
	}
	// what structures do we want to keep?
	var keep = init.baseTypes[baseType][(playerData[player].difficulty == INSANE) ? "insaneKeep" : (isSpectator(player) ? "SpectatorKeep" : "normalKeep")];
	var structures = enumStruct(player);
	structures.forEach(function(structure) { // keep this structure?
		if (!keep.some(function(stattype) { // check it's not in keep list
			return (structure.stattype == stattype);
		})) removeStruct(structure); // remove it if not in list
	});
}
