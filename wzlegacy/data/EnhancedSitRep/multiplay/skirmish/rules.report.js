// rules.report.js v0.102
// Gameplay situation reporting
//
// The functions in here are called from stuff in rules.js.
//
// audioAlert() function comes from audioAlert.js
//
// Requires Warzone 3.1 Beta 5 or above due to DROID_SUPERTRANSPORTER
//
// /////////////////////////////////////////////////////////////////

// This is based on Shadow Wolf TJC's "Enhanced SitRep Mod":
// http://forums.wz2100.net/viewtopic.php?f=10&t=9392


// /////////////////////////////////////////////////////////////////
// MAKE SURE CONSTANTS ARE DEFINED

if (typeof scavengerPlayer == "undefined")		const scavengerPlayer = (scavengers) ? Math.max(7,maxPlayers) : -1;
if (typeof DORDER_RTB == "undefined")			const DORDER_RTB = 13;
if (typeof DORDER_RTR == "undefined")			const DORDER_RTR = 14;
if (typeof DORDER_RECYCLE == "undefined")		const DORDER_RECYCLE = 21;
if (typeof DORDER_PATROL == "undefined")		const DORDER_PATROL = 31;
if (typeof DORDER_REARM == "undefined")			const DORDER_REARM = 32;
if (typeof DORDER_RTR_SPECIFIED == "undefined")	const DORDER_RTR_SPECIFIED = 35;
if (typeof DORDER_CIRCLE == "undefined")		const DORDER_CIRCLE = 40;

// /////////////////////////////////////////////////////////////////
// ALLOWS POWER STATUS REPORT TO BE QUEUED

function checkPowerStatus() {
	report.power();
}

// /////////////////////////////////////////////////////////////////
// DEFINE REPORT OBJECT IN A FUNCTION CLOSURE TO KEEP THINGS TIDY

var report = (function() { // see rules.js for example of using report system

	// /////////////////////////////////////////////////////////////////
	// DETERMINE WHICH FACTION A GAME OBJECT BELONGS TO

	// Get player faction string
	function getFaction(obj) {
		if (!obj || !("player" in obj)) { // no faction
			return "";
		} else if (obj.player == selectedPlayer) { // it's me!
			return ".Me";
		} else if (obj.player == scavengerPlayer) { // scavenger
			return ".Scav";
		} else if (allianceExistsBetween(obj.player,selectedPlayer)) { // ally
			return ".Ally";
		} else if (obj.player <= maxPlayers) { // enemy
			return ".Enemy";
		} else { // probably a feature, so no faction
			return "";
		}
	}

	// /////////////////////////////////////////////////////////////////
	// GAME OBJECT CLASSIFICATION
	
	// NOTE: Loads of little helper functions at bottom of this script are used in this section
	
	// Ugly hack to fix .stattype for LASSAT objects - http://developer.wz2100.net/ticket/3506
	function fixStatType(obj) {
		//console(obj.id+" / "+obj.name);
		return (obj.name == "Laser Satellite Command Post") ? LASSAT : obj.stattype;
	}

	// classify object, starting with it's type
	function classify(obj) {
		//console("type: "+obj.type+", name: "+obj.name);
		switch (obj.type) {
			case DROID: {
				return "droid."+classify.droid(obj);
			}
			case STRUCTURE: {
				if (fixStatType(obj) == DEFENSE) {
					return "defense."+classify.defense(obj);
				} else {
					return "base."+classify.base(obj);
				}
			}
			case FEATURE: {
				return "feature."+classify.feature(obj);
			}
			default: {
				return obj.type;
			}
		}
	}
	
	// classify droid type
	classify.droid = function(obj) {
		if (isECM(obj)) return "ecm";
		if (isTruck(obj)) return "truck";
		if (isRepair(obj)) return "repair";
		if (isCommander(obj)) return "command";
		if (isSensor(obj)) return "sensor";
		if (isPerson(obj)) return "person";
		if (isTransport(obj)) return "transport";
		if (isMissile(obj)) return "missile";
		if (isBattery(obj)) return "battery";
		if (isCyborg(obj)) return "cyborg";
		if (isNexus(obj)) return "nexus";
		if (isFixVTOL(obj)) return "vtol";
		return "weapon"; // default to a tank
	}
	
	// classify defense type
	classify.defense = function(obj) {
		if (isCB(obj)) return "sensor.cb";
		if (isECM(obj)) return "ecm";
		if (isSensor(obj)) return "sensor";
		if (isAntiAir(obj)) return "antiair";
		if (isNexus(obj)) return "nexus";
		if (isMissile(obj)) return "missile";
		if (isBattery(obj)) return "battery";
		return "weapon"; // default to weapon defence
	}
	
	// classify base type
	classify.base = function(obj) {
		switch (fixStatType(obj)) {
			case COMMAND_CONTROL: return "command";
			case CYBORG_FACTORY: return "factory.cyborg";
			case FACTORY: return "factory.tank";
			case GATE: return "gate";
			case HQ: return "hq";
			case LASSAT: return "lassat";
			case POWER_GEN: return "generator";
			case REARM_PAD: return "rearm";
			case REPAIR_FACILITY: return "repair";
			case RESEARCH_LAB: return "research";
			case RESOURCE_EXTRACTOR: return "derrick";
			case SAT_UPLINK: return "uplink"
			case VTOL_FACTORY: return "factory.vtol";
			case WALL: return "wall";
		}
		return "unknown."+obj.stattype;
		// Missile Silo = stattype 20 (nuke symbol one) or 6 (toothlock doors one).
	}

	// classify a feature	
	var featureList = {
		"Oil Drum": "barrel",
		"Oil Resource": "resorce",
		"Crate": "artefact",
		"*NuclearReactor*": "reactor",
		"*CoolingTower*": "tower",
	}
	classify.feature = function(obj) {
		//console("feature: "+obj.name+" --> "+(obj.name in featureList ? featureList[obj.name] : ""));
		return (obj.name in featureList) ? featureList[obj.name] : obj.name;
	}

	// /////////////////////////////////////////////////////////////////
	// SITUATION REPORTING CODE

	var throttle = {}; // used to throttle reports of each specific message
	var cache = {}; // used to cache category.path.Faction lookups
		
	var report = function(path,obj) {
		var faction = getFaction(obj);
		var msg = path+faction;
		//console("DEBUG: "+msg);
		
		if (msg in cache) { // found it

			msg = cache[msg];
			// note: playReport() updates throttle object
			if ( !msg || (msg in throttle && throttle[msg] > gameTime) ) return;

			report.playMessage(msg,obj);

		} else { // find object, cache it, play it

			var topics = path.split(".");
			var search = msg;
			
			do {
				if (search in report.msg) { // found it
					cache[msg] = search;
					//console(msg+" --> "+search);
					report.playMessage(search,obj);
					break;
				} else { // reduce path and try again
					topics.pop();
					search = topics.join(".")+faction;
				}
			} while (topics.length);
			
			if (!topics.length) {
				console("404: "+msg);
				cache[msg] = false; // cache that we didn't find it
			}
		}
	};

	// Convey a message object to the user...
	var consoleTokens = ["player","name"];
	report.playMessage = function(msg,obj) {
		if (!msg) return;
		// get message data
		var data = report.msg[msg];
		// when can this message next be processed?
		throttle[msg] = gameTime + ("despam" in data ? data.despam : 2000);
		// process audio
		if ("ogg" in data) {
			var audio = data.ogg;
			if (audio instanceof Array) {
				audio = audio[getRandomInt(0,audio.length-1)]; // grab a random file name from array
				//console("picked random audio: "+audio);
			}
			(obj && "z" in obj) ? playSound(audio+".ogg", obj.x, obj.y, obj.z) : playSound(audio+".ogg");
		}
		// process console
		if ("msg" in data) {
			msg = data.msg;
			if (obj) for (token in consoleTokens) if (token in obj) msg = msg.split("%"+token+"%").join(obj[token]);
			console(msg);
		}
	}

	// /////////////////////////////////////////////////////////////////
	// PLAYER LIVING REPORTS - report.playerAlive()
	
	// Player status updates
	report.playerAlive = function(player, alive) {
		var obj = {player: player};
		report("player."+(alive ? "join" : "leave"),obj);
	}	

	// /////////////////////////////////////////////////////////////////
	// POWER STATUS REPORTS - report.power()
	
	// Note: Ally power is handled in rules.js
	
	// Check power status and report any issues...
	report.power = function() {
		if (playerPower(selectedPlayer) < 100)		report("hint.power.low.Me");
		if (generatorRequired())					report("hint.power.generator.Me");
	}
	
	// /////////////////////////////////////////////////////////////////
	// RESEARCH REPORTS - report.researched(research[,structure])
	
	// Researched something...
	report.researched = function(research,obj) {
		queue("checkPowerStatus"); // deferred oil/power status check

		// messages currently hard-coded in to Warzone C++ code - see http://forums.wz2100.net/viewtopic.php?f=32&t=9419
	}

	// /////////////////////////////////////////////////////////////////
	// DETECTION REPORTS - report.objectSeen(by,obj)

	// Sensors detect something...
	report.objectSeen = function(by, obj) {
		if (isWallOrGate(obj)) return; // ignore walls/gates
		report("objectSeen."+(report.objectSeen.custom(by, obj) || classify(obj)), obj);
	}
	// custom detection reports...
	report.objectSeen.custom = function(by, obj) {
		if (by.type != DROID) return false;
		// custom: our droid detects a nexus tower/turret
		if (isNexus(obj)) return "custom."+(obj.type == DROID ? "droid" : "defense")+".nexus";
		if (isFixVTOL(by)) {
			if (by.order == DORDER_PATROL || by.order == DORDER_CIRCLE) {
				// custom: our circling/patrol vtol spots enemy
				return "custom."+(obj.type == DROID ? "locked" : "attack");
			} else if (isFixVTOL(obj)) {
				// custom: our non-circling/patrol vtol spots enemy vtol
				return "custom.vtol";
			} else {
				// custom: our non-circling/patrol vtol spots any other enemy object
				return "custom.located";
			}
		}
		// custom: our droid spots structure
		if (obj.type == STRUCTURE) return "custom."+(obj.stattype == DEFENSE ? "defense" : "base");
		if (obj.type == DROID) {
			// custom: our droid spots enemy transport
			if (isTransport(obj)) return "custom.droid.transport";
			// custom: our droid spots enemy droid
			return "custom.droid";
		}
		// custom: our droid detects a feature
		if (obj.type == FEATURE) return "custom"+classify.feature(obj);
		return false;
	}
	
	// Structure built...
	report.structureBuilt = function(obj, droid) {
		queue("checkPowerStatus"); // deferred oil/power status check
		report("structureBuilt."+classify(obj), obj);
	}
	
	// Structure ready...
	report.structureReady = function(obj) {
		report("structureReady."+classify(obj), obj);
	}
	
	// Droid built...
	report.droidBuilt = function(obj) {
		queue("checkPowerStatus"); // deferred oil/power status check
		report("droidBuilt."+classify(obj), obj);
	}
	
	// Droid idle...
	report.droidIdle = function(obj) {
		// not currently used
	}
	
	// We're under attack...
	report.attacked = function(obj, attacker) {
		report("attacked."+(report.attacked.custom(obj, attacker) || classify(obj)), obj);
	}
	// custom attacked reports...
	report.attacked.custom = function(obj, attacker) {
		if (obj.type != DROID) return false;
		// custom: vtols attacked while fleeing the battlefield
		if (isFixVTOL(obj)) {
			if (!!attacker && isAntiAir(attacker)) return "custom.abort";
			if (isRepairing(obj)) return "custom.rearm";
			if (isRetreating(obj)) return "custom.return";
		} else {
			if (isRepairing(obj)) return "custom.repairs";
			if (isRetreating(obj)) return "custom.retreat";
		}
		return false;
	}
	
	// Something destroyed...
	report.destroyed = function(obj) {
		if (isWallOrGate(obj)) return; // ignore walls/gates
		report("destroyed."+(report.destroyed.custom(obj) || classify(obj)), obj);
	}
	// custom destroyed reports...
	report.destroyed.custom = function(obj) {
		if (obj.type == DROID && isRecycling(obj)) return "custom.recycle";
		if (obj.type == STRUCTURE && isDemolished(obj)) return "custom.demolish";
		return false;
	}
	
	// Objects transferred between players...
	report.objectTransfer = function(obj,from) {
		if (from == selectedPlayer) { // I lost or transferred a unit
			return report ("objectTransfer.droid.to", obj);
		} else { // I gained a unit
			return report ("objectTransfer.droid.from", setPlayer(obj,from));
		}
	}

	// /////////////////////////////////////////////////////////////////
	// VARIOUS HELPER FUNCTIONS

	// random integer between min and max
	var getRandomInt = function(min, max) {  
		return Math.floor(Math.random() * (max - min + 1)) + min;  
	} 
	
	// Return object with new player set
	var setPlayer = function(obj,player) {
		var newObj = {};
		for (var i in obj) newObj[i] = obj[i];
		newObj.player = player;
		return newObj;
	}
		
	// Determine if player is ally...
	var isAlly = function(player,my) {
		return (player == selectedPlayer || allianceExistsBetween(player, selectedPlayer));
	}

	// helper function for nameContains()
	var inNameOf = function(turret) {
		return (this.name.indexOf(turret) != -1);
	}
	// Determine if a game object's name contains one of the strings in passed in array
	// This is a crufty way to classify objects should more advanced API features be unavailable
	var nameContains = function(obj, list) {
		return list.some(inNameOf, obj);
	}

	// A work-around for buggy JS API isVTOL function
	var isFixVTOL = function(obj) {
		if (obj.type != DROID) return;
		try {
			return ( ("isVTOL" in obj && obj.isVTOL) || isVTOL(obj) );
		} catch(e) {
			console("isFixVTOL(): "+e.message);
		}
	}

	// Deterine if a game object is a transporter...
	var isTransport = function(obj) {
		return (obj.type == DROID && (obj.droidType == DROID_TRANSPORTER || obj.droidType == DROID_SUPERTRANSPORTER));
	}

	// Determine if a game object has CB sensor... (ignoring wide spectrum sensor)
	var isCB = function(obj) {
		return ( ("isCB" in obj && obj.isCB) || (obj.name.indexOf("CB") != -1) );
	}

	// Determine if a game object has sensors...
	var sensorTurrets = ["Sensor","CB","Radar","Satellite","Strike"];
	var isSensor = function(obj) {
		if (obj.type == DROID) return (obj.droidType == DROID_SENSOR);
		if (obj.type == STRUCTURE) {
			if ("isSensor" in obj) { // wz3.2
				return (obj.isSensor || obj.isCB || obj.isRadarDetector);
			} else { // wz3.1
				return nameContains(obj, sensorTurrets);
			}
		}
	}

	// Determine if a game object has sensors...
	var antiairTurrets = ["Hurricane","Whirlwind","Stormbringer","Avenger","Flak"];
	var isAntiAir = function(obj) {
		if ("canHitAir" in obj) { // wz3.2
			return obj.canHitAir;
		} else { // wz3.1
			// yes I know it's not full list of turrets that can hit air, but it is enough for purpose of this script
			return nameContains(obj, antiairTurrets);
		}
	}

	// Determine if a game object has long-range weapon...
	var batteryTurrets = ["Mortar","Bombard","Pepper","Howitz","Shaker","Hellstorm"];
	var isBattery = function(obj) {
		if ("hasIndirect" in obj) { // wz3.2
			return (obj.hasIndirect && obj.range > 9); // I assume .range is in tiles?
		} else { // wz3.1
			return nameContains(obj, batteryTurrets);
		}
	}

	// Determine if a game object has a missile weapon
	var missileTurrets = ["Missile","Ripple","HvART","MdART","ArtMiss","Atmiss"];
	var isMissile = function(obj) {
		// yes, I know ripple rockets aren't missiles, but the sentiment is the same
		return nameContains(obj, missileTurrets);
	}

	// Determine if a game object has a nexus weapon
	var nexusTurrets = ["SpyTower","SpyTurret","NEXUS","Nexus"];
	var isNexus = function(obj) {
		// yes, I know ripple rockets aren't missiles, but the sentiment is the same
		return nameContains(obj, nexusTurrets);
	}

	// Determine if a game object is a construction droid
	var isTruck = function(obj) {
		return (obj.type == DROID && obj.droidType == DROID_CONSTRUCT);
	}

	// Determine if a game object can repair droids
	var isRepair = function(obj) {
		return ( (obj.type == DROID && obj.droidType == DROID_REPAIR) || (obj.type == STRUCTURE && obj.stattype == REPAIR_FACILITY) );
	}

	// Determine if a game object is a construction droid
	var isCyborg = function(obj) {
		return (obj.type == DROID && obj.droidType == DROID_CYBORG);
	}

	// Determine if a game object is a command droid
	var isCommander = function(obj) {
		return (obj.type == DROID && obj.droidType == DROID_COMMAND);
	}

	// Determine if a game object has an ECM (radar jammer) turret
	var isECM = function(obj) {
		if (obj.type == DROID) return (obj.droidType == DROID_ECM);
		if (obj.type == STRUCTURE) return (obj.name.indexOf("Jammer") != -1 || obj.name.indexOf("ECM") != -1);
	}

	// Determine if a game object is a person
	var isPerson = function(obj) {
		return (obj.type == DROID && obj.droidType == DROID_PERSON);
	}

	// Determine if a DROID oject is retreating...
	var isRetreating = function(obj) {
		return (obj.order == DORDER_RTB || obj.order == DORDER_REARM);
	}

	// Determine if a DROID object is returning for repair...
	var isRepairing = function(obj) {
		return (obj.order == DORDER_RTR || obj.order == DORDER_RTR_SPECIFIED || obj.order == DORDER_REARM);
	}

	// Determine if a DROID object is being recycled...
	var isRecycling = function(obj) {
		return (obj.order == DORDER_RECYCLE);
	}

	// Determine if a STRUCTURE object is being demolished
	var isDemolished = function(obj) {
		return (!obj.status); // status == 0 if structure was demolished
	}

	// Determine if a STRUCTURE object is a wall or gate...
	var isWallOrGate = function(obj) {
		return (obj.type == STRUCTURE && (obj.stattype == WALL || obj.stattype == GATE));
	}

	// Determine if we need more power generators...
	var generatorRequired = function() {
		return (enumStruct(selectedPlayer,RESOURCE_EXTRACTOR).length > enumStruct(selectedPlayer,POWER_GEN).length*4);
	}

	// /////////////////////////////////////////////////////////////////
	// RETURN PUBLIC FUNCTION SO OTHER SCRIPTS CAN ACCESS IT
	
	return report;
})();