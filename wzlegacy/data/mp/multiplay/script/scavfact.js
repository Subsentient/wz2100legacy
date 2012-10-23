
var MIN_ATTACKERS = 12;
var MAX_SENSORS; // initialized at game start
var MAX_DEFENDERS = 6;
var MAX_GLOBAL_DEFENDERS = 15;

// unit limit constant
function atLimits() {
	return enumDroid(me).length>=130;
}

// random integer between 0 and max-1 (for convenience)
function random(max) {
	if (max<=0)
		return 0;
	return Math.floor(Math.random() * max);
}

const factory = "A0BaBaFactory";

const templates = [
	["B4body-sml-trike01","bTrikeMG"],
	["B4body-sml-trike01","bTrikeMG"],
	["B4body-sml-trike01","bTrikeMG"],
	["B4body-sml-trike01","bTrikeMG"],
	["B4body-sml-trike01","bTrikeMG"],
	["B4body-sml-trike01","bTrikeMG"],
	["B3body-sml-buggy01","BuggyMG"],
	["B3body-sml-buggy01","BuggyMG"],
	["B3body-sml-buggy01","BuggyMG"],
	["B3body-sml-buggy01","BuggyMG"],
	["B3body-sml-buggy01","BuggyMG"],
	["B2JeepBody","BJeepMG"],
	["B2JeepBody","BJeepMG"],
	["B2JeepBody","BJeepMG"],
	["B2JeepBody","BJeepMG"],
	["B3bodyRKbuggy01","BabaRocket"],
	["B3bodyRKbuggy01","BabaRocket"],
	["B3bodyRKbuggy01","BabaRocket"],
	["B2RKJeepBody","BabaRocket"],
	["B2RKJeepBody","BabaRocket"],
	["B2RKJeepBody","BabaRocket"],
	["BusBody","BusCannon"],
	["BusBody","BusCannon"],
	["BusBody","BusCannon"],
	["BusBody","BabaPitRocketAT"],
	["FireBody","BabaFlame"],
	["FireBody","BusCannon"],
	["FireBody","BabaPitRocket"],
	["FireBody","BabaPitRocketAT"],
];

// scav groups
var globalDefendGroup; // tanks that defend all bases
var needToPickGroup; // a group

var baseInfo=[];

function constructBaseInfo(x,y) {
	this.x = x;
	this.y = y;
	this.defendGroup = newGroup(); // tanks to defend the base
	this.builderGroup = newGroup(); // trucks to build base structures and defenses
	this.attackGroup = newGroup(); // tanks to attack nearby things
}

function findNearestBase(x,y) {
	var minDist=Infinity, minIdx=0;
	for (var i=0; i<baseInfo.length; ++i) {
		var d=distBetweenTwoPoints(baseInfo[i].x,baseInfo[i].y,x,y);
		if (d<minDist) {
			minDist=d;
			minIdx=i;
		}
	}
	return minIdx;
}

function factoriesOfBase(base) {
	var list=enumStruct(me,factory);
	for (var i=0; i<list.length; ++i)
		if (findNearestBase(list[i].x,list[i].y)!=base)
			list.splice(i);
	return list;
}

function reviseGroups() {
	var list=enumGroup(needToPickGroup);
	for (var i=0; i<list.length; ++i) {
		var droid=list[i];
		addDroidToSomeGroup(droid);
		// anti-stuck-at-base algorithm
		// couldn't have done it eventDroidBuilt, cause it gets overwritten 
		var x=droid.x+random(15)-7;
		if (x<0) x=0;
		if (x>=mapWidth-1) x=mapWidth-2;
		var y=droid.y+random(15)-7;
		if (y<0) y=0;
		if (y>=mapHeight-1) y=mapHeight-2;
		orderDroidLoc(droid,DORDER_MOVE,x,y); 
	}
}

function addDroidToSomeGroup(droid) {
	var base=findNearestBase(droid.x,droid.y);
	switch(droid.droidType) {
		case DROID_WEAPON:
			var i = random(5);
			switch(i) {
			case 0: 
				if (groupSize(globalDefendGroup)<MAX_GLOBAL_DEFENDERS) {
					groupAddDroid(globalDefendGroup, droid); 
					return;
				}
			case 1: 
				if (groupSize(baseInfo[base].defendGroup)<MAX_DEFENDERS) {
					groupAddDroid(baseInfo[base].defendGroup, droid); 
					return;
				}
			default: 
				groupAddDroid(baseInfo[base].attackGroup, droid); break;
			}
			return;
		case DROID_SENSOR: 
			groupAddDroid(baseInfo[base].attackGroup, droid); 
			return;
		case DROID_PERSON:
			groupAddDroid(baseInfo[base].defendGroup, droid);
			return;
	}
}

function groupOfTank(droid) {
	for (var i=0; i<baseInfo.length; ++i) {
		if (droid.group == baseInfo[i].attackGroup)
			return baseInfo[i].attackGroup;
	}
}

function produceDroid(fac) {
	var i=random(10);
	switch(i) {
		case 0: 
			if (enumDroid(me,DROID_SENSOR).length < MAX_SENSORS) {
				buildDroid(fac, "Sensor", "BusBody", "BaBaProp", "", DROID_SENSOR, "SensorTurret1Mk1"); 
				break;
			}
		default: 
			var j=random(templates.length);
			buildDroid(fac, "Scavenger Tank", templates[j][0], "BaBaProp", "", DROID_WEAPON, templates[j][1]); 
			break;
	}
}

function findNearestTarget(x,y) {
	var minDist=Infinity, minTarget;
	for (var i=0; i<maxPlayers; ++i) {
		var list;
		list=enumStruct(i);
		for (var j=0; j<list.length; ++j) {
			var d=distBetweenTwoPoints(list[j].x,list[j].y,x,y);
			if (d<minDist) {
				minDist=d;
				minTarget=list[j];
			}
		}
	}
	return minTarget;
}

function attackWithDroid(droid,target,force) {
	if (typeof(DORDER_OBSERVE)=="undefined")
		var DORDER_OBSERVE = 9; // HACK: waiting until this constant is exported to scripts ...
	if (droid.droidType == DROID_WEAPON)
		if (droid.order != DORDER_ATTACK || force)
			orderDroidLoc(droid,DORDER_SCOUT,target.x+random(5)-2,target.y+random(5)-2);
	else // sensor
		if (droid.order != DORDER_OBSERVE || force)
			orderDroidObj(droid,DORDER_OBSERVE,target);	
}

function attackStuff() {
	for (var j=0; j<baseInfo.length; ++j) {
		var list=enumGroup(baseInfo[j].attackGroup);
		if (list.length < MIN_ATTACKERS && !atLimits())
			continue;
		var target=findNearestTarget(baseInfo[j].x,baseInfo[j].y);
		if (typeof(target)=="undefined") 
			return;
		for (var i=0; i<list.length; ++i)
			attackWithDroid(list[i],target,false);
	}
}

function eventAttacked(victim, attacker) {
	if (attacker.player==me) // don't quarrel because of friendly splash damage
		return;
	if (victim.type==STRUCTURE) {
		var base=findNearestBase(victim.x,victim.y);
		var list=enumGroup(baseInfo[base].defendGroup);
		list = list.concat(enumGroup(globalDefendGroup));
		for (var i=0; i<list.length; ++i) {
			var droid=list[i];
			if (droid.order != DORDER_SCOUT) {
				orderDroidLoc(droid,DORDER_SCOUT,attacker.x,attacker.y);
			}
		}
	} else if (victim.type==DROID_WEAPON) {
		var gr=groupOfTank(victim);
		if (typeof(gr)=="undefined") 
			return;
		var list=enumGroup(gr);
		for (var i=0; i<list.length; ++i)
			attackWithDroid(list[i],attacker,true);
	}
}

function eventDroidBuilt(droid, fac) {
	groupAddDroid(needToPickGroup,droid);
	queue("reviseGroups",200);
	if (fac != null) // unit not transfered but actually built
		produceDroid(fac);
}

function eventGameInit() {
	for (var i=0; i<templates.length; ++i) {
		makeComponentAvailable(templates[i][0], me);
		makeComponentAvailable(templates[i][1], me);
		if (typeof(templates[i][2])!="undefined")
			makeComponentAvailable(templates[i][2], me);
		if (typeof(templates[i][3])!="undefined")
			makeComponentAvailable(templates[i][3], me);
	}
	makeComponentAvailable("SensorTurret1Mk1", me);
	makeComponentAvailable("BaBaProp", me);
}

function eventStartLevel() {
	var list=enumStruct(me,factory);
	for (var i=0; i<list.length; ++i) {
		var fac=list[i];
		baseInfo[i]=new constructBaseInfo(fac.x,fac.y);
		produceDroid(fac);
	}
	MAX_SENSORS=baseInfo.length;
	list=enumDroid(me);
	for (var i=0; i<list.length; ++i)
		addDroidToSomeGroup(list[i]);
	globalDefendGroup = newGroup();
	needToPickGroup = newGroup();
	setTimer("attackStuff", 3000);
}

