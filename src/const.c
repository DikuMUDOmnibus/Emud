/***************************************************************************
 * Emud  2.2 by Igor van den Hoven, Michiel Lange, and Martin Bethlehem.   *
 *                                                                         *
 * MrMud 1.4 by David Bills and Dug Michael.                               *
 *                                                                         *
 * Merc  2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael      *
 * Chastain, Michael Quan, and Mitchell Tse.                               *
 *                                                                         *
 * Original Diku Mud copyright (C) 1990 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeld, Tom Madsen, and Katje Nyboe.     *
 ***************************************************************************/

#include "emud.h"


const	struct	god_type 		god_table		[MAX_GOD]		=
{
	{
		"the Gods",
		"The sands of time slowly swirl until your return.",
		"This player likes living a peaceful life.",
		"You receive %d experience points.",
		"The favour of the Gods makes you stronger.",
		"The gods have retreived your soul from oblivion.",
		"The gods have forsaken you.",
		"Pure energy flows from $N's corpse, seeping into you.",
		"Pure energy flows from $N's corpse, seeping into $n.",
		"%s has recalled from the erratic frenzy of %s!",
		"{138}",
		" ",
		DAM_NONE,
		0, 0, 0
	},

	{
		"Demise",
		"Flames dance as Demise sizzles a goodbye awaiting your return.",
		"This player delights in Fire and follows Demise.",
		"The power of Fire triumphs, you gain %d experience points.",
		"The warmth of fire spreads through your body.",
		"Lord Demise has retreived your soul from the fires below.",
		"Demise beckons you to your destiny.",
		"Bolts of orange fire lift from $N's corpse, engulfing you.",
		"Bolts of orange fire lift from $N's corpse, engulfing $n.",
		"%s has recalled from the dark fury of %s!",
		"{118}",
		"D",
		DAM_FIRE,
		5, 1, 1
	},

	{
		"Manwe",
		"A bluebird sings sweetly as Manwe wishes you farewell and a swift return.",
		"This player loves the Air and follows Manwe.",
		"Manwe boosts your experience high up in the air : %d exp gained!",
		"Manwe comes to you in a storm.",
		"Your soul flies back to your body at the wish of Manwe.",
		"Manwe is too high for your to reach.",
		"Bolts of white lightning shoot from $N's corpse, crashing into you.",
		"Bolts of white lightning shoot from $N's corpse, crashing into $n.",
		"%s has recalled from the stormy anger of %s!",
		"{158}",
		"M",
		DAM_LIGHTNING,
		3, 3, 3
	},

	{
		"Hypnos",
		"Your mind swirls and is quiet as Hypnos whispers goodbye and sweet dreams.",
		"This player has a twisted mind and follows Hypnos.",
		"With the help of Hypnos, you gain %d experience points.",
		"Hypnos sharpens your mind.",
		"Trough a trick of the mind Hypnos places you back on earth.",
		"Hypnos twists your mind and has abandoned you.",
		"Bolts of power streams from $N's corpse, flowing into you.",
		"Bolts of power streams from $N's corpse, flowing into $n.",
		"%s has recalled from the mind of %s!",
		"{168}",
		"H",
		DAM_PSIONIC,
		1, 8, 1
	},

	{
		"Ulmo",
		"Peaceful waters flow as Ulmo bades you farewell and awaits your return.",
		"This player loves the Seas and follows Ulmo.",
		"Ulmo shows you the power of Water, you gain %d experience points.",
		"In water you delight.",
		"The waves bring your body back on earth at the command of Ulmo.",
		"Ulmo smites you back from underwater.",
		"Bolts of ice rush from $N's corpse, melting into you.",
		"Bolts of ice rush from $N's corpse, melting into $n.",
		"%s has recalled from the tidal anger of %s!",
		"{148}",
		"U",
		DAM_COLD,
		2, 5, 2
	},

	{
		"Gaia",
		"A lion stands watch as Gaia wishes you safe journeys and a safe return.",
		"This player loves the Earth and follows Gaia.",
		"The sovereignty of the Earth reigns supreme, you gain %d experience points.",
		"The power of the earth comes with Gaia.",
		"From the soil of Earth Gaia has retreived you and placed back on it.",
		"Gaia thinks you're on the right place on earth.",
		"Bolts of green energy rise from $N's corpse, seeping into you.",
		"Bolts of green energy rise from $N's corpse, seeping into $n.",
		"%s has recalled from the unearthly anger of %s!",
		"{128}",
		"G",
		DAM_LIFE,
		4, 2, 2
	},

	{
		"Iluvatar",
		"The force of Iluvatar embraces your soul to preserve you for your return.",
		"This player enjoys Life and follows Iluvatar.",
		"You feel more alife than before and gain %d experience points.",
		"The favour of Iluvatar makes you strong.",
		"Iluvatar gives your life back to you.",
		"Iluvatar is sleeping atm.",
		"The essence of Life flows from $N's corpse, seeping into you.",
		"The essence of Life flows from $N's corpse, seeping into $n.",
		"%s has recalled from the lifely anger of %s!",
		"{178}",
		"I",
		DAM_LIFE,
		3, 3, 3
	}

};

const	struct	sector_type	sector_table	[SECT_MAX] =
{
	{	"inside",
		2,
		7 + 0 * 8 + 0 * 128,
		SFLAG_INDOORS|SFLAG_NOWEATHER,
		50,
		25
	},

	{
		"city",
		2,
		5 + 7 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		30
	},
	{
		"field",
		3,
		0 + 3 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		25
	},
	{
		"forest",
		4,
		1 + 2 * 8 + 0 * 128,
		SFLAG_NONE,
		25,
		50
	},
	{
		"hills",
		5,
		0 + 7 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		40
	},
	{
		"mountain",
		6,
		4 + 7 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		40
	},
	{
		"lake",
		4,
		4 + 6 * 8 + 0 * 128,
		SFLAG_SWIM,
		50,
		25
	},
	{
		"river",
		4,
		7 + 6 * 8 + 0 * 128,
		SFLAG_SWIM,
		50,
		25
	},
	{
		"ocean",
		6,
		6 + 4 * 8 + 0 * 128,
		SFLAG_SWIM,
		50,
		25
	},
	{	"air",
		1,
		0 + 6 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		25
	},
	{	"desert",
		8,
		1 + 3 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		30
	},
	{
		"lava",
		10,	3 + 1 * 8 + 0 * 128,
		SFLAG_NONE,
		100,
		50
	},
	{
		"ethereal",
		1,
		2 + 0 * 8 + 0 * 128,
		SFLAG_NOWEATHER,
		100,
		50
	},
	{
		"astral",
		1,
		3 + 0 * 8 + 0 * 128,
		SFLAG_NOWEATHER,
		100,
		50
	},
	{
		"underwater",
		10,
		4 + 0 * 8 + 0 * 128,
		SFLAG_NOWEATHER,
		-100,
		75
	},
	{
		"underground",
		3,
		5 + 0 * 8 + 0 * 128,
		SFLAG_INDOORS|SFLAG_NOWEATHER,
		-50,
		50
	},
	{
		"deepearth",
		3,
		1 + 0 * 8 + 0 * 128,
		SFLAG_INDOORS|SFLAG_NOWEATHER,
		-50,
		50
	},
	{
		"road",
		1,
		3 + 7 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		25
	},
	{
		"swamp",
		6,
		2 + 3 * 8 + 0 * 128,
		SFLAG_NONE,
		25,
		50
	},
	{
		"beach",
		4,
		7 + 3 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		25
	},
	{
		"tundra",
		3,
		1 + 7 * 8 + 0 * 128,
		SFLAG_NONE,
		50,
		30
	},
	{
		"edge",
		10,
		0 + 0 * 8 + 1 * 128,
		SFLAG_NONE,
		0,
		25
	}
};

const	struct	race_type	race_table	[MAX_RACE]	=
{
	{
		"Human",
		{0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		 2,	 2,	 0,
		 0,	 0,	 0,
		"Enhanced healing, swim.",
		"Humans are the most numerous, plainest and flexible race in the realms.",
		 0,		10,
		 165,	6,
		 0,
		 0,
		 0,
		 0,
		 RSPEC_SWIM|RSPEC_FASTHEAL,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM|BODY_HEAD|BODY_TORSO|BODY_HIP|BODY_MOUTH
	},

	{
		"Elf",
		{-1,1,1,1,-1},
		{1,0,1,1,0,0,0,1},
		 3,	 1,	 1,
		-1,	 3,	 1,
		"Enhanced reviving.",
		"Tall, slender creatures with an affinity for the woods and nature, Elves may lack physical stature, but have superior agility, both in thought and movement.",
		 1,		5,
		 150,	6,
		 DAM_MAGIC,
		 0,
		 0,
		 DAM_LIFE,
		 RSPEC_FASTREVIVE,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM
	},

	{
		"Darkelf",
		{1,1,-1,-1,1},
		{0,0,0,0,1,1,1,0},
		 3,	 2,	 2,
		 1,	 0,	-1,
		"Glob of darkness.",
		"The dark cousin of the elf, the Drow live underground in a vast network of caverns and cities. They are lightning fast and posses dark powers.",
		 1,		5,
		 160,	6,
		 DAM_MAGIC,
		 0,
		 0,
		 DAM_LIGHT,
		 RSPEC_NONE,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM
	},

	{
		"Dwarf",
		{1,-1,0,-1,1},
		{0,0,1,1,0,1,0,1},
		 1,	 2,	2,
		 2,	-2,	0,
		"Magic resistance.",
		"Dwarves are relatively short, stocky and powerfully built creatures. Long lived and extremely sturdy, they are known famed for their hardiness.",
		 0,		4,
		 130,	4,
		 DAM_MAGIC|DAM_PSIONIC|DAM_EVIL|DAM_POISON,
		 0,
		 DAM_PIERCE,
		 DAM_LIGHT,
		 RSPEC_MAGIC_DEFENSE,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM|BODY_HEAD|BODY_TORSO
	},

	{
		"Gnome",
		{0,-1,1,1,0},
		{1,1,1,1,0,0,0,1},
		 1,	2,	 0,
		-1,	2,	-1,
		"Forage for food.",
		"A short and quirky race, gnomes are not the mightiest of races, though they are well versed in their studies of knowledges, magics and religions.",
		 0,		3,
		 65,		2,
		 DAM_MAGIC,
		 0,
		 0,
		 DAM_LIFE,
		 RSPEC_FORAGE,
		 BODY_FOOT|BODY_HAND
	},

	{
		"Orc",
		{1,1,-2,-1,1},
		{0,0,0,0,1,1,1,1},
		 2,	 1,	 1,
		 2,	-1,	-1,
		"Street fighting.",
		"Large and dull of wit, with short but broad bodies, their poor mental dexterity is offset by their iron constitution and impressive physical strength.",
		 0,		2,
		 180,	6,
		 DAM_THRUST,
		 0,
		 DAM_HOLY,
		 0,
		 RSPEC_BRAWLING,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM|BODY_HEAD|BODY_MOUTH
	},

	{
		"Griffon",
		{-2,0,0,1,1},
		{0,1,0,1,1,0,1,0},
		 3,	3,	0,
		 1,	0,	1,
		"Flying.",
		"Griffons are some kind of bird",
		 0,		4,
		 750,	16,
		 0,
		 0,
		 0,
		 0,
		 RSPEC_FLYING,
		 BODY_WING|BODY_BEAK|BODY_CLAW
	},

	{
		"Halfelf",
		{-1,1,1,1,-2},
		{1,0,0,0,0,0,1,1},
		 3,	1,	1,
		 0,	1,	1,
		"Swim.",
		"Halfelves are a blend between Humans and Elves.",
		 0,		2,
		 155,	6,
		 0,
		 0,
		 0,
		 0,
		 RSPEC_SWIM,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM
	},

	{
		"Ogre",
		{3,-1,-2,-1,1},
		{0,0,1,1,1,1,0,0},
		 1,	 4,	1,
		 3,	-1,	1,
		"Intimidate.",
		"Ogres are VERY rough...",
		 0,		4,	
		 400,	8,
		 DAM_BASH,
		 0,
		 DAM_HOLY,
		 0,
		 RSPEC_INTIMIDATE,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM|BODY_TORSO|BODY_HEAD
	},

	{
		"Hobbit",
		{-1,1,0,0,1},
		{1,1,0,0,0,0,1,1},
		 2,	 2,	 0,
		-1,	 1,	 1,
		"Sneaking barefoot.",
		"Hobbit's have furry feet.",
		 0,		3,
		 90,		3,
		 DAM_MAGIC,
		 0,
		 DAM_LIFE,
		 0,
		 RSPEC_NONE,
		 BODY_FOOT|BODY_HAND|BODY_LEG|BODY_ARM
	},

	{
		"Sprite",
		{-3,2,2,2,-3},
		{1,1,1,1,0,0,0,0},
		 3,	1,	0,
		 2,	3,	0,
		"Enhanced reviving, Flying, Pass door.",
		"Sprites are small creatures that delight in fun and magic.",
		 0,		2,
		 15,		1,
		 DAM_MAGIC|DAM_PSIONIC,
		 0,
		 DAM_BASH|DAM_POISON,
		 0,
		 RSPEC_FLYING|RSPEC_FASTREVIVE|RSPEC_PASS_DOOR,
		 BODY_FOOT|BODY_WING|BODY_EYE
	},

	{
		"Shade",
		{-3,3,0,1,0},
		{1,1,0,0,1,0,1,0},
		 4,	 0,	 2,
		 1,	 1,	 1,
		"Flying, Undead, Pass door.",
		"Shades are shady.",
		 1,		5,
		 180,	7,
		 DAM_PIERCE|DAM_SLICE|DAM_BASH|DAM_CHOP|DAM_THRUST,
		 DAM_ACID,
		 0,
		 DAM_LIGHT|DAM_FIRE,
		 RSPEC_FLYING|RSPEC_UNDEAD|RSPEC_PASS_DOOR,
		 BODY_EYE
	},

	{
		"Roc",
		{3,0,-1,-1,1},
		{0,1,1,1,0,1,0,0},
		 3,	3,	1,
		 1,	1,	1,
		"Flying.",
		"Rocs are rocky",
		 2,		6,
		 1200,	21,
		 0,
		 0,
		 0,
		 0,
		 RSPEC_FLYING,
		 BODY_WING|BODY_MOUTH|BODY_CLAW
	},

	{
		"Thri Kreen",
		{2,1,-1,1,-1},
		{0,0,0,1,0,0,1,1},
		 3,	 1,	 0,
		 2,	-3,	 2,
		"No rings, natural dual-wield keeping hold/shield.",
		"Thri Kreens are big ants",
		 2,	 	6,
		 180,	7,
		 DAM_THRUST|DAM_PSIONIC,
		 0,
		 0,
		 DAM_COLD,
		 RSPEC_AGE_AC|RSPEC_MULTI_ARMS,
		 BODY_CLAW|BODY_MOUTH|BODY_FOOT|BODY_LEG
	},

	{
		"Werewolf",
		{2,1,-2,-1,2},
		{0,1,0,0,1,1,0,1},
		 3,	 2,	2,
		 2,	-2,	0,
		"Undead, Ability to morph into a wolf.",
		"Werewolfs are humans that can change into a wolf.",
		 2,		6,
	 	 200,	6,
		 DAM_SLICE|DAM_BASH|DAM_REND|DAM_THRUST,
		 0,
		 DAM_LIGHT|DAM_FIRE,
		 0,
		 RSPEC_UNDEAD|RSPEC_VAMPIRIC,
		 BODY_HAND|BODY_FOOT|BODY_LEG|BODY_ARM|BODY_HEAD
	},

	{
		"Demon",
		{3,0,-3,0,3},
		{0,1,1,0,1,0,1,0},
		 2,	 2,	 2,
		 4,	-4,	-1,
		"Natural ability for flight and curse.",
		"Demons are cruel.",
		 3,		7,
		 225,	7,
		 0,
		 DAM_FIRE,
		 DAM_COLD,
		 DAM_COLD,
		 RSPEC_UNDEAD|RSPEC_FLYING|RSPEC_BRAWLING,
		 BODY_CLAW|BODY_WING|BODY_FOOT|BODY_MOUTH|BODY_LEG|BODY_ARM
	},

	{
		"Gargoyle",
		{3,-1,-2,1,2},
		{0,0,0,1,1,1,1,1},
		 2,	 2,	1,
		 3,	-2,	0,
		"Natural ability for flight and stoneskin.",
		"Gargoyles are some kind of race too...",
		 3,		7,
		 500,	7,
		 DAM_PSIONIC,
		 DAM_POISON|DAM_LIFE|DAM_PIERCE|DAM_SLICE|DAM_REND|DAM_THRUST,
		 0,
		 DAM_MAGIC|DAM_VIBE,
		 RSPEC_FLYING|RSPEC_AGE_AC,
		 BODY_WING|BODY_CLAW|BODY_FOOT|BODY_MOUTH|BODY_LEG|BODY_ARM
	},

	{
		"Wraith",
		{-1,-1,3,3,-1},
		{1,1,1,1,0,0,0,0},
		 1,	4,	 0,
		-2,	5,	-1,
		"Natural ability to passdoor and resist magic.",
		"Wraits are undead mages.",
		 3,		7,
		 90,		6,
		 DAM_PSIONIC,
		 DAM_MAGIC,
		 0,
		 DAM_LIGHT|DAM_FIRE,
		 RSPEC_UNDEAD|RSPEC_VAMPIRIC|RSPEC_MAGIC_DEFENSE|RSPEC_PASS_DOOR,
		 BODY_EYE
	},

	{
		"Troll",
		{5,-1,-4,-1,5},
		{0,0,0,1,1,1,0,1},
		 2,	 4,	 1,
		 4,	-3,	 0,
		"Fast Regenerate.",
		"Uhm...",
		 4,		8,
		 400,	8,
		 DAM_BASH|DAM_THRUST,
		 0,
		 0,
		 DAM_FIRE|DAM_LIGHT,
		 RSPEC_INTIMIDATE|RSPEC_AGE_DR|RSPEC_FASTHEAL|RSPEC_FASTREVIVE|RSPEC_BRAWLING,
		 BODY_HAND|BODY_FOOT|BODY_LEG|BODY_ARM|BODY_HEAD|BODY_TORSO
	},

	{
		"Vampire",
		{1,1,0,1,1},
		{0,1,0,0,1,0,1,0},
		 3,	 0,	 2,
		 3,	 1,	 0,
		"Regenerate by drinking victim's blood",
		"Vampires are some kind of Dracula (or is it the other way around ?)",
		 4,		8,
		 165,	6,
		 DAM_THRUST|DAM_BASH,
		 0,
		 0,
		 DAM_LIGHT|DAM_FIRE,
		 RSPEC_UNDEAD|RSPEC_FLYING|RSPEC_VAMPIRIC|RSPEC_AGE_HR,
		 BODY_HAND|BODY_FOOT|BODY_LEG|BODY_ARM|BODY_HEAD
	},

	{
		"Phoenix",
		{0,2,2,0,0},
		{0,0,0,1,0,0,1,1},
		 4,	0,	0,
		 2,	2,	2,
		"Can fly",
		"Phoenixes are firebirds",
		 4,		8,
		 600,	15,
		 DAM_FIRE,
		 DAM_FIRE,
		 DAM_COLD,
		 DAM_COLD,
		 RSPEC_FLYING|RSPEC_AGE_AC|RSPEC_EASY_FLEE|RSPEC_UNDEAD|RSPEC_FIREWALK,
		 BODY_WING|BODY_CLAW|BODY_MOUTH|BODY_EYE
	},

	{
		"Doppelganger",
		{-2,4,3,2,-1},
		{1,0,0,0,0,0,1,0},
		 3,	 0,	 1,
		 2,	 4,	 2,
		"polymorph.",
		"Shapeshifter..",
		 5,		9,
		 200,	7,
		 DAM_MAGIC,
		 DAM_MAGIC,
		 0,
		 0,
		 0,
		 BODY_HAND|BODY_FOOT|BODY_EYE|BODY_LEG|BODY_ARM
	},

	{
		"Arachnid",
		{3,3,2,-2,0},
		{0,1,1,1,1,0,0,0},
		 3,	1,	2,
		 3,	3,	3,
		"Some giant spider aka Shelob.",
		"Not sure yet.",
		 5,		9,
		 400,	8,
		 DAM_THRUST,
		 DAM_POISON|DAM_PSIONIC,
		 DAM_COLD,
		 DAM_FIRE,
		 RSPEC_AGE_AC,
		 BODY_PINCERS|BODY_SPINNARET|BODY_EYE
	},

	{
		"Titan",
		{3,-3,3,0,3},
		{0,1,1,1,0,0,0,1},
		 2,	 1,	 0,
		 4,	 2,	-4,
		"Aging Hitroll.",
		"Stood up against the gods, now are less mighty than they were.",
		 5,		9,
		 500,	10,
		 DAM_MAGIC,
		 DAM_PSIONIC,
		 0,
		 DAM_LIFE,
		 RSPEC_AGE_HR,
		 BODY_HAND|BODY_FOOT|BODY_LEG|BODY_ARM|BODY_HEAD|BODY_TORSO|BODY_HIP
	},

	{
		"Eagle",
		{0,6,2,2,0},
		{0,0,0,0,0,0,0,0},
		 5,	0,	1,
		 3,	3,	3,
		"They came from the skies above.",
		"The Eagles are the champions of Manwe",
		 10,		10,
		 1200,	21,
		 DAM_LIGHTNING,
		 DAM_LIGHTNING,
		 0,
		 0,
		 RSPEC_FLYING|RSPEC_INTIMIDATE|RSPEC_EASY_FLEE|RSPEC_MAGIC_DEFENSE|RSPEC_FASTHEAL|RSPEC_FASTREVIVE|RSPEC_AGE_AC|RSPEC_AGE_HR|RSPEC_AGE_DR,
		 BODY_WING|BODY_CLAW|BODY_MOUTH
	},

	{
		"Kraken",
		{2,0,0,6,2},
		{0,0,0,0,0,0,0,0},
		 4,	1,	2,
		 2,	5,	2,
		"They came from the depths of the ocean.",
		"The Krakens are the champions of Ulmo",
		 10,		10,
		 2500,	60,
		 DAM_COLD,
		 DAM_COLD|DAM_BASH|DAM_THRUST,
		 0,
		 0,
		 RSPEC_SWIM|RSPEC_INTIMIDATE|RSPEC_EASY_FLEE|RSPEC_MAGIC_DEFENSE|RSPEC_FASTHEAL|RSPEC_FASTREVIVE|RSPEC_AGE_AC|RSPEC_AGE_HR|RSPEC_AGE_DR|RSPEC_BREATH_WATER,
		 BODY_TENTACLE
	},

	{
		"Balrog",
		{6,2,0,0,2},
		{0,0,0,0,0,0,0,0},
		 4,	1,	2,
		 5,	1,	1,
		"They came from the fires below.",
		"Balrogs are the champions of Demise",
		 10,		10,
		 1800,	18,
		 DAM_FIRE,
		 DAM_FIRE,
		 0,
		 0,
		 RSPEC_INTIMIDATE|RSPEC_EASY_FLEE|RSPEC_MAGIC_DEFENSE|RSPEC_AGE_AC|RSPEC_AGE_HR|RSPEC_AGE_DR|RSPEC_PASS_DOOR|RSPEC_UNDEAD|RSPEC_FASTHEAL|RSPEC_FASTREVIVE,
		 BODY_WING|BODY_CLAW|BODY_FOOT|BODY_MOUTH
	},

	{
		"Dragon",
		{0,2,6,2,0},
		{0,0,0,0,0,0,0,0},
		 4,	1,	1,
		 1,	7,	1,
		"They came forth from your nightmares.",
		"Dragons are the champions of Hypnos",
		 10,	10,	1500,	21,
		 DAM_PSIONIC,
		 DAM_PSIONIC,
		 0,
		 0,
		 RSPEC_FLYING|RSPEC_ETHEREAL|RSPEC_ASTRAL|RSPEC_INTIMIDATE|RSPEC_MAGIC_DEFENSE|RSPEC_FASTHEAL|RSPEC_FASTREVIVE|RSPEC_AGE_AC|RSPEC_AGE_HR|RSPEC_AGE_DR,
		 BODY_EYE
	},

	{
		"Ent",
		{2,0,2,0,6},
		{0,0,0,0,0,0,0,0},
		 4,	1,	1,
		 4,	2,	2,
		"They grew from the earth.",
		"Ents are the champions of Gaia",
		 10,		10,
		 1000,	16,
		 DAM_LIFE,
		 DAM_LIFE,
		 0,
		 0,
		 RSPEC_INTIMIDATE|RSPEC_EASY_FLEE|RSPEC_MAGIC_DEFENSE|RSPEC_FASTHEAL|RSPEC_FASTREVIVE|RSPEC_AGE_AC|RSPEC_AGE_HR|RSPEC_AGE_DR,
		 BODY_BRANCH|BODY_TRUNK|BODY_ROOT
	}
};


const	struct	class_type	class_table	[MAX_CLASS]	=
{
	{
		"War", "Warrior", OBJ_VNUM_SCHOOL_SWORD,
		9809, 95, 18,  6,  15,20,   1, 3,
		"Basic, but strong and deadly fighters.",
		CSPEC_NOMANA|CSPEC_SHIELD_ALLOWED|CSPEC_NO_BRANDISH|CSPEC_NO_ZAP,
		APPLY_CON, APPLY_STR, 2, 2
	},

	{
		"Gla",  "Gladiator", OBJ_VNUM_SCHOOL_SPEAR,
		9766, 90, 18,  7,  12,17,   1, 5,
		"The famous and quick arenafighters.",
		CSPEC_NOMANA|CSPEC_NO_BRANDISH|CSPEC_NO_ZAP,
		APPLY_DEX, APPLY_CON, 2, 2
	},

	{
		"Mar",  "Marauder", OBJ_VNUM_SCHOOL_DAGGER,
		9813, 97, 18,  7,  13,18,   1, 7,
		"Less known, but very deadly fighters.",
		CSPEC_NOMANA|CSPEC_SHIELD_ALLOWED|CSPEC_NO_BRANDISH|CSPEC_NO_ZAP,
		APPLY_DEX, APPLY_CON, 3, 1
	},

	{
		"Nin",  "Ninja",	0,
		9782, 93, 15,  1,  11,16,   2, 8,
		"The ninja shows up to kill when least expected.",
		CSPEC_NOMANA|CSPEC_NO_BRANDISH|CSPEC_NO_ZAP,
		APPLY_DEX, APPLY_STR, 3, 1
	},

	{
		"Dru",  "Druid",	OBJ_VNUM_SCHOOL_MACE,
		9801, 95, 18, 10,   10,15,   4,12,
		"Druids are the caretakers of a part of Nature, they will defend it and will receive the thanks from the protected land.",
		CSPEC_SHIELD_ALLOWED|CSPEC_NO_ZAP|CSPEC_NO_QUAFF,
		APPLY_WIS, APPLY_DEX, 2, 2
	},

	{
		"Sor",  "Sorcerer",	OBJ_VNUM_SCHOOL_STAFF,
		9796, 97, 18,  6,   9,14,   6,14,
		"They are the offensive mages of the realm, beware, they are quick to anger.",
		CSPEC_NO_PILLS|CSPEC_NO_QUAFF,
		APPLY_INT, APPLY_WIS, 3, 1
	},

	{
		"Sha",  "Shaman",	0,
		9807,  93, 17,  7,  10,16,   3,11,
		"Caretakers of Nature as are the druids, but having their own ways.",
		CSPEC_SHIELD_ALLOWED|CSPEC_QUICK_FLEE|CSPEC_NO_PILLS|CSPEC_NO_ZAP,
		APPLY_STR, APPLY_CON, 3, 1
	},

	{
		"Wlc",  "Warlock", OBJ_VNUM_SCHOOL_WHIP,
		9796, 99, 18,  6,   8,14,   7,15,
		"Evil darkside magicians who are known with all the dark magics.",
		CSPEC_NO_PILLS|CSPEC_NO_QUAFF,
		APPLY_WIS, APPLY_INT, 3, 1
	}
};


/*
	Titles.
*/

/* these are cool, but big trouble thinking them up -dug
   these lists are for level 1-40, and indeed lotsa trouble -Michiel

char *	const			title_table	[MAX_CLASS][MAX_LEVEL+1][2] =
{
	{
		{ "Man",					"Woman"				},

		{ "Apprentice of Magic",		"Apprentice of Magic"	},
		{ "Spell Student",			"Spell Student"		},
		{ "Scholar of Magic",		"Scholar of Magic"		},
		{ "Delver in Spells",		"Delveress in Spells"	},
		{ "Medium of Magic",		"Medium of Magic"		},

		{ "Scribe of Magic",		"Scribess of Magic"		},
		{ "Seer",					"Seeress"				},
		{ "Sage",					"Sage"				},
		{ "Illusionist",			"Illusionist"			},
		{ "Abjurer",				"Abjuress"			},

		{ "Invoker",				"Invoker"				},
		{ "Enchanter",				"Enchantress"			},
		{ "Conjurer",				"Conjuress"			},
		{ "Magician",				"Witch"				},
		{ "Creator",				"Creator"				},

		{ "Savant",				"Savant"				},
		{ "Magus",				"Craftess"			},
		{ "Wizard",				"Wizard"				},
		{ "Warlock",				"War Witch"			},
		{ "Sorcerer",				"Sorceress"			},

		{ "Elder Sorcerer",			"Elder Sorceress"		},
		{ "Grand Sorcerer",			"Grand Sorceress"		},
		{ "Great Sorcerer",			"Great Sorceress"		},
		{ "Golem Maker",			"Golem Maker"			},
		{ "Greater Golem Maker",		"Greater Golem Maker"	},

		{ "Maker of Stones",		"Maker of Stones",		},
		{ "Maker of Potions",		"Maker of Potions",		},
		{ "Maker of Scrolls",		"Maker of Scrolls",		},
		{ "Maker of Wands",			"Maker of Wands",		},
		{ "Maker of Staves",		"Maker of Staves",		},

		{ "Demon Summoner",			"Demon Summoner"		},
		{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
		{ "Dragon Charmer",			"Dragon Charmer"		},
		{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
		{ "Master of all Magic",		"Master of all Magic"	},

		{ "Mage Hero",				"Mage Heroine"			},
		{ "Angel of Magic",			"Angel of Magic"		},
		{ "Deity of Magic",			"Deity of Magic"		},
		{ "Supremity of Magic",		"Supremity of Magic"	},
		{ "Implementor",			"Implementress"		}
	},

	{
		{ "Man",					"Woman"				},

		{ "Believer",				"Believer"			},
		{ "Attendant",				"Attendant"			},
		{ "Acolyte",				"Acolyte"				},
		{ "Novice",				"Novice"				},
		{ "Missionary",			"Missionary"			},

		{ "Adept",				"Adept"				},
		{ "Deacon",				"Deaconess"			},
		{ "Vicar",				"Vicaress"			},
		{ "Priest",				"Priestess"			},
		{ "Minister",				"Lady Minister"		},

		{ "Canon",				"Canon"				},
		{ "Levite",				"Levitess"			},
		{ "Curate",				"Curess"				},
		{ "Monk",					"Nun"				},
		{ "Healer",				"Healess"				},

		{ "Chaplain",				"Chaplain"			},
		{ "Expositor",				"Expositress"			},
		{ "Bishop",				"Bishop"				},
		{ "Arch Bishop",			"Arch Lady of the Church"},
		{ "Patriarch",				"Matriarch"			},

		{ "Elder Patriarch",		"Elder Matriarch"		},
		{ "Grand Patriarch",		"Grand Matriarch"		},
		{ "Great Patriarch",		"Great Matriarch"		},
		{ "Demon Killer",			"Demon Killer"			},
		{ "Greater Demon Killer",	"Greater Demon Killer"	},

		{ "Cardinal of the Sea",		"Cardinal of the Sea"	},
		{ "Cardinal of the Earth",	"Cardinal of the Earth"	},
		{ "Cardinal of the Air",		"Cardinal of the Air"	},
		{ "Cardinal of the Ether",	"Cardinal of the Ether"	},
		{ "Cardinal of the Heavens",	"Cardinal of the Heavens"},

		{ "Avatar of an Immortal",	"Avatar of an Immortal"	},
		{ "Avatar of a Deity",		"Avatar of a Deity"		},
		{ "Avatar of a Supremity",	"Avatar of a Supremity"	},
		{ "Avatar of an Implementor",	"Avatar of an Implementor"},
		{ "Master of all Divinity",	"Mistress of all Divinity"},

		{ "Holy Hero",				"Holy Heroine"			},
		{ "Angel",				"Angel"				},
		{ "Deity",				"Deity"				},
		{ "Supreme Master",			"Supreme Mistress"		},
		{ "Implementor",			"Implementress"		}
	},

	{
		{ "Man",					"Woman"				},
		{ "Pilferer",				"Pilferess"			},
		{ "Footpad",				"Footpad"				},
		{ "Filcher",				"Filcheress"			},
		{ "Pick-Pocket",			"Pick-Pocket"			},
		{ "Sneak",				"Sneak"				},

		{ "Pincher",				"Pincheress"			},
		{ "Cut-Purse",				"Cut-Purse"			},
		{ "Snatcher",				"Snatcheress"			},
		{ "Sharper",				"Sharpress"			},
		{ "Rogue",				"Rogue"				},

		{ "Robber",				"Robber"				},
		{ "Magsman",				"Magswoman"			},
		{ "Highwayman",			"Highwaywoman"			},
		{ "Burglar",				"Burglaress"			},
		{ "Thief",				"Thief"				},

		{ "Knifer",				"Knifer"				},
		{ "Quick-Blade",			"Quick-Blade"			},
		{ "Killer",				"Murderess"			},
		{ "Brigand",				"Brigand"				},
		{ "Cut-Throat",			"Cut-Throat"			},

		{ "Spy",					"Spy"				},
		{ "Grand Spy",				"Grand Spy"			},
		{ "Master Spy",			"Master Spy"			},
		{ "Assassin",				"Assassin"			},
		{ "Greater Assassin",		"Greater Assassin"		},

		{ "Master of Vision",		"Mistress of Vision"	},
		{ "Master of Hearing",		"Mistress of Hearing"	},
		{ "Master of Smell",		"Mistress of Smell"		},
		{ "Master of Taste",		"Mistress of Taste"		},
		{ "Master of Touch",		"Mistress of Touch"		},

		{ "Crime Lord",			"Crime Mistress"		},
		{ "Infamous Crime Lord",		"Infamous Crime Mistress"},
		{ "Greater Crime Lord",		"Greater Crime Mistress"	},
		{ "Master Crime Lord",		"Master Crime Mistress"	},
		{ "Godfather",				"Godmother"			},

		{ "Assassin Hero",			"Assassin Heroine"		},
		{ "Angel of Death",			"Angel of Death"		},
		{ "Deity of Assassins",		"Deity of Assassins"	},
		{ "Supreme Master",			"Supreme Mistress"		},
		{ "Implementor",			"Implementress"		}
	},

	{
		{ "Man",					"Woman"				},
		{ "Swordpupil",			"Swordpupil"			},
		{ "Recruit",				"Recruit"				},
		{ "Sentry",				"Sentress"			},
		{ "Fighter",				"Fighter"				},
		{ "Soldier",				"Soldier"				},
		{ "Warrior",				"Warrior"				},
		{ "Veteran",				"Veteran"				},
		{ "Swordsman",				"Swordswoman"			},
		{ "Fencer",				"Fenceress"			},
		{ "Combatant",				"Combatess"			},

		{ "Hero",					"Heroine"				},
		{ "Myrmidon",				"Myrmidon"			},
		{ "Swashbuckler",			"Swashbuckleress"		},
		{ "Mercenary",				"Mercenaress"			},
		{ "Swordmaster",			"Swordmistress"		},

		{ "Lieutenant",			"Lieutenant"			},
		{ "Champion",				"Lady Champion"		},
		{ "Dragoon",				"Lady Dragoon"			},
		{ "Cavalier",				"Lady Cavalier"		},
		{ "Knight",				"Lady Knight"			},

		{ "Grand Knight",			"Grand Knight"			},
		{ "Master Knight",			"Master Knight"		},
		{ "Paladin",				"Paladin"				},
		{ "Grand Paladin",			"Grand Paladin"		},
		{ "Demon Hunter",			"Demon Hunter"			},

		{ "Demon Slayer",			"Demon Slayer"			},
		{ "Dragon Hunter",			"Dragon Hunter"		},
		{ "Dragon Slayer",			"Dragon Slayer"		},
		{ "Underlord",				"Underlord"			},
		{ "Overlord",				"Overlord"			},

		{ "Baron of Thunder",		"Baroness of Thunder"	},
		{ "Baron of Storms",		"Baroness of Storms"	},
		{ "Baron of Tornadoes",		"Baroness of Tornadoes"	},
		{ "Baron of Hurricanes",		"Baroness of Hurricanes"	},
		{ "Baron of Meteors",		"Baroness of Meteors"	},

		{ "Knight Hero",			"Knight Heroine"		},
		{ "Angel of War",			"Angel of War"			},
		{ "Deity of War",			"Deity of War"			},
		{ "Supreme Master of War",	"Supreme Mistress of War"},
		{ "Implementor",			"Implementress"		}
	}
};
*/


/*
 * Attribute bonus tables.
 */

const	struct	str_app_type	str_app		[101]		=
{
	{ -5, -4,    0,  0 },  /* 0  */
	{ -5, -4,    3,  1 },
	{ -3, -2,    3,  2 },
	{ -3, -1,   10,  3 },
	{ -2, -1,   25,  4 },
	{ -2, -1,   55,  5 },  /* 5  */
	{ -1,  0,   80,  6 },
	{ -1,  0,   90,  7 },
	{  0,  0,  100,  8 },
	{  0,  0,  100,  9 },
	{  0,  0,  115, 10 }, /* 10  */
	{  0,  0,  115, 11 },
	{  0,  0,  140, 12 },
	{  0,  0,  140, 13 }, /* 13  */
	{  0,  1,  170, 14 },
	{  1,  1,  200, 15 }, /* 15  */
	{  1,  2,  240, 16 },
	{  2,  3,  290, 18 },
	{  2,  4,  350, 20 }, /* 18  */
	{  3,  5,  420, 22 },
	{  3,  6,  500, 24 }, /* 20  */
	{  4,  7,  590, 26 },
	{  5,  7,  690, 28 },
	{  6,  8,  700, 30 },
	{  7,  8,  820, 33 },
	{  8,  9,  950, 36 }, /* 25   */
	{  9,  9, 1090, 39 },
	{ 10, 10, 1240, 42 },
	{ 11, 10, 1400, 45 },
	{ 12, 11, 1600, 48 },
	{ 13, 11, 1800, 52 }, /* 30 */
	{ 14, 12, 2000, 56 },
	{ 15, 12, 2200, 60 },
	{ 16, 13, 2400, 64 },
	{ 17, 13, 2600, 68 },
	{ 18, 14, 2800, 72 },  /* 35  */ /* to hit, +dam, carry, weapon weight */
	{ 19, 14, 3000, 76 },
	{ 20, 15, 3200, 80 },
	{ 21, 15, 3400, 84 },
	{ 22, 16, 3600, 88 },
	{ 23, 16, 3800, 92 }, /* 40 */
	{ 24, 17, 4000, 96 },
	{ 25, 17, 4200, 100 },
	{ 26, 18, 4400, 104 },
	{ 27, 18, 4600, 108 },
	{ 28, 19, 4800, 112 }, /* 45 */
	{ 29, 19, 5000, 116 },
	{ 30, 20, 5200, 120 },
	{ 31, 20, 5400, 124 },
	{ 32, 21, 5800, 128 },
	{ 33, 21, 6000, 132 }, /* 50 */
	{ 34, 22, 6200, 136 },
	{ 35, 22, 6400, 140 },
	{ 36, 23, 6600, 144 },
	{ 37, 23, 6800, 148 },
	{ 38, 24, 7000, 152 }, /* 55 */
	{ 39, 24, 7200, 156 },
	{ 40, 25, 7400, 160 },
	{ 41, 25, 7600, 164 },
	{ 42, 26, 7800, 168 },
	{ 43, 26, 8000, 172 }, /* 60 */
	{ 44, 27, 8200, 176 },
	{ 45, 27,12000, 180 },
	{ 46, 28,12300, 184 },
	{ 47, 28,12600, 188 },
	{ 48, 29,12900, 192 }, /* 65 */
	{ 49, 29,13200, 196 },
	{ 50, 30,13500, 200 },
	{ 51, 30,13800, 204 },
	{ 52, 31,14100, 208 },
	{ 53, 31,14400, 212 }, /* 70 */
	{ 54, 32,14700, 216 },
	{ 55, 32,15000, 220 },
	{ 56, 33,15300, 224 },
	{ 57, 33,15600, 228 },
	{ 58, 34,15900, 232 }, /* 75 */
	{ 59, 34,16200, 236 },
	{ 60, 35,16500, 240 },
	{ 61, 35,16800, 244 },
	{ 62, 36,17100, 248 },
	{ 63, 36,17400, 252 }, /* 80 */
	{ 64, 37,17700, 256 },
	{ 65, 37,18000, 260 },
	{ 66, 38,18300, 264 },
	{ 67, 38,18600, 268 },
	{ 68, 39,18900, 272 }, /* 85 */
	{ 69, 39,19200, 276 },
	{ 70, 40,19500, 280 },
	{ 71, 40,19800, 284 },
	{ 72, 41,20100, 288 },
	{ 73, 41,20400, 292 }, /* 90 */
	{ 74, 42,20700, 296 },
	{ 75, 42,21000, 300 },
	{ 76, 43,21300, 304 },
	{ 77, 43,21600, 308 },
	{ 78, 44,21900, 312 }, /* 95 */
	{ 79, 44,22200, 316 },
	{ 80, 45,22500, 320 },
	{ 81, 45,22800, 324 },
	{ 82, 46,23100, 328 }, /* 99 */
	{ 83, 46,23400, 332 }
};

const	struct	int_app_type	int_app		[101]		=
{
	{  1, -5 },	/*  0 */
	{  2, -5 },	/*  1 */
	{  3, -4 },
	{  4, -4 },	/*  3 */
	{  5, -3 },
	{  6, -3 },	/*  5 */
	{  7, -2 },
	{  8, -2 },
	{  9, -1 },
	{ 10, -1 },
	{ 12, 0 },	/* 10 */
	{ 14, 0 },
	{ 16, 0 },
	{ 18, 0 },
	{ 20, 0 },
	{ 22, 0 },	/* 15 */
	{ 25, 0 },
	{ 28, 0 },
	{ 31, 0 },	/* 18 */
	{ 34, 0 },
	{ 37, 0 },	/* 20 */
	{ 41, 0 },
	{ 46, 1 },
	{ 52, 1 },
	{ 59, 1 },
	{ 67, 1 },	/* 25 */
	{ 75, 1 },
	{ 85, 2 },
	{ 90, 2 },
	{ 95, 2 },
	{100, 2 },	/* 30 */
	{105, 3 },
	{110, 3 },
	{115, 3 },
	{120, 4 },
	{125, 4 },	/* 35 */
	{130, 5 },
	{135, 5 },
	{140, 6 },
	{145, 6 },
	{160, 7 },	/* 40 */
	{200, 7 },
	{250, 8 },
	{320, 9 },
	{450, 10},
	{500, 11},	/* 45 */
	{600, 12},
	{600, 13},
	{600, 14},
	{600, 15},
	{600, 16},	/* 50 */
	{700, 17},
	{700, 18},
	{700, 19},
	{700, 20},
	{700, 21},	/* 55 */
	{700, 22},
	{700, 23},
	{700, 11 },
	{700, 12 },
	{800, 12 },	/* 60 */
	{800, 12 },
	{800, 13 },
	{800, 13 },
	{800, 13 },
	{800, 14 },	/* 65 */
	{800, 14 },
	{800, 14 },
	{800, 15 },
	{800, 15 },
	{800, 15 },	/* 70 */
	{900, 16 },
	{900, 16 },
	{900, 16 },
	{900, 17 },
	{900, 17 },	/* 75 */
	{900, 17 },
	{900, 18 },
	{900, 18 },
	{900, 18 },
	{999, 19 },	/* 80 */
	{999, 19 },
	{999, 19 },
	{999, 20 },
	{999, 20 },
	{999, 20 },	/* 85 */
	{999, 21 },
	{999, 21 },
	{999, 21 },
	{999, 22 },
	{999, 99 },	/* 90 */
	{999, 99 },
	{999, 99 },
	{999, 99 },
	{999, 99 },
	{999, 99 },	/* 95 */
	{999, 99 },
	{999, 99 },
	{999, 99 },
	{999, 99 },
	{999, 99 }
};



const	struct	wis_app_type	wis_app		[101]		=
{
	{ 0 },	/*  0 */
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 1 },	/*  5 */
	{ 1 },
	{ 1 },
	{ 1 },
	{ 2 },
	{ 2 },	/* 10 */
	{ 2 },
	{ 2 },
	{ 2 },
	{ 2 },
	{ 3 },	/* 15 */
	{ 3 },
	{ 4 },
	{ 4 },	/* 18 */
	{ 5 },
	{ 5 },	/* 20 */
	{ 6 },
	{ 6 },
	{ 7 },
	{ 7 },
	{ 8 },
	{ 8 },
	{ 9 },
	{ 9 },
	{10 },
	{10 },	/* 30 */
	{11 },
	{11 },
	{12 },
	{12 },
	{13 },	/* 35 */
	{13 },
	{14 },
	{14 },
	{15 },
	{15 },	/* 40 */
	{16 },
	{16 },
	{17 },
	{17 },
	{18 },	/* 45 */
	{18 },
	{19 },
	{19 },
	{20 },
	{20 },	/* 50 */
	{21 },
	{21 },
	{22 },
	{22 },
	{23 },	/* 55 */
	{23 },
	{24 },
	{24 },
	{25 },
	{25 },	/* 60 */
	{26 },
	{26 },
	{27 },
	{27 },
	{28 },	/* 65 */
	{28 },
	{29 },
	{29 },
	{30 },
	{30 },	/* 70 */
	{31 },
	{31 },
	{32 },
	{32 },
	{33 },	/* 75 */
	{33 },
	{34 },
	{34 },
	{35 },
	{35 },	/* 80 */
	{36 },
	{36 },
	{37 },
	{37 },
	{38 },	/* 85 */
	{38 },
	{39 },
	{39 },
	{40 },
	{40 },	/* 90 */
	{41 },
	{41 },
	{42 },
	{42 },
	{43 },	/* 95 */
	{43 },
	{44 },
	{44 },
	{45 },
	{45 }
};



const	struct	dex_app_type	dex_app		[101]		=
{
	{   75, -30 }, /* 0 */
	{   70, -24 }, /* 1 */
	{   65, -18 },
	{   50, -13 },
	{   55,  -9 },
	{   50,  -6 }, /* 5 */
	{   45,  -4 },
	{   40,  -3 },
	{   35,  -2 },
	{   30,  -2 },
	{   25,  -1 }, /* 10 */
	{   20,  -1 },
	{   15,  -1 },
	{   10,   0 },
	{    5,   0 },
	{    0,   0 }, /* 15 */
	{   -5,   1 },
	{  -10,   1 },
	{  -15,   1 },
	{  -20,   1 },
	{  -25,   2 }, /* 20 */
	{  -30,   2 },
	{  -35,   2 },
	{  -40,   3 },
	{ - 45,   3 },
	{ - 50,   3 }, /* 25 */
	{ - 55,   4 },
	{ - 60,   4 },
	{ - 65,   4 },
	{ - 70,   5 },
	{ - 75,   5 }, /* 30 */
	{ - 80,   6 },
	{ - 85,   6 },
	{ - 90,   7 },
	{ - 95,   7 },
	{ -100,   8 }, /* 35 */
	{ -105,   8 },
	{ -110,   9 },
	{ -115,   9 },
	{ -120,  10 },
	{ -125,  10 }, /* 40 */
	{ -130,  11 },
	{ -135,  11 },
	{ -140,  12 },
	{ -145,  12 },
	{ -150,  13 }, /* 45 */
	{ -155,  13 },
	{ -160,  14 },
	{ -165,  14 },
	{ -170,  15 },
	{ -175,  15 }, /* 50 */
	{ -180,  16 },
	{ -185,  16 },
	{ -190,  17 },
	{ -195,  17 },
	{ -200,  18 }, /* 55 */
	{ -205,  18 },
	{ -210,  19 },
	{ -215,  19 },
	{ -220,  20 },
	{ -225,  20 }, /* 60 */
	{ -230,  21 },
	{ -235,  21 },
	{ -240,  22 },
	{ -245,  22 },
	{ -250,  23 }, /* 65 */
	{ -255,  23 },
	{ -260,  24 },
	{ -265,  24 },
	{ -270,  25 },
	{ -275,  25 }, /* 70 */
	{ -280,  26 },
	{ -285,  26 },
	{ -290,  27 },
	{ -295,  27 },
	{ -300,  28 }, /* 75 */
	{ -305,  28 },
	{ -310,  29 },
	{ -315,  29 },
	{ -320,  30 },
	{ -325,  30 }, /* 80 */
	{ -330,  31 },
	{ -335,  31 },
	{ -340,  32 },
	{ -345,  32 },
	{ -350,  33 }, /* 85 */
	{ -355,  33 },
	{ -360,  34 },
	{ -365,  34 },
	{ -370,  35 },
	{ -375,  35 }, /* 90 */
	{ -380,  36 },
	{ -385,  36 },
	{ -390,  37 },
	{ -395,  37 },
	{ -400,  38 }, /* 95 */
	{ -405,  38 },
	{ -410,  39 },
	{ -415,  39 },
	{ -420,  40 },
	{ -425,  40 }  /*100 */
};



const	struct	con_app_type	con_app		[101]		=
{
	{ -4, 20 },
	{ -3, 25 },
	{ -2, 30 },
	{ -2, 35 },
	{ -1, 40 },
	{ -1, 45 },	/*  5 */
	{ -1, 50 },
	{  0, 55 },
	{  0, 60 },
	{  0, 65 },
	{  0, 70 },	/* 10 */
	{  0, 75 },
	{  0, 80 },
	{  0, 85 },
	{  0, 88 },
	{  1, 90 },	/* 15 */
	{  2, 95 },
	{  2, 97 },
	{  3, 99 },
	{  3, 99 },
	{  4, 99 },	/* 20 */
	{  4, 99 },
	{  5, 99 },
	{  5, 99 },
	{  6, 99 },
	{  6, 99 },	/* 25 */
	{  7, 99 },
	{  7, 99 },
	{  8, 99 },
	{  8, 99 },
	{  9, 99 },	/* 30 */
	{  9, 99 },
	{ 10, 99 },
	{ 10, 99 },
	{ 11, 99 },
	{ 11, 99 },	/* 35 */
	{ 12, 99 },
	{ 12, 99 },
	{ 13, 99 },
	{ 13, 99 },
	{ 14, 99 },	/* 40 */
	{ 14, 99 },
	{ 15, 99 },
	{ 15, 99 },
	{ 16, 99 },
	{ 16, 99 }, 	/* 45 */
	{ 17, 99 },
	{ 17, 99 },
	{ 18, 99 },
	{ 18, 99 },
	{ 19, 99 },	/* 50 */
	{ 19, 99 },
	{ 20, 99 },
	{ 20, 99 },
	{ 21, 99 },
	{ 21, 99 },	/* 55 */
	{ 22, 99 },
	{ 22, 99 },
	{ 23, 99 },
	{ 24, 99 },
	{ 24, 99 }, 	/* 60 */
	{ 25, 99 },
	{ 25, 99 },
	{ 26, 99 },
	{ 26, 99 },
	{ 27, 99 },	/* 65 */
	{ 27, 99 },
	{ 28, 99 },
	{ 28, 99 },
	{ 29, 99 },
	{ 29, 99 },	/* 70 */
	{ 30, 99 },
	{ 30, 99 },
	{ 31, 99 },
	{ 31, 99 },
	{ 32, 99 },	/* 75 */
	{ 32, 99 },
	{ 33, 99 },
	{ 33, 99 },
	{ 34, 99 },
	{ 34, 99 },	/* 80 */
	{ 35, 99 },
	{ 35, 99 },
	{ 36, 99 },
	{ 36, 99 },
	{ 37, 99 },	/* 85 */
	{ 37, 99 },
	{ 38, 99 },
	{ 38, 99 },
	{ 39, 99 },
	{ 39, 99 }, 	/* 90 */
	{ 40, 99 },
	{ 40, 99 },
	{ 41, 99 },
	{ 41, 99 },
	{ 42, 99 },	/* 95 */
	{ 42, 99 },
	{ 43, 99 },
	{ 43, 99 },
	{ 44, 99 },
	{ 44, 99 }
};



/*
 * Liquid properties.
 * Used in world.obj.
 */

const	struct	liq_type	liq_table	[LIQ_MAX]	=
{
	{ "water",			"clear",		{  0, 0, 10 }	},  /*  0 */
	{ "beer",				"amber",		{  2, 0,  5 }	},
	{ "wine",				"rose",		{  5, 0,  5 }	},
	{ "ale",				"brown",		{  2, 1,  5 }	},
	{ "dark ale",			"dark",		{  1, 1,  5 }	},

	{ "whisky",			"golden",		{  6, 0,  3 }	},  /*  5 */
	{ "lemonade",			"pink",		{  0, 0,  8 }	},
	{ "firebreather",		"boiling",	{ 10, 0,  0 }	},
	{ "local specialty",	"everclear",	{  3, 0,  3 }	},
	{ "slime mold juice",	"green",		{  0, 1, -6 }	},

	{ "milk",				"white",		{  0, 2,  6 }	},  /* 10 */
	{ "tea",				"tan",		{  0, 0,  6 }	},
	{ "coffee",			"black",		{  0, 0,  6 }	},
	{ "blood",			"red",		{  0, 3, -1 }	},
	{ "salt water",		"clear",		{  0, 0, -3 }	},

	{ "cola",				"cherry",		{  0, 0,  6 }	},  /* 15 */
	{ "orange juice",		"orange",		{  0, 0,  6 }  },
	{ "brandy",			"golden",		{ 11, 0,  2 }  },
	{ "icewine",			"purple",		{  4, 0,  4 }  },
	{ "rum",				"amber",		{  7, 0,  2 }  },

	{ "vodka",			"clear",		{  7, 0,  2 }  },  /* 20 */
	{ "champagne",			"golden",		{  5, 0,  5 }  }
};


char *	const	dir_name		[]		=
{
	"north", "east", "south", "west", "up", "down"
};

const	sh_int	rev_dir		[]		=
{
	DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP
};

char *	const	wind_dir_name	[]		=
{
	"northern", "northeastern", "eastern", "southeastern",
	"southern", "southwestern", "western", "northwestern"
};

/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */

#define SLOT(n)	n

const	struct	body_type	body_table	[MAX_BODY]	=
{
	{
		"head", "%s's head",
		"%s's head lies here.",
		"%s's head is slowly staring into space.",
		"head butt",
		"$n's head slowly falls off.",
		DAM_BASH
	},

	{
		"jaw", "%s's jaw bone",
		"%s's jaw bone lies here.",
		"%s's jaw bone has a few scraps of meat left on it.",
		"bite",
		"$n's jaw drops to the floor.",
		DAM_PIERCE
	},

	{
		"eye eyeball", "%s's eye",
		"%s's eyeball lies here.",
		"%s's eyeball looks very peircing.",
		"evil eye",
		"$n's eyeball pops out.",
		DAM_EVIL
	},

	{
		"heart", "%s's heart",
		"The torn-out heart of %s rots here.",
		"%s's heart does not have too many maggots on it.",
		"shoulder slam",
		"$n's heart is torn from $s chest.",
		DAM_BASH
	},

	{
		"hip", "%s's hip",
		"%s's hip is reeking here.",
		"%s's hip has some meat left on it.",
		"hip slam",
		"$n falls to pieces.",
		DAM_BASH
	},

	{
		"leg", "%s's leg",
		"%s's bloody leg rests on the ground.",
		"%s's leg is black, blue, and red.",
		"knee",
		"$n kicks $s leg off.",
		DAM_THRUST
	},

	{
		"arm", "%s's arm",
		"%s's arm is resting on the ground.",
		"%s's arm looks very strong.",
		"elbow slam",
		"$n's arm seems to have been unattached.",
		DAM_THRUST
	},

	{
		"wing", "%s's wing",
		"%s's wing rests here.",
		"%s's wing has a few cuts in it.",
		"wing slap",
		"$n is flying nowhere.",
		DAM_THRUST
	},

	{
		"tail", "%s's tail",
		"%s's tail is laying on the ground.",
		"%s's tail looks only slightly edible.",
		"tail slap",
		"$n can no longer wag $s tail.",
		DAM_BASH
	},

	{
		"tentacle", "%s's tentacle",
		"%s's tentacle rests here.",
		"%s's tentacle looks very slimy.",
		"tentacle whip",
		"The stump of $n's tentacle spurts blood.",
		DAM_REND
	},

	{
		"horn", "%s's horn",
		"%s's horn rests here.",
		"%s's horn has a bit of meat stuck to it.",
		"horn jab",
		"$n's horn bumps you as it falls to the floor.",
		DAM_PIERCE
	},

	{
		"claw", "%s's claw",
		"%s's claw is here.",
		"%s's claw seems to have gotten quite a bit of use.",
		"claw",
		"The claw of $n gets clipped.",
		DAM_REND
	},

	{
		"hand", "%s's hand",
		"%s's hand sits on the ground here.",
		"%s's hand is in no shape to be reattached.",
		"punch",
		"$n gives you the finger, right before losing $s hand.",
		DAM_THRUST
	},

	{
		"foot", "%s's foot",
		"%s's foot stands here.",
		"%s's foot is only slightly smelly.",
		"kick",
		"$n stumbles, as $s foot falls off.",
		DAM_THRUST
	},

	{
		"branch", "%s's branch",
		"%s's branch lies broken here.",
		"%s's branch is broken and splintered.",
		"branch sweep",
		"$n's branch crashes to the ground.",
		DAM_REND
	},

	{
		"root", "%s's root",
		"%s's root is tangled here.",
		"%s's root has been ripped, bent and torn.",
		"root slap",
		"$n's root shakes out of the ground.",
		DAM_THRUST
	},

	{
		"twig", "%s's twig",
		"%s's twig lies trampled here.",
		"%s's twig is twisted and stripped from most of its leaves.",
		"twig rake",
		"$n's twig snaps and falls to ground.",
		DAM_REND
	},

	{
		"trunk", "%s's trunk",
		"%s's trunk is rotting here.",
		"%s's knotted trunk is covered in rough bark.",
		"trunk crash",
		"$n's trunk splinters as it falls.",
		DAM_BASH
	},

	{
		"pincers", "%s's pincers",
		"%s's pincers lay here.",
		"%s's pincers still look sharp.",
		"savage bite",
		"$n's pincers have bitten the dust.",
		DAM_PIERCE
	},

	{
		"spinnaret", "%s's spinnaret",
		"%s's spinnaret is gathering dust here.",
		"%s's spinnaret can spin no more webs.",
		"caustic web",
		"$n's spinnaret has spun its last web and falls off.",
		DAM_ACID
	},

	{
		"beak",	"%s's beak",
		"%s's sharp beak lies here.",
		"%s's beak opens dangerously.",
		"peck",
		"$n's beak is pecking no more.",
		DAM_PIERCE
	}
};

/*
	table wherein the spells are defined,
	modified by Manwe.
	mod 1. : added levels for new classes. 10/10/1999
	mod 2. : added a flag to determine wether skill or spell 12/10/1999
	mod 3. : added flag to see which god you should follow ??/??/1999
	mod 4. : added new spells/skills

	WAR  GLA  MAR  NIN  DRU  SOR  SHA  WLC
*/

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{
	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"slot -1",				{ 99, 99, 99, 99, 99, 99, 99, 99 },
		0,						TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( -1),	 0,	0,
		"",						""
	},

	/*
		Used by mpaset - Scandum
	*/
	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"object rape",				{ 99, 99, 99, 99, 99, 99, 99, 99 },
		0,						TAR_IGNORE,		POS_STANDING,
		&gsn_object_rape,			SLOT(  0),	 0,	0,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"armor",					{ 99,  99, 99, 99, 2, 99, 99, 99 },
		spell_armor,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(  1),	 5,	PULSE_VIOLENCE,
		"",						"Your armor returns to its mundane value."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"teleport",				{ 99, 99, 99, 99, 27, 99, 99, 99 },
		spell_teleport,			TAR_CHAR_SELF,		POS_FIGHTING,
		NULL,	 				SLOT(  2),	35,	PULSE_VIOLENCE,
		"",						"!Teleport!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"bless",					{ 99, 99, 99, 99, 99, 99, 2, 99 },
		spell_bless,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(  3),	 15,	PULSE_VIOLENCE,
		"",						"Your blessing fades away."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_EVIL,
		"blindness",				{ 99, 99, 99, 99, 99, 20, 99, 99 },
		spell_blindness,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_blindness,			SLOT(  4),	 5,	PULSE_VIOLENCE,
		"spell",					"Your vision returns."
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_FIRE,
		"burning hands",			{  99, 99, 99, 99, 15, 5, 99, 10 },
		spell_burning_hands,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(  5),	10,	PULSE_VIOLENCE,
		"burning hands",			"!Burning Hands!"
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_LIGHTNING,
		"call lightning",			{ 99, 99, 99, 99, 26, 99, 99, 99 },
		spell_call_lightning,		TAR_IGNORE,		POS_FIGHTING,
		NULL,					SLOT(  6),	30,	PULSE_VIOLENCE,
		"lightning bolt",			"!Call Lightning!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_PSIONIC,
		"charm person",			{ 99, 99, 99, 99, 99, 99, 99, 20 },
		spell_charm_person,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		&gsn_charm_person,			SLOT(  7),	 25,	PULSE_VIOLENCE,
		"charm spell",				"You regain your free will."
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_COLD,
		"chill touch",				{ 99, 99, 99, 99, 15, 5, 99, 10 },
		spell_chill_touch,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(  8),	10,	PULSE_VIOLENCE,
		"chilling touch",			"You feel less cold."
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"stone fist",				{ 99, 99, 99, 99, 31, 99, 24, 99 },
		spell_stone_fist,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		&gsn_stone_fist,			SLOT(  9),	25,	PULSE_VIOLENCE * 3 / 2,
		"",						"Your fists feel soft again."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIGHT,
		"color spray",				{ 99, 99, 99, 99, 99, 19, 99, 99 },
		spell_color_spray,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 10),	15,	PULSE_VIOLENCE,
		"color spray",				"!Color Spray!"
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_MAGIC,
		"control weather",			{ 99, 99, 99, 99, 12, 18, 14, 99 },
		spell_control_weather,		TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 11),	25,	PULSE_VIOLENCE,
		"",						"!Control Weather!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"create food",				{ 99, 99, 99, 99, 99, 99,  6, 99 },
		spell_create_food,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 12),	 5,	PULSE_VIOLENCE,
		"",						"!Create Food!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_GOOD,
		"cure blindness",			{ 99, 99, 99, 99, 99, 99, 4, 99 },
		spell_cure_blindness,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 14),	 5,	PULSE_VIOLENCE,
		"",						"!Cure Blindness!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"cure critical",			{ 99, 99, 99, 99, 99, 99, 13, 99 },
		spell_cure_critical,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 15),	20,	PULSE_VIOLENCE,
		"",						"!Cure Critical!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"cure light",				{ 99, 99, 99, 99, 99, 99,  3, 99 },
		spell_cure_light,			TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 16),	10,	PULSE_VIOLENCE,
		"",						"!Cure Light!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_DEMON,
		DAM_EVIL,
		"curse",					{ 99, 99, 99, 99, 99, 99, 99, 30 },
		spell_curse,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_curse,				SLOT( 17),	20,	PULSE_VIOLENCE,
		"curse spell",				"The curse wears off."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"detect evil",				{ 99, 99, 99, 99, 99, 99, 11, 99 },
		spell_detect_evil,			TAR_CHAR_SELF,		POS_STANDING,
		&gsn_detect_evil,			SLOT( 18),	 5,	PULSE_VIOLENCE,
		"",						"The red outlines fade from your vision."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"detect invis",			{  99,  99, 99, 99, 99, 3, 99, 99 },
		spell_detect_invis,			TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT( 19),	 5,	PULSE_VIOLENCE,
		"",						"You no longer see invisible objects."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"detect poison",			{ 99, 99, 99, 99, 99,  99, 12, 99 },
		spell_detect_poison,		TAR_OBJ_INV,		POS_STANDING,
		NULL,					SLOT( 21),	 5,	PULSE_VIOLENCE,
		"",						"!Detect Poison!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_GOOD,
		"dispel evil",				{ 99, 99, 99, 99, 99, 99, 16, 99 },
		spell_dispel_evil,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 22),	15,	PULSE_VIOLENCE,
		"dispel evil",				"!Dispel Evil!"
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_VIBE,
		"earthquake",				{ 99, 99, 99, 99, 30, 99, 99, 99 },
		spell_earthquake,			TAR_IGNORE,		POS_FIGHTING,
		NULL,					SLOT( 23),	15,	PULSE_VIOLENCE,
		"earthquake",				"!Earthquake!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"enchant weapon",			{ 99, 99, 99, 99, 99, 99, 99, 24 },
		spell_enchant_weapon,		TAR_OBJ_INV,		POS_STANDING,
		NULL,					SLOT( 24),	100,	PULSE_VIOLENCE * 2,
		"",						"!Enchant Weapon!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"energy drain",			{ 99, 99, 99, 99, 99, 99, 99, 17 },
		spell_energy_drain,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 25),	25,	PULSE_VIOLENCE,
		"energy drain",			"!Energy Drain!"
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_FIRE,
		"fireball",				{ 99, 99, 99, 99, 21, 99, 99, 99 },
		spell_fireball,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 26),	15,	PULSE_VIOLENCE,
		"fireball",				"!Fireball!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"harm",					{ 99, 99, 99, 99, 99, 99, 22, 99 },
		spell_harm,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 27),	35,	PULSE_VIOLENCE,
		"harm spell",				"!Harm!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"heal",					{ 99, 99, 99, 99, 99, 99, 20, 99 },
		spell_heal,				TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 28),	50,	PULSE_VIOLENCE,
		"",						"!Heal!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"invis",					{ 99, 99, 99, 99, 99, 18, 99, 99 },
		spell_invis,				TAR_OBJ_CHAR_DEF,	POS_STANDING,
		&gsn_invis,				SLOT( 29),	 5,	PULSE_VIOLENCE * 3 / 2,
		"",						"You are no longer invisible.",
								"$p fades into existance."
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_LIFE,
		"petrifying touch",			{ 99, 99, 99, 99, 10, 5, 99, 10 },
		spell_petrifying_touch,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 30),	10,	PULSE_VIOLENCE,
		"petrifying touch",			"Your skin feels less hard."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"locate object",			{ 99, 99, 99, 99, 99, 10, 99, 99 },
		spell_locate_object,		TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 31),	20,	PULSE_VIOLENCE * 3,
		"",						"!Locate Object!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"magic missile",			{ 99, 99, 99, 99, 9,  2, 99, 99 },
		spell_magic_missile,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 32),	  5,	PULSE_VIOLENCE / 3,
		"magic missile",			"!Magic Missile!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_POISON,
		"poison",					{ 99, 99, 99, 99, 99, 99, 99, 16 },
		spell_poison,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_poison,				SLOT( 33),	10,	PULSE_VIOLENCE,
		"poison spell",			"You feel less sick."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"protection evil",			{ 99, 99, 99, 99, 99, 99, 17, 99 },
		spell_protection_fe,		TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT( 34),	 5,	PULSE_VIOLENCE,
		"",						"You feel less protected from evil."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_GOOD,
		"remove curse",			{ 99, 99, 99, 99, 99, 99, 21, 99 },
		spell_remove_curse,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 35),	 5,	PULSE_VIOLENCE,
		"",						"!Remove Curse!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_HOLY,
		"sanctuary",				{ 99, 99, 99, 99, 25, 99, 99, 99 },
		spell_sanctuary,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(36),		75,	PULSE_VIOLENCE,
		"",						"The white aura around your body fades."
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_LIGHTNING,
		"shocking grasp",			{ 99, 99, 99, 99, 15, 5, 20, 10 },
		spell_shocking_grasp,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 37),	10,	PULSE_VIOLENCE,
		"shocking grasp",			"!Shocking Grasp!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"sleep",					{ 99, 99, 99, 99, 99, 99, 99, 34 },
		spell_sleep,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		&gsn_sleep,				SLOT( 38),	15,	PULSE_VIOLENCE,
		"",						"You feel less tired."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"giant strength",			{ 99, 99, 99, 99, 99, 99, 14, 99 },
		spell_giant_strength,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 39),	20,	PULSE_VIOLENCE,
		"",						"You feel weaker."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_PSIONIC,
		"ventriloquate",			{ 99, 99, 99, 99, 99,  7,  99, 99 },
		spell_ventriloquate,		TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 41),	 5,	PULSE_VIOLENCE,
		"",						"!Ventriloquate!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"word of recall",			{ 99, 99, 99, 99, 26, 99, 99, 99 },
		spell_word_of_recall,		TAR_CHAR_SELF,		POS_RESTING,
		&gsn_word_of_recall,		SLOT( 42),	5,	PULSE_VIOLENCE,
		"",						"!Word of Recall!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"cure poison",				{ 99, 99, 99, 99, 99, 99, 15, 99 },
		spell_cure_poison,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 43),	 5,	PULSE_VIOLENCE,
		"",						"!Cure Poison!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"detect hidden",			{  99, 99, 99, 99, 99, 8, 99, 99 },
		spell_detect_hidden,		TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT( 44),	 5,	PULSE_VIOLENCE,
		"",						"You feel less aware of your surroundings."
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_VIBE,
		"windblast",				{ 99, 99, 99, 99, 21, 99, 99, 99 },
		spell_windblast,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 45),	15,	PULSE_VIOLENCE,
		"windblast",				"!Windblast!"
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_BASH,
		"waterburst",				{ 99, 99, 99, 99, 21, 99, 99, 99 },
		spell_waterburst,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 46),	15,	PULSE_VIOLENCE,
		"waterburst",				"!Waterburst!"
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_PSIONIC,
		"mindblast",				{ 99, 99, 99, 99, 21, 99, 99, 99 },
		spell_mindblast,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 47),	15,	PULSE_VIOLENCE,
		"mindblast",				"!Mindblast!"
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_POISON,
		"snake dart",				{ 99, 99, 99, 99, 21, 99, 99, 99 },
		spell_snake_dart,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 48),	15,	PULSE_VIOLENCE,
		"snake dart",				"!snake dart!"
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_PSIONIC,
		"paralyzing embrace",		{ 99, 99, 99, 99, 10, 5, 99, 10 },
		spell_paralyzing_embrace,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 48),	10,	PULSE_VIOLENCE,
		"paralyzing embrace",		"Your grasp on reality feels strong again."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"identify",				{ 99, 99, 99, 99, 99, 24, 99, 10 },
		spell_identify,			TAR_OBJ_INV,		POS_STANDING,
		NULL,					SLOT( 53),	12,	PULSE_VIOLENCE / 2,
		"",						"!Identify!"
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_MAGIC,
		"fly",					{ 99,  99, 99, 99, 18, 23, 11, 28 },
		spell_fly,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		&gsn_fly,					SLOT( 56),	10,	PULSE_VIOLENCE * 3 / 2,
		"",						"You slowly float to the ground."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"continual light",			{ 99, 99, 99, 99, 99, 15, 99, 99 },
		spell_continual_light,		TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 57),	 7,	PULSE_VIOLENCE,
		"",						"!Continual Light!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"dispel magic",			{ 99, 99, 99, 99, 99, 99, 99, 8 },
		spell_dispel_magic,			TAR_OBJ_CHAR_DEF,	POS_FIGHTING,
		NULL,					SLOT( 59),	15,	PULSE_VIOLENCE,
		"",						"!Dispel Magic!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"cure serious",			{ 99, 99, 99, 99, 99, 99, 8, 99 },
		spell_cure_serious,			TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 61),	15,	PULSE_VIOLENCE,
		"",						"!Cure Serious!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"cause light",				{ 99, 99, 99, 99, 99, 99,  3, 99 },
		spell_cause_light,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 62),	5,	PULSE_VIOLENCE,
		"spell",					"!Cause Light!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"cause critical",			{ 99, 99, 99, 99, 99, 99, 13, 99 },
		spell_cause_critical,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 63),	15,	PULSE_VIOLENCE,
		"spell",					"!Cause Critical!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"cause serious",			{ 99, 99, 99, 99, 99, 99,  8, 99 },
		spell_cause_serious,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 64),	10,	PULSE_VIOLENCE,
		"spell",					"!Cause Serious!"
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_FIRE,
		"flamestrike",				{ 99, 99, 99, 99, 99, 99, 23, 99 },
		spell_flamestrike,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 65),	20,	PULSE_VIOLENCE,
		"flamestrike",				"!Flamestrike!"
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"stone skin",				{ 99, 99, 99, 99 , 15, 99, 99, 99},
		spell_stone_skin,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 66),	50,	PULSE_VIOLENCE * 3 / 2,
		"",						"Your skin feels soft again."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"shield",					{ 99, 99, 99, 99, 99, 99, 99, 15 },
		spell_shield,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(67),	12,		PULSE_VIOLENCE * 3 / 2,
		"",						"Your force shield shimmers then fades away."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"weaken",					{ 99, 99, 99, 99, 99, 99, 99,  4 },
		spell_weaken,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 68),	20,	PULSE_VIOLENCE,
		"weaken spell",			"You feel stronger."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"mass invis",				{ 99, 99, 99, 99, 99, 22, 99, 99 },
		spell_mass_invis,			TAR_IGNORE,	POS_STANDING,
		NULL,					SLOT( 69),	 20,	PULSE_VIOLENCE * 2,
		"",						"!Mass Invis!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_ACID,
		"acid blast",				{ 99, 99, 99, 99, 99, 31, 99, 99},
		spell_acid_blast,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT( 70),	20,	PULSE_VIOLENCE,
		"acid blast",				"!Acid Blast!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_DARKELF,
		DAM_LIGHT,
		"faerie fire",				{  99, 99, 99, 99, 99, 14, 99, 99 },
		spell_faerie_fire,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_faerie_fire,			SLOT( 72),	 5,	PULSE_VIOLENCE,
		"faerie fire",				"The pink aura around you fades away."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIGHT,
		"faerie fog",				{ 99, 99, 99, 99, 99, 16, 99, 99 },
		spell_faerie_fog,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 73),	12,	PULSE_VIOLENCE,
		"faerie fog",				"!Faerie Fog!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"pass door",				{ 99, 99, 99, 99, 24, 99, 99, 99 },
		spell_pass_door,			TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT( 74),	20,	PULSE_VIOLENCE,
		"",						"You feel solid again."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"summon",					{ 99, 99, 99, 99, 99, 99, 99, 64 },
		spell_summon,				TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 76),	50,	PULSE_VIOLENCE,
		"summon spell",			"!Summon!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"infravision",				{ 99, 99, 99, 99, 99, 21, 99, 99 },
		spell_infravision,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 77),	 5,	PULSE_VIOLENCE,
		"",						"You no longer see in the dark."
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_LIFE,
		"create spring",			{ 99, 99, 99, 99, 15, 99, 99, 99 },
		spell_create_spring,		TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT( 80),	20,	PULSE_VIOLENCE,
		"",						"!Create Spring!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"refresh",				{ 99, 99, 99, 99, 8, 99, 99, 99 },
		spell_refresh,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 81),	12,	PULSE_VIOLENCE,
		"refresh",				"!Refresh!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"change sex",				{ 99, 99, 99, 99, 99, 11, 99, 99 },
		spell_change_sex,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 82),	15,	PULSE_VIOLENCE,
		"",						"Your body feels familiar again."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"gate",					{ 99, 99, 99, 99, 99, 60, 99, 99 },
		spell_gate,				TAR_IGNORE,	POS_STANDING,
		NULL,					SLOT( 83),	50,	PULSE_VIOLENCE,
		"",						"!Gate!"
	},

	/*
		Breath spells
	*/

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_ACID,
		"acid breath",				{ 99, 99, 99, 99, 99, 99, 99, 99 },
		spell_acid_breath,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(200),	 30,	 PULSE_VIOLENCE,
		"blast of acid",			"!Acid Breath!"
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_FIRE,
		"fire breath",				{ 99, 99, 99, 99, 99, 72, 99, 99 },
		spell_fire_breath,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(201),	 45,	 PULSE_VIOLENCE * 2,
		"blast of flame",			"!Fire Breath!"
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_COLD,
		"frost breath",			{ 99, 99, 99, 99, 99, 74, 99, 99 },
		spell_frost_breath,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(202),	 45,	 PULSE_VIOLENCE * 2,
		"blast of frost",			"!Frost Breath!"
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_POISON,
		"gas breath",				{ 99, 99, 99, 99, 99, 76, 99, 99 },
		spell_gas_breath,			TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,					SLOT(203),	 45,	 PULSE_VIOLENCE * 2,
		"blast of gas",			"!Gas Breath!"
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_LIGHTNING,
		"lightning breath",			{ 99, 99, 99, 99, 99, 78, 99, 99 },
		spell_lightning_breath,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(204),	 45,	 PULSE_VIOLENCE * 2,
		"blast of lightning",		"!Lightning Breath!"
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_PSIONIC,
		"psionic shockwave",		{ 99, 99, 99, 99, 99, 71, 99, 99 },
		spell_psionic_shockwave,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(205),	 45,	 PULSE_VIOLENCE * 2,
		"psionic shockwave",		"!Psionic Shockwave!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"high explosive",			{ 99, 99, 99, 99, 99, 99, 99, 99 },
		spell_high_explosive,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(502),	0,	PULSE_VIOLENCE,
		"high explosive ammo",		"!High Explosive Ammo!"
	},

	/*
		Originally for Mortal Realms by Chaos and Order
	*/

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"block area",				{ 99, 99, 99, 99, 40, 99, 99, 99 },
		spell_block_area,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(601),	25,	PULSE_VIOLENCE,
		"",						"!block area!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"write spell",				{ 99, 99, 99, 99, 99, 99, 99, 19 },
		spell_write_spell,			TAR_IGNORE,		POS_RESTING,
		&gsn_write_spell,			SLOT(602),	5,	PULSE_VIOLENCE,
		"",						"!write spell!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"homonculous",				{ 99, 99, 99, 99, 99, 99, 99, 18 },
		spell_homonculous,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(603),	90,	PULSE_VIOLENCE,
		"",						"!homonculous!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"demon",					{ 99, 99, 99, 99, 99, 99, 99, 13 },
		spell_demon,				TAR_IGNORE,	 POS_STANDING,
		NULL,					SLOT(604),	90,	PULSE_VIOLENCE,
		"",						"!Demon!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"beast",					{ 99, 99, 99, 99, 99, 17, 99, 99 },
		spell_beast,				TAR_IGNORE,		POS_FIGHTING,
		NULL,					SLOT(605),	20,	PULSE_VIOLENCE,
		"",						"!Beast!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"shade",					{ 99, 99, 99, 99, 99, 9, 99, 99 },
		spell_shade,				TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(606),	50,	PULSE_VIOLENCE,
		"",						"!Shade!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"phantasm",				{ 99, 99, 99, 99, 99, 23, 99, 99 },
		spell_phantasm,			TAR_IGNORE,		POS_FIGHTING,
		NULL,					SLOT(607),	30,	PULSE_VIOLENCE,
		"",						"!Phantasm!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"rift",					{ 99, 99, 99, 99, 99, 99, 99, 27 },
		spell_rift,				TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(608),	90,	PULSE_VIOLENCE,
		"",						"!rift!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"rip",					{ 99, 99, 99, 99, 99, 99, 99, 23 },
		spell_rip,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(609),	75,	PULSE_VIOLENCE,
		"",						"!rip!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"tongues",				{ 99, 99, 99, 99, 10, 99, 99, 99 },
		spell_tongues,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(610),	10,	PULSE_VIOLENCE / 2,
		"",						"You no longer speak in tongues!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"understand",				{ 99, 99, 99, 99,  5, 99, 99, 99 },
		spell_understand,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(611),	10,	PULSE_VIOLENCE / 2,
		"",						"You lose all understanding!"
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_MAGIC,
		"haste",					{ 58, 54, 44, 46, 13, 26, 22, 36 },
		spell_haste,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(613),	 10,	PULSE_VIOLENCE * 2,
		"",						"You slow down."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"enhanced rest",			{ 99, 99, 99, 99, 34, 99, 99, 99 },
		spell_enhanced_rest,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(614),	10,	PULSE_VIOLENCE * 2,
		"",						"You no longer rest easily."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"enhanced heal",			{ 99, 99, 99, 99, 99, 99, 35, 99 },
		spell_enhanced_heal,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(615),	10,	PULSE_VIOLENCE * 2,
		"",						"You no longer heal easily."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"enhanced revive",			{ 99, 99, 99, 99, 99, 99, 99, 35 },
		spell_enhanced_revive,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(616),	10,	PULSE_VIOLENCE * 2,
		"",						"You no longer revive easily."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_EVIL,
		"animate dead",			{ 99, 99, 99, 99, 99, 99, 99, 39 },
		spell_animate_dead,			TAR_IGNORE,	POS_STANDING,
		NULL,					SLOT(617),	50,	PULSE_VIOLENCE * 2,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"banish",					{ 99, 99, 99, 99, 99, 99, 99, 38 },
		spell_banish,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(618),	80,	PULSE_VIOLENCE,
		"banish spell",			""
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"enhance object",			{ 99, 99, 99, 99, 28, 99, 99 ,99 },
		spell_enhance_object,		TAR_OBJ_INV,	POS_STANDING,
		NULL,					SLOT(619),	80,	PULSE_VIOLENCE * 2,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"mage shield",				{ 99, 99, 99, 99, 99, 99, 99, 25 },
		spell_mage_shield,			TAR_CHAR_SELF,	POS_STANDING,
		NULL,					SLOT(620),	80,	PULSE_VIOLENCE,
		"",						"The shimmering glow fades from about your body."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_HOLY,
		"dispel undead",			{ 99, 99, 99, 99, 99, 99, 19, 99 },
		spell_dispel_undead,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(621),	20,	PULSE_VIOLENCE,
		"dispel",					"!Dispel Undead!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_EVIL,
		"dispel good",				{ 99, 99, 99, 99, 99, 99, 37, 99 },
		spell_dispel_good,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(622),	20,	PULSE_VIOLENCE,
		"dispel",					"!Dispel Good!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_EVIL,
		"remove fear",				{ 99, 99, 99, 99, 29, 99, 99, 99 },
		spell_remove_fear,			TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		&gsn_remove_fear,			SLOT(623),	40,	PULSE_VIOLENCE,
		"spell",					"You are no longer fearless."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"feast",					{ 99, 99, 99, 99, 35, 99, 99, 99 },
		spell_feast,				TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT(624),	75,	PULSE_VIOLENCE * 2,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"restore",				{ 99, 99, 99, 99, 99, 99, 45, 99 },
		spell_restore,				TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(625),	20,	PULSE_VIOLENCE * 2,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_VIBE,
		"tremor",					{ 99, 99, 99, 99, 43, 99, 99, 99 },
		spell_tremor,				TAR_IGNORE,		POS_FIGHTING,
		NULL,					SLOT(626),	45,	PULSE_VIOLENCE,
		"tremor",					"!Tremor!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"energy shift",			{ 99, 99, 99, 99, 99, 99, 99, 32 },
		spell_energy_shift,			TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(627),	20,	PULSE_VIOLENCE * 2,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"induction",	  			{ 99, 99, 99, 99, 32, 99, 99, 99 },
		spell_induction,			TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT(628),	 20,	PULSE_VIOLENCE / 4,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_MAGIC,
		"illusion",				{ 99, 99, 99, 99, 99, 39, 99, 99 },
		spell_illusion,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(629),	75,	PULSE_VIOLENCE * 2,
		"illusion",				"!illusion!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"mirror image",			{ 99, 99, 99, 99, 99, 27, 99, 99 },
		spell_mirror_image,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(630),	  80,	PULSE_VIOLENCE * 2,
		"mirror image",			"Your images fold back into you."
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_PSIONIC,
		"hallucinate",				{ 99, 99, 99, 99, 99, 22, 39, 99 },
		spell_hallucinate,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(631),	25,	PULSE_VIOLENCE,
		"hallucinate spell",		"Reality shifts back into focus."
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"stability",				{ 99, 99, 99, 99, 27, 36, 30, 44 },
		spell_stability,			TAR_CHAR_DEFENSIVE,	POS_RESTING,
		NULL,					SLOT(632),	 10,	PULSE_VIOLENCE,
		"",						"You no longer feel the strength of the earth."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"ethereal travel",			{ 99, 99, 99, 99, 99, 99, 99, 28 },
		spell_ethereal,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(633),	35,	PULSE_VIOLENCE * 2,
		"",						"You become solid."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"astral projection",		{ 99, 99, 99, 99, 99, 28, 99, 99 },
		spell_astral,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(634),	 45,	PULSE_VIOLENCE * 2,
		"",						"You step back into your body."
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_MAGIC,
		"breath water",			{ 99, 99, 99, 99, 31, 31, 31, 31 },
		spell_breath_water,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(635),	15,	PULSE_VIOLENCE,
		"breath water",			"You can no longer breath water."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_PSIONIC,
		"mage blast",				{ 99, 99, 99, 99, 99, 99, 99, 45 },
		spell_mage_blast,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_mage_blast,			SLOT(636),	45,	PULSE_VIOLENCE,
		"mage blast",				"You feel less vulnerable."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_PSIONIC,
		"confusion",				{ 99, 99, 99, 99, 99, 38, 99, 99 },
		spell_confusion,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(637),	30,	PULSE_VIOLENCE * 3 / 2,
		"confusion spell",			"Your eyes refocus."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"sanctify",				{ 99, 99, 99, 99, 53, 99, 99, 99 },
		spell_sanctify,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(638),	75,	PULSE_VIOLENCE * 2,
		"",						""
	},

	/*
		new spells by Martin Gallwey - 4/8/98
	*/

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"soothing touch",			{ 99, 99, 99, 99, 99, 99, 81, 99 },
		spell_soothing_touch,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(639),	150,	PULSE_VIOLENCE * 2,
		"",						"!Soothing Touch!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"benediction",				{ 99, 99, 99, 99, 61, 99, 99, 99 },
		spell_benediction,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 640),	 45,	PULSE_VIOLENCE * 2,
		"",						"The powerful blessing of the gods fades away."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"righteous fury",			{ 99, 99, 99, 99, 37, 99, 99, 99 },
		spell_righteous_fury,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT( 641),	 75,	PULSE_VIOLENCE * 2,
		"",						"Your righteous fury abruptly leaves you, you feel drained and weak."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"farheal",				{ 99, 99, 99, 99, 99, 99, 91, 99 },
		spell_farheal,				TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(642),	200,	PULSE_VIOLENCE * 3,
		"",						"!Far Heal!"
	},

/*
	Uncoded

	Ideas:

	The target is first_affect for small number of hours, like 4/5 when the
	monk is 95th level. Holy word increases the affect of cure light/cure
	serious/cure critical by 50% while unholy word increases the affect of
	cause light/cause serious/cause critical - Martin

	Or simple mass damage to evil or good aligned creatures, must be of
	the correct alignment to use it - Scandum
*/

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_UNHOLY,
		"unholy word",				{ 99, 99, 99, 99, 99, 99, 99, 99 },
		spell_unholy_word,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(643),	17,	PULSE_VIOLENCE,
		"unholy word",				"!Unholy Word!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_HOLY,
		"holy word",	 			{ 99, 99, 99, 99, 99, 99, 99, 99 },
		spell_holy_word,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(644),	17,	PULSE_VIOLENCE,
		"holy word",				"!Holy Word!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"invigorate",				{ 99, 99, 99, 99, 41, 99, 99, 99 },
		spell_invigorate,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(645),	20,	PULSE_VIOLENCE * 2,
		"",						"!Invigorate!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"improved invis",			{ 99, 99, 99, 99, 99, 51, 99, 99 },
		spell_improved_invis,		TAR_CHAR_SELF,		POS_STANDING,
		&gsn_improved_invis,		SLOT(646),	45,	PULSE_VIOLENCE * 2,
		"",						"The shroud of invisibility fades from about your body."
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_MAGIC,
		"truesight",				{  99,  99, 99, 99, 71, 61, 71, 61 },
		spell_truesight,			TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT(647),	 50,	PULSE_VIOLENCE * 3 / 2,
		"",						"The clarity in your vision fades away."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"recharge",				{ 99, 99, 99, 99, 99, 99, 99, 59 },
		spell_recharge,			TAR_OBJ_INV,		POS_STANDING,
		NULL,					SLOT(648),	50,	PULSE_VIOLENCE,
		"",						"!recharge!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"transport",				{ 99, 99, 99, 99, 99, 99, 99, 76 },
		spell_transport,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(649),	50,	PULSE_VIOLENCE,
		"",						"!transport!"
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_MAGIC,
		"anti-magic shell",			{ 99, 99, 99, 99, 99, 99, 99, 91 },
		spell_anti_magic_shell,		TAR_CHAR_SELF,	POS_STANDING,
		&gsn_anti_magic_shell,		SLOT(650),	80,	PULSE_VIOLENCE * 3,
		"",						"The shimmering glow fades from about your body."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_PSIONIC,
		"possess",				{ 99, 99, 99, 99, 99, 99, 99, 81 },
		spell_possess,				TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(651),	25,	PULSE_VIOLENCE * 2,
		"possess spell",			"Your mind is wrenched back to it's rightful home."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_PSIONIC,
		"nightmare",				{ 99, 99, 99, 99, 99, 89, 99, 99 },
		spell_nightmare,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_nightmare,			SLOT(653),	60,	PULSE_VIOLENCE,
		"spell",					"Your panic recedes and your breathing returns to normal."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"smoke",					{ 99, 99, 99, 99, 99, 61, 99, 99 },
		spell_smoke,	 			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(654),	80,	PULSE_VIOLENCE,
		"smoke",					"!Smoke!"
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"slow",		 			{ 99, 99, 99, 99, 99, 99, 40, 99 },
		spell_slow,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_slow,				SLOT(655),	 10,	PULSE_VIOLENCE,
		"slow spell",				"You speed up."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"brew potion",	  			{ 99, 99, 99, 99, 46, 99, 99, 99 },
		spell_brew_potion,			TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT(656),	 10,	PULSE_VIOLENCE,
		"",						NULL
	},

	{
		FSKILL_SPELL,				SFOL_ALL,			RSKILL_NONE,
		DAM_MAGIC,
		"elemental",				{ 99, 99, 99, 99, 71, 99, 99, 99 },
		spell_elemental,			TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT(657),	 10,	PULSE_VIOLENCE * 2,
		"",						NULL
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_MAGIC,
		"fireshield",				{ 91, 91, 81, 81, 71, 61, 71, 61 },
		spell_fire_shield,			TAR_CHAR_SELF,		POS_STANDING,
		&gsn_fire_shield,			SLOT(658),	 50,	PULSE_VIOLENCE * 2,
		"fireshield",				"The flames around your body wink out."
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"unbarring ways",			{ 99, 99, 99, 99, 99, 99, 68, 99 },
		spell_unbarring_ways,		TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(659),	 10,	PULSE_VIOLENCE * 2,
		"",						NULL
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIFE,
		"vampiric touch",			{ 99, 99, 99, 99, 99, 99, 99, 52 },
		spell_vampiric_touch,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_vampiric_touch,		SLOT(660),	35,	PULSE_VIOLENCE,
		"vampiric touch",			"!Vampiric Touch!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_EVIL,
		"protection good",			{ 99, 99, 99, 99, 99, 99, 30, 99 },
		spell_protection_fg,		TAR_CHAR_SELF,		POS_STANDING,
		NULL,					SLOT(661),	 12,	PULSE_VIOLENCE,
		"",						"You feel less protected from good."
	},

/*
	new spells by Manwe
*/

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_MAGIC,
		"metamorphose liquids", 		{99, 99, 99, 99, 11, 99, 99, 99 },
		spell_metamorphose_liquids,	TAR_OBJ_INV,	POS_STANDING,
		NULL,					SLOT(663),	15,	PULSE_VIOLENCE,
		"", 						"!metamorphose liquids!"
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_MAGIC,
		"firewalk",				{99, 99, 99, 99, 35, 35, 35, 35},
		spell_firewalk,			TAR_CHAR_DEFENSIVE, POS_STANDING,
		NULL,					SLOT(664),	30,	PULSE_VIOLENCE * 2,
		"",						"You are no longer fire proof."
	},
	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"steelhand",				{99, 99, 99, 99, 99, 99, 99, 99},
		spell_steelhand,			TAR_CHAR_DEFENSIVE, POS_STANDING,
		NULL,					SLOT(665),	15,	PULSE_VIOLENCE * 2,
		"",						"Your hands become flesh again."
	},

/*
	new spells by Scandum
*/

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"detect good",				{99, 99, 99, 99, 99, 99, 25, 99},
		spell_detect_good,			TAR_CHAR_SELF,	POS_STANDING,
		&gsn_detect_good,			SLOT(665),	5,	PULSE_VIOLENCE,
		"",						"The gold outlines fade from your vision."
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_MAGIC,
		"divine inspiration",		{ 99, 99, 99, 99, 22, 26, 99, 99 },
		spell_divine_inspiration,	TAR_CHAR_DEFENSIVE, POS_STANDING,
		NULL,					SLOT(666),	75,	PULSE_VIOLENCE * 2,
		"",						"Your divine inspiration abruptly fades, leaving you sad and depressed."
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_UNHOLY,
		"black aura",				{99, 99, 99, 99, 99, 99, 99, 86},
		spell_black_aura,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_black_aura,			SLOT(667),	75,	PULSE_VIOLENCE * 2,
		"black aura spell",			"The black aura around your body fades."
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_MAGIC,
		"mana shield",				{99, 99, 99, 99, 99, 72, 99, 86},
		spell_mana_shield,			TAR_CHAR_SELF,	POS_STANDING,
		&gsn_mana_shield,			SLOT(668),	150,	PULSE_VIOLENCE * 2,
		"",						"The blue aura around your body fades."
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_MAGIC,
		"magic mirror",			{99, 99, 99, 99, 99, 71, 99, 99},
		spell_magic_mirror,			TAR_CHAR_SELF,	POS_STANDING,
		&gsn_magic_mirror,			SLOT(669),	150,	PULSE_VIOLENCE * 2,
		"",						"The magical mirror around your body shatters."
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_MAGIC,
		"animate object",			{99, 99, 99, 99, 33, 99, 99, 99},
		spell_animate_object,		TAR_OBJ_INV,	POS_STANDING,
		NULL,					SLOT(670),	75,	PULSE_VIOLENCE * 2,
		"",						"!Animate Object!"
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_LIGHTNING,
		"mana shackles",			{ 99, 99, 99, 99, 99, 99, 99, 37 },
		spell_mana_shackles,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_mana_shackles,			SLOT(671),	50,	PULSE_VIOLENCE * 2,
		"mana shackles spell",		"Your mana shackles disentangle and fall away."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_DARKELF,
		DAM_MAGIC,
		"globe of darkness",		{ 99, 99, 99, 99, 99, 99, 99, 99 },
		spell_globe_of_darkness,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(672),	75,	PULSE_VIOLENCE * 2,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"earthbind",				{ 99, 99, 99, 99, 99, 99, 99, 33 },
		spell_earthbind,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_earthbind,			SLOT(673),	35,	PULSE_VIOLENCE * 3 / 2,
		"earthbind",				"You are no longer bound to the earth."
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_MAGIC,
		"quicken",				{99, 99, 99, 99, 99, 42, 99, 99},
		spell_quicken,				TAR_CHAR_SELF,	POS_STANDING,
		NULL,					SLOT(674),	50,	PULSE_VIOLENCE,
		"",						"Time moves swiftly as your thoughts slow down."
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_FIRE,
		"torrid balm",				{ 99, 99, 99, 99, 99, 99, 99, 41 },
		spell_torrid_balm,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(675),	100,	PULSE_VIOLENCE * 3 / 2,
		"torrid balm",				"The smell of seared flesh dissipates as the torrid balm evaporates."
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_MAGIC,
		"song of the seas",			{ 99, 99, 99, 99, 36, 99, 99, 99 },
		spell_song_of_the_seas,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(676),	25,	PULSE_VIOLENCE,
		"spell",					"You feel unsure and guideless as the song of the seas dies down inside you."
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_MAGIC,
		"ice arrow",				{ 99, 99, 99, 22, 99, 99, 99, 99 },
		spell_ice_arrow,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(677),	10,	PULSE_VIOLENCE / 4,
		"",						"!Ice Arrow!"
	},

	{
		FSKILL_SPELL,				SFOL_GAIA,		RSKILL_NONE,
		DAM_MAGIC,
		"earthen spirit",			{ 99, 99, 99, 99, 36, 99, 99, 99 },
		spell_earthen_spirit,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(678),	25,	PULSE_VIOLENCE,
		"",						"The spirit of the earth recedes."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_LIGHTNING,
		"energy bolt",				{ 99, 99, 99, 99, 99, 99, 99,  3 },
		spell_energy_bolt,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(679),	5,	PULSE_VIOLENCE,
		"energy bolt",				"!Energy Bolt!"
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_MAGIC,
		"ice layer",				{ 99, 99, 99, 99, 44, 99, 99, 99 },
		spell_ice_layer,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,					SLOT(680),	75,	PULSE_VIOLENCE * 2,
		"",						""
	},

	{
		FSKILL_SPELL,				SFOL_ULMO,		RSKILL_NONE,
		DAM_MAGIC,
		"icicle armor",			{ 99, 99, 99, 99, 99, 99, 99, 37 },
		spell_icicle_armor,			TAR_CHAR_SELF,		POS_STANDING,
		&gsn_icicle_armor,			SLOT(681),	75,	PULSE_VIOLENCE,
		"icicle armor",			"Your icicle armor slides back into your body."
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_FIRE,
		"flamewave",				{ 99, 99, 99, 99, 30, 99, 99, 99 },
		spell_flamewave,			TAR_IGNORE,		POS_FIGHTING,
		NULL,					SLOT(682),	20,	PULSE_VIOLENCE,
		"flamewave",				"!Flamewave!"
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_MAGIC,
		"envision",				{ 99, 99, 99, 99, 99, 99, 99, 55 },
		spell_envision,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(683),	100,	PULSE_VIOLENCE,
		"",						"!Envision!"
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_MAGIC,
		"winged call",				{ 99, 99, 99, 99, 99, 54, 99, 99 },
		spell_winged_call,			TAR_IGNORE,		POS_STANDING,
		NULL,					SLOT(684),	150,	PULSE_VIOLENCE,
		"",						"!Winged Call!"
	},

	{
		FSKILL_SPELL,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_EVIL,
		"fear",					{ 99, 99, 99, 99, 99, 99, 99, 26 },
		spell_fear,				TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		&gsn_fear,				SLOT(685),	30,	PULSE_VIOLENCE,
		"fear spell",				"You feel less frightened."
	},

	{
		FSKILL_SPELL,				SFOL_MANWE,		RSKILL_NONE,
		DAM_LIGHTNING,
		"chain lightning",			{ 99, 99, 99, 99, 99, 36, 99, 99 },
		spell_chain_lightning,		TAR_IGNORE,		POS_FIGHTING,
		NULL,					SLOT(686),	45,	PULSE_VIOLENCE,
		"chain lightning",			"!Chain Lightning!"
	},

	{
		FSKILL_SPELL,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_FIRE,
		"flame blade",				{ 99, 99, 99, 99, 99, 56, 99 ,99 },
		spell_flame_blade,			TAR_OBJ_INV,	POS_STANDING,
		&gsn_flame_blade,			SLOT(687),	500,	PULSE_VIOLENCE * 5,
		"flame",					"",
								"The flames surrounding $p wink out."
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"totem",					{ 99, 99, 99, 99, 99, 99, 38 ,99 },
		spell_totem,				TAR_IGNORE,	POS_STANDING,
		NULL,					SLOT(688),	100,	PULSE_VIOLENCE * 2,
		"",						""
	},

	/* Sloth spell created by M. J. Bethlehem (Hypnos) 20041114 */
	{
		FSKILL_SPELL,				SFOL_HYPNOS|SFOL_ILUVATAR,		RSKILL_NONE,
		DAM_MAGIC,
		"sloth",				{ 99, 99, 99, 99, 99, 55, 55, 99 },
		spell_sloth,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(689),	145, PULSE_VIOLENCE / 2,
		"",						"Your are able to move normally again."
	},

	/* Touch of idiocy spell created by M. J. Bethlehem (Hypnos) 20041115 */
	{
		FSKILL_SPELL,				SFOL_HYPNOS,					RSKILL_NONE,
		DAM_MAGIC,
		"touch of idiocy",			{ 99, 99, 99, 99, 26, 26, 26, 26 },
		spell_touch_of_idiocy,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					SLOT(690),	145, PULSE_VIOLENCE / 2,
		"",						"Your mental faculties return to normal."
	},
/*
	Adding slot numbers to affect skills for reference - Scandum 12-04-2002
*/
	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"hide",					{ 99, 99, 99,  2, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_hide,				SLOT(800),	16,	PULSE_VIOLENCE / 2,
		"",						"!Hide!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"sneak",					{ 99, 99, 36, 3, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_sneak,				SLOT(801),	 8,	PULSE_VIOLENCE / 2,
		"",						NULL
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"stealth",				{ 99, 99, 99, 25, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_stealth,				SLOT(802),	 16,	 PULSE_VIOLENCE / 2,
		"",						NULL
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"clear path",				{ 19, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_clear_path,			SLOT(803),	24,	PULSE_VIOLENCE / 2,
		"",						"You stop clearing paths."
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"hunt",		 			{ 13, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_hunt,				SLOT(804),	24,	PULSE_VIOLENCE / 2,
		"",						"You stop hunting for today.", "hunt"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"berserk",				{ 51, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,	POS_FIGHTING,
		&gsn_berserk,				SLOT(805),	 24,	PULSE_VIOLENCE,
		"berserk",				"You start to feel more rational.", "berserk"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_REND,
		"critical hit",			{ 99, 99, 99, 41, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,	POS_FIGHTING,
		&gsn_critical_hit,			SLOT(806),	 0,	0,
		"critical hit",			"Your wounds stop bleeding.", "critical hit"
	},

	{
		FSKILL_NONE,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_NONE,
		"blood frenzy",			{ 15, 15, 25, 25, 99, 99, 35, 99 },
		spell_null,				TAR_IGNORE,	POS_FIGHTING,
		&gsn_bloodfrenzy,			SLOT(807),	24,	PULSE_VIOLENCE * 3 / 2,
		"bloodfrenzy",				"Your blood stops boiling.", "bloodfrenzy"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"cripple",				{ 99, 99, 49, 99, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_cripple,				SLOT(808),	 24,	PULSE_VIOLENCE,
		"low kick",				"You are no longer crippled.", "cripple"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_DOPPELGANGER,
		DAM_NONE,
		"polymorph",				{ 99, 99, 99, 99, 91, 99, 99, 99 },
		spell_null,				TAR_IGNORE,	POS_FIGHTING,
		&gsn_polymorph,			SLOT(809),	600, PULSE_VIOLENCE * 2,
		"",						NULL, "polymorph"
	},

/*
	Skills
*/

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_PIERCE,
		"backstab",				{ 99, 99,  3, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_backstab,				SLOT(  0),	16,	PULSE_VIOLENCE,
		"backstab",				"!Backstab!", "backstab"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_PIERCE,
		"knife",					{ 99, 99, 99, 33, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_knife,				SLOT(  0),	24,	PULSE_VIOLENCE * 2,
		"knife",					"!knife!", "knife"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"martial arts",			{ 99, 99, 99, 1, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_martial_arts,			SLOT(  0),	 0,	0,
		"martial arts",			"!martial arts!", "martial arts"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"acupunch",				{ 99, 99, 99, 13, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_acupunch,				SLOT(  0),	16,	PULSE_VIOLENCE * 3 / 2,
		"acupunch",				"!acupunch!", "acupunch"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"kick",					{  2, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_kick,				SLOT(  0),	16,	PULSE_VIOLENCE,
		"kick",					"!Kick!", "kick"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_TITAN,
		DAM_BASH,
		"bash",					{ 81, 99, 99,  99, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_bash,				SLOT(  0),	 50,	PULSE_VIOLENCE * 3 / 2,
		"bash",					"!bash!", "bash"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"gouge",					{ 99, 99, 46, 99, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_gouge,				SLOT(  0),	20,	PULSE_VIOLENCE,
		"gouge",					"Your vision returns.", "gouge"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_PIERCE,
		"circle",					{ 99, 99, 81, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_circle,				SLOT(  0),	60,	PULSE_VIOLENCE * 3 / 2,
		"circle",					"!circle!", "circle"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"trip",					{ 99, 99, 22, 99, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_trip,				SLOT(  0),	 24,	PULSE_VIOLENCE * 3 / 2,
		"trip",					"!trip!", "trip"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"disarm",					{ 99, 99,  9, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_disarm,				SLOT(  0),	32,	PULSE_VIOLENCE,
		"",						"!Disarm!", "disarm"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"dodge",					{ 99, 99,  6, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_dodge,				SLOT(  0),	 0,	 0,
		"",						"!Dodge!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"enhanced damage",			{  11, 21, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_enhanced_damage,		SLOT(  0),	 0,	 0,
		"",						"!Enhanced Damage!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"parry",					{ 99,  3, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_parry,				SLOT(  0),	 0,	 0,
		"",						"!Parry!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"peek",					{ 99, 99,  7, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_peek,				SLOT(  0),	 0,	 0,
		"",						"!Peek!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"pick lock",				{ 99, 99,  10, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_pick_lock,			SLOT(  0),	 8,	PULSE_VIOLENCE / 2,
		"",						"!Pick!", "pick"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"rescue",					{  7, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_rescue,				SLOT(  0),	 36,	PULSE_VIOLENCE,
		"",						"!Rescue!", "rescue"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"second attack",			{ 4, 7, 8, 18, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_second_attack,			SLOT(  0),	 0,	 0,
		"",						"!Second Attack!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"third attack",			{ 14, 17, 61, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_third_attack,			SLOT(  0),	0,	 0,
		"",						"!Third Attack!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"fourth attack",			{ 66, 71, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_fourth_attack,			SLOT(  0),	 0,	 0,
		"",						"!Fourth Attack!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"fifth attack",			{ 99, 81, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_fifth_attack,			SLOT(  0),	0,	0,
		"",						"!Fifth Attack!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"climb",					{ 99, 99, 33, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_climb,				SLOT(  0),	 5,	PULSE_VIOLENCE / 4,
		"",						"!Climb!", "climb"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"steal",					{ 99, 99,  2, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_steal,				SLOT(  0),	16,	PULSE_VIOLENCE,
		"",						"!Steal!", "steal"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_PIERCE,
		"shoot",					{ 99, 99, 99, 8, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_shoot,				SLOT(  0),	8,	PULSE_VIOLENCE * 2,
		"shot",  					"!shoot!", "shoot"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"track",					{ 17, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_track,				SLOT(0),	 8,	PULSE_VIOLENCE,
		"track",					"!track!", "track"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"notice",					{ 18, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_notice,				SLOT(0),	8,		PULSE_VIOLENCE / 2,
		"notice",					"!notice!", "notice"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_PIERCE,
		"throw",					{ 99, 99,  4, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_throw,				SLOT(  0),	8,	PULSE_VIOLENCE * 2,
		"throw",  				"!throw!", "throw"
	},

	{
	   	FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
	   	DAM_NONE,
		"rearm",					{ 23, 13, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_rearm,				SLOT(0),	0,	0,
		"",						"!rearm!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"shadow",					{ 99, 99, 99,  7, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_shadow,				SLOT(  0),	 8,	 PULSE_VIOLENCE / 2,
		"",						"!shadow!", "shadow"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"divert",					{ 99, 99, 99, 11, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_divert,				SLOT(  0),	 32,	 PULSE_VIOLENCE,
		"",						"!divert!", "divert"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"voice",					{ 99, 99, 99, 17, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_RESTING,
		&gsn_voice,				SLOT(  0),	 0,	 4,
		"",						"!voice!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"bargain",				{ 99, 99, 11, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_bargain,				SLOT(  0),	 0,	 0,
		"",						"!bargain!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"camp",					{ 31, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_RESTING,
		&gsn_camp,				SLOT(  0),	50,	PULSE_VIOLENCE * 4,
		"",						"!camp!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"lock lock",				{ 99, 99, 13, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_lock,				SLOT(  0),	 0,	 4,
		"",						"!lock lock!", "lock"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_POISON,
		"makepoison",				{ 99, 99, 99, 47, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_make_poison,			SLOT(  0),	16,	PULSE_VIOLENCE * 2,
		"poison",	 				"!Poison !"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"disguise",				{ 99, 99, 99, 26, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_RESTING,
		&gsn_disguise,				SLOT(  0),	 4,	PULSE_VIOLENCE / 2,
		"",						"!disguise!", "disguise"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"spy",					{ 99, 99, 99, 31, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_spy,					SLOT(  0),	 8,	 PULSE_VIOLENCE,
		"",						"!spy!", "spy"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"attack",					{ 99, 99, 14, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_attack,				SLOT(  0),	 0,	 0,
		"",						"!attack!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"armor usage",				{ 99, 99, 16, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_armor_usage,			SLOT(  0),	 0,	 0,
		"",						"!armor usage!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"distract",				{ 99, 99, 19, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_distract,				SLOT(  0),	24,	PULSE_VIOLENCE / 2,
		"",						"!distract!", "distract"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"greater peek",			{ 99, 99, 20, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_greater_peek,			SLOT(  0),	 0,	 0,
		"",						"!greater peek!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"snatch",					{ 99, 99, 23, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_RESTING,
		&gsn_snatch,				SLOT(  0),	 4,	PULSE_VIOLENCE / 4,
		"",						"!snatch!", "snatch"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"greater pick",			{ 99, 99, 25, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_greater_pick,			SLOT(  0),	 0,	 0,
		"",						"!greater pick!"
	},

	{
		FSKILL_CMD,				SFOL_ALL,			RSKILL_NONE,
		DAM_NONE,
		"assassinate",				{ 99, 99, 99, 36, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_assassinate,			SLOT(  0),	24,	PULSE_VIOLENCE * 3 / 2,
		"",						"!assassinate!", "assassinate"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"glance",					{ 99, 99, 99, 28, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_glance,				SLOT(  0),	 0,	 0,
		"",						"!glance!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"guard",					{ 99, 99, 99, 66, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_guard,				SLOT(  0),	 0,	 0,
		"",						"!guard!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"forge",					{ 99, 99, 53, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_forge,				SLOT(  0),	 0,	PULSE_VIOLENCE * 6,
		"",						"!forge!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"detect forgery",			{ 99, 99, 27, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_detect_forgery,		SLOT(  0),	 0,	 0,
		"",						"!detect forgery!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"plant",					{ 99, 99, 31, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_plant,				SLOT(  0),	 8,	PULSE_VIOLENCE / 4,
		"",						"!plant!"
	},

	/*
		New skills for Mortal Realms by Martin Gallwey - 4/8/98
	*/

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"greater backstab",			{ 99, 99, 71, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_greater_backstab,		SLOT(  0),	 0,	 0,
		"",						"!greater backstab!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"anatomy",				{ 99, 99, 41, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_anatomy,				SLOT(  0),	 0,	 0,
		"",						"!anatomy!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"greater stealth",			{ 99, 99, 99, 91, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_greater_stealth,		SLOT(  0),	 0,	 0,
		"",						NULL
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"quick draw",	  			{ 99, 99, 99, 56, 99, 99, 99, 99 },
		spell_null,	  			TAR_IGNORE,	POS_STANDING,
		&gsn_quick_draw,			SLOT(0),	0,	0,
		"",						"!quick draw!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"flash powder",		 	{ 99, 99, 99, 61, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_flash_powder,			SLOT(  0),	 0,	 0,
		"",		 				NULL
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"muffle",	 				{ 99, 99, 99, 39, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_muffle,				SLOT(  0),	 0,	 0,
		"",		 				NULL
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"pass without trace",		{ 99, 99, 99, 72, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_pass_without_trace,		SLOT(  0),	 0,	 0,
		"",						NULL
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_BASH,
		"door bash",				{ 36, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_door_bash,			SLOT(  0),	 60,	PULSE_VIOLENCE * 3 / 2,
		"door bash",				"!door bash!", "doorbash"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"greater hunt",			{ 61, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_greater_hunt,			SLOT(  0),	 0,	0,
		"",						"!greater hunt!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"greater track",			{ 46, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_greater_track,			SLOT(  0),	 0,	 0,
		"",						"!greater track!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_KRAKEN,
		DAM_NONE,
		"dual wield",				{ 99, 91, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_dual_wield,			SLOT(  0),	0,	 0,
		"",						"!dual wield!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"mass",					{ 99, 99, 99, 99, 99, 46, 44, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_mass,				SLOT(  0),	 0,	 PULSE_VIOLENCE,
		"",						"!mass!"
	},

	/*
		New skills by Scandum
	*/

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"shield block",			{ 16, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_shield_block,			SLOT(  0),	 0,	 0,
		"",						"!shield block!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"fisticuffs",				{ 99, 99, 99, 99, 99, 99,  1, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_fisticuffs,			SLOT(  0),	0,	0,
		"fist blow",				"!fisticuffs!"
	},

	{
		FSKILL_SPELL,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"range cast",				{ 99, 99, 99, 99, 12, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_range_cast,			SLOT(  0),	0,	PULSE_VIOLENCE,
		"",						"!range cast!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_VAMPIRE|RSKILL_WEREWOLF|RSKILL_WRAITH,
		DAM_LIFE,
		"drain",					{ 99, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_drain,				SLOT(  0),	16,	PULSE_VIOLENCE * 3 / 2,
		"vampiric bite",			"!drain!",	"drain"
	},

	{
		FSKILL_CMD,				SFOL_MANWE,		RSKILL_NONE,
		DAM_THRUST,
		"whirl",					{76, 62, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE, POS_FIGHTING,
		&gsn_whirl,				SLOT(  0),	32,	PULSE_VIOLENCE * 3 / 2,
		"whirl",					"!whirl!",	"whirl"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"reposte",				{99, 37, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_reposte,				SLOT(  0),	0,	0,
		"",						"!reposte!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_BASH,
		"head butt",				{ 99, 99, 99, 99, 99, 99, 26, 99 },
		spell_null,				TAR_CHAR_OFFENSIVE, POS_FIGHTING,
		&gsn_head_butt,			SLOT(  0),	20,	PULSE_VIOLENCE * 3 / 2,
		"head butt",				"!headbutt!", "headbutt"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"sword fighting",			{ 01, 04, 17, 99, 99, 26, 99, 99 },
		spell_null,				TAR_IGNORE, 		POS_STANDING,
		&gsn_weapon_sword,			SLOT(  0),	0,	0,
		"",						"!sword fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"dagger fighting",			{  9, 14, 01, 23, 99, 6, 99, 12 },
		spell_null,				TAR_IGNORE, 		POS_STANDING,
		&gsn_weapon_dagger,			SLOT(  0),	0,	0,
		"",						"!dagger fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"axe fighting",			{ 20, 26, 99, 99, 22, 99, 56, 99 },
		spell_null,				TAR_IGNORE, 		POS_STANDING,
		&gsn_weapon_axe,			SLOT(  0),	0,	0,
		"",						"!axe fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"mace fighting",			{ 42, 99, 99, 99, 01, 99, 28, 99 },
		spell_null,				TAR_IGNORE, 		POS_STANDING,
		&gsn_weapon_mace,			SLOT(  0),	0,	0,
		"",						"!mace fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"staff fighting",			{ 99, 32, 99, 76, 11, 01, 99, 22 },
		spell_null,				TAR_IGNORE, 		POS_STANDING,
		&gsn_weapon_staff,			SLOT(  0),	0,	0,
		"",						"!staff fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"whip fighting",			{ 99, 18, 43, 42, 99, 99, 99, 01 },
		spell_null,				TAR_IGNORE, 		POS_STANDING,
		&gsn_weapon_whip,			SLOT(  0),	0,	0,
		"",						"!whip fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"flail fighting",			{ 33, 23, 38, 99, 30, 99, 77, 99 },
		spell_null,				TAR_IGNORE, 		POS_STANDING,
		&gsn_weapon_flail,			SLOT(  0),	0,	0,
		"",						"!flail fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"spear fighting",			{ 54, 01, 24, 51, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_weapon_spear,			SLOT(  0),	0,	0,
		"",						"!spear fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"claw fighting",			{ 99, 99, 99, 10, 99, 99, 64, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_weapon_claw,			SLOT(  0),	0,	0,
		"",						"!claw fighting!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_THRUST,
		"brawling",				{ 99, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_brawling,				SLOT(  0),	 0,	0,
		"",						"!brawling!"
	},

	{
		FSKILL_NONE,				SFOL_GAIA,		RSKILL_NONE,
		DAM_THRUST,
		"resilience",				{ 28, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_resilience,			SLOT(  0),	0,	0,
		"",						"!resilience!"
	},

	{
		FSKILL_NONE,				SFOL_MANWE,		RSKILL_NONE,
		DAM_THRUST,
		"lightning reflexes",		{ 99, 99, 99, 15, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_lightning_reflexes,		SLOT(  0),	0,	0,
		"",						"!lightning reflexes!"
	},

	{
		FSKILL_NONE,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_THRUST,
		"precision",				{ 99, 27, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_precision,			SLOT(  0),	0,	0,
		"",						"!precision!"
	},

	{
		FSKILL_NONE,				SFOL_DEMISE,		RSKILL_NONE,
		DAM_NONE,
		"warmth",					{ 99, 99, 99, 99, 99, 99, 99, 26 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_warmth,				SLOT(  0),	0,	0,
		"",						"warmth"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"recite",					{ 99, 99, 99, 99,  6,  4,  7,  2 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_recite,				SLOT(  0),	 4,	PULSE_VIOLENCE * 3 / 2,
		"",						"!recite!",		"recite"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"brandish",				{ 99, 99, 99, 99,  4, 12,  9,  7 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_brandish,				SLOT(  0),	 4,	PULSE_VIOLENCE * 3 / 2,
		"",						"!brandish!",		"brandish"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_NONE,
		DAM_MAGIC,
		"zap",					{ 99, 99, 99, 99, 99,  4, 99,  6 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_zap,					SLOT(  0),	 4,	PULSE_VIOLENCE * 3 / 2,
		"zap",					"!zap!",			"zap"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_NONE,
		DAM_NONE,
		"scroll mastery",			{ 99, 99, 99, 99, 99, 99, 99, 31 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_scroll_mastery,		SLOT(  0),	0,	0,
		"",						"scroll mastery"
	},

	{
		FSKILL_CMD,				SFOL_GAIA,		RSKILL_NONE,
		DAM_NONE,
		"ambush",					{ 99, 99, 99, 44, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_ambush,				SLOT(0),		8,	PULSE_VIOLENCE,
		"",						"!ambush!"
	},

	{
		FSKILL_NONE,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_NONE,
		"first strike",			{ 99, 49, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_first_strike,			SLOT(0),	 0,	0,
		"",						"!first strike!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_ELF|RSKILL_HALFELF|RSKILL_HUMAN|RSKILL_WRAITH|RSKILL_VAMPIRE,
		DAM_NONE,
		"mount",					{ 22, 99, 48, 82, 34, 64, 99, 36 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_mount,				SLOT(0),	 16,		PULSE_VIOLENCE,
		"",						"!mount!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_ELF|RSKILL_HALFELF|RSKILL_DEMON,
		DAM_NONE,
		"tame",					{ 72, 99, 99, 99, 48, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_tame,				SLOT(0),	 64,		PULSE_VIOLENCE * 2,
		"",						"!tame!"
	},

	{
		FSKILL_CMD,				SFOL_NONE,		RSKILL_HOBBIT,
		DAM_NONE,
		"withdraw",				{ 99, 99, 29, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_withdraw,				SLOT(0),	0,		0,
		"",						"!withdraw!"
	},

	{
		FSKILL_NONE,				SFOL_GAIA,		RSKILL_ORC,
		DAM_NONE,
		"endurance",				{ 15, 99, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_endurance,			SLOT(0),		0,	0,
		"",						"!withdraw!"
	},

	{
		FSKILL_NONE,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_NONE,
		"perception",				{ 99, 99, 99, 19, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_perception,			SLOT(0),		0,	0,
		"",						"!perception!"
	},

	{
		FSKILL_NONE,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_PIERCE,
		"impale",					{ 99, 63, 99, 99, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_impale,				SLOT(  0),	0,	0,
		"impaling thrust",			"!impale!"
	},

	{
		FSKILL_CMD,				SFOL_HYPNOS,		RSKILL_NONE,
		DAM_NONE,
		"strangle",				{ 99, 99, 99, 53, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_strangle,				SLOT(  0),	50,	PULSE_VIOLENCE * 1,
		"",						"!strangle!"
	},

	{
		FSKILL_NONE,				SFOL_NONE,		RSKILL_KRAKEN,
		DAM_NONE,
		"tumble",					{ 99, 99, 99, 35, 99, 99, 99, 99 },
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_tumble,				SLOT(  0),	 0,	 0,
		"",						"!Tumble!"
	}
};

const	struct	topic_data topic_table	[MAX_TOPIC]	=
{
	{"Announcements",			 5},
	{"Game Changes and Ideas", 	 5},
	{"Area Creators", 			 5},
	{"Bugs and Typos",			 5},
	{"Complaints", 			 5},
	{"Chat",					 5},
	{"Private Note Boards",		 5}
};

const	struct	bitvector_type bitvector_table	[MAX_BITVECTOR]	=
{
	{ "ACT_IS_NPC",			BV01		},
	{ "ACT_SENTINEL",			BV02		},
	{ "ACT_SCAVENGER",			BV03		},
	{ "ACT_DRUNK",				BV04		},
	{ "ACT_MOUNT",				BV05		},
	{ "ACT_AGGRESSIVE",			BV06		},
	{ "ACT_STAY_SECTOR",		BV07		},
	{ "ACT_WIMPY",				BV08		},
	{ "ACT_PET",				BV00		},
	{ "ACT_TRAIN",				BV10		},
	{ "ACT_PRACTICE",			BV11		},
	{ "ACT_WEAK",				BV12		},
	{ "ACT_SMART",				BV13		},
	{ "ACT_ONE_FIGHT",			BV14		},
	{ "ACT_NO_ORDER",			BV15		},
	{ "ACT_RIDE",				BV16		},
	{ "ACT_BODY",				BV17		},
	{ "ACT_RACE",				BV18		},
	{ "ACT_UNDEAD",			BV19		},
	{ "ACT_ELEMENTAL",			BV20		},
	{ "ACT_CLAN_GUARD",			BV21		},
	{ "ACT_CLAN_HEALER",		BV22		},
	{ "ACT_WILL_DIE",			BV23		},
	{ "ACT_NO_DAMAGE",			BV24		},
	{ "ACT_CLASS",				BV25		},

	{ "AFF_BLIND",				BV01		},
	{ "AFF_INVISIBLE",			BV02		},
	{ "AFF_IMP_INVISIBLE",		BV03		},
	{ "AFF_DETECT_INVIS",		BV04		},
	{ "AFF_RESERVED_B",			BV05		},
	{ "AFF_DETECT_HIDDEN",		BV06		},
	{ "AFF_TRUESIGHT",			BV07		},
	{ "AFF_SANCTUARY",			BV08		},
	{ "AFF_RESERVED_C",			BV09		},
	{ "AFF_INFRARED",			BV10		},
	{ "AFF_CURSE",				BV11		},
	{ "AFF_UNDERSTAND",			BV12		},
	{ "AFF_POISON",			BV13		},
	{ "AFF_PROTECT_EVIL",		BV14		},
	{ "AFF_PROTECT_GOOD",		BV15		},
	{ "AFF_SNEAK",				BV16		},
	{ "AFF_HIDE",				BV17		},
	{ "AFF_SLEEP",				BV18		},
	{ "AFF_CHARM",				BV19		},
	{ "AFF_FLYING",			BV20		},
	{ "AFF_PASS_DOOR",			BV21		},
	{ "AFF_STEALTH",			BV22		},
	{ "AFF_CLEAR",				BV23		},
	{ "AFF_HUNT",				BV24		},
	{ "AFF_TONGUES",			BV25		},
	{ "AFF_ETHEREAL",			BV26		},
	{ "AFF_HASTE",				BV27		},
	{ "AFF_STABILITY",			BV28		},

	{ "AFLAG_NODEBUG",			BV01		},
	{ "AFLAG_NOTELEPORT",		BV02		},
	{ "AFLAG_NOGOHOME",			BV03		},
	{ "AFLAG_NORECALL",			BV04		},
	{ "AFLAG_NOCASTLE",			BV05		},
	{ "AFLAG_NORIP",			BV06		},
	{ "AFLAG_FREEQUIT",			BV07		},
	{ "AFLAG_NOSUMMON",			BV08		},
	{ "AFLAG_AUTOSAVE",			BV09		},
	{ "AFLAG_MODIFIED",			BV10		},
	{ "AFLAG_WEATHER",			BV11		},

	{ "APPLY_NONE",			0		},
	{ "APPLY_STR",				1		},
	{ "APPLY_DEX",				2		},
	{ "APPLY_INT",				3		},
	{ "APPLY_WIS",				4		},
	{ "APPLY_CON",				5		},
	{ "APPLY_SEX",				6		},
	{ "APPLY_RACE",			7		},
	{ "APPLY_LEVEL",			8		},
	{ "APPLY_AGE",				9		},
	{ "APPLY_HEIGHT",			10		},
	{ "APPLY_WEIGHT",			11		},
	{ "APPLY_MANA",			12		},
	{ "APPLY_HIT",				13		},
	{ "APPLY_MOVE",			14		},
	{ "APPLY_AC",				17		},
	{ "APPLY_HITROLL",			18		},
	{ "APPLY_DAMROLL",			19		},
	{ "APPLY_SAVING_PARA",		20		},
	{ "APPLY_SAVING_ROD",		21		},
	{ "APPLY_SAVING_PETRI",		22		},
	{ "APPLY_SAVING_BREATH",		23		},
	{ "APPLY_SAVING_SPELL",		24		},

	{ "BODY_HEAD",				BV01		},
	{ "BODY_MOUTH",			BV02		},
	{ "BODY_EYE",				BV03		},
	{ "BODY_TORSO",			BV04		},
	{ "BODY_HIP",				BV05		},
	{ "BODY_LEG",				BV06		},
	{ "BODY_ARM",				BV07		},
	{ "BODY_WING",				BV08		},
	{ "BODY_TAIL",				BV09		},
	{ "BODY_TENTACLE",			BV10		},
	{ "BODY_HORN",				BV11		},
	{ "BODY_CLAW",				BV12		},
	{ "BODY_HAND",				BV13		},
	{ "BODY_FOOT",				BV14		},
	{ "BODY_BRANCH",			BV15		},
	{ "BODY_ROOT",				BV16		},
	{ "BODY_TWIG",				BV17		},
	{ "BODY_TRUNK",			BV18		},
	{ "BODY_PINCERS",			BV19		},
	{ "BODY_SPINNARET",			BV20		},
	{ "BODY_BEAK",				BV21		},

	{ "CLASS_WARRIOR",			0		},
	{ "CLASS_GLADIATOR",		1		},
	{ "CLASS_MARAUDER",			2		},
	{ "CLASS_NINJA",			3		},
	{ "CLASS_DRUID",			4		},
	{ "CLASS_SORCERER",			5		},
	{ "CLASS_SHAMAN",			6		},
	{ "CLASS_WARLOCK",			7		},

	{ "CONT_CLOSEABLE",			BV01		},
	{ "CONT_PICKPROOF",			BV02		},
	{ "CONT_CLOSED",			BV03		},
	{ "CONT_LOCKED",			BV04		},
	{ "CONT_MAGICAL_LOCK",		BV05		},
	{ "CONT_TRAPPED_LOCK",		BV06		},
	{ "CONT_MECHANICAL_LOCK",	BV07		},
	{ "CONT_SMALL_LOCK",		BV08		},
	{ "CONT_BIG_LOCK",			BV08		},
	{ "CONT_EASY_PICK",			BV10		},
	{ "CONT_HARD_PICK",			BV11		},

	{ "DIR_NORTH",				0		},
	{ "DIR_EAST",				1		},
	{ "DIR_SOUTH",				2		},
	{ "DIR_WEST",				3		},
	{ "DIR_UP",				4		},
	{ "DIR_DOWN",				5		},

	{ "DOOR_OPEN",				 0		},
	{ "DOOR_CLOSED",			 1		},
	{ "DOOR_CLOSED_LOCKED",		 2		},

	{ "EX_ISDOOR",				BV01		},
	{ "EX_CLOSED",				BV02		},
	{ "EX_LOCKED",				BV03		},
	{ "EX_HIDDEN",				BV04		},
	{ "EX_RIP",				BV05		},
	{ "EX_PICKPROOF",			BV06		},
	{ "EX_BASHPROOF",			BV07		},
	{ "EX_MAGIC_PROOF",			BV08		},
	{ "EX_BASHED",				BV09		},
	{ "EX_UNBARRED",			BV10		},
	{ "EX_BACKDOOR",			BV11		},
	{ "EX_CLAN_BACKDOOR",		BV12		},
	{ "EX_PASSPROOF",			BV13		},
	{ "EX_MAGICAL_LOCK",		BV14		},
	{ "EX_TRAPPED_LOCK",		BV15		},
	{ "EX_MECHANICAL_LOCK",		BV16		},
	{ "EX_SMALL_LOCK",			BV17		},
	{ "EX_BIG_LOCK",			BV18		},
	{ "EX_EASY_PICK",			BV19		},
	{ "EX_HARD_PICK",			BV20		},

	{ "FLAG_CLASS_WARRIOR",		BV01		},
	{ "FLAG_CLASS_GLADIATOR",	BV02		},
	{ "FLAG_CLASS_MARAUDER",		BV03		},
	{ "FLAG_CLASS_NINJA",		BV04		},
	{ "FLAG_CLASS_DRUID",		BV05		},
	{ "FLAG_CLASS_SORCERER",		BV06		},
	{ "FLAG_CLASS_SHAMAN",		BV07		},
	{ "FLAG_CLASS_WARLOCK",		BV08		},

	{ "FURNITURE_SLEEP_IN",		1		},
	{ "FURNITURE_SLEEP_ON",		2		},
	{ "FURNITURE_REST_IN",		4		},
	{ "FURNITURE_REST_ON",		8		},
	{ "FURNITURE_SIT_IN",		16		},
	{ "FURNITURE_SIT_ON",		32		},
	{ "FURNITURE_STAND_IN",		64		},
	{ "FURNITURE_STAND_ON",		128		},

	{ "GATE_RANDOM",			1		},
	{ "GATE_GOWITH",			2		},
	{ "GATE_NOFLEE",			4		},
	{ "GATE_STEP_THROUGH",		8		},
	{ "GATE_STEP_INTO",			16		},
	{ "GATE_RANDOM_AREA",		32		},

	{ "GOD_NEUTRAL",			0		},
	{ "GOD_DEMISE",			1		},
	{ "GOD_MANWE",				2		},
	{ "GOD_HYPNOS",			3		},
	{ "GOD_ULMO",				4		},
	{ "GOD_GAIA",				5		},
	{ "GOD_ILUVATAR",			6		},

	{ "ITEM_FLAG_GLOW",			BV01		},
	{ "ITEM_FLAG_HUM",			BV02		},
	{ "ITEM_FLAG_DARK",			BV03		},
	{ "ITEM_FLAG_LOCK",			BV04		},
	{ "ITEM_FLAG_EVIL",			BV05		},
	{ "ITEM_FLAG_INVIS",		BV06		},
	{ "ITEM_FLAG_MAGIC",		BV07		},
	{ "ITEM_FLAG_NODROP",		BV08		},
	{ "ITEM_FLAG_BLESS",		BV09		},
	{ "ITEM_FLAG_ANTI_GOOD",		BV10		},
	{ "ITEM_FLAG_ANTI_EVIL",		BV11		},
	{ "ITEM_FLAG_ANTI_NEUTRAL",	BV12		},
	{ "ITEM_FLAG_NOREMOVE",		BV13		},
	{ "ITEM_FLAG_INVENTORY",		BV14		},
	{ "ITEM_FLAG_BURNING",		BV15		},
	{ "ITEM_FLAG_NOT_VALID",		BV16		},
	{ "ITEM_FLAG_AUTO_ENGRAVE",	BV17		},
	{ "ITEM_FLAG_ETHEREAL",		BV19		},
	{ "ITEM_FLAG_MOUNT",		BV20		},
	{ "ITEM_FLAG_MODIFIED",		BV21		},

	{ "ITEM_TYPE_NOTHING",		0		},
	{ "ITEM_TYPE_LIGHT",		1		},
	{ "ITEM_TYPE_SCROLL",		2		},
	{ "ITEM_TYPE_WAND",			3		},
	{ "ITEM_TYPE_STAFF",		4		},
	{ "ITEM_TYPE_WEAPON",		5		},
	{ "ITEM_TYPE_TREASURE",		8		},
	{ "ITEM_TYPE_ARMOR",		9		},
	{ "ITEM_TYPE_POTION",		10		},
	{ "ITEM_TYPE_FURNITURE",		12		},
	{ "ITEM_TYPE_TRASH",		13		},
	{ "ITEM_TYPE_CONTAINER",		15		},
	{ "ITEM_TYPE_DRINK_CON",		17		},
	{ "ITEM_TYPE_KEY",			18		},
	{ "ITEM_TYPE_FOOD",			19		},
	{ "ITEM_TYPE_MONEY",		20		},
	{ "ITEM_TYPE_BOAT",			22		},
	{ "ITEM_TYPE_CORPSE_NPC",	23		},
	{ "ITEM_TYPE_CORPSE_PC",		24		},
	{ "ITEM_TYPE_FOUNTAIN",		25		},
	{ "ITEM_TYPE_PILL",			26		},
	{ "ITEM_TYPE_PORTAL",		27		},
	{ "ITEM_TYPE_ARTIFACT",		28		},
	{ "ITEM_TYPE_LOCKPICK",		29		},
	{ "ITEM_TYPE_AMMO",			30		},
	{ "ITEM_TYPE_TOTEM",		31		},
	{ "ITEM_TYPE_PLANT",		32		},

	{ "ITEM_WEAR_TAKE",			1		},
	{ "ITEM_WEAR_FINGER",		2		},
	{ "ITEM_WEAR_NECK",			4		},
	{ "ITEM_WEAR_BODY",			8		},
	{ "ITEM_WEAR_HEAD",			16		},
	{ "ITEM_WEAR_LEGS",			32		},
	{ "ITEM_WEAR_FEET",			64		},
	{ "ITEM_WEAR_HANDS",		128		},
	{ "ITEM_WEAR_ARMS",			256		},
	{ "ITEM_WEAR_SHIELD",		512		},
	{ "ITEM_WEAR_ABOUT",		1024		},
	{ "ITEM_WEAR_WAIST",		2048		},
	{ "ITEM_WEAR_WRIST",		4096		},
	{ "ITEM_WEAR_WIELD",		8192		},
	{ "ITEM_WEAR_HOLD",			16384	},

	{ "LIQ_WATER",				 0		},
	{ "LIQ_BEER",				 1		},
	{ "LIQ_WINE",				 2		},
	{ "LIQ_ALE",				 3		},
	{ "LIQ_DARKALE",			 4		},
	{ "LIQ_WHISKY",			 5		},
	{ "LIQ_LEMONADE",			 6		},
	{ "LIQ_FIREBRT",			 7		},
	{ "LIQ_LOCALSPC",			 8		},
	{ "LIQ_SLIME",				 9		},
	{ "LIQ_MILK",				10		},
	{ "LIQ_TEA",				11		},
	{ "LIQ_COFFEE",			12		},
	{ "LIQ_BLOOD",				13		},
	{ "LIQ_SALTWATER",			14		},
	{ "LIQ_COKE",				15		},
	{ "LIQ_ORANGEJUICE",		16		},
	{ "LIQ_BRANDY",			17		},
	{ "LIQ_ICEWINE",			18		},
	{ "LIQ_RUM",				19		},
	{ "LIQ_VODKA",				20		},
	{ "LIQ_CHAMPAGNE",			21		},

	{ "OAPPLY_HIT",			1		},
	{ "OAPPLY_MOVE",			2		},
	{ "OAPPLY_MANA",			3		},
	{ "OAPPLY_ALIGNMENT",		4		},

	{ "OIF_MCLASS_WARRIOR",		 0		},
	{ "OIF_MCLASS_GLADIATOR",	 1		},
	{ "OIF_MCLASS_MARAUDER",		 2		},
	{ "OIF_MCLASS_NINJA",		 3		},
	{ "OIF_MCLASS_DRUID",		 4		},
	{ "OIF_MCLASS_SORCERER",		 5		},
	{ "OIF_MCLASS_SHAMAN",		 6		},
	{ "OIF_MCLASS_WARLOCK",		 7		},
	{ "OIF_WEAR_LOC",			 8		},
	{ "OIF_TIME_OF_DAY",		 9		},
	{ "OIF_WEATHER",			10		},
	{ "OIF_RANDOM_PERCENT",		11		},
	{ "OIF_USER_PERCENT_HITPT",	12		},
	{ "OIF_USER_PERCENT_MANA",	13		},
	{ "OIF_USER_PERCENT_MOVE",	14		},
	{ "OIF_USER_SECTOR",		15		},
	{ "OIF_USER_ALIGNMENT",		16		},
	{ "OIF_USER_GOLD",			17		},
	{ "OIF_USER_CLASS",			18		},
	{ "OIF_USER_WHICH_GOD",		19		},
	{ "OIF_USER_AREA",			20		},
	{ "OIF_USER_LEVEL",			21 		},
	{ "OIF_USER_POSITION",		22		},
	{ "OIF_USER_RACE",			23		},
	{ "OIF_USER_SEX",			24		},
	{ "OIF_USER_ROOM_NUM",		25		},
	{ "OIF_USER_FIGHTING",		26		},

	{ "OPROG_ECHO",			 0		},
	{ "OPROG_GOD_COMMAND",		 1		},
	{ "OPROG_GOD_ARGUMENT",		 2		},
	{ "OPROG_COMMAND",			 3		},
	{ "OPROG_ARGUMENT",			 4		},
	{ "OPROG_IF_HAS_OBJECT",		 5		},
	{ "OPROG_IF",				 6		},
	{ "OPROG_JUNK",			 7		},
	{ "OPROG_QUEST_SET",		 8		},
	{ "OPROG_QUEST_ADD",		 9		},
	{ "OPROG_OBJECT_QUEST_IF",	10		},
	{ "OPROG_PLAYER_QUEST_IF",	11		},
	{ "OPROG_APPLY",			12		},

	{ "PICK_MAGICAL_LOCK",		BV01		},
	{ "PICK_TRAPPED_LOCK",		BV02		},
	{ "PICK_MECHANICAL_LOCK",	BV03		},
	{ "PICK_SMALL_LOCK",		BV04		},
	{ "PICK_BIG_LOCK",			BV05		},
	{ "PICK_EASY_PICK",			BV06		},
	{ "PICK_HARD_PICK",			BV07		},

	{ "POISONED_FALSE",			0		},
	{ "POISONED_TRUE",			1		},

	{ "POS_DEAD",				0		},
	{ "POS_MORTAL",			1		},
	{ "POS_INCAP",				2		},
	{ "POS_STUNNED",			3		},
	{ "POS_SLEEPING",			4		},
	{ "POS_RESTING",			5		},
	{ "POS_SITTING",			6		},
	{ "POS_FIGHTING",			7		},
	{ "POS_STANDING",			8		},

	{ "RACE_HUMAN",			0		},
	{ "RACE_ELF",				1		},
	{ "RACE_DARKELF",			2		},
	{ "RACE_DWARF",			3		},
	{ "RACE_GNOME",			4		},
	{ "RACE_ORC",				5		},
	{ "RACE_GRIFFON",			6		},
	{ "RACE_HALFELF",			7		},
	{ "RACE_OGRE",				8		},
	{ "RACE_HOBBIT",			9		},
	{ "RACE_SPRITE",			10		},
	{ "RACE_SHADE",			11		},
	{ "RACE_ROC",				12		},
	{ "RACE_THREEKREEN",		13		},
	{ "RACE_WEREWOLF",			14		},
	{ "RACE_DEMON",			15		},
	{ "RACE_GARGOYLE",			16		},
	{ "RACE_WRAITH",			17		},
	{ "RACE_TROLL",			18		},
	{ "RACE_VAMPIRE",			19		},
	{ "RACE_PHOENIX",			20		},
	{ "RACE_DOPPELGANGER",		21		},
	{ "RACE_ARACHNID",			22		},
	{ "RACE_TITAN",			23		},
	{ "RACE_EAGLE",			24		},
	{ "RACE_KRAKEN",			25		},
	{ "RACE_BALROG",			26		},
	{ "RACE_DRAGON",			27		},
	{ "RACE_ENT",				28		},

	{ "ROOM_DARK",				BV01		},
	{ "ROOM_SMOKE",			BV02		},
	{ "ROOM_NO_MOB",			BV03		},
	{ "ROOM_INDOORS",			BV04		},
	{ "ROOM_GLOBE",			BV05		},
	{ "ROOM_NO_GOHOME",			BV06		},
	{ "ROOM_CLAN_DONATION",		BV07		},
	{ "ROOM_ALTAR_N",			BV08		},
	{ "ROOM_BANK",				BV09		},
	{ "ROOM_PRIVATE",			BV10		},
	{ "ROOM_SAFE",				BV11		},
	{ "ROOM_SOLITARY",			BV12		},
	{ "ROOM_PET_SHOP",			BV13		},
	{ "ROOM_NO_RECALL",			BV14		},
	{ "ROOM_RIP",				BV15		},
	{ "ROOM_BLOCK",			BV16		},
	{ "ROOM_NO_SAVE",			BV17		},
	{ "ROOM_MORGUE",			BV18		},
	{ "ROOM_ALLOW_WAR",			BV19		},
	{ "ROOM_ALLOW_GLA",			BV20		},
	{ "ROOM_ALLOW_MAR",			BV21		},
	{ "ROOM_ALLOW_NIN",			BV22		},
	{ "ROOM_ALLOW_DRU",			BV23		},
	{ "ROOM_ALLOW_SOR",			BV24		},
	{ "ROOM_ALLOW_SHA",			BV25		},
	{ "ROOM_ALLOW_WLC",			BV26		},
	{ "ROOM_IS_CASTLE",			BV27		},
	{ "ROOM_IS_ENTRANCE",		BV28		},
	{ "ROOM_NOTE_BOARD",		BV29		},
	{ "ROOM_NO_CASTLE",			BV30		},
	{ "ROOM_NO_RIP",			BV31		},
	{ "ROOM_MAZE",				BV32		},
	{ "ROOM_ICE",				BV33		},
	{ "ROOM_DYNAMIC",			BV34		},
	{ "ROOM_WILDERNESS",		BV35		},

	{ "SECT_INSIDE",			0		},
	{ "SECT_CITY",				1		},
	{ "SECT_FIELD",			2		},
	{ "SECT_FOREST",			3		},
	{ "SECT_HILLS",			4		},
	{ "SECT_MOUNTAIN",			5		},
	{ "SECT_LAKE",				6		},
	{ "SECT_RIVER",			7		},
	{ "SECT_OCEAN",			8		},
	{ "SECT_AIR",				9		},
	{ "SECT_DESERT",			10		},
	{ "SECT_LAVA",				11		},
	{ "SECT_ETHEREAL",			12		},
	{ "SECT_ASTRAL",			13		},
	{ "SECT_UNDER_WATER",		14		},
	{ "SECT_UNDER_GROUND",		15		},
	{ "SECT_DEEP_EARTH",		16		},
	{ "SECT_ROAD",				17		},
	{ "SECT_SWAMP",			18		},
	{ "SECT_BEACH",			19		},
	{ "SECT_TUNDRA",			20		},
	{ "SECT_EDGE",				21		},

	{ "SEX_NEUTRAL",			0		},
	{ "SEX_MALE",				1		},
	{ "SEX_FEMALE",			2		},

	{ "SKILL_HIDE",			800		},
	{ "SKILL_SNEAK",			801		},
	{ "SKILL_STEALTH",			802		},
	{ "SKILL_CLEAR_PATH",		803		},
	{ "SKILL_HUNT",			804		},
	{ "SKILL_BERSERK",			805		},
	{ "SKILL_CRITICAL_HIT",		806		},
	{ "SKILL_BLOOD_FRENZY",		807		},
	{ "SKILL_CRIPPLE",			808		},
	{ "SKILL_POLYMORPH",		809		},

	{ "SPELL_NONE",			0		},
	{ "SPELL_ARMOR",			1		},
	{ "SPELL_TELEPORT",			2		},
	{ "SPELL_BLESS",			3		},
	{ "SPELL_BLINDNESS",		4		},
	{ "SPELL_BURNING_HANDS",		5		},
	{ "SPELL_CALL_LIGHTNING",	6		},
	{ "SPELL_CHARM_PERSON",		7		},
	{ "SPELL_CHILL_TOUCH",		8		},
	{ "SPELL_STONE_FIST",		9		},
	{ "SPELL_COLOUR_SPRAY",		10		},
	{ "SPELL_CONTROL_WEATHER",	11		},
	{ "SPELL_CREATE_FOOD",		12		},
	{ "SPELL_CURE_BLINDNESS",	14		},
	{ "SPELL_CURE_CRITICAL",		15		},
	{ "SPELL_CURE_LIGHT",		16		},
	{ "SPELL_CURSE",			17		},
	{ "SPELL_DETECT_EVIL",		18		},
	{ "SPELL_DETECT_INVIS",		19		},
	{ "SPELL_DETECT_POISON",		21		},
	{ "SPELL_DISPEL_EVIL",		22		},
	{ "SPELL_EARTHQUAKE",		23		},
	{ "SPELL_ENCHANT_WEAPON",	24		},
	{ "SPELL_ENERGY_DRAIN",		25		},
	{ "SPELL_FIREBALL",			26		},
	{ "SPELL_HARM",			27		},
	{ "SPELL_HEAL",			28		},
	{ "SPELL_INVIS",			29		},
	{ "SPELL_PETRIFYING_TOUCH",	30		},
	{ "SPELL_LOCATE_OBJECT",		31		},
	{ "SPELL_MAGIC_MISSILE",		32		},
	{ "SPELL_POISON",			33		},
	{ "SPELL_PROTECTION_EVIL",	34		},
	{ "SPELL_REMOVE_CURSE",		35		},
	{ "SPELL_SANCTUARY",		36		},
	{ "SPELL_SHOCKING_GRASP",	37		},
	{ "SPELL_SLEEP",			38		},
	{ "SPELL_GIANT_STRENGTH",	39		},
	{ "SPELL_VENTRILOQUATE",		41		},
	{ "SPELL_WORD_OF_RECALL",	42		},
	{ "SPELL_CURE_POISON",		43		},
	{ "SPELL_DETECT_HIDDEN",		44		},
	{ "SPELL_WINDBLAST",		45		},
	{ "SPELL_WATERBLAST",		46		},
	{ "SPELL_MINDBLAST",		47		},
	{ "SPELL_PARALYZING_EMBRACE",	48		},
	{ "SPELL_SNAKE_DART",		49		},
	{ "SPELL_IDENTIFY",			53		},
	{ "SPELL_FLY",				56		},
	{ "SPELL_CONTINUAL_LIGHT",	57		},
	{ "SPELL_DISPEL_MAGIC",		59		},
	{ "SPELL_CURE_SERIOUS",		61		},
	{ "SPELL_CAUSE_LIGHT",		62		},
	{ "SPELL_CAUSE_CRITICAL",	63		},
	{ "SPELL_CAUSE_SERIOUS",		64		},
	{ "SPELL_FLAMESTRIKE",		65		},
	{ "SPELL_STONE_SKIN",		66		},
	{ "SPELL_SHIELD",			67		},
	{ "SPELL_WEAKEN",			68		},
	{ "SPELL_MASS_INVIS",		69		},
	{ "SPELL_ACID_BLAST",		70		},
	{ "SPELL_FAERIE_FIRE",		72		},
	{ "SPELL_FAERIE_FOG",		73		},
	{ "SPELL_PASS_DOOR",		74		},
	{ "SPELL_SUMMON",			76		},
	{ "SPELL_INFRAVISION",		77		},
	{ "SPELL_CREATE_SPRING",		80		},
	{ "SPELL_REFRESH",			81		},
	{ "SPELL_CHANGE_SEX",		82		},
	{ "SPELL_GATE",			83		},
	{ "SPELL_ACID_BREATH",		200		},
	{ "SPELL_FIRE_BREATH",		201		},
	{ "SPELL_FROST_BREATH",		202		},
	{ "SPELL_GAS_BREATH",		203		},
	{ "SPELL_LIGHTNING_BREATH",	204		},
	{ "SPELL_GENERAL_PURPOSE",	501		},
	{ "SPELL_HIGH_EXPLOSIVE",	502		},
	{ "SPELL_BLOCK_AREA",		601		},
	{ "SPELL_WRITE_SPELL",		602		},
	{ "SPELL_HOMONCULOUS",		603		},
	{ "SPELL_DEMON",			604		},
	{ "SPELL_SHADOW",			605		},
	{ "SPELL_SHADE",			606		},
	{ "SPELL_PHANTASM",			607		},
	{ "SPELL_RIFT",			608		},
	{ "SPELL_RIP",				609		},
	{ "SPELL_TONGUES",			610		},
	{ "SPELL_UNDERSTAND",		611		},
	{ "SPELL_HASTE",			613		},
	{ "SPELL_ENHANCED_REST",		614		},
	{ "SPELL_ENHANCED_HEAL",		615		},
	{ "SPELL_ENHANCED_REVIVE",	616		},
	{ "SPELL_ANIMATE_DEAD",		617		},
	{ "SPELL_BANISH",			618		},
	{ "SPELL_ENHANCE_OBJECT",	619		},
	{ "SPELL_MAGE_SHIELD",		620		},
	{ "SPELL_DISPEL_UNDEAD",		621		},
	{ "SPELL_DISPEL_GOOD",		622		},
	{ "SPELL_REMOVE_FEAR",		623		},
	{ "SPELL_FEAST",			624		},
	{ "SPELL_RESTORE",			625		},
	{ "SPELL_TREMOR",			626		},
	{ "SPELL_ENERGY_SHIFT",		627		},
	{ "SPELL_INDUCTION",		628		},
	{ "SPELL_ILLUSION",			629		},
	{ "SPELL_MIRROR_IMAGE",		630		},
	{ "SPELL_HALLUCINATE",		631		},
	{ "SPELL_STABILITY",		632		},
	{ "SPELL_ETHEREAL_TRAVEL",	633		},
	{ "SPELL_ASTRAL_PROJECTION",	634		},
	{ "SPELL_BREATH_WATER",		635		},
	{ "SPELL_MAGE_BLAST",		636		},
	{ "SPELL_CONFUSION",		637		},
	{ "SPELL_SANCTIFY",			638		},
	{ "SPELL_SOUTHING_TOUCH",	639		},
	{ "SPELL_BENEDICTION",		640		},
	{ "SPELL_RIGHTEOUS_FURY",	641		},
	{ "SPELL_FARHEAL",			642		},
	{ "SPELL_UNHOLY_WORD",		643		},
	{ "SPELL_HOLY_WORD",		644		},
	{ "SPELL_INVIGORATE",		645		},
	{ "SPELL_IMPROVED_INVIS",	646		},
	{ "SPELL_TRUESIGHT",		647		},
	{ "SPELL_RECHARGE",			648		},
	{ "SPELL_TRANSPORT",		649		},
	{ "SPELL_ANTI_MAGIC_SHELL",	650		},
	{ "SPELL_POSSESS",			651		},
	{ "SPELL_NIGHTMARE",		653		},
	{ "SPELL_SMOKE",			654		},
	{ "SPELL_SLOW",			655		},
	{ "SPELL_BREW_POTION",		656		},
	{ "SPELL_FIRESHIELD",		658		},
	{ "SPELL_VAMPIRIC_TOUCH",	660		},
	{ "SPELL_PROTECTION_GOOD",	661		},
	{ "SPELL_FIREWALK",			664		},
	{ "SPELL_DETECT_GOOD",		665		},
	{ "SPELL_DIVINE_INSPIRATION",	666		},
	{ "SPELL_BLACK_AURA",		667		},
	{ "SPELL_MANA_SHIELD",		668		},
	{ "SPELL_MAGIC_MIRROR",		669		},
	{ "SPELL_QUICKEN",			674		},
	{ "SPELL_TORRID_BALM",		675		},
	{ "SPELL_SONG_OF_THE_SEAS",	676		},
	{ "SPELL_EARTHEN_SPIRIT",	678		},
	{ "SPELL_ENERGY_BOLT",		679		},
	{ "SPELL_TOTEM",			688		},
	{ "SPELL_SLOTH",			689		},
	{ "SPELL_TOUCHOFIDOCY",		690		},

	{ "TRIG_COMMAND",			BV01		},
	{ "TRIG_VOID",				BV02		},
	{ "TRIG_UNKNOWN",			BV03		},
	{ "TRIG_TICK",				BV04		},
	{ "TRIG_DAMAGE",			BV05		},
	{ "TRIG_HIT",				BV06		},
	{ "TRIG_WEAR",				BV07		},
	{ "TRIG_REMOVE",			BV08		},
	{ "TRIG_SACRIFICE",			BV09		},
	{ "TRIG_DROP",				BV10		},
	{ "TRIG_GET",				BV11		},
	{ "TRIG_ROOM_COMMAND",		BV12		},
	{ "TRIG_ROOM_UNKNOWN",		BV13		},
	{ "TRIG_WEAR_COMMAND",		BV14		},
	{ "TRIG_WEAR_UNKNOWN",		BV15		},

	{ "WEAPONTYPE_WEAPON",		0		},
	{ "WEAPONTYPE_SWORD",		1		},
	{ "WEAPONTYPE_DAGGER",		2		},
	{ "WEAPONTYPE_AXE",			3		},
	{ "WEAPONTYPE_MACE",		4		},
	{ "WEAPONTYPE_STAFF",		5		},
	{ "WEAPONTYPE_WHIP",		6		},
	{ "WEAPONTYPE_FLAIL",		7		},
	{ "WEAPONTYPE_SPEAR",		8		},
	{ "WEAPONTYPE_CLAW",		9		},
	{ "WEAPONTYPE_SHORTBOW",		10		},
	{ "WEAPONTYPE_LONGBOW",		11		},
	{ "WEAPONTYPE_CROSSBOW",		12		},
	{ "WEAPONTYPE_BLOWPIPE",		13		},

	{ "WEAPON_SLICE",			1		},
	{ "WEAPON_STAB",			2		},
	{ "WEAPON_SLASH",			3		},
	{ "WEAPON_WHIP",			4		},
	{ "WEAPON_CLAW",			5		},
	{ "WEAPON_BLAST",			6		},
	{ "WEAPON_POUND",			7		},
	{ "WEAPON_CRUSH",			8		},
	{ "WEAPON_GREP",			9		},
	{ "WEAPON_BITE",			10		},
	{ "WEAPON_PIERCE",			11		},
	{ "WEAPON_CHOP",			12		},

	{ "WEAR_NONE",				-1		},
	{ "WEAR_LIGHT",			 0		},
	{ "WEAR_FINGER_L",			 1		},
	{ "WEAR_FINGER_R",			 2		},
	{ "WEAR_NECK_A",			 3		},
	{ "WEAR_NECK_B",			 4		},
	{ "WEAR_BODY",				 5		},
	{ "WEAR_HEAD",				 6		},
	{ "WEAR_LEGS",				 7		},
	{ "WEAR_FEET",				 8		},
	{ "WEAR_HANDS",			 9		},
	{ "WEAR_ARMS",				10		},
	{ "WEAR_SHIELD",			11		},
	{ "WEAR_ABOUT",			12		},
	{ "WEAR_WAIST",			13		},
	{ "WEAR_WRIST_L",			14		},
	{ "WEAR_WRIST_R",			15		},
	{ "WEAR_WIELD",			16		},
	{ "WEAR_DUAL_WIELD",		17		},
	{ "WEAR_HOLD",				18		},
	{ "WEAR_HEART",			19		}
};
