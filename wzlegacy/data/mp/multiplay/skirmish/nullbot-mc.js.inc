
	/***************************/
	/* MC branch specific code */
	/***************************/
	
function constructPersonality() {
	
	this.MIN_BUILDERS = 2;		// the usual number of trucks that construct base structures
	this.MAX_BUILDERS = 3;		// the maximum number of trucks that construct base structures
	this.MIN_OILERS = 1;		// the usual number of trucks used for oil hunting
	this.MAX_OILERS = 4;		// the maximum number of trucks used for oil hunting
	this.MIN_DEFENDERS = 14;		// the minimum number of tanks before producing more trucks than necessary
	this.DEFENSIVENESS = 4;		// regulates chance of spending money on defensive structures
	this.PEACE_TIME = 5;		// the amount of minutes for free scouting (regrouping disabled, no defenses built)

	this.THIS_AI_MAKES_TANKS = true;
	this.THIS_AI_MAKES_CYBORGS = true;
	this.THIS_AI_MAKES_VTOLS = true;

	this.researchPathPrimary = [
		"R-Wpn-MG3Mk1",
		"R-Vehicle-Prop-Halftracks",
		"R-Struc-PowerModuleMk1",
		"R-Wpn-Cannon-Damage04",
		"R-Struc-RepairFacility",
		"R-Struc-Research-Upgrade01",
	];

	this.researchPathFundamental = standardResearchPathFundamental;

	this.researchPathAT = standardResearchPathCannons;

	this.researchPathAP = standardResearchPathMachineguns;
	this.researchPathAP = this.researchPathAP.concat(standardResearchPathLasers);

	this.researchPathAA = [
		"R-Defense-AASite-QuadMg1",
		"R-Wpn-AAGun-ROF01",
		"R-Defense-AASite-QuadBof",
		"R-Wpn-AAGun-ROF02",
		"R-Defense-AASite-QuadRotMg",
		"R-Wpn-AAGun-ROF06",
		"R-Wpn-AAGun-Damage06",
		"R-Defense-AA-Laser",
	];

	this.researchPathAB = standardResearchPathHowitzers;

	this.tankBodies = [ standardTankBodiesLight, ];
	this.tankBodiesAP = [ standardTankBodiesHeavy, ];

	this.tankPropulsions = [
	[
		"tracked01", // tracks
		"HalfTrack", // half-track
		"wheeled01", // wheels
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
	this.apWeapons[0]=this.apWeapons[0].concat(standardTankMachinegun);

	this.atWeapons = [
		standardTankCannonOne,
		standardTankCannonTwo,
	];

	this.abWeapons = [
	[
		"RailGun3Mk1", // gauss cannon (don't build mortars in late-game)
		"RailGun2Mk1", // rail gun (don't build mortars in late-game)
		"Mortar2Mk1", // bombard
		"Mortar-Incenediary", // incendiary mortar
		"Mortar3ROTARYMk1", // pepperpot
		"Mortar1Mk1", // mortar
	],
	];
	
	this.aaWeapons = [
	[
		"AAGunLaser", // stormbringer
		"QuadRotAAGun", // whirlwind
		"AAGun2Mk1", // flak cannon
		"QuadMg1AAGun", // hurricane
	],
	];

	this.vtolWeapons = [
		"RailGun2-VTOL", // rail gun
		"Bomb2-VTOL-HvHE", // heap bombs
		"RailGun1-VTOL", // needle
		"Laser2PULSE-VTOL", // pulse laser
		"Cannon5Vulcan-VTOL", // ac
		"MG4ROTARY-VTOL", // ag
		"Cannon4AUTO-VTOL", // hpv
		"Bomb1-VTOL-LtHE", // cluster bombs
		"MG3-VTOL", // hmg
	];

	this.apCyborgStats = [
		standardCyborgLaser,
	]; 
	this.apCyborgStats[0]=this.apCyborgStats[0].concat(standardCyborgMachinegunMortar);

	this.atCyborgStats = [
		standardCyborgCannonOne,
		standardCyborgCannonTwo,
		standardCyborgCannonThree,
	];

	this.defenses = [
	[
		"Emplacement-Rail3", // gauss emplacement
		"Emplacement-Rail2", // railgun emplacement
		"GuardTower-Rail1", // needle gun tower
		"PillBox-Cannon6", // tac bunker
		"Emplacement-HPVcannon", // hpv emplacement
		"PillBox4", // lc bunker
	],
	[
		"GuardTower-BeamLas", // pulse laser tower
		"Emplacement-PrisLas", // flashlight emplacement
		"Pillbox-RotMG", // ag bunker
		"PillBox1", // mg bunker
		"GuardTower1", // mg tower
	],
	[
		"X-Super-MassDriver", // super mass driver
		"X-Super-Cannon", // super cannon
	],
	];

	this.artillery = [
	[
		"Emplacement-Howitzer150", // ground shaker
		"Emplacement-RotHow", // hellstorm
		"Emplacement-Howitzer105", // howitzer
		"Emplacement-MortarPit02", // bombard
		"Emplacement-MortarPit01", // mortar
	],
	];

	this.antiair = [
		"P0-AASite-Laser", // stormbringer
		"AASite-QuadRotMg", // whirlwind
		"AASite-QuadBof", // flak cannon
		"AASite-QuadMg1", // hurricane
	];

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
	];

}

function buildOrder() {
	// Builds initial structures in the following order:
	//     factory, lab, cc, factory, factory, generator, generator, lab, lab
	//     * Doesn't build 3rd lab until 10th minute of the game.
	// Also make sure we have at least one generator before our initial money runs out
	var labCount = enumStruct(me, lab).length;
	var factoryCount = enumStruct(me, factory).length;
	var commandCount = enumStruct(me, command).length;
	var genCount = enumStruct(me, generator).length;
	var derrickCount = enumStruct(me, derrick).length;
	var borgCount = enumStruct(me, borgfac).length;
	var vtolCount = enumStruct(me, vtolfac).length;
	if (genCount < 2 && playerPower(me) < EXTREME_LOW_POWER && isStructureAvailable(generator,me))
		return buildBasicStructure(generator); 
	if (factoryCount < 1 && isStructureAvailable(factory,me))
		return buildBasicStructure(factory); 
	if (labCount < 1 && isStructureAvailable(lab,me))
		return buildBasicStructure(lab); 
	if (commandCount < 1 && isStructureAvailable(command,me))
		return buildBasicStructure(command); 
	if (factoryCount < 3 && isStructureAvailable(factory,me))
		return buildBasicStructure(factory,0); 
	if ((genCount < 2) && isStructureAvailable(generator,me))
		return buildBasicStructure(generator); 
	if (labCount < 2 && isStructureAvailable(lab,me))
		return buildBasicStructure(lab,0); 
	if (labCount < 3 && isStructureAvailable(lab,me) && (!earlyGame(10)&&!UNUSUAL_SITUATION))
		return buildBasicStructure(lab,0); 
	if (borgCount < 2 && isStructureAvailable(borgfac,me) && RATE_TANK>0)
		return buildBasicStructure(borgfac,0); 
	if (vtolCount < 1 && isStructureAvailable(vtolfac,me))
		return buildBasicStructure(vtolfac,0); 
	return false;
}