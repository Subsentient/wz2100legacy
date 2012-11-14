
	/***************************/
	/* MR branch specific code */
	/***************************/
	
function constructPersonality() {
	
	this.MIN_BUILDERS = 2;		// the usual number of trucks that construct base structures
	this.MAX_BUILDERS = 3;		// the maximum number of trucks that construct base structures
	this.MIN_OILERS = 3;		// the usual number of trucks used for oil hunting
	this.MAX_OILERS = 4;		// the maximum number of trucks used for oil hunting
	this.MIN_DEFENDERS = 16;		// the minimum number of tanks before producing more trucks than necessary
	this.DEFENSIVENESS = 3;		// regulates chance of spending money on defensive structures
	this.PEACE_TIME = 5;		// the amount of minutes for free scouting (regrouping disabled, no defenses built)

	this.THIS_AI_MAKES_TANKS = true;
	this.THIS_AI_MAKES_CYBORGS = true;
	this.THIS_AI_MAKES_VTOLS = true;

	this.researchPathPrimary = [
		"R-Vehicle-Prop-Halftracks",
		"R-Wpn-MG-Damage02",
		"R-Wpn-Rocket-Damage01",
		"R-Struc-PowerModuleMk1",
		"R-Wpn-MG3Mk1",
		"R-Defense-Pillbox01",
		"R-Wpn-Rocket-Damage02",
		"R-Defense-Pillbox06",
		"R-Vehicle-Body05",
		"R-Struc-RepairFacility",
		"R-Defense-WallTower01",
		"R-Wpn-Rocket-ROF01",
		"R-Defense-MRL",
		"R-Defense-WallTower06",
		"R-Struc-Research-Upgrade01",
	];

	this.researchPathFundamental = standardResearchPathFundamental;

	this.researchPathAT = standardResearchPathRockets;

	this.researchPathAP = standardResearchPathMachineguns;
	this.researchPathAP = this.researchPathAP.concat(standardResearchPathLasers);

	this.researchPathAA = [
		"R-Defense-AASite-QuadMg1",
		"R-Wpn-AAGun-ROF02",
		"R-Defense-Sunburst",
		"R-Wpn-AAGun-Accuracy01",
		"R-Wpn-AAGun-ROF03",
		"R-Defense-SamSite1",
		"R-Defense-SamSite2",
		"R-Defense-AA-Laser",
	];

	this.researchPathAB = standardResearchPathBunkerBuster;
	this.researchPathAB = this.researchPathAB.concat(standardResearchPathRipples);

	this.tankBodies = [
	[
		"Body14SUP", // dragon
		"Body13SUP", // wyvern
		"Body7ABT", // retribution
		"Body8MBT", // scorpion
		"Body6SUPP", // panther
		"Body5REC", // cobra
		"Body3MBT", // retaliation
		"Body4ABT", // bug
		"Body2SUP", // leopard
		"Body1REC", // viper
	],
	];
	
	this.tankBodiesAT = [
	[
		"Body14SUP", // dragon
		"Body13SUP", // wyvern
		"Body7ABT", // retribution
		"Body6SUPP", // panther
		"Body8MBT", // scorpion
		"Body5REC", // cobra
		"Body3MBT", // retaliation
		"Body2SUP", // leopard
		"Body4ABT", // bug
		"Body1REC", // viper
	],
	];

	this.tankPropulsions = [
	[
		"HalfTrack", // half-track
		"wheeled01", // wheels
	],
	];
	
	this.tankPropulsionsAT = [
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
		"Body3MBT", // retaliation
		"Body2SUP", // leopard
		"Body4ABT", // bug
		"Body1REC", // viper
	];

	this.apWeapons = [
		standardTankLaser,
		standardTankAPMachinegunRocket,
	];
	this.apWeapons[0]=this.apWeapons[0].concat(standardTankMachinegun);

	this.atWeapons = [
		standardTankRocket,
	];

	this.abWeapons = [
	[
		"Missile-MdArt", // seraph
		"Rocket-BB", // bunker buster
		"Rocket-MRL", // mra
	],
	];
	
	this.aaWeapons = [
	[
		"AAGunLaser", // stormbringer
		"Missile-HvySAM", // vindicator
		"Missile-LtSAM", // avenger
		"QuadRotAAGun", // whirlwind
		"Rocket-Sunburst", // sunburst
		"QuadMg1AAGun", // hurricane
	],
	];

	this.vtolWeapons = [
		"Missile-VTOL-AT", // scourge
		"Rocket-VTOL-HvyA-T", // tk
		"Rocket-VTOL-LtA-T", // lancer
		"Rocket-VTOL-Pod", // minipod
	];

	this.apCyborgStats = [
		standardCyborgLaser,
	]; 
	this.apCyborgStats[0]=this.apCyborgStats[0].concat(standardCyborgMachinegun);

	this.atCyborgStats = [
		standardCyborgRocket,
	];

	this.defenses = [
	[
		"GuardTower-ATMiss", // scourge tower
		"Emplacement-HvyATrocket", // tk emplacement
		"GuardTower5", // lancer tower
		"GuardTower6", // minipod tower
	],
	[
		"Emplacement-MdART-pit", // seraph emplacement
		"GuardTower-BeamLas", // pulse laser tower
		"Emplacement-PrisLas", // flashlight emplacement
		"Pillbox-RotMG", // ag bunker
		"Emplacement-MRL-pit", // mra emplacement
		"PillBox1", // mg bunker
		"GuardTower1", // mg tower
	],
	[
		"X-Super-Missile", // super missile fort
		"X-Super-Rocket", // super rocket fort
	],
	];

	this.artillery = [
	[
		"Emplacement-HvART-pit", // archangel
		"Emplacement-Rocket06-IDF", // ripples
	],
	];

	this.antiair = [
		"P0-AASite-Laser", // stormbringer
		"P0-AASite-SAM2", // vindicator
		"P0-AASite-SAM1", // avenger
		"AASite-QuadRotMg", // whirlwind
		"P0-AASite-Sunburst", // sunburst
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
		"WallTower-Atmiss", // scourge
		"WallTower-HvATrocket", // tk
		"WallTower06", // lancer
	],
	];

}

function buildOrder() {
	return standardBuildOrderRFFRC();
}