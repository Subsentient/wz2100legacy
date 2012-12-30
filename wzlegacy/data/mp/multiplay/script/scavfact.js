// Subsentient Scavenger AI System (SSAS) v1.3
// Designed to present a more powerful opposition than the standard scavenger AI, with a wink of superior intelligence.

// Various constants, declared here for convenience only
const maxDroids = 25;		// max guys to handle.

// scav group
var attackGroup;
var lastAttack = 0;

function activateProduction(fac)
{
	// Remind factory to produce
	if (structureIdle(fac))
	{
		switch (Math.floor(Math.random() * 10))
		{ //Give us variety in our production.
		case 0:	buildDroid(fac, "Bloke", "B1BaBaPerson01", "BaBaLegs", "", DROID_PERSON, "BaBaMG"); break;
		case 1: buildDroid(fac, "Buggy", "B3body-sml-buggy01", "BaBaProp", "", DROID_WEAPON, "BuggyMG"); break;
		case 2: buildDroid(fac, "Jeep", "B2JeepBody", "BaBaProp", "", DROID_WEAPON, "BJeepMG"); break;
		case 3: buildDroid(fac, "Rocket Jeep", "B2RKJeepBody", "BaBaProp", "", DROID_WEAPON, "BabaRocket"); break;
		case 4: buildDroid(fac, "Cannonbus", "BusBody", "BaBaProp", "", DROID_WEAPON, "BusCannon"); break;
		case 5: buildDroid(fac, "Firebus", "BusBody", "BaBaProp", "", DROID_WEAPON, "BabaFlame"); break;
		default: buildDroid(fac, "Trike", "B4body-sml-trike01", "BaBaProp", "", DROID_WEAPON, "bTrikeMG"); break;
		}
	}
}

// Regularly check back on our scavs
function scavtick()
{
	// enum functions now return a list of results
	var factorylist = enumStruct(me, "A0BaBaFactory");

	// one way of dealing with lists is running a function on each member of the list
	if (factorylist)
	{
		factorylist.forEach(activateProduction);
	}

	if ((gameTime - lastAttack) > 30000) //Increase delay from 9 seconds to 30.
	{
		lastAttack = gameTime;

		// Return to nearest factory (ie base)
		var droidlist = enumGroup(attackGroup);

		if (droidlist && factorylist)
		{
			// another way of dealing with lists is to iterate over them
			// note, you must NOT use the for (... in ...) construct to iterate over an array of objects with properties!
			for (var i = 0; i < droidlist.length; i++)
			{
				var droid = droidlist[i];
				var current = 0;
				var closest = 9999;
				var clfac;		// starts undefined

				// Find closest factory; notice that we still have the factory list from earlier, which
				// saves us a few expensive scripting calls
				for (var j = 0; j < factorylist.length; j++)
				{
					var fact = factorylist[j];
					current = distBetweenTwoPoints(fact.x, fact.y, droid.x, droid.y);
					if (current < closest)
					{
						closest = current;
						clfac = fact;
					}
				}

				// If we found a factory, return to it. If clfac remains undefined, it evaluates false.
				if (clfac && droid.order != DORDER_ATTACK && droid.order != DORDER_PATROL) //Don't do anything if we already have an order.
				{
					orderDroidLoc(droid, DORDER_MOVE, clfac.x, clfac.y);
				}
			}
		}
	}
}

function eventGameInit()
{
	makeComponentAvailable("B4body-sml-trike01", me);
	makeComponentAvailable("B3body-sml-buggy01", me);
	makeComponentAvailable("B2JeepBody", me);
	makeComponentAvailable("B2RKJeepBody", me);
	makeComponentAvailable("BusBody", me);
	makeComponentAvailable("B1BaBaPerson01", me);
	makeComponentAvailable("BaBaProp", me);
	makeComponentAvailable("BaBaLegs", me);
	makeComponentAvailable("bTrikeMG", me);
	makeComponentAvailable("BuggyMG", me);
	makeComponentAvailable("BJeepMG", me);
	makeComponentAvailable("BusCannon", me);
	makeComponentAvailable("BabaFlame", me);
	makeComponentAvailable("BabaRocket", me);
	makeComponentAvailable("BaBaMG", me);
	attackGroup = newGroup();	// allocate a new group
	groupAddArea(attackGroup, 0, 0, mapWidth, mapHeight);
}

function eventStartLevel()
{
	scavtick();
	setTimer("scavtick", 15000);	// start a constant timer function
}

// deal with a droid being built by us
function eventDroidBuilt(droid, fac1)
{
	groupAddDroid(attackGroup, droid);

	// Build another
	activateProduction(fac1);
}

// watch for structures being attacked. Send the cavalry as required. If it's not a structure under attack, obliterate the enemy.
function eventAttacked(victim, attacker)
{
	if (victim && attacker && (gameTime - lastAttack) > 3000)
	//Simplify this, if it's a structure under attack, give it priority. If not, treat it as a small threat.
	{
		lastAttack = gameTime;
		var squadsize = 0;
		var baseUnderSiege = false;
		var factorylist = enumStruct(me, "A0BaBaFactory");
		var wholearmy = enumGroup(attackGroup);

		if (attacker.player == me) { //If we are shooting through a wall, don't send more on alert.
		return; }

		if (victim.type == STRUCTURE) { //If it's a structure, send 25 units, otherwise, only 10.
		 baseUnderSiege = true;
		 squadsize = 25; }

		else {
		 squadsize = 10; }

		if (baseUnderSiege && attacker.type == STRUCTURE) { //Oh no you did NOT just build an MG tower next to us!
		 squadsize = 50; }

		for (var i = 0; i < squadsize; i++)
		{
			var unit = wholearmy[i];

			if (typeof unit == 'undefined') { //Prevent nonexistant units from being handled, filling logs with poo.
			 squadsize++;
			 return; }

			if (unit.order != DORDER_ATTACK) {
			 if (attacker.health > 20) { //Don't go after a unit probably about to die. -Subsentient
			  orderDroidObj(unit, DORDER_ATTACK, attacker); } //SPARTA!!!! Kill the enemy!
			 else { //Stay on the lookout if our enemy is dying or dead for other enemies.
			  orderDroidLoc(unit, DORDER_PATROL, attacker.x, attacker.y); } }
			else {
			 squadsize++; }
			
		}

		if (factorylist && baseUnderSiege) //Accelerate production since we're under siege at our base.
		{
		factorylist.forEach(activateProduction);
		}


	}
}
