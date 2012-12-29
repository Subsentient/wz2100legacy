
	/***************************/
	/* FL branch specific code */
	/***************************/
	
function constructPersonality() {
	
	this.MIN_BUILDERS = 2;		// the usual number of trucks that construct base structures
	this.MAX_BUILDERS = 3;		// the maximum number of trucks that construct base structures
	this.MIN_OILERS = 3;		// the usual number of trucks used for oil hunting
	this.MAX_OILERS = 4;		// the maximum number of trucks used for oil hunting
	this.MIN_DEFENDERS = 0;		// the minimum number of tanks before producing more trucks than necessary
	this.DEFENSIVENESS = 10;		// regulates chance of spending money on defensive structures
	this.PEACE_TIME = 0;		// the amount of minutes for free scouting (regrouping disabled, no defenses built)

	this.THIS_AI_MAKES_TANKS = true;
	this.THIS_AI_MAKES_CYBORGS = true;
	this.THIS_AI_MAKES_VTOLS = true;

	this.researchPathPrimary = [
		"R-Wpn-Flamer-ROF01",
		"R-Wpn-Flamer-Damage03",
		"R-Struc-Factory-Cyborg",
		"R-Struc-PowerModuleMk1",
		"R-Vehicle-Body05",
		"R-Struc-RepairFacility",
		"R-Defense-HvyFlamer",
		"R-Cyborg-Metals04",
		"R-Vehicle-Prop-Hover",
		"R-Vehicle-Body11",
		"R-Defense-AASite-QuadMg1",
		"R-Struc-RprFac-Upgrade01",
		"R-Sys-Sensor-Tower02",
		"R-Cyborg-Armor-Heat01",
		"R-Struc-Power-Upgrade01c",
		"R-Wpn-AAGun-Damage01",
		"R-Struc-VTOLPad-Upgrade02",
		"R-Cyborg-Armor-Heat02",
		"R-Struc-Materials03",
		"R-Wpn-Bomb03",
		"R-Wpn-AAGun-Damage02",
		"R-Wpn-Flamer-ROF03",
		"R-Vehicle-Body08",
		"R-Defense-MortarPit-Incenediary",
		"R-Wpn-AAGun-Damage03",
		"R-Sys-CBSensor-Tower01",
		"R-Wpn-Bomb04",
		"R-Vehicle-Body12",
		"R-Struc-Power-Upgrade03a",
		"R-Struc-RprFac-Upgrade04",
		"R-Defense-PlasmiteFlamer",
		"R-Wpn-Flamer-Damage09",
		"R-Defense-PulseLas",
		"R-Defense-WallTower-PulseLas",
		"R-Vehicle-Body09",
		"R-Defense-Howitzer-Incenediary",
		"R-Wpn-Bomb-Accuracy03",
		"R-Vehicle-Body10",
		"R-Wpn-Bomb05",
		"R-Struc-Research-Upgrade09",
		"R-Defense-AA-Laser",
		"R-Wpn-Energy-ROF03",
		"R-Cyborg-Metals09",
		"R-Cyborg-Armor-Heat09",
		"R-Struc-VTOLPad-Upgrade06",
		"R-Vehicle-Body14",
		"R-Wpn-LasSat",
		"R-Struc-Materials09",
		"R-Struc-RprFac-Upgrade06",
	];
	
	this.researchPathFundamental = [
	];

	this.researchPathAT = [
	];

	this.researchPathAP = [
	];

	this.researchPathAA = [
	];

	this.researchPathAB = [
	];

	this.tankBodies = [
	[
		"Body14SUP", // dragon
		"Body13SUP", // wyvern
		"Body10MBT", // vengeance
		"Body7ABT", // retribution
		"Body9REC", // tiger
		"Body12SUP", // mantis
		"Body11ABT", // python
		"Body6SUPP", // panther
		"Body8MBT", // scorpion
		"Body5REC", // cobra
	],
	];

	this.tankPropulsions = [
	[
		"hover01", // hover
		"HalfTrack", // half-track
	],
	];

	this.vtolBodies = [
		"Body7ABT", // retribution
		"Body6SUPP", // panther
		"Body8MBT", // scorpion
		"Body5REC", // cobra
	];

	this.apWeapons = [
		standardTankLaser,
	];
	this.apWeapons[0]=this.apWeapons[0].concat(standardTankFlamer);

	this.atWeapons = this.apWeapons;

	
	this.abWeapons = [
	[
		"Laser2PULSEMk1", // pulse laser (don't build mortars in late-game)
		"Mortar-Incenediary", // incendiary mortar
		"Mortar2Mk1", // bombard
		"Mortar1Mk1", // mortar
	],
	];
	
	// Flame personality doesn't invent anti-air weapons on its own, apart from hurricane and stormbringer
	// but it uses them if they are suddenly available (eg., researched by teammates)
	this.aaWeapons = [
	[
		"AAGunLaser", // stormbringer
		"Missile-HvySAM", // vindicator
		"Missile-LtSAM", // avenger
		"QuadRotAAGun", // whirlwind
		"AAGun2Mk1", // flak cannon
		"Rocket-Sunburst", // sunburst
		"QuadMg1AAGun", // hurricane
	],
	];

	this.vtolWeapons = [
		"Bomb5-VTOL-Plasmite", // plasmite bombs
		"Bomb4-VTOL-HvyINC", // thermite bombs
		"Bomb3-VTOL-LtINC", // phosphor bombs
		"Bomb1-VTOL-LtHE", // cluster bombs
	];

	this.apCyborgStats = [
		standardCyborgLaser,
	];
	this.apCyborgStats[0]=this.apCyborgStats[0].concat(standardCyborgFlamer);

	this.atCyborgStats = this.apCyborgStats;

	this.defenses = [
	[
		"GuardTower-BeamLas", // pulse tower
		"Emplacement-PrisLas", // flashlight emplacement
		"Plasmite-flamer-bunker", // plasmite bunker
		"Tower-Projector", // inferno bunker
		"PillBox5", // flamer bunker
	],
	];

	this.artillery = [
	[
		"Emplacement-Howitzer-Incenediary", // incendiary mortar
		"Emplacement-MortarPit-Incenediary", // incendiary mortar
	],
	];

	// Flame personality doesn't invent anti-air defenses on its own, apart from hurricane and stormbringer
	// but it uses them if they are suddenly available (eg., researched by teammates)
	this.antiair = [
		"P0-AASite-Laser", // stormbringer
		"P0-AASite-SAM2", // vindicator
		"AASite-QuadRotMg", // whirlwind
		"P0-AASite-SAM1", // avenger
		"P0-AASite-Sunburst", // sunburst
		"AASite-QuadBof", // flak cannon
		"AASite-QuadMg1", // hurricane
	];

	// Flame personality doesn't invent hardpoints defenses on its own, apart from pulse lasers,
	// but it uses them if they are suddenly available (eg., researched by teammates)
	this.hardpoints = [
	[
		"WallTower-PulseLas", // pulse laser
		"WallTower-TwinAssaultGun", // tag
		"Wall-RotMg", // ag
		"WallTower01", // hmg
	],
	[
		"WallTower-Rail3", // gauss
		"WallTower-Rail2", // railgun
		"WallTower04", // hc
		"Wall-VulcanCan", // ac
		"WallTower-HPVcannon", // hpv
		"WallTower03", // mc
		"WallTower02", // lc
	],
	[
		"WallTower-Atmiss", // scourge
		"WallTower-HvATrocket", // tk
		"WallTower06", // lancer
	],
	[
		"WallTower-EMP", // emp
	],
	];

}

// Builds initial structures in the following order:
//     lab, factory, lab, generator, generator, command center, cyborg factory, cyborg factory, lab, cyborg factory
// Demolishes the original factory until hovers are available
// Then build extra structures if necessary.
// Also make sure we have at least one generator before our initial money runs out
function buildOrder() {
	var labCount = enumStruct(me, lab).length;
	var factoryCount = enumStruct(me, factory).length;
	var commandCount = enumStruct(me, command).length;
	var genCount = enumStruct(me, generator).length;
	var derrickCount = enumStruct(me, derrick).length;
	var borgCount = enumStruct(me, borgfac).length;
	var vtolCount = enumStruct(me, vtolfac).length;
	const	COMP_PROPULSION=3; // HACK: define it here until these constants are exposed to scripts
	if (genCount < 2 && playerPower(me) < EXTREME_LOW_POWER && isStructureAvailable(generator,me))
		return buildBasicStructure(generator);
	if (labCount < 1 && isStructureAvailable(lab,me))
		return buildBasicStructure(lab);
	if (factoryCount < 1 && isStructureAvailable(factory,me) && (iHaveHovers() || !isStructureAvailable(borgfac,me)))
		return buildBasicStructure(factory);
	if (labCount < 2 && isStructureAvailable(lab,me))
		return buildBasicStructure(lab,0);
	if ((genCount < 2) && isStructureAvailable(generator,me))
		return buildBasicStructure(generator);
	if (commandCount < 1 && isStructureAvailable(command,me))
		return buildBasicStructure(command);
	if (factoryCount < 2 && isStructureAvailable(factory,me) && iHaveHovers())
		return buildBasicStructure(factory,0);
	if (borgCount < 1 && isStructureAvailable(borgfac,me) && RATE_TANK>0)
		return buildBasicStructure(borgfac);
	if (borgCount < 3 && isStructureAvailable(borgfac,me) && RATE_TANK>0)
		return buildBasicStructure(borgfac);
	if (vtolCount < 2 && isStructureAvailable(vtolfac,me))
		return buildBasicStructure(vtolfac,0);
	if (labCount < 3 && isStructureAvailable(lab,me) && borgCount>=2)
		return buildBasicStructure(lab,0);
	return false;
}

