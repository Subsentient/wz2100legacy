// rules.init.baseType.js v0.1
//
// These settings will be applied to all players based on the game's base type setting
//
// Useful references:
// * https://warzone.atlassian.net/wiki/display/jsapi/Base+types
// * https://warzone.atlassian.net/wiki/display/mod/Player+Initialisation
//
// Processed by rules.init.js
//
// /////////////////////////////////////////////////////////////////



// /////////////////////////////////////////////////////////////////
// researchTechnologies list
//
// Varying numbers of these will be researched based on 'init.baseTypes' settings at bottom of file:

init.researchTechnologies = [
	"R-Vehicle-Prop-Wheels",
	"R-Sys-Spade1Mk1",
	"R-Vehicle-Body01",
	"R-Comp-SynapticLink",
	"R-Wpn-MG1Mk1",
	"R-Defense-HardcreteWall",
	"R-Vehicle-Prop-Wheels",
	"R-Sys-Spade1Mk1",
	"R-Struc-Factory-Cyborg",
	"R-Defense-Pillbox01",
	"R-Defense-Tower01",
	"R-Vehicle-Body01",
	"R-Sys-Engineering01",
	"R-Struc-CommandRelay",
	"R-Vehicle-Prop-Halftracks",
	"R-Comp-CommandTurret01",
	"R-Sys-Sensor-Turret01",
	"R-Wpn-Flamer01Mk1",
	"R-Vehicle-Body05",
	"R-Struc-Research-Module",
	"R-Struc-PowerModuleMk1",
	"R-Struc-Factory-Module",
	"R-Struc-RepairFacility",
	"R-Sys-MobileRepairTurret01",
	"R-Vehicle-Engine01",
	"R-Wpn-MG3Mk1",
	"R-Wpn-Cannon1Mk1",
	"R-Wpn-Mortar01Lt",
	"R-Defense-Pillbox05",
	"R-Defense-TankTrap01",
	"R-Defense-WallTower02",
	"R-Sys-Sensor-Tower01",
	"R-Defense-Pillbox04",
	"R-Wpn-MG2Mk1",
	"R-Wpn-Rocket05-MiniPod",
	"R-Wpn-MG-Damage01",
	"R-Wpn-Rocket-Damage01",
	"R-Defense-WallTower01"
];


// /////////////////////////////////////////////////////////////////
// baseTypes definitions
//
// One of these per baseType. Defines what effects the baseType will have.
//
// Properties:
//   power:         Starting power
//   numTechs:      How many researchTechnologies to complete
//   normalKeep:    What structures do normal players keep
//   spectatorKeep: What structures do normal players keep
//   insaneKeep:    What structures do 'INSANE' AI players keep

init.baseTypes[CAMP_CLEAN] = {
	power: 1300,
	numTechs: 4,
	normalKeep:    [],
	spectatorKeep: [SAT_UPLINK],
	insaneKeep:    [DEFENSE,GATE,RESOURCE_EXTRACTOR,WALL]
}
init.baseTypes[CAMP_BASE] = {
	power: 2500,
	numTechs: 18,
	normalKeep: [FACTORY,HQ,LASSAT,POWER_GEN,REARM_PAD,REPAIR_FACILITY,RESEARCH_LAB,RESOURCE_EXTRACTOR,SAT_UPLINK,VTOL_FACTORY],
	spectatorKeep: [SAT_UPLINK],
	insaneKeep: [DEFENSE,FACTORY,HQ,LASSAT,POWER_GEN,REARM_PAD,REPAIR_FACILITY,RESEARCH_LAB,RESOURCE_EXTRACTOR,SAT_UPLINK,VTOL_FACTORY,WALL]
}
init.baseTypes[CAMP_WALLS] = {
	power: 2500,
	numTechs: init.researchTechnologies.length,
	normalKeep:    [COMMAND_CONTROL,CYBORG_FACTORY,DEFENSE,FACTORY,GATE,HQ,LASSAT,POWER_GEN,REARM_PAD,REPAIR_FACILITY,RESEARCH_LAB,RESOURCE_EXTRACTOR,SAT_UPLINK,VTOL_FACTORY,WALL],
	spectatorKeep: [SAT_UPLINK],
	insaneKeep:    [COMMAND_CONTROL,CYBORG_FACTORY,DEFENSE,FACTORY,GATE,HQ,LASSAT,POWER_GEN,REARM_PAD,REPAIR_FACILITY,RESEARCH_LAB,RESOURCE_EXTRACTOR,SAT_UPLINK,VTOL_FACTORY,WALL]
}
