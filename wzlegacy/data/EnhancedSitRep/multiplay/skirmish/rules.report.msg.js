/*

HOW TO ADD / CUSTOMISE MESSAGE REPORTS

The report.msg object below lists all the report messages that have been defined.

Each report is referenced by a message key, such as "objectSeen.droid.transport.Scav".

The bit on the end defines the faction that an object belongs to and can be:

* ".Scav"  = scavenger faction
* ".Enemy" = any enemy player (human or AI)
* ".Ally"  = any of your allies (human or AI)
* ".Me"    = you!
* ""       = no faction, eg. for features.

This lets you play different message based on the faction.

When a report is triggered, a message key will usually be created by the code in rules.report.js,
then it looks in report.msg object in this file to determine which message to play.

If the message key isn't found, the non-faction bit of it will be reduced one chunk at a time
until something is found (or not).

For example, if the message key is "objectSeen.base.factory.vtol.Enemy" and that key isn't found,
then each one of the following reductions will be checked in turn until a message is found:

* objectSeen.base.factory.Enemy
* objectSeen.base.Enemy
* objectSeen.Enemy

Once a message is found, it will be played. If no message is found, nothing will be played.
The message key searching is cached so subsequent calls to the associated report will be
much faster.

Here's a list of common game events (the first bit of the message key):

* researched.    - this is currently disabled
* objectSeen.    - something detected on sensors
* structureBuilt - you just built a structure
* structureReady - special feature of a sturcture is ready (eg. laser satellite ready)
* droidBuilt     - you just produced a unit
* droidIdle      - this is currently disabled
* attacked       - something you own was attacked
* destroyed      - something you own was destroyed
* objectTransfer - an object was transferred betweeen you and someone else

Each of those events has a game object associated with it - a droid, structure or feature.
Structures are split between "defense" or "base" to make message key defaults more flexible.

So, after the event name, you can have one of the following:

* droid   - a unit
* defense - a defensive structure (wall, tower, emplacement, etc)
* base    - a base structure (HQ, factory, oil derrick, etc)
* feature - a map feature (tree, skyscraper, oil resource, artefact, etc)

The object is then further classified - like the VTOL factory above was depicted as
"<event>.base.factory.vtol.<faction>". These classifications depend on the object type...

DROIDS:   droid.vtol, droid.truck, droid.repair, droid.transport, droid.command, droid.missile,
          droid.battery, droid.cyborg, droid.sensor, droid.nexus, droid.ecm, droid.person, droid.weapon

DEFENSES: defense.sensor, defense.antiair, defense.missile, defense.battery, defense.nexus. defense.weapon

BASES:    base.command, base.factory.cyborg, base.factory.tank, base.factory.vtol, base.gate, base.hq,
          base.lassat, base.generator, base.rearm, base.repair, base.research, base.derrick, base.uplink,
          base.wall
          
Note: "base.gate" and "base.wall" are excluded from the "objectSeen" and "destroyed" events.

FEATURES: feature.artefact, feature.resource, feature.barrel (oil barrel), feature.<name>

Note: If a feature doesn't have a friendly classification defined in rules.report.js then it's
      internal name will be used.

Each message key has an associated object that defines what happens when the message is played.

The object has the following properties:

* "ogg"    - If specified, an audio file will be played. Specify a file name or an array of file
             names. If you specify an array, one of the files will be played at random.
             Audio must be correctly configured for it to play - for more information see:
             https://warzone.atlassian.net/wiki/pages/viewpage.action?pageId=6586402
* "msg"    - If specified, a console message will be displayed. You can use the "%player%" token
             to display the player ID associated with the message. You can use the "%name%" token
             to display the name of the object assoiciated with the message.
* "despam" - By default a message will be throttled so that it can only be played once every
             two seconds. You can override the throttle time using the "despam" property - the
             time should be specified in milliseconds, eg. 120000 is two minutes.

If you want to block message key searching (eg. to prevent a message being played for a certain
message key) omit the "ogg" and "msg" properties and set a high "despam" value.

So, assuming you know the event and can deduce the object classifcation and faction from what's
written above, ou should now be able to add and customise messages in the "report.msg" object
below (:

*/

// used as despam value when you don't want the message played again
const TOMORROW = 1000 * 60 * 60 * 24; // 24 hours
// used as ogg value to indicate where sounds are needed
const TODO = "beep8";

report.msg = {

	// /////////////////////////////////////////////////////////////////
	// LEVEL START REPORTS - report.startLevel()

	// when the game starts
	"startLevel.player":						{ogg:"pcv476", msg:"Entering Warzone!"}, // entering warzone
	"startLevel.spectator":						{ogg:"pcv413", msg:"**** SPECTATOR COMMAND CONSOLE ACTIVATED ****"}, // command console activated
	
	// /////////////////////////////////////////////////////////////////
	// PLAYER LIVING REPORTS - report.playerAlive()
	
	// when a player is revived by an ally
	"player.join.Me":							{ogg:"pcv476", msg:"You're back in the game!"}, // entering warzone
	"player.join.Ally":							{xogg:TODO, msg:"Ally %player% has been revived!"}, // ally entering warzone
	"player.join.Enemy":						{xogg:TODO, msg:"Enemy %player% has been revived!"}, // enemy entering warzone
	// when a player is killed by their enemy
	"player.leave.Me":							{ogg:"pcv470", msg:"You have been defeated!"}, // you are defeated
	"player.leave.Ally":						{xogg:TODO, msg:"Player %player% needs a truck!"}, // ally eradicated
	"player.leave.Enemy":						{ogg:"pcv646", msg:"Enemy %player% eradicated!"}, // enemy eradicated
	"player.leave.Scav":						{xogg:TODO, msg:"Scavengers eradicated!"}, // scavengers eradicated
	
	// /////////////////////////////////////////////////////////////////
	// PLAYER HINTS - eg. via report.power()
	
	// low power
	"hint.power.low.Me":						{ogg:"pcv343", despam:6000}, // power low
	"hint.power.low.Ally":						{xogg:TODO, msg:"Player %player% needs power", despam:120000}, // ally power low
	// generators needed
	"hint.power.generator.Me":					{ogg:"pcv349", despam:16000}, // power generator required
	
	// /////////////////////////////////////////////////////////////////
	// RESEARCH REPORTS - report.researched(research[,structure])
	
	// currently hard-coded in to Warzone C++ code - see http://forums.wz2100.net/viewtopic.php?f=32&t=9419
	
	// /////////////////////////////////////////////////////////////////
	// DETECTION REPORTS - report.objectSeen(by,obj)
	
	// deafult detection messages (rarely heard)
	"objectSeen.Ally":							{ogg:"pcv380", despam: TOMORROW}, // ally detected
	"objectSeen.Enemy":							{ogg:"nmedeted", despam: 120000}, // enemy detected
	"objectSeen.Scav":							{ogg:"pcv373", despam: 60000}, // scavengers detected
	// droids
	"objectSeen.droid.Enemy":					{ogg:"pcv378"}, // enemy unit detected
	"objectSeen.droid.Scav":					{ogg:"pcv373"}, // scavengers detected
	"objectSeen.droid.vtol.Enemy":				{ogg:"pcv388"}, // enemy vtols detected
	"objectSeen.droid.nexus.Enemy":				{ogg:"pcv386"}, // nexus unit detected
	"objectSeen.droid.nexus.Scav":				{ogg:"pcv386"}, // nexus unit detected
	"objectSeen.droid.transport.Enemy":			{ogg:"pcv381"}, // enemy transport detected
	"objectSeen.droid.transport.Scav":			{ogg:"pcv634"}, // incoming air strike
	"objectSeen.droid.battery.Enemy":			{ogg:"pcv387"}, // enemy battery detected
	"objectSeen.driod.missile.Enemy":			{xogg:TODO}, // enemy missile detected
	// base structures
	"objectSeen.base.Ally":						{xogg:TODO, despam: TOMORROW}, // ally base detected
	"objectSeen.base.Enemy":					{ogg:"pcv379", despam:5000}, // enemy base detected
	"objectSeen.base.Scav":						{ogg:"pcv374"}, // scavenger base detected
	"objectSeen.base.lassat.Ally":				{xogg:TODO}, // ally laser satellite detected
	"objectSeen.base.lassat.Enemy":				{xogg:TODO, msg:"WARNING: Enemy has Laser Satellite"}, // warning: enemy laser satellite detected
	// defense structures
	"objectSeen.defense.Enemy":					{xogg:TODO}, // enemy outpost detected
	"objectSeen.defense.Scav":					{ogg:"pcv375"}, // scavener outpost detected
	"objectSeen.defense.nexus.Enemy":			{ogg:"pcv385"}, // nexus tower detected
	"objectSeen.defense.nexus.Scav":			{ogg:"pcv385"}, // nexus tower detected
	"objectSeen.defense.sensor.Enemy":			{xogg:TODO},
	"objectSeen.defense.battery.Enemy":			{ogg:["pcv387","pcv406"]}, // enemy battery detected || enemy battery located
	"objectSeen.defense.missile.Enemy":			{xogg:TODO},
	"objectSeen.defense.antiair.Enemy":			{xogg:TODO},
	// features
	"objectSeen.feature.artefact": 				{ogg:"pcv377"}, // artefeact detected
	"objectSeen.feature.resource": 				{ogg:"pcv376"}, // resource detected
	"objectSeen.feature.barrel":				{ogg:"pcv376"}, // resource detected
	// custom: our droid detects a nexus tower/turret
	"objectSeen.custom.droid.nexus.Enemy":		{ogg:["com033","com034"], despam:4000}, // nexus turret detected || nexus detected (enemy nexus droid)
	"objectSeen.custom.defense.nexus.Enemy":	{ogg:["com032","com034"], despam:4000}, // nexus tower detected || nexus detected (enemy nexus droid)
	// custom: our circling/patrol vtol spots enemy
	"objectSeen.custom.locked.Enemy":			{ogg:["v-locon1","v-locon2","v-locon3"], despam:4000}, // locked on (enemy droid)
	"objectSeen.custom.locked.Scav":			{ogg:["v-locon1","v-locon2","v-locon3"], despam:4000}, // locked on (scav droid)
	"objectSeen.custom.attack.Enemy":			{ogg:["v-atkrn3","v-atkrn2"], despam:4000}, // commencing attack run (enemy structure)
	"objectSeen.custom.attack.Scav":			{ogg:["v-atkrn3","v-atkrn2"], despam:4000}, // commencing attack run (scav structure)
	// custom: our non-circling/patrol vtol spots enemy vtol
	"objectSeen.custom.vtol.Enemy":				{ogg:"com036", despam:4000}, // enemy vtols detected (enemy vtol)
	// custom: our non-circling/patrol vtol spots any other enemy object
	"objectSeen.custom.located.Enemy":			{ogg:["v-eloc1","v-eloc2","v-eloc3"], despam:4000}, // enemy located (enemy object)
	// custom: our droid spots structure
	"objectSeen.custom.base.Enemy":				{ogg:"com027", despam:4000}, // enemy base detected (base structure)
	"objectSeen.custom.base.Scav":				{ogg:"com022", despam:4000}, // scavenger base detected (base structure)
	"objectSeen.custom.defense.Enemy":			{ogg:"com037", despam:4000}, // route obstructed (defense structure)
	"objectSeen.custom.defense.Scav":			{ogg:"com023", despam:4000}, // scavenger outpost detected (defense structure)
	// custom: our droid spots enemy transport
	"objectSeen.custom.droid.transport.Enemy":	{ogg:"com029", despam:4000}, // enemy transport detected (droid)
	// custom: our droid spots enemy droid
	"objectSeen.custom.droid.Enemy":			{ogg:"com026", despam:4000}, // enemy detected (droid)
	"objectSeen.custom.droid.Scav":				{ogg:"com021", despam:4000}, // scavengers detected (droid)
	// custom: our droid detects a feature
	"objectSeen.custom.resource":				{ogg:"com024", despam:4000}, // resource detected
	"objectSeen.custom.artefact":				{ogg:"com025", despam:4000}, // artefact detected
	
	// /////////////////////////////////////////////////////////////////
	// CONSTRUCTION REPORTS - report.structureBuilt(structure[,droid])
	
	// default construction message
	"structureBuilt.Me":						{ogg:"pcv336"}, // construction completed
	// base structures
	"structureBuilt.base.hq.Me":				{ogg:"beep2"}, // (sound effect)
	"structureBuilt.base.lassat.Me":			{ogg:"lascomp"}, // laser satellite completed
	"structureBuilt.base.uplink.Me":			{ogg:"uplink"}, // (sound effect)
	"structureBuilt.base.research.Me":			{}, // (currently hard-coded in to warzone)
	"structureBuilt.base.generator.Me":			{}, // (currently hard-coded in to warzone)
	"structureBuilt.base.derrick.Me":			{}, // (currently hard-coded in to warzone)
	"structureBuilt.base.factory.tank.Me":		{}, // (currently hard-coded in to warzone)
	// defense structures
	"structureBuilt.defense.Me":				{ogg:"pcv607"}, // defensive structure completed
	"structureBuilt.defense.sensor.Me":			{ogg:"ecmtower"}, // (sound effect)
	"structureBuilt.defense.sensor.cb.Me":		{ogg:"cbcomp"}, // counter battery radar completed
	"structureBuilt.defense.battery.Me":		{ogg:"batcomp"},
	"structureBuilt.defense.missile.Me":		{ogg:"pcv656"}, // missile silo
	"structureBuilt.defense.antiair.Me":		{xogg:TODO},
	
	// /////////////////////////////////////////////////////////////////
	// READY STATUS REPORTS - report.structureReady(structure)
	
	"structureReady.base.lassat.Me":			{ogg:"lasactiv"}, // laser satellite activated
	
	// /////////////////////////////////////////////////////////////////
	// PRODUCTION REPORTS - report.droidBuilt(droid[,structure])
	
	// default production messages
	"droidBuilt.Me":							{ogg:"pcv367"}, // production completed
	"droidBuilt.droid.Me":						{ogg:"pcv367"}, // production completed
	// specific production messages
	"droidBuilt.droid.truck.Me":				{xogg:TODO}, // truck production completed
	"droidBuilt.droid.command.Me":				{xogg:TODO}, // commander production completed
	"droidBuilt.droid.sensor.Me":				{xogg:TODO}, // sensor production completed
	"droidBuilt.droid.cyborg.Me":				{ogg:"pcv610"}, // cyborg production completed
	"droidBuilt.droid.vtol.Me":					{ogg:"com041"}, // heading to rally point

	// /////////////////////////////////////////////////////////////////
	// IDLE REPORTS - report.droidIdle(droid)
	
	// not currently used
	
	// /////////////////////////////////////////////////////////////////
	// ATTACK REPORTS - report.attacked(victim,attacker)
	
	// unknown attacks on me
	"attacked.Me":								{xogg:TODO},
	// base attacks
	"attacked.base.Me":							{ogg:"pcv337"}, // structure under attack
	"attacked.base.derrick.Me":					{ogg:"pcv345"}, // derrick under attack
	// defense attacks
	"attacked.defense.Me":						{ogg:"pcv337", despam:4000}, // structure under attack
	// droid attacks
	"attacked.droid.Me":						{ogg:"pcv399"}, // unit under attack
	"attacked.droid.transport.Me":				{ogg:"pcv443"}, // transport under attack
	"attacked.droid.vtol.Me":					{ogg:"pcv410"}, // vtols engaging
	// custom: vtols attacked while fleeing the battlefield
	"attacked.custom.abort.Me":					{ogg:["v-abtrn2","v-abtrn2"], despam:5000}, // aborting attack run (vtol attacked by antiair)
	"attacked.custom.rearm.Me":					{ogg:"pcv409", despam:3000}, // re-arming (vtol attacked when returning to rearm)
	"attacked.custom.return.Me":				{ogg:["v-retba2","v-retba3"], despam:3000}, // returning to base (vtol attacked when returning to base)
	// custom: tanks attacked while fleeing the battlefield
	"attacked.custom.repairs.Me":				{ogg:["pcv402","com040"], despam:3000}, // unit returning for repair || returning for repair (tank attacked when RTR)
	"attacked.custom.retreat.Me":				{ogg:"pcv401"}, // unit retreating (non-vtol droid attacked when retreating)
	
	// /////////////////////////////////////////////////////////////////
	// DESTROYED REPORTS - report.destroyed(obj)
	
	// droid destroyed
	"destroyed.droid.Me":						{ogg:"pcv400"}, // unit destroyed
	// base structure destroyed
	"destroyed.base.Me":						{ogg:"pcv620"}, // structure destroyed
	"destroyed.base.derrick.Me":				{ogg:"pcv346"}, // derrick destroyed
	"destroyed.base.hq.Me":						{ogg:"gmeshtdn"}, // (sound effect)
	"destroyed.base.lassat.Me":					{ogg:"pcv651a"}, // laser satellite disabled
	"destroyed.base.generator.Me":				{ogg:"pcv342"}, // power generator destroyed
	"destroyed.base.uplink.Me":					{xogg:TODO}, // satellite uplink disabled
	// defense structure destroyed
	"destroyed.defense.Me":						{ogg:"pcv620"}, // structure destroyed
	"destroyed.defense.sensor.Me":				{xogg:TODO},
	"destroyed.defense.battery.Me":				{xogg:TODO},
	"destroyed.defense.missile.Me":				{xogg:TODO},
	"destroyed.defense.antiair.Me":				{xogg:TODO},
	// feature object destroyed
	"destroyed.feature.barrel.Me":				{xogg:"power-transferred"}, // power transferred
	"destroyed.feature.resource.Me":			{xogg:"pcv335"}, // construction started
	// custom: recycling/demolition
	"destroyed.custom.recycle.Me":				{ogg:"pcv428", despam:3000}, // recycling
	"destroyed.custom.demolish.Me":				{ogg:"pcv340", despam:4000}, // structure demolished
	
	// /////////////////////////////////////////////////////////////////
	// TRANSFER REPORTS - report.objectTransfer(obj,from)
	
	"objectTransfer.droid.from.Ally":			{ogg:"pcv482"}, // gift received
	"objectTransfer.droid.to.Ally":				{ogg:"pcv486"}, // units transferred
	
	"objectTransfer.droid.from.Enemy":			{ogg:"pcv617", msg:"You captured an enemy %name% droid!"}, // unit captured
	"objectTransfer.droid.to.Enemy":			{ogg:["untnut","untabsrd"], msg:"Player %player% stole a %name% droid!"}, // unit neutralised/absorbed (nexus voice)
	
	"objectTransfer.droid.from.Scav":			{ogg:"pcv617", msg:"You captured a scavenger %name% droid!"}, // unit captured
	"objectTransfer.droid.to.Scav":				{ogg:["untnut","untabsrd"], msg:"Scavengers stole a %name% droid!"} // unit neutralised/absorbed (nexus voice)
}