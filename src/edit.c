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

void		expand_mob_prog	args( ( MOB_INDEX_DATA *mob, MPROG_DATA *prog) );
void		do_rredit			args( ( CHAR_DATA *ch, char *argument ) );
void		obj_prog_if_dest	args( ( OBJ_INDEX_DATA *obj ) );

char *  const   race_specs [] =
{
	"flying",			"ethereal",		"swim",			"undead",
	"seeagro",		"forage",			"intimidate",		"multi_arms",
	"knowalign",		"easy_flee",		"magic_defense",	"nohaste",
	"fastheal",		"fastrevive",		"astral",			"age_ac",
	"age_hr",			"age_dr",			"vampiric",		"pass_door",
	"firewalk",		"*"
};

char *  const   mprog_flags [] =
{
	"act",			"speech",			"rand",			"fight",
	"death",			"hitprcnt",		"entry",			"greet",
	"allgreet",		"give",			"bribe",			"range",
	"social",			"kill",			"group_greet",		"time",
	"repop",			"delay",			"exit",			"trigger",
	"desc",			"*"
};

char *  const   oprog_flags [] =
{
	"command",		"void",			"unknown",		"tick",
	"damage",			"hit",			"wear",			"remove",
	"sacrifice",		"drop",			"get",			"roomcommand",
	"roomunknown",		"wearcommand",		"wearunknown",		"*"
};

char *  const   oprog_commands [] =
{
	"echo",			"godcommand",		"godargument",		"command",
	"argument",		"ifhasobj",		"if",			"junk",
	"questset",		"questadd",		"ifobjquest",		"ifplrquest",
	"apply",			"*"
};

char *  const   oprog_applies [] =
{
	"@",				"hp",			"move",			"mana",
	"alignment",		"*"
};

char *  const   oprog_weathers [] =
{
	"clear",			"cloudy",			"raining",		"storms",
	"*"
};

char *  const	 boolean []=
{
	"false",			"true",			"*"
};

char *  const   oprog_ifchecks [] =
{
	"mcwar",			"mcgla",			"mcmar",			"mcnin",
	"mcdru",			"mcsor",			"mcsha",			"mcwlc",
	"wearloc",		"time",			"weather",		"rand",
	"hitprcnt",		"manaprcnt",		"moveprcnt",		"sector",
	"alignment",		"gold",			"class",			"god",
	"area",			"level",			"position",		"race",
	"sex",			"room",			"fighting",		"*"
};

char *  const   area_flags [] =
{
	"@",				"noteleport",		"nogohome",		"norecall",
	"nocastle",		"norip",			"@",				"nosummon",
	"autosave",		"modified",		"weather",		"*"
};

char * const    cont_flags []=
{
	"canopen",		"pickproof",		"closed",			"locked",
	"magical_lock",	"trapped_lock",	"mechanical_lock",	"small_lock",
	"big_lock",		"easy_pick",		"hard_pick",		"*"
};

char *  const   exit_flags [] =
{
	"isdoor",			"closed",			"locked",			"hidden",
	"@",				"pickproof",		"bashproof",		"magicproof",
	"@",				"@",				"@",				"@",
	"passproof",		"magical_lock",	"trapped_lock",	"mechanical_lock",
	"small_lock",		"big_lock",		"easy_pick",		"hard_pick",
	"*"
};

char *  const  lp_flags [] =
{
	"magical_lock",	"trapped_lock",	"mechanical_lock",	"small_lock",
	"big_lock",		"*"
};

char *  const   reset_exit_types [] =
{
	"open",			"close",			"lock",			"*"
};

char * const    reset_rand_types [] =
{
	"@",				"ne",			"nes",			"nesw",
	"neswu",			"neswud",			"*"
};

char *  const   f_flags [] =
{
	"sleep_in",		"sleep_on",		"rest_in",		"rest_on",
	"sit_in",			"sit_on",			"stand_in",		"stand_on",
	"*"
};

char *  const   r_flags [] =
{
	"dark",			"smoke",			"nomob",			"indoors",
	"globe",			"nogohome",		"@",				"altar",
	"bank",			"private",		"safe",			"solitary",
	"petshop",		"norecall",		"rip",			"block",
	"no_save",		"morgue",			"allow_war",		"allow_gla",
	"allow_mar",		"allow_nin",		"allow_dru",		"allow_sor",
	"allow_sha",		"allow_wlc",		"@",				"@",
	"noteboard",		"nocastle",		"norip",			"maze",
	"ice",			"dynamic",		"@",				"hash",
	"*"
};

char *  const   o_flags [] =
{
	"glow",			"hum",			"dark",			"lock",
	"evil",			"invis",			"magic",			"nodrop",
	"bless",			"antigood",		"antievil",		"antineutral",
	"noremove",		"inventory",		"burning",		"@",
	"autoengrave",		"@",				"ethereal",		"mount",
	"*"
};

char *  const   act_flags [] =
{
	"@",				"sentinel",		"scavenger",		"drunk",
	"mount",			"aggressive",		"staysector",		"wimpy",
	"pet",			"train",			"practice",		"weak",
	"smart",			"onefight",		"noorder",		"@",
	"body",			"race",			"undead",			"@",
	"@",				"@",				"@",				"nodamage",
	"class",			"*"
};

char *  const   a_flags [] =
{
	"blind",			"invisible",		"imp_invisible",	"detect_invis",
	"@",				"detect_hidden",	"truesight",		"sanctuary",
	"@",				"@",				"@",				"understand",
	"poison",			"protect_evil",	"protect_good",	"sneak",
	"hide",			"@",				"@",				"flying",
	"pass_door",		"stealth",		"clear",			"hunt",
	"tongues",		"ethereal",		"haste",			"stability",
	"*"
};

char *  const   oa_flags1 [] =
{
	"blind",			"invisible",		"imp invisible",	"detect_invis",
	"@",				"detect_hidden",	"truesight",		"sanctuary",
	"@",				"infrared",		"curse",			"understand",
	"poison",			"protect_evil",	"protect_good",	"sneak",
	"hide",			"@",				"@",				"flying",
	"pass_door",		"stealth",		"clear",			"hunt",
	"tongues",		"@",				"haste",			"stability",
	"*"
};

char *  const   oa_flags2 [] =
{
	"enhanced_rest",	"enhanced_heal",	"enhanced_revive",	"divine_inspiration",
	"@",				"berserk",		"@",				"hallucinate",
	"@",				"earthbind",		"ethereal",		"astral",
	"breath_water",	"@",				"quicken",		"confusion",
	"flashpowder",		"@",				"fireshield",		"@",
	"firewalk",		"paralyze",		"*"
};

char *  const   plr_flags [] =
{
	"npc",			"games",			"exp_tnl",		"autoexits",
	"autoloot",		"autosac",		"blank",			"brief",
	"repeat",			"combine",		"prompt",			"@",
	"holylight",		"wizinvis",		"wiztime",		"silence",
	"outcast",		"mute",			"wizcloak",		"log",
	"deny",			"freeze",			"thief", 			"killer",
	"damage",			"split",			"quiet",			"pager",
	"chat",			"plan",			"afk",			"battle",
	"vict_list",		"hearlog",		"buildlight",		"fos",
	"*"
};

char *  const   wear_locs [] =
{
	"light",			"finger1",		"finger2",		"neck1",
	"neck2",			"body",			"head",			"legs",
	"feet",			"hands",			"arms",			"shield",
	"about",			"waist",			"wrist1",			"wrist2",
	"wield",			"dual-wield",		"hold",			"heart",
	"*"
};

char *  const   oprog_wlocs [] =
{
	"none",			"light",			"finger1",		"finger2",
	"neck1",			"neck2",			"body",			"head",
	"legs",			"feet",			"hands",			"arms",
	"shield",			"about",			"waist",			"wrist1",
	"wrist2",			"wield",			"dual-wield",		"hold",
	"heart",			"*"
};

char *  const   part_flags [] =
{
	"head",			"mouth",			"eye",			"torso",
	"hip",			"leg",			"arm",			"wing",
	"tail",			"tentacle",		"horn",			"claw",
	"hand",			"foot",			"branch",			"root",
	"twig",			"trunk",			"pincers",		"spinnaret",
	"beak",			"*"
};

char *  const   o_types [] =
{
	"nothing",		"light",			"scroll",			"wand",
	"staff",			"weapon",			"@",				"@",
	"treasure",		"armor",			"potion",			"@",
	"furniture",		"trash",			"@",				"container",
	"@",				"drinkcon",		"key",			"food",
	"money",			"@",				"boat",			"@",
	"@",				"fountain",		"pill",			"portal",
	"artifact",		"lockpick",		"ammo",			"totem",
	"*"
};

char *  const   w_flags [] =
{
	"take",			"finger",			"neck",			"body",
	"head",			"legs",			"feet",			"hands",
	"arms",			"shield",			"about",			"waist",
	"wrist",			"wield",			"hold",			"@",
	"*"
};

char *  const   a_types [] =
{
	"none",			"str",			"dex",			"int",
	"wis",			"con",			"sex",			"@",
	"@",				"@",				"@",				"@",
	"mana",			"hp",			"move",			"@",
	"@",				"ac",			"hit",			"dam",
	"save_para",		"save_rod",		"save_petri",		"save_breath",
	"save_spell",		"*"
};

char *  const	 p_types [] =
{
	"@",				"@",				"@",				"@",
	"sleeping",		"resting",		"sitting",		"fighting",
	"standing",		"*"
};

char *  const   sex_types [] =
{
	"neutral",		"male",			"female",			"*"
};

char *  const   r_types [] =
{
	"human",			"elf",			"darkelf",		"dwarf",
	"gnome",			"orc",			"griffon",		"halfelf",
	"ogre",			"hobbit",			"sprite",			"shade",
	"roc",			"thrikreen",		"werewolf",		"demon",
	"gargoyle",		"wraith",			"troll",			"vampire",
	"phoenix",		"myrddraal",		"arachnid",		"titan",
	"eagle",			"kraken",			"balrog",			"dragon",
	"ent",			"*"
};

char *  const  sect_types [] =
{
	"inside",			"city",			"field",			"forest",
	"hills",			"mountain",		"lake",			"river",
	"ocean",			"air",			"desert",			"lava",
	"ethereal",		"astral",			"under_water",		"under_ground",
	"deep_earth",		"road",			"swamp",			"beach",	
	"tundra",			"edge",			"*"
};

char * const   weap_types [] =
{
	"weapon",			"sword",			"dagger",			"axe",
	"mace",			"staff",			"whip",			"flail",
	"spear",			"claw",			"shortbow",		"longbow",
	"crossbow",		"blowpipe",		"*"
};

char * const   bow_types [] =
{
	"@",				"@",				"@",				"@",
	"@",				"@",				"@",				"@",
	"@",				"@",				"shortbow",		"longbow",
	"crossbow",		"blowpipe",		"*"
};

char * const	noun_types [] =
{
	"hit",			"slice",			"stab",			"slash",
	"whip",			"claw",			"blast",			"pound",
	"crush",			"grep",			"bite",			"pierce",
	"*"
};

char * const	liq_types [] =
{
	"water",			"beer",			"wine",			"ale",
	"darkale",		"whisky",			"lemonade",		"firebrt",
	"localspc",		"slime",			"milk",			"tea",
	"coffee",			"blood",			"saltwater",		"coke",
	"orangejuice",		"brandy",			"icewine",		"rum",
	"vodka",			"champagne",		"*"
};

char * const	portal_flags [] =
{
	"teleport_world",	"gowith",			"noflee",			"stepthrough",
	"stepinto",		"teleport_area",	"*"
};

char * const	poison_types [] =
{
	"notpoisoned",		"poisoned",		"*"
};

char * const   god_types [] =
{
	"neutral",		"@",				"@",				"demise",
	"@",				"@",				"@",				"@",
	"manwe",			"hypnos",			"ulmo",			"gaia",
	"@",				"*"
};

char * const	class_types [] =
{
	"warrior",		"gladiator",		"marauder",		"ninja",
	"druid",			"sorcerer",		"shaman",			"warlock",
	"*"
};

char *flag_string( long long bitvector, char * const array[] )
{
	static char buf[MAX_STRING_LENGTH];
	int x;

	push_call("flag_string(%p,%p)",bitvector,array);

	buf[0] = '\0';

	for (x = 0 ; x < 64 ; x++)
	{
		if (!strcmp("@", array[x]))
		{
			continue;
		}
		if (!strcmp("*", array[x]))
		{
			break;
		}
		if (IS_SET(bitvector, 1LL << x))
		{
			strcat( buf, array[x] );
			strcat( buf, " " );
		}
	}

	if ((x = strlen(buf)) > 0)
	{
		buf[x-1] = '\0';
	}
	pop_call();
	return buf;
}

char *type_string( long long bitvector, char * const array[] )
{
	int x;

	push_call("type_string(%p,%p)",bitvector,array);

	for (x = 0 ; x < 64 ; x++)
	{
		if (!strcmp("@", array[x]))
		{
			continue;
		}
		if (!strcmp("*", array[x]) || bitvector == x)
		{
			pop_call();
			return(array[x]);
		}
	}
	pop_call();
	return "";
}

long long get_flag( char *flag, char * const array[])
{
	int x;

	push_call("get_flag(%p)",flag);

	for (x = 0 ; x < 64 ; x++)
	{
		if (!strcmp("@", array[x]))
		{
			continue;
		}
		if (!strcmp("*", array[x]))
		{
			break;
		}
		if (!strcasecmp(flag, array[x]))
		{
			pop_call();
			return x;
		}
	}
	pop_call();
	return -1;
}


bool toggle_flags(void *field, char *flag, char * const array[])
{
	char arg[MAX_INPUT_LENGTH];
	long long *value;
	int x;

	push_call("toggle_flags(%p,%p,%p)",field,flag,array);

	value = field;

	if (*flag == '\0')
	{
		pop_call();
		return FALSE;
	}

	for (flag = one_argument(flag, arg) ; arg[0] != '\0' ; flag = one_argument(flag, arg))
	{
		for (x = 0 ; x < 64 ; x++)
		{
			if (!strcmp("@", array[x]))
			{
				continue;
			}
			if (!strcmp("*", array[x]))
			{
				break;
			}
			if (!strcasecmp(arg, array[x]))
			{
				TOGGLE_BIT(*value, 1LL << x);
				break;
			}
		}
		if (!strcmp("*", array[x]))
		{
			pop_call();
			return FALSE;
		}
	}
	pop_call();
	return TRUE;
}

char *give_flags( char * const array[])
{
	static char buf[MAX_STRING_LENGTH];
	int x;

	push_call("give_flags(%p)",array);

	for (buf[0] = '\0', x = 0 ; x < 64 ; x++)
	{
		if (!strcmp("@", array[x]))
		{
			continue;
		}
		if (!strcmp("*", array[x]))
		{
			break;
		}
		cat_sprintf(buf, "%s ", array[x]);
	}
	buf[strlen(buf)-1] = '\0';

	pop_call();
	return buf;
}


bool can_olc_modify( CHAR_DATA *ch, int vnum )
{
	push_call("can_olc_modify(%p,%p)",ch,vnum);

	if (IS_NPC(ch) || !IS_IMMORTAL(ch))
	{
		log_string("BUG OLC Security Breach.");
		dump_stack();
		pop_call();
		return FALSE;
	}

	if (ch->level >= MAX_LEVEL)
	{
		if (IS_GOD(ch))
		{
			pop_call();
			return TRUE;
		}
		else
		{
			log_printf("Illegal God");
			dump_stack();
		}
	}

	if (get_area_from_vnum(vnum) && is_name(ch->name, get_area_from_vnum(vnum)->authors))
	{
		pop_call();
		return TRUE;
	}

	if (ch->pcdata->a_range_lo == 0)
	{
		pop_call();
		return FALSE;
	}

	if (vnum >= ch->pcdata->a_range_lo && vnum <= ch->pcdata->a_range_hi)
	{
		pop_call();
		return TRUE;
	}


	pop_call();
	return FALSE;
}

bool show_build_vnum ( CHAR_DATA *ch, int vnum )
{
	push_call("show_build_vnum(%p,%p)",ch,vnum);

	if (IS_NPC(ch))
	{
		pop_call();
		return FALSE;
	}

	if (!IS_SET(ch->act, PLR_BUILDLIGHT))
	{
		pop_call();
		return FALSE;
	}

	if (!can_olc_modify(ch, vnum))
	{
		pop_call();
		return FALSE;
	}
	pop_call();
	return TRUE;
}

EXTRA_DESCR_DATA *set_ed_room(ROOM_INDEX_DATA *room, char *keywords )
{
	EXTRA_DESCR_DATA *ed;

	push_call("set_ed_room(%p,%p)",room,keywords);

	for (ed = room->first_extradesc ; ed ; ed = ed->next)
	{
		if (is_name(keywords, ed->keyword) || !strcasecmp(keywords, ed->keyword))
		{
			break;
		}
	}

	if (ed == NULL)
	{
		ALLOCMEM(ed, EXTRA_DESCR_DATA, 1);
		LINK(ed, room->first_extradesc, room->last_extradesc, next, prev);
		ed->keyword     = STRALLOC( keywords );
		ed->description = STRALLOC( "" );
		mud->top_ed++;
	}
	pop_call();
	return ed;
}

EXTRA_DESCR_DATA *set_ed_obj(OBJ_INDEX_DATA *obj, char *keywords )
{
	EXTRA_DESCR_DATA *ed;

	push_call("set_ed_obj(%p,%p)",obj,keywords);

	for (ed = obj->first_extradesc ; ed ; ed = ed->next)
	{
		if (is_name(keywords, ed->keyword) || !strcasecmp(keywords, ed->keyword))
		{
			break;
		}
	}

	if (ed == NULL)
	{
		ALLOCMEM(ed, EXTRA_DESCR_DATA, 1);
		LINK(ed, obj->first_extradesc, obj->last_extradesc, next, prev);
		ed->keyword     = STRALLOC( keywords );
		ed->description = STRALLOC( "" );
		mud->top_ed++;
	}
	pop_call();
	return ed;
}

EXTRA_DESCR_DATA *set_ed_mob(MOB_INDEX_DATA *mob, char *keywords )
{
	EXTRA_DESCR_DATA *ed;

	push_call("set_ed_mob(%p,%p)",mob,keywords);

	for (ed = mob->first_extradesc ; ed ; ed = ed->next)
	{
		if (is_name(keywords, ed->keyword) || !strcasecmp(keywords, ed->keyword))
		{
			break;
		}
	}

	if (ed == NULL)
	{
		ALLOCMEM(ed, EXTRA_DESCR_DATA, 1);
		LINK(ed, mob->first_extradesc, mob->last_extradesc, next, prev);
		ed->keyword     = STRALLOC( keywords );
		ed->description = STRALLOC( "" );
		mud->top_ed++;
	}
	pop_call();
	return ed;
}

bool del_ed_room( ROOM_INDEX_DATA *room, char *keywords )
{
	EXTRA_DESCR_DATA *ed;

	push_call("del_ed_room(%p,%p)",room,keywords);

	for (ed = room->first_extradesc ; ed ; ed = ed->next)
	{
		if (is_name(keywords, ed->keyword) || !strcasecmp(keywords, ed->keyword))
		{
			break;
		}
	}

	if (ed)
	{
		UNLINK(ed, room->first_extradesc, room->last_extradesc, next, prev);
		STRFREE(ed->keyword);
		STRFREE(ed->description);
		FREEMEM(ed);
		mud->top_ed--;
		pop_call();
		return TRUE;
	}
	pop_call();
	return FALSE;
}

bool del_ed_obj( OBJ_INDEX_DATA *obj, char *keywords )
{
	EXTRA_DESCR_DATA *ed;

	push_call("del_ed_obj(%p,%p)",obj,keywords);

	for (ed = obj->first_extradesc ; ed ; ed = ed->next)
	{
		if (is_name(keywords, ed->keyword) || !strcasecmp(keywords, ed->keyword))
		{
			break;
		}
	}

	if (ed)
	{
		UNLINK(ed, obj->first_extradesc, obj->last_extradesc, next, prev);
		STRFREE(ed->keyword);
		STRFREE(ed->description);
		FREEMEM(ed);
		mud->top_ed--;
		pop_call();
		return TRUE;
	}
	pop_call();
	return FALSE;
}


bool del_ed_mob( MOB_INDEX_DATA *mob, char *keywords )
{
	EXTRA_DESCR_DATA *ed;

	push_call("del_ed_mob(%p,%p)",mob,keywords);

	for (ed = mob->first_extradesc ; ed ; ed = ed->next)
	{
		if (is_name(keywords, ed->keyword) || !strcasecmp(keywords, ed->keyword))
		{
			break;
		}
	}

	if (ed)
	{
		UNLINK(ed, mob->first_extradesc, mob->last_extradesc, next, prev);
		STRFREE(ed->keyword);
		STRFREE(ed->description);
		FREEMEM(ed);
		mud->top_ed--;
		pop_call();
		return TRUE;
	}
	pop_call();
	return FALSE;
}

char *get_ed(CHAR_DATA *ch, EXTRA_DESCR_DATA *ed)
{
	static char buf[MAX_STRING_LENGTH];

	push_call("get_ed(%p)",ed);

	for (buf[0] = '\0' ; ed ; ed = ed->next)
	{
		cat_sprintf(buf, "%s", ed->keyword);
		if (ed->next)
		{
			cat_sprintf(buf, "%s", ansi_translate_text(ch, "{078}, {178}"));
		}
	}
	pop_call();
	return buf;
}

void set_obj_affect( OBJ_INDEX_DATA *obj, int value, int modif, int type, long long bitv )
{
	int count;
	AFFECT_DATA *aff;

	push_call("set_obj_affect(%p,%p,%p,%p)",obj,value,modif,type);

	for (count = 0, aff = obj->first_affect ; aff ; aff = aff->next)
	{
		if (++count == value)
		{
			break;
		}
	}
	if (aff == NULL)
	{
		ALLOCMEM(aff, AFFECT_DATA, 1);
		aff->type      =  0;
		aff->duration  = -1;
		aff->bitvector =  0;
		LINK(aff, obj->first_affect, obj->last_affect, next, prev);
		mud->top_affect++;
	}
	if (type != -1)
	{
		aff->location = type;
	}
	if (modif != -1)
	{
		aff->modifier = modif;
	}
	if (bitv != -1)
	{
		aff->bitvector = bitv;
	}

	if (aff->bitvector)
	{
		aff->bittype = AFFECT_TO_CHAR;
	}
	else
	{
		aff->bittype = AFFECT_TO_NONE;
	}

	if (type == -2)
	{
		UNLINK(aff, obj->first_affect, obj->last_affect, next, prev);
		FREEMEM(aff);
		mud->top_affect--;
	}
	pop_call();
	return;
}

void set_link( int room, int door, int dest)
{
	EXIT_DATA  *pexit;
	RESET_DATA *pReset;

	push_call("set_link(%p,%p,%p)",room,door,dest);

	if (room < 1 || dest < -1
	||  room_index[room] == NULL
	||  door < 0 || door > 5
	|| (dest != -1 && room_index[dest] == NULL))
	{
		log_printf("Bad room connect at %d door %d to %d", room, door, dest);
		pop_call();
		return;
	}



	if (room_index[room]->exit[door] == NULL && dest > 0)
	{
		create_exit(room_index[room], door);
	}

	pexit = room_index[room]->exit[door];

	if (dest > 0)
	{
		pexit->vnum	= dest;
		pexit->to_room	= dest;
	}
	else
	{
		if (pexit)
		{
			delete_exit(room_index[room], door);

			for (pReset = room_index[room]->area->first_reset ; pReset ; )
			{
				if (is_room_reset(pReset, room_index[room]) && is_door_reset(pReset, door))
				{
					delete_reset(room_index[room]->area, pReset);

					pReset = room_index[room]->area->first_reset;
				}
				else
				{
					pReset = pReset->next;
				}
			}
		}
	}
	pop_call();
	return;
}

void do_link( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument )
{
	int dest_vnum, rev_dest_vnum;
	int door_num;
	char arg1 [MAX_INPUT_LENGTH];
	char arg2 [MAX_INPUT_LENGTH];
	char arg3 [MAX_INPUT_LENGTH];

	push_call("do_connect(%p,%p)",ch,argument);

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	strcpy( arg3, argument );

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Syntax: <dir> link <destination vnum> [both]\n\r", ch);
		pop_call();
		return;
	}

	door_num		= direction_door(arg1);
	dest_vnum 	= atoi( arg2 );
	rev_dest_vnum	= 0;

	if (door_num < 0 || door_num > 5 )
	{
		send_to_char( "Invalid direction.\n\r",ch);
		pop_call();
		return;
	}

	if (get_room_index(dest_vnum) == NULL && dest_vnum != -1)
	{
		send_to_char( "A room with that vnum does not exist.\n\r",ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && dest_vnum != -1 && !can_olc_modify(ch, dest_vnum))
	{
		ch_printf(ch, "That vnum is not in your allocated range.\n\r");
		pop_call();
		return;
	}

	if (room->exit[door_num] && room->exit[door_num]->vnum > 0)
	{
		rev_dest_vnum = room->exit[door_num]->vnum;
	}

	set_link(room->vnum, door_num, dest_vnum);

	if (arg3[0] == 'b' || arg3[0] == 'B')
	{
		if (dest_vnum != -1)
		{
			set_link(dest_vnum, rev_dir[door_num], room->vnum);
		}
		else
		{
			set_link(rev_dest_vnum, rev_dir[door_num], -1);
		}
	}

	pop_call();
	return;
}

void start_editing( CHAR_DATA *ch, char *data )
{
	EDITOR_DATA *edit;
	char c, buffer[MAX_INPUT_LENGTH];
	sh_int line     = 0;
	sh_int leng     = 0;
	sh_int size     = 0;
	sh_int max_line = 150;
	sh_int max_size = 8000;

	push_call("start_editing(%p,%p)",ch,data);

	if (ch->level >= LEVEL_IMMORTAL)
	{
		max_line = 600;
		max_size = 32000;
	}

	if (ch->pcdata->editor) /* likely called by /w in the editor */
	{
		edit = ch->pcdata->editor;

		while (edit->numlines)
		{
			edit->numlines--;
			STRFREE(edit->line[edit->numlines]);
		}
	}
	else
	{
		ch->desc->connected = CON_EDITING;

		send_to_char( "Begin entering your text now (/? = help /s = save /c = clear /l = list)\n\r", ch );
		send_to_char( "-----------------------------------------------------------------------\n\r", ch );

		ALLOCMEM(edit, EDITOR_DATA, 1);
	}

	while (TRUE)
	{
		c = data[size++];

		switch (c)
		{
			case '\0':
				if (leng)
				{
					buffer[leng] = 0;
					edit->line[line++] = STRALLOC(buffer);
				}
				goto endofloop;

			case '\r':
				continue;

			case '\n':
				buffer[leng] = 0;
				edit->line[line++] = STRALLOC(buffer);
				leng = 0;
				break;

			default:
				buffer[leng++] = c;
				break;
		}

		if (line == max_line)
		{
			break;
		}

		if (size == max_size)
		{
			buffer[leng] = 0;
			edit->line[line++] = STRALLOC(buffer);
			break;
		}
	}
	endofloop:

	edit->numlines     = line;
	edit->size         = size;
	edit->on_line      = line;

	ch->pcdata->editor = edit;

	edit_buffer(ch, "/p");
	pop_call();
	return;
}

/*
	Copy the buffer, strips tailing spaces - Scandum
*/

char *copy_buffer( CHAR_DATA *ch )
{
	char buf1[MAX_INPUT_LENGTH], buf2[MAX_STRING_LENGTH];
	int x, y;

	push_call("copy_buffer(%p)",ch);

	for (x = buf2[0] = '\0' ; x < ch->pcdata->editor->numlines ; x++)
	{
		strcpy(buf1, ch->pcdata->editor->line[x]);

		for (y = strlen(buf1) - 1 ; y >= 0 ; y--)
		{
			if (buf1[y] == ' ')
			{
				buf1[y] = '\0';
			}
			else
			{
				break;
			}
		}
		cat_sprintf(buf2, "%s\n\r", buf1);
	}
	smash_tilde(buf2);

	pop_call();
	return STRALLOC(buf2);
}

void stop_editing( CHAR_DATA *ch )
{
	int x;

	push_call("stop_editing(%p)",ch);

	if (ch->pcdata->editor)
	{
		for (x = 0 ; x < ch->pcdata->editor->numlines ; x++)
		{
			STRFREE(ch->pcdata->editor->line[x]);
		}
		FREEMEM(ch->pcdata->editor);
		ch->pcdata->editor = NULL;
	}
	send_to_char_color("{300}Done.\n\r", ch);

	ch->pcdata->editmode  = ch->pcdata->tempmode;
	ch->pcdata->tempmode  = MODE_NONE;

	if (ch->desc)
	{
		ch->desc->connected = CON_PLAYING;
	}
	pop_call();
	return;
}

void stop_olc_editing( CHAR_DATA *ch, AREA_DATA *area)
{
	push_call("stop_olc_editing(%p,%p)",ch,area);

	ch->pcdata->edittype = EDIT_TYPE_NONE;
	ch->pcdata->editmode = MODE_NONE;
	ch->pcdata->tempmode = MODE_NONE;
	ch->pcdata->edit_ptr = NULL;
	ch->pcdata->temp_ptr = NULL;

	if (ch->pcdata && ch->pcdata->subprompt)
	{
		STRFREE(ch->pcdata->subprompt);
		ch->pcdata->subprompt = NULL;
	}
	CHECK_AUTOSAVE(area);

	pop_call();
	return;
}

/*
	Simple but nice and handle line editor.                     Martin
*/

void edit_buffer( CHAR_DATA *ch, char *argument )
{
	DESCRIPTOR_DATA *d;
	EDITOR_DATA *edit;
	char cmd[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	char prompt[15];
	char word1[MAX_INPUT_LENGTH];
	char word2[MAX_INPUT_LENGTH];
	char word3[MAX_INPUT_LENGTH];
	char *data;
	sh_int x        = 0;
	sh_int y        = 0;
	sh_int line     = 0;
	sh_int max_line = 150;
	sh_int max_leng = 1000;
	sh_int max_size = 8000;
	bool save       = FALSE;

	push_call("edit_buffer(%p,%p)",ch,argument);

	if (ch->level >= LEVEL_IMMORTAL)
	{
		max_line = 600;
		max_size = 32000;
	}

	if ((d = ch->desc) == NULL)
	{
		send_to_char( "You have no descriptor.\n\r", ch );
		pop_call();
		return;
	}

	sprintf(prompt, "%s> ", get_color_string(ch, COLOR_PROMPT, VT102_DIM));

	edit = ch->pcdata->editor;

	if (argument[0] == '/' || argument[0] == '\\')
	{
		argument = one_argument(argument, cmd);

		switch (cmd[1])
		{
			case '?':
				ch_printf_color(ch,
					"Editing commands\n\r"
					"-----------------------------------\n\r"
					"/c                 clear buffer    \n\r"
					"/d [line] [amount] delete lines    \n\r"
					"/g <line>          goto line       \n\r"
					"/i [line] [amount] insert lines    \n\r"
					"/l [range]         list buffer     \n\r"
					"/r <old> <new>     global replace  \n\r"
					"/w                 global word wrap\n\r"
					"/a                 abort editing   \n\r"
					"/s                 save buffer     \n\r");

				write_to_buffer( ch->desc, prompt, 1000000 );
				break;

			case 'p':
				write_to_buffer( ch->desc, prompt, 1000000 );
				break;

			case 'c':
				for (x = 0 ; x < edit->numlines ; x++)
				{
					STRFREE(edit->line[x]);
				}
				edit->numlines = 0;
				edit->on_line  = 0;
				edit->size     = 0;
				send_to_char( "Buffer cleared.\n\r", ch );
				write_to_buffer( ch->desc, prompt, 1000000 );
				break;

			case 'w':
				if (ch->pcdata->editmode == MODE_MPROG_EDIT || ch->pcdata->editmode == MODE_OPROG_EDIT)
				{
					send_to_char("You wouldn't want to word wrap a program buffer.\n\r", ch);
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				else
				{
					for (x = 0 ; x < edit->numlines ; x++)
					{
						strcpy(buf, ansi_justify(edit->line[x], 80));
						RESTRING(edit->line[x], buf);
					}

					data = copy_buffer(ch);

					start_editing(ch, data);

					STRFREE(data);

					ch_printf(ch, "Word wrapped text between line 1 and %d\n\r", x);
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				break;

			case 'r':
				{
					char *wptr, *lwptr;
					int count, wordln, word2ln;

					argument = one_argument_nolower(argument, word1);
					argument = one_argument_nolower(argument, word2 );

					if (word1[0] == '\0' || word2[0] == '\0')
					{
						send_to_char( "You need a word to replace, and it's replacement.\n\r", ch);
						write_to_buffer( ch->desc, prompt, 1000000 );
						pop_call();
						return;
					}

					count   = 0;
					wordln  = strlen(word1);
					word2ln = strlen(word2);

					ch_printf( ch, "Replacing all occurrences of %s with %s...\n\r", word1, word2 );

					for (x = 0 ; x < edit->numlines ; x++)
					{
						lwptr = strcpy(buf, edit->line[x]);
	
						while ((wptr = strstr(lwptr, word1)) != NULL)
						{
							if (edit->size + (word2ln - wordln) >= max_size)
							{
								break;
							}
							if (strlen(buf) + (word2ln - wordln) >= max_leng)
							{
								break;
							}
							lwptr = wptr + word2ln;

							sprintf(word3, "%s%s", word2, wptr + wordln);
							strcpy(wptr, word3);

							edit->size += (word2ln - wordln);
							count      += 1;
						}
						RESTRING(edit->line[x], buf);
					}
					ch_printf( ch, "Found and replaced %d occurrence%s between line 1 and %d.\n\r", count, count == 1 ? "" : "s",  edit->numlines);
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				break;

			case 'i':
				if (edit->numlines >= max_line)
				{
					send_to_char( "Buffer is full.\n\r", ch );
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				else
				{
					int count;

					argument = one_argument_nolower(argument, word1);
					argument = one_argument_nolower(argument, word2 );

					if (word1[0] != '\0')
					{
						line = atoi(word1) - 1;
					}
					else
					{
						line = edit->on_line;
					}
					count = URANGE(1, atoi(word2), max_line - edit->numlines);

					if (line < 0 || line > edit->numlines)
					{
						send_to_char( "Out of range.\n\r", ch );
						write_to_buffer( ch->desc, prompt, 1000000 );
					}
					else
					{
						for (y = 0 ; y < count ; y++)
						{
							for (x = ++edit->numlines ; x > line ; x--)
							{
								edit->line[x] = edit->line[x-1];
							}
							edit->line[line] = STRDUPE(str_empty);
						}
						ch_printf(ch, "Line%s inserted.\n\r", count > 1 ? "s" : "");
						write_to_buffer( ch->desc, prompt, 1000000 );
					}
				}
				break;

			case 'd':
				if (edit->numlines == 0)
				{
					send_to_char( "Buffer is empty.\n\r", ch );
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				else
				{
					int count;

					argument = one_argument_nolower(argument, word1);
					argument = one_argument_nolower(argument, word2 );

					if (word1[0] != '\0')
					{
						line = atoi(word1) - 1;
					}
					else
					{
						line = URANGE(0, edit->on_line, edit->numlines - 1);
					}
					count = URANGE(1, atoi(word2), edit->numlines - line);

					if (line < 0 || line > edit->numlines)
					{
						send_to_char( "Out of range.\n\r", ch );
						write_to_buffer( ch->desc, prompt, 1000000 );
					}
					else
					{
						for (y = 0 ; y < count ; y++)
						{
							edit->size -= strlen(edit->line[line]);

							STRFREE(edit->line[line]);

							for (x = line ; x < (edit->numlines - 1) ; x++)
							{
								edit->line[x] = edit->line[x+1];
							}

							edit->numlines--;

							if (edit->on_line > edit->numlines)
							{
								edit->on_line = edit->numlines;
							}

						}
						ch_printf(ch, "Line%s deleted.\n\r", count > 1 ? "s" : "");
						write_to_buffer( ch->desc, prompt, 1000000 );
					}
				}
				break;

			case 'g':
				if (edit->numlines == 0)
				{
					send_to_char( "Buffer is empty.\n\r", ch );
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				else
				{
					if (argument[0] != '\0')
					{
						line = atoi(argument) - 1;
					}
					else
					{
						send_to_char( "Goto what line?\n\r", ch );
						write_to_buffer( ch->desc, prompt, 1000000 );
						pop_call();
						return;
					}
					if (line < 0 || line > edit->numlines)
					{
						send_to_char( "Out of range.\n\r", ch );
						write_to_buffer( ch->desc, prompt, 1000000 );
					}
					else
					{
						edit->on_line = line;
						ch_printf( ch, "(On line %d)\n\r", line+1 );
						write_to_buffer( ch->desc, prompt, 1000000 );
					}
				}
				break;

			case 'l':
				if (edit->numlines == 0)
				{
					send_to_char( "Buffer is empty.\n\r", ch );
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				else
				{
					argument = one_argument(argument, word1);
					argument = one_argument(argument, word2);

					x = atoi(word1)-1;
					y = atoi(word2);

					x = x ? URANGE(0, x, edit->numlines) : 0;
					y = y ? URANGE(0, y, edit->numlines) : edit->numlines;

					send_to_char("\n\r", ch);

					for ( ; x < y ; x++)
					{
						ch_printf_color(ch, "%c%2d> %s\n\r", x == edit->on_line ? '*' : ' ', x + 1, edit->line[x] );
					}
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				break;

			case 'v':
				if (edit->numlines == 0)
				{
					send_to_char( "Buffer is empty.\n\r", ch );
					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				else
				{
					argument = one_argument(argument, word1);
					argument = one_argument(argument, word2);

					x = atoi(word1)-1;
					y = atoi(word2);

					x = x ? URANGE(0, x, edit->numlines) : 0;
					y = y ? URANGE(0, y, edit->numlines) : edit->numlines;

					ch_printf(ch, "------------------\n\r");

					for ( ; x < y ; x++)
					{
						ch_printf(ch, "%s\n\r", edit->line[x]);
					}

					ch_printf(ch, "------------------\n\r");

					write_to_buffer( ch->desc, prompt, 1000000 );
				}
				break;

			case 'a':
				send_to_char( "\n\rAborting...\n\r", ch );
				stop_editing( ch );
				break;

			case 's':
				d->connected = CON_PLAYING;

				if (ch->pcdata->last_cmd)
				{
					(*ch->pcdata->last_cmd) ( ch, "" );
				}
				break;

			default:
				ch_printf(ch, "Unknown command: /%c, /? = help\n\r", cmd[1]);
				write_to_buffer(ch->desc, prompt, 1000000);
				break;
		}
		pop_call();
		return;
	}

	if (strlen(argument) >= max_leng)
	{
		argument[max_leng] = '\0';
	}

	if (edit->size + strlen(argument) >= max_size)
	{
		send_to_char( "Max buffer size reached.\n\r", ch );
		save = TRUE;
	}
	else if (edit->numlines >= max_line)
	{
		send_to_char( "Max buffer length reached.\n\r", ch );
		save = TRUE;
	}
	else
	{
		edit->size += strlen(argument);

		edit->line[edit->on_line] = STRALLOC(argument);

		if (++edit->on_line > edit->numlines)
		{
			edit->numlines++;
		}

		if (edit->numlines >= max_line)
		{
			send_to_char("Max buffer length reached.\n\r", ch );
			save = TRUE;
		}
	}

	if (save)
	{
		d->connected = CON_PLAYING;
		if (ch->pcdata->last_cmd)
		{
			(*ch->pcdata->last_cmd) ( ch, "" );
		}
		pop_call();
		return;
	}

	ch_printf_color(ch, "%s\n\r", edit->line[edit->on_line-1]);

	write_to_buffer( ch->desc, prompt, 1000000 );
	pop_call();
	return;
}

/*
	The OLC editing stuff
*/


void do_rdupe( CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA	*room;
	bool	door;

	push_call("do_rdupe(%p,%p)",ch,argument);

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor.\n\r", ch );
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, ch->in_room->vnum))
	{
		send_to_char("The room you are standing in is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if ((room = get_room_index(atol(argument))) == NULL)
	{
		send_to_char("That room does not exists.\n\r", ch);
		pop_call();
		return;
	}

	RESTRING(ch->in_room->name,		room->name);
	RESTRING(ch->in_room->description,	room->description);
	ch->in_room->sector_type = room->sector_type;

	for (door = 0 ; door < 6 ; door++)
	{
		if (room->exit[door] == NULL)
		{
			continue;
		}
		if (ch->in_room->exit[door] == NULL)
		{
			create_exit(ch->in_room, door);
		}
		ch->in_room->exit[door]->exit_info	= room->exit[door]->exit_info;
		ch->in_room->exit[door]->key		= room->exit[door]->key;

		RESTRING(ch->in_room->exit[door]->keyword,		room->exit[door]->keyword);
		RESTRING(ch->in_room->exit[door]->description,	room->exit[door]->description);
	}

	send_to_char("Room duplicated.\n\r", ch);

	pop_call();
	return;
}

void do_rcreate( CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA *pRoomIndex;
	int vnum;

	push_call("do_rcreate(%p,%p)",ch,argument);

	if (argument[0] == '\0')
	{
		ch_printf(ch, "Syntax: edit room create <vnum>");
		pop_call();
		return;
	}

	vnum = atoi(argument);

	if (!can_olc_modify(ch, vnum))
	{
		ch_printf(ch, "Vnum: %d is not in your allocated range.\n\r", vnum);
		pop_call();
		return;
	}

	if (get_room_index(vnum) != NULL)
	{
		ch_printf(ch, "There is already a room with vnum %d.\n\r", vnum);
		pop_call();
		return;
	}

	pRoomIndex = make_room(vnum);

	if (pRoomIndex)
	{
		ch_printf(ch, "Room %d added and ready to be connected to %s.\n\r", vnum, pRoomIndex->area->name);
		CHECK_AUTOSAVE(pRoomIndex->area);
	}
	else
	{
		ch_printf(ch, "Your area doesn't seem to have a valid olc vnum range.\n\r");
	}
	pop_call();
	return;
}


void do_mcreate( CHAR_DATA *ch, char *argument )
{
	MOB_INDEX_DATA *pMobIndex;
	int vnum;

	push_call("do_mcreate(%p,%p)",ch,argument);

	vnum = is_number(argument) ? atoi(argument) : -1;

	if (vnum == -1 || argument[0] == '\0' )
	{
		send_to_char("Usage: mcreate <vnum>\n\r", ch );
		pop_call();
		return;
	}

	if (vnum < 1 || vnum > MAX_VNUM)
	{
		send_to_char( "Vnum out of range.\n\r", ch );
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, vnum))
	{
		send_to_char("Vnum is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if (get_mob_index(vnum))
	{
		send_to_char( "A mobile with that number already exists.\n\r", ch );
		pop_call();
		return;
	}

	pMobIndex = make_mobile(vnum);

	if (pMobIndex)
	{
		ch_printf(ch, "[%d] Mobile Created.\n\r", vnum);
		CHECK_AUTOSAVE(pMobIndex->area);
	}
	else
	{
		send_to_char( "Error, failed to create a new mobile.\n\r", ch );
	}
	pop_call();
	return;
}


void do_ocreate( CHAR_DATA *ch, char *argument )
{
	OBJ_INDEX_DATA      *pObjIndex;
	int                  vnum;

	push_call("do_ocreate(%p,%p)",ch,argument);

	vnum = is_number(argument) ? atoi(argument) : -1;

	if (vnum == -1 || !argument || argument[0] == '\0')
	{
		send_to_char( "Usage:  ocreate <vnum>\n\r", ch );
		pop_call();
		return;
	}

	if (vnum < 1 || vnum > MAX_VNUM)
	{
		send_to_char( "Vnum out of range.\n\r", ch );
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, vnum))
	{
		ch_printf(ch, "Vnum is not in your allocated range.\n\r");
		pop_call();
		return;
	}

	if (get_obj_index(vnum))
	{
		send_to_char( "An object with that number already exists.\n\r", ch );
		pop_call();
		return;
	}

	pObjIndex = make_object(vnum);

	if (pObjIndex)
	{
		ch_printf(ch, "[%d] Object Created.\n\r", vnum);
		CHECK_AUTOSAVE(pObjIndex->area);
	}
	else
	{
		send_to_char( "Error. Failed to create a new object.\n\r", ch );
	}
	pop_call();
	return;
}


void do_hcreate( CHAR_DATA *ch, char *argument )
{
	HELP_DATA *help;
	AREA_DATA *area;
	char arg[MAX_INPUT_LENGTH];

	push_call("do_hcreate(%p,%p)",ch,argument);

	if (argument[0] == '\0')
	{
		ch_printf(ch, "Syntax: edit help create <area name> <helpfile name>\n\r");
		pop_call();
		return;
	}

	argument = one_argument(argument, arg);

	if ((area = lookup_area(arg)) == NULL)
	{
		ch_printf(ch, "Area: %s not found.\n\r", arg);
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, area->low_r_vnum))
	{
		send_to_char("That area is not in your allocated vnum range.\n\r", ch);
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		ch_printf(ch, "You must enter a helpfile name.\n\r");
		pop_call();
		return;
	}

	if ((help = get_help(ch, argument)) != NULL)
	{
		ch_printf(ch, "A help file with that name already exists.\n\r");
		pop_call();
		return;
	}

	help = make_helpfile(capitalize_all(argument), area);

	if (help)
	{
		ch_printf(ch, "[%d] Helpfile %s Created.\n\r", help->area->low_r_vnum, help->keyword);
		CHECK_AUTOSAVE(help->area);
	}
	else
	{
		send_to_char( "Error. Failed to create a new helpfile.\n\r", ch );
	}
	pop_call();
	return;
}

void do_screate( CHAR_DATA *ch, char *argument )
{
	MOB_INDEX_DATA *mob;
	int vnum;

	push_call("do_screate(%p,%p)",ch,argument);

	vnum = is_number(argument) ? atoi(argument) : -1;

	if (vnum == -1 || argument[0] == '\0' )
	{
		send_to_char("Usage: screate <mob vnum>\n\r", ch );
		pop_call();
		return;
	}

	if (vnum < 1 || vnum > MAX_VNUM)
	{
		send_to_char( "Vnum out of range.\n\r", ch );
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, vnum))
	{
		send_to_char("Vnum is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if ((mob = get_mob_index(vnum)) == NULL)
	{
		send_to_char( "A mobile with that number does not exist.\n\r", ch );
		pop_call();
		return;
	}

	create_shop(mob);

	ch_printf(ch, "[%d] Shop created.\n\r", mob->vnum);
	CHECK_AUTOSAVE(mob->area);
	pop_call();
	return;
}

void do_rdelete( CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA *room;

	push_call("do_rdelete(%p,%p)",ch,argument);

	if (argument[0] == '\0' )
	{
		send_to_char( "Syntax: delete room <vnum>\n\r", ch );
		pop_call();
		return;
	}

	if ((room = get_room_index(atoi(argument))) == NULL)
	{
		send_to_char("No room has that vnum.\n\r", ch);
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, room->vnum))
	{
		send_to_char("That room is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if (delete_room(room))
	{
		send_to_char( "Room deleted.\n\r", ch );
	}
	else
	{
		send_to_char( "Room not deleted.\n\r", ch);
	}
	pop_call();
	return;
}


void do_odelete( CHAR_DATA *ch, char *argument )
{
	OBJ_INDEX_DATA *obj;

	push_call("do_odelete(%p,%p)",ch,argument);

	if (argument[0] == '\0' )
	{
		send_to_char( "Syntax: delete obj <vnum>\n\r", ch );
		pop_call();
		return;
	}

	if ((obj = get_obj_index(atoi(argument))) == NULL)
	{
		send_to_char("No mobile loaded has that vnum.\n\r", ch);
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, obj->vnum))
	{
		send_to_char("That room is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	delete_obj(obj);

	send_to_char( "Object deleted.\n\r", ch );

	pop_call();
	return;
}


void do_mdelete( CHAR_DATA *ch, char *argument )
{
	MOB_INDEX_DATA *mob;

	push_call("do_mdelete(%p,%p)",ch,argument);

	if (argument[0] == '\0' )
	{
		send_to_char( "Syntax: delete mob <vnum>\n\r", ch );
		pop_call();
		return;
	}

	if ((mob = get_mob_index(atoi(argument))) == NULL)
	{
		send_to_char("No mobile loaded has that vnum.\n\r", ch);
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, mob->vnum))
	{
		send_to_char("That mobile is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	delete_mob( mob );

	send_to_char( "Mob deleted.\n\r", ch );
	pop_call();
	return;
}


void do_hdelete( CHAR_DATA *ch, char *argument )
{
	HELP_DATA *help;

	push_call("do_hdelete(%p,%p)",ch,argument);

	if (argument[0] == '\0' )
	{
		send_to_char( "Syntax: delete help <helpfile name>\n\r", ch );
		pop_call();
		return;
	}

	if ((help = get_help(ch, argument)) == NULL)
	{
		send_to_char("No helpfile has that name.\n\r", ch);
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, help->area->olc_range_lo))
	{
		send_to_char("That helpfile is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	delete_help( help );

	send_to_char( "Helpfile deleted.\n\r", ch );
	pop_call();
	return;
}

void do_sdelete( CHAR_DATA *ch, char *argument )
{
	MOB_INDEX_DATA *mob;

	push_call("do_mdelete(%p,%p)",ch,argument);

	if (argument[0] == '\0' )
	{
		send_to_char( "Syntax: delete mob <vnum>\n\r", ch );
		pop_call();
		return;
	}

	if ((mob = get_mob_index(atoi(argument))) == NULL)
	{
		send_to_char("No mobile loaded has that vnum.\n\r", ch);
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, mob->vnum))
	{
		send_to_char("That mobile is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if (mob->pShop == NULL)
	{
		ch_printf(ch, "Mobile %d is not a shop keeper.\n\r", mob->vnum);
		pop_call();
		return;
	}

	delete_shop( mob );

	send_to_char( "Shop deleted.\n\r", ch );
	pop_call();
	return;
}

void break_bits( FILE *fp, long long number, char *vector , bool linear)
{
	long long bit, cnt;
	bool found;

	push_call("break_bits(%p,%p,%p,%p)",fp,number,vector,linear);

	if (IS_SET(mud->flags, MUD_STRAIGHTNUMBERS))
	{
		fprintf( fp, "%lld" , number);
		pop_call();
		return;
	}

	found	= FALSE;
	bit		= 0;

	if (!linear)
	{
		while (bit <= number && bit >= 0) /* Scandum - 10-09-2002 */
		{
			if (IS_SET(number, bit))
			{
				for (cnt = mud->bitvector_ref[*vector - 'A'] ; cnt < MAX_BITVECTOR ; cnt++)
				{
					if (bit == bitvector_table[cnt].value)
					{
						if (!str_prefix(vector, bitvector_table[cnt].name))
						{
							if (found)
							{
								fprintf( fp, "|" );
							}
							fprintf( fp, "%s", bitvector_table[cnt].name );
							found = TRUE;
							break;
						}
					}
				}
				if (cnt == MAX_BITVECTOR)
				{
					log_printf("unknown vector: %s", vector);

					if (found)
					{
						fprintf( fp, "|" );
					}

					fprintf( fp, "%lld", bit );
					found = TRUE;
				}
			}
			if (bit != 0)
			{
				bit *= 2LL;
			}
			else
			{
				bit++;
			}
		}
	}
	else	for (cnt = mud->bitvector_ref[*vector - 'A'] ; cnt < MAX_BITVECTOR ; cnt++)
	{
		if (number == bitvector_table[cnt].value)
		{
			if (!str_prefix(vector, bitvector_table[cnt].name))
			{
				fprintf( fp, "%s", bitvector_table[cnt].name );
				found = TRUE;
				break;
			}
		}
	}
	if (!found)
	{
		fprintf( fp, "%lld", number );
	}
	pop_call();
	return;
}

char fixit[MAX_STRING_LENGTH];


char *fixer_mprog( MPROG_DATA *mprog)
{
	NPC_TOKEN *token;
	int cnt;
	char lbuf[300];

	push_call("fixer_mprog(%p)",mprog);

	fixit[0] = '\0';

	for (token = mprog->first_token ; token ; token = token->next)
	{
		for (cnt = 0 ; cnt < token->level*2 ; cnt++)
		{
			lbuf[cnt] = ' ';
		}
		lbuf[cnt] = '\0';

		switch (token->type)
		{
			case MPTOKEN_SOCIAL:
				cat_sprintf(fixit, "%s%s %s\n", lbuf, (char *) token->function, token->string);
				break;
			case MPTOKEN_COMMAND:
				cat_sprintf(fixit, "%s%s %s\n", lbuf, cmd_table[token->value].name, token->string);
				break;
			case MPTOKEN_IF:
				if (strstr(token->string, "actorhasobjnum"))
				{
					token->string[strlen(token->string) - 1] = 0;
	
					cat_sprintf(fixit, "%sif hasobjnum ($n) == %s\n", lbuf, &token->string[16]);
	
					token->string[strlen(token->string)] = ')';
				}
				else
				{
					cat_sprintf(fixit, "%sif %s\n", lbuf, token->string);
				}
				break;
			case MPTOKEN_OR:
				if (strstr(token->string, "actorhasobjnum"))
				{
					token->string[strlen(token->string) - 1] = 0;

					cat_sprintf(fixit, "%sor hasobjnum ($n) == %s\n", lbuf, &token->string[16]);

					token->string[strlen(token->string)] = ')';
				}
				else
				{
					cat_sprintf(fixit, "%sor %s\n", lbuf, token->string);
				}
				break;
			case MPTOKEN_ELSE:
				cat_sprintf(fixit, "%selse\n", lbuf);
				break;
			case MPTOKEN_ENDIF:
				cat_sprintf(fixit, "%sendif\n", lbuf);
				break;
			case MPTOKEN_BREAK:
				cat_sprintf(fixit, "%sbreak\n", lbuf);
				break;
			case MPTOKEN_SWITCH:
				cat_sprintf(fixit, "%sswitch %s\n", lbuf, token->string);
				break;
			case MPTOKEN_CASE:
				cat_sprintf(fixit, "%scase %s\n", lbuf, token->string);
				break;
			case MPTOKEN_DEFAULT:
				cat_sprintf(fixit, "%sdefault\n", lbuf);
				break;
			case MPTOKEN_ENDSWITCH:
				cat_sprintf(fixit, "%sendswitch\n", lbuf);
				break;
			case MPTOKEN_IFNOT:
				cat_sprintf(fixit, "%sifnot %s\n", lbuf, token->string);
				break;
			case MPTOKEN_ORNOT:
				cat_sprintf(fixit, "%sornot %s\n", lbuf, token->string);
				break;
			default:
				cat_sprintf(fixit, "%sUnknown %s\n", lbuf, token->string);
				break;
		}
	}
	pop_call();
	return fixit;
}

char *fixer( char *arg)
{
	char *str,*fix;

	push_call("fixer(%p)",arg);

	fixit[0] = '\0';

	if (arg == NULL)
	{
		pop_call();
		return fixit;
	}

	for (str = arg, fix = fixit ; *str != '\0' ; str++)
	{
		if (*str != '\r')
		{
			*fix = *str ;
			fix++;
		}
	}
	*fix = '\0';

	pop_call();
	return fixit;
}

void save_header( FILE *fp, AREA_DATA *area)
{
	push_call("save_header(%p,%p)",fp,area);

	fprintf( fp, "#AREA %s~\n",			area->name);
	fprintf( fp, "#AUTHORS %s~\n",		area->authors);
	fprintf( fp, "#VERSION %d\n",			AREA_VERSION);
	fprintf( fp, "#RANGES %d %d %d %d\n",	area->low_soft_range, area->hi_soft_range, area->low_hard_range, area->hi_hard_range);
	if (area->olc_range_hi != 0)
	{
		fprintf( fp, "#OLC_RANGES %d %d\n",	area->olc_range_lo, area->olc_range_hi);
	}

	if (*area->resetmsg)
	{
		fprintf( fp, "#RESETMSG %s~\n\n", area->resetmsg );
	}

	if (area->flags)
	{
		fprintf( fp, "#FLAGS " );
		break_bits( fp, area->flags, "AFLAG_", FALSE);
		fprintf( fp, "\n");
	}

	if (IS_SET(area->flags, AFLAG_WEATHER))
	{
		fprintf( fp, "#WEATHER %d %d %d %d %d\n", area->weather_info->temp_winter,area->weather_info->temp_summer, area->weather_info->temp_daily, area->weather_info->wet_scale, area->weather_info->wind_scale);
	}
	pop_call();
	return;
}

void save_rooms( FILE *fp, AREA_DATA *area)
{
	ROOM_INDEX_DATA *room;
	ROOM_TIMER_DATA *rtd;
	int vnum, door;
	char buf[MAX_STRING_LENGTH];
	EXTRA_DESCR_DATA *ed;

	push_call("save_rooms(%p,%p)",fp,area);

	fprintf( fp, "\n\n#ROOMS\n" );

	for (rtd = mud->f_room_timer ; rtd ; rtd = rtd->next)
	{
		if (rtd->vnum >= area->low_r_vnum && rtd->vnum <= area->hi_r_vnum)
		{
			switch (rtd->type)
			{
				case ROOM_TIMER_SANCTIFY:	REMOVE_BIT(room_index[rtd->vnum]->room_flags, ROOM_SAFE);	rtd->timer = 1;	break;
				case ROOM_TIMER_SMOKE:		REMOVE_BIT(room_index[rtd->vnum]->room_flags, ROOM_SMOKE);	rtd->timer = 1;	break;
				case ROOM_TIMER_GLOBE:		REMOVE_BIT(room_index[rtd->vnum]->room_flags, ROOM_GLOBE);	rtd->timer = 1;	break;
				case ROOM_TIMER_BLOCK:		REMOVE_BIT(room_index[rtd->vnum]->room_flags, ROOM_BLOCK);	rtd->timer = 1;	break;
				case ROOM_TIMER_ICE:		REMOVE_BIT(room_index[rtd->vnum]->room_flags, ROOM_ICE);	rtd->timer = 1;	break;
				case ROOM_TIMER_UNBLOCK:		   SET_BIT(room_index[rtd->vnum]->room_flags, ROOM_BLOCK);	rtd->timer = 1;	break;
			}
		}
	}

	for (vnum = area->low_r_vnum ; vnum <= area->hi_r_vnum ; vnum++)
	{
		if ((room = get_room_index(vnum)) == NULL)
		{
			continue;
		}
		if (room->vnum >= MAX_VNUM && IS_SET(room->room_flags, ROOM_HASH))
		{
			continue;
		}
		if (room->area != area)
		{
		/*
			log_printf("Found room from area %s while saving %s.", room->area->name, area->name);
		*/
			continue;
		}
		fprintf( fp, "#%d\n", vnum );
		fprintf( fp, "%s~\n", fixer(room->name));
		fprintf( fp, "%s~\n", fixer(room->description));
		if (IS_SET(room->room_flags, ROOM_IS_CASTLE))
		{
			fprintf( fp, "%d ", room->creator_pvnum);
		}
		else
		{
			fprintf( fp, "%d ", 0);
		}
		break_bits( fp, room->room_flags, "ROOM_", FALSE);
		fprintf( fp, " ");
		break_bits( fp, room->sector_type, "SECT_", TRUE);
		fprintf( fp, "\n");
		for (door = 0 ; door < 6 ; door++)
		{
			if (room->exit[door] != NULL)
			{
				if (room->exit[door]->vnum <= 0)
				{
					continue;
				}

				REMOVE_BIT(room->exit[door]->exit_info, EX_BASHED);
				REMOVE_BIT(room->exit[door]->exit_info, EX_UNBARRED);
				fprintf( fp, "D" );
				break_bits( fp, door, "DIR_" , TRUE);
				fprintf( fp, "\n%s~\n", fixer( room->exit[door]->description ));
				if (IS_SET(room->exit[door]->exit_info, EX_ISDOOR))
				{
					fprintf( fp, "%s~\n", fixer(room->exit[door]->keyword) );
				}
				else
				{
					fprintf( fp, "~\n");
				}
				break_bits(fp, room->exit[door]->exit_info, "EX_" , FALSE);
				fprintf( fp, " %d %u\n", room->exit[door]->key, room->exit[door]->vnum);
			}
		}

		for (ed = room->first_extradesc ; ed != NULL ; ed = ed->next)
		{
			fprintf( fp, "E\n" );
			fprintf( fp, "%s~\n", fixer(ed->keyword ));
			sprintf(buf, "%s", fixer(ed->description ));
			if (buf[0] == ' ')
			{
				fprintf( fp, "." );
			}
			fprintf( fp, "%s~\n", buf);
		}
		fprintf( fp, "S\n");
	}
	fprintf( fp, "#0\n" );
	pop_call();
	return;
}

void save_mobiles( FILE *fp, AREA_DATA *area)
{
	MOB_INDEX_DATA *pMobIndex;
	MPROG_DATA *mpg;
	EXTRA_DESCR_DATA *ed;
	int vnum;

	push_call("save_mobiles(%p,%p)",fp,area);

	fprintf( fp, "\n\n#MOBILES\n" );

	for (vnum = area->low_m_vnum ; vnum <= area->hi_m_vnum ; vnum++)
	{
		if ((pMobIndex = get_mob_index(vnum)) == NULL)
		{
			continue;
		}
		if (pMobIndex->area != area)
		{
			log_printf("Found mob from area %s while saving %s.", pMobIndex->area->name, area->name);
			continue;
		}
		REMOVE_BIT(pMobIndex->act, ACT_IS_NPC);

		fprintf( fp, "#%d\n", 	vnum 				);
		fprintf( fp, "%s~\n", 	fixer(pMobIndex->player_name)	);
		fprintf( fp, "%s~\n", 	fixer(pMobIndex->short_descr)	);
		fprintf( fp, "%s~\n", 	fixer(pMobIndex->long_descr)	);
		fprintf( fp, "%s~\n", 	fixer(pMobIndex->description)	);
		break_bits( fp, pMobIndex->act, "ACT_", FALSE		);
		fprintf( fp, "\n");
		break_bits( fp, pMobIndex->affected_by, "AFF_", FALSE	);
		fprintf( fp, "\n%d S\n",	pMobIndex->alignment		);
		fprintf( fp, "%d ",	pMobIndex->level		);
		if (IS_SET(pMobIndex->act,ACT_BODY))
		{
			break_bits( fp, pMobIndex->body_parts, "BODY_", FALSE);
			fprintf( fp, " ");
			break_bits( fp, pMobIndex->attack_parts, "BODY_", FALSE);
		}
		else
		{
			fprintf( fp, "0 0");
		}
		fprintf( fp, " %dd%d+%d %dd%d+%d\n",
		pMobIndex->hitnodice,pMobIndex->hitsizedice,pMobIndex->hitplus, pMobIndex->damnodice,pMobIndex->damsizedice,pMobIndex->damplus);
		if (IS_SET(pMobIndex->act, ACT_RACE))
		{
			fprintf( fp, "%d ",pMobIndex->gold);
			break_bits( fp, pMobIndex->race, "RACE_", TRUE);
			fprintf( fp, "\n");
		}
		else
		{
			fprintf(fp, "%d 0\n", pMobIndex->gold);
		}
		break_bits( fp, pMobIndex->position, "POS_", TRUE);
		fprintf( fp, " ");
		break_bits( fp, pMobIndex->position, "POS_", TRUE);
		fprintf( fp, " ");
		break_bits( fp, pMobIndex->sex, "SEX_", TRUE);
		fprintf( fp, "\n");
		for (ed = pMobIndex->first_extradesc ; ed ; ed = ed->next)
		{
			fprintf( fp, "E\n" );
			fprintf( fp, "%s~\n", fixer(ed->keyword ));
			fprintf( fp, "%s~\n", fixer(ed->description ));
		}
		if (IS_SET(pMobIndex->act, ACT_CLASS))
		{
			fprintf( fp, "C " );
			break_bits( fp, pMobIndex->class, "CLASS_", TRUE);
			fprintf( fp, "\n" );
		}
		if (pMobIndex->first_prog)
		{
			for (mpg = pMobIndex->first_prog ; mpg ; mpg = mpg->next)
			{
				fprintf( fp, ">%s ",	mprog_type_to_name(mpg->type));
				fprintf( fp, "%s~\n",	mpg->arglist);
				fprintf( fp, "%s~\n",	fixer_mprog(mpg));
			}
			fprintf( fp, "|\n");
		}
	}
	fprintf( fp, "#0\n" );
	pop_call();
	return;
}

void save_helps( FILE *fp, AREA_DATA *area)
{
	HELP_DATA *help;
	HELP_MENU_DATA *menu;

	push_call("save_helps(%p,%p)",fp,area);

	fprintf(fp,"\n\n#HELPS\n");

	for (help = area->first_help ; help ; help = help->next)
	{
		fprintf(fp,"%d %s~\n", help->level, fixer(help->keyword));
		fprintf(fp, "%s", fixer(help->text));

		for (menu = help->first_menu ; menu ; menu = menu->next)
		{
			fprintf(fp, "{%c}%s\n", menu->option, menu->help->keyword);
		}
		fprintf(fp, "~\n\n");
	}
	fprintf(fp, "0 $~\n");
	pop_call();
	return;
}


void save_objects( FILE *fp, AREA_DATA *area)
{
	OBJ_INDEX_DATA *pObjIndex;
	EXTRA_DESCR_DATA *ed;
	AFFECT_DATA *aff;
	OBJ_PROG *prg;
	int vnum;
	char buf[MAX_STRING_LENGTH], buf2[200], *pt;

	push_call("save_objects(%p,%p)",fp,area);

	fprintf(fp, "\n\n#OBJECTS\n");

	for (vnum = area->low_o_vnum ; vnum <= area->hi_o_vnum ; vnum++)
	{
		if ((pObjIndex = get_obj_index(vnum)) == NULL)
		{
			continue;
		}
		if (pObjIndex->area != area)
		{
			log_printf("Found object from area %s while saving %s.", pObjIndex->area->name, area->name);
			continue;
		}
		fprintf( fp, "#%d\n", vnum );
		strcpy( buf, pObjIndex->name );
		sprintf( buf2, "i%d", pObjIndex->vnum );
		if ((pt = strstr(buf, buf2)) != NULL)
		{
			*( pt -1 ) = '\0';
		}
		fprintf( fp, "%s~\n", fixer( buf ));
		fprintf( fp, "%s~\n", fixer(pObjIndex->short_descr));
		fprintf( fp, "%s~\n", fixer(pObjIndex->long_descr));
		fprintf( fp, "%s~\n", fixer(pObjIndex->description));
		break_bits( fp, pObjIndex->item_type, "ITEM_TYPE_", TRUE);
		fprintf( fp, "\n");
		break_bits( fp, pObjIndex->extra_flags, "ITEM_FLAG_", FALSE);
		fprintf( fp, "\n");
		break_bits( fp, pObjIndex->wear_flags, "ITEM_WEAR_", FALSE);
		fprintf( fp, "\n");
		switch (pObjIndex->item_type)
		{
			case ITEM_SCROLL:
			case ITEM_POTION:
			case ITEM_PILL:
				fprintf( fp, "%d ",pObjIndex->value[0]);
				break_bits( fp, skill_table[pObjIndex->value[1]].slot, "SPELL_", TRUE);
				fprintf( fp, " ");
				break_bits( fp, skill_table[pObjIndex->value[2]].slot, "SPELL_", TRUE);
				fprintf( fp, " ");
				break_bits( fp, skill_table[pObjIndex->value[3]].slot, "SPELL_", TRUE);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_WAND:
			case ITEM_STAFF:
				fprintf( fp, "%d %d %d ",pObjIndex->value[0],pObjIndex->value[1],pObjIndex->value[2]);
				break_bits( fp, skill_table[pObjIndex->value[3]].slot, "SPELL_", TRUE);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_TOTEM:
				break_bits( fp, skill_table[pObjIndex->value[0]].slot, "SPELL_", TRUE);
				fprintf( fp, " ");
				break_bits( fp, skill_table[pObjIndex->value[1]].slot, "SPELL_", TRUE);
				fprintf( fp, " ");
				break_bits( fp, skill_table[pObjIndex->value[2]].slot, "SPELL_", TRUE);
				fprintf( fp, " ");
				break_bits( fp, skill_table[pObjIndex->value[3]].slot, "SPELL_", TRUE);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_WEAPON:
				break_bits(fp, pObjIndex->value[0], "WEAPONTYPE_", TRUE);
				fprintf(fp, " %d %d ",pObjIndex->value[1],pObjIndex->value[2]);
				break_bits(fp, pObjIndex->value[3], "WEAPON_", TRUE);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_CONTAINER:
				fprintf(fp, "%d ",pObjIndex->value[0]);
				break_bits( fp, pObjIndex->value[1], "CONT_", FALSE);
				fprintf(fp, " %d %d",pObjIndex->value[2],pObjIndex->value[3]);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_DRINK_CON:
			case ITEM_FOUNTAIN:
				fprintf(fp, "%d %d ", pObjIndex->value[0], pObjIndex->value[1]);
				break_bits(fp, pObjIndex->value[2], "LIQ_", TRUE);
				fprintf(fp, " 0");
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_FOOD:
				fprintf(fp, "%d %d %d %d", pObjIndex->value[0], pObjIndex->value[1], pObjIndex->value[2], pObjIndex->value[3]);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_PORTAL:
				fprintf(fp, "%d %d ", pObjIndex->value[0], pObjIndex->value[1]);
				break_bits(fp, pObjIndex->value[2], "GATE_", FALSE);
				fprintf(fp, " %d", pObjIndex->value[3]);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_FURNITURE:
				fprintf(fp, "%d %d ", pObjIndex->value[0], pObjIndex->value[1]);
				break_bits(fp, pObjIndex->value[2], "FURNITURE_", FALSE);
				fprintf(fp, " %d", pObjIndex->value[3]);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			case ITEM_LIGHT:
				fprintf(fp, "0 %d %d %d",  /*pObjIndex->value[0],*/ pObjIndex->value[1], pObjIndex->value[2], pObjIndex->value[3]);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
			default:
				fprintf(fp, "%d %d %d %d",  pObjIndex->value[0], pObjIndex->value[1], pObjIndex->value[2], pObjIndex->value[3]);
				fprintf(fp, " %d %d %d %d", pObjIndex->value[4], pObjIndex->value[5], pObjIndex->value[6], pObjIndex->value[7]);
				break;
		}
		fprintf( fp, "\n");
		fprintf( fp, "%d %d %d\n",pObjIndex->weight,pObjIndex->cost,pObjIndex->level);

		for (ed = pObjIndex->first_extradesc ; ed ; ed = ed->next)
		{
			fprintf( fp, "E\n%s~\n%s~\n", ed->keyword, fixer(ed->description));
		}

		for (aff = pObjIndex->first_affect ; aff ; aff = aff->next)
		{
			fprintf(fp, "A %s %d %lld\n", broken_bits(aff->location, "APPLY_", TRUE), aff->modifier, aff->bitvector);
		}

		if (pObjIndex->attack_string[0] != '\0' || pObjIndex->class_flags)
		{
			fprintf( fp, "C\n%s~\n%s\n", pObjIndex->attack_string, broken_bits(pObjIndex->class_flags, "FLAG_CLASS_", FALSE));
		}

		for (prg = pObjIndex->first_prog ; prg ; prg = prg->next)
		{
			int keyword;

			fprintf( fp, "P %d\n", prg->index);

			keyword = prg->trigger;

			break_bits( fp, keyword, "TRIG_", FALSE);

			switch (keyword)
			{
				case TRIG_COMMAND:
				case TRIG_ROOM_COMMAND:
				case TRIG_WEAR_COMMAND:
					fprintf( fp, " %d %s\n",prg->percentage,cmd_table[prg->cmd].name);
					break;
				case TRIG_UNKNOWN:
				case TRIG_ROOM_UNKNOWN:
				case TRIG_WEAR_UNKNOWN:
					fprintf( fp, " %d %s\n",prg->percentage, prg->unknown);
					break;
				case TRIG_TICK:
				case TRIG_HIT:
				case TRIG_DAMAGE:
				case TRIG_WEAR:
				case TRIG_REMOVE:
				case TRIG_SACRIFICE:
					fprintf( fp, " %d\n", prg->percentage);
					break;
				case TRIG_VOID:
					fprintf( fp, "\n");
					break;
				default:
					fprintf( fp, "\n");
					log_printf( "save_objects: Bad obj_command type %d", keyword);
					break;
			}
			break_bits( fp, prg->obj_command, "OPROG_", TRUE);

			switch(prg->obj_command)
			{
				case OPROG_ECHO:
					fprintf( fp, "\n%s~\n", fixer(prg->argument));
					break;
				case OPROG_GOD_COMMAND:
				case OPROG_GOD_ARGUMENT:
				case OPROG_COMMAND:
				case OPROG_ARGUMENT:
					fprintf( fp, "\n%s~\n", fixer(prg->argument));
					break;
				case OPROG_QUEST_SET:
				case OPROG_QUEST_ADD:
					fprintf( fp, " %d %d %d\n", prg->quest_offset, prg->quest_bits, prg->if_value );
					break;
				case OPROG_JUNK:
					fprintf( fp, "\n" );
					break;
				case OPROG_APPLY:
					fprintf( fp, " ");
					break_bits( fp, prg->if_check, "OAPPLY_", TRUE);
					fprintf( fp, " %d\n", prg->if_value );
					break;
				case OPROG_IF_HAS_OBJECT:
					fprintf( fp, " %d %d %d\n", prg->if_value, prg->if_true,prg->if_false);
					break;
				case OPROG_PLAYER_QUEST_IF:
				case OPROG_OBJECT_QUEST_IF:
					fprintf( fp, " %d %d %c %d %d %d\n", prg->quest_offset, prg->quest_bits, prg->if_symbol, prg->if_value, prg->if_true, prg->if_false);
					break;
				case OPROG_IF:
					fprintf( fp, " " );
					break_bits( fp, prg->if_check, "OIF_", TRUE);
					fprintf( fp, " %c ", prg->if_symbol);
					switch (prg->if_check)
					{
						case OIF_WEAR_LOC:
							break_bits( fp, prg->if_value, "WEAR_",	TRUE);
							break;
						case OIF_USER_CLASS:
							break_bits( fp, prg->if_value, "CLASS_",TRUE);
							break;
						case OIF_USER_POSITION:
							break_bits( fp, prg->if_value, "POS_",	TRUE);
							break;
						case OIF_USER_RACE:
							break_bits( fp, prg->if_value, "RACE_",	TRUE);
							break;
						case OIF_USER_SEX:
							break_bits( fp, prg->if_value, "SEX_",	TRUE);
							break;
						case OIF_USER_SECTOR:
							break_bits( fp, prg->if_value, "SECT_",	TRUE);
							break;
						case OIF_USER_WHICH_GOD:
							break_bits( fp, prg->if_value, "GOD_",	TRUE);
							break;
						default:
							fprintf( fp, "%d",prg->if_value);
							break;
					}
					fprintf( fp, " %d %d\n",prg->if_true,prg->if_false);
					break;
				default:
					log_string( "save_objects: Bad obj_command reaction type");
					break;
			}
		}
	}
	fprintf(fp,"#0\n");
	pop_call();
	return;
}


void save_shops( FILE *fp, AREA_DATA *area)
{
	MOB_INDEX_DATA *mob;
	int vnum, i;

	push_call("save_shops(%p,%p)",fp,area);

	fprintf(fp, "\n\n#SHOPS\n");

	for (vnum = area->low_m_vnum ; vnum <= area->hi_m_vnum ; vnum++)
	{
		if ((mob = get_mob_index(vnum)) == NULL)
		{
			continue;
		}

		if (mob->area != area)
		{
			continue;
		}

		if (mob->pShop == NULL)
		{
			continue;
		}

		fprintf(fp,"%d ", mob->pShop->keeper);

		for (i = 0 ; i < MAX_TRADE ; i++)
		{
			break_bits(fp, mob->pShop->buy_type[i], "ITEM_TYPE_", TRUE);
			fprintf(fp, " ");
		}
		fprintf(fp," %3d %3d %2d %2d\n", mob->pShop->profit_buy, mob->pShop->profit_sell, mob->pShop->open_hour, mob->pShop->close_hour);
	}
	fprintf(fp,"0\n");
	pop_call();
	return;
}

void save_resets( FILE *fp, AREA_DATA *area)
{
	RESET_DATA *reset;

	push_call("save_resets(%p,%p)",fp,area);

	fprintf(fp, "\n\n#RESETS\n");

	for (reset = area->first_reset ; reset ; reset = reset->next)
	{
		switch (reset->command)
		{
			case 'M':
				fprintf(fp, "%c %d %3d %d\n",
					reset->command,
					reset->arg1,
					reset->arg2,
					reset->arg3);
				break;
			case 'O':
				fprintf(fp, "%c %d %3d %d\n",
					reset->command,
					reset->arg1,
					URANGE(0, reset->arg2, 999),
					reset->arg3);
				break;
			case 'G':
				fprintf(fp, "%c %d %3d %d\n",
					reset->command,
					reset->arg1,
					URANGE(0, reset->arg2, 999),
					reset->arg3);
				break;
			case 'E':
				fprintf(fp, "%c %d %3d %s\n",
					reset->command,
					reset->arg1,
					URANGE(0, reset->arg2, 999),
					broken_bits(reset->arg3, "WEAR_", TRUE));
				break;
			case 'P':
				fprintf(fp, "%c %d %3d %d\n",
					reset->command,
					reset->arg1,
					URANGE(0, reset->arg2, 999),
					reset->arg3);
				break;
			case 'D':
				fprintf(fp, "%c %d %-9s ",
					reset->command,
					reset->arg1,
					broken_bits(reset->arg2, "DIR_",  TRUE));
				fprintf(fp, "%s\n",
					broken_bits(reset->arg3, "DOOR_", TRUE));
				break;
			case 'R':
				fprintf(fp, "%c %d %d %d\n",
					reset->command,
					reset->arg1,
					reset->arg2,
					reset->arg3);
				break;
			default:
				fprintf(fp, "%c %d %d %d\n",
					reset->command,
					reset->arg1,
					reset->arg2,
					reset->arg3);
				break;
		}
	}
	fprintf(fp,"S\n");
	pop_call();
	return;
}

void save_specials( FILE *fp, AREA_DATA *area)
{
	MOB_INDEX_DATA *pMobIndex;
	int vnum;

	push_call("save_specials(%p,%p)",fp,area);

	fprintf(fp, "\n\n#SPECIALS\n");

	for (vnum = area->low_m_vnum ; vnum <= area->hi_m_vnum ; vnum++)
	{
		if ((pMobIndex = get_mob_index(vnum)) == NULL)
		{
			continue;
		}
		if (pMobIndex->area != area)
		{
			continue;
		}
		if (!pMobIndex->spec_fun)
		{
			continue;
		}
		fprintf(fp,"M %d %s\n", vnum, fixer(spec_name_lookup(pMobIndex->spec_fun)));
	}
	fprintf(fp,"S\n");
	pop_call();
	return;
}


void do_savearea( CHAR_DATA *ch, char *arg)
{
	AREA_DATA *area = NULL;
	bool forreal;
	bool numbers;

	push_call("do_savearea(%p,%p)",ch,arg);

	if (ch == NULL)
	{
		for (area = mud->f_area ; area ; area = area->next)
		{
			if (IS_SET(area->flags, AFLAG_MODIFIED))
			{
				save_area(area, TRUE);
			}
		}
		pop_call();
		return;
	}

	if (IS_NPC(ch))
	{
		log_printf("un authorized savearea");
		dump_stack();
		pop_call();
		return;
	}

	forreal =  strcasecmp(arg, "backup");
	numbers = !strcasecmp(arg, "numbers");

	if (numbers && IS_GOD(ch))
	{
		TOGGLE_BIT(mud->flags, MUD_STRAIGHTNUMBERS);

		if (IS_SET(mud->flags, MUD_STRAIGHTNUMBERS))
		{
			send_to_char("Bitvectors are now saved as numbers.\n\r", ch);
		}
		else
		{
			send_to_char("Bitvectors are now saved as words.\n\r", ch);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "all"))
	{
		if (IS_GOD(ch))
		{
			for (area = mud->f_area ; area ; area = area->next)
			{
				log_god_printf("%s saving area %s.", ch->name, area->name);
				save_area( area, TRUE );
			}
			send_to_char("Finished.\n\r", ch );
		}
		else
		{
			send_to_char("Only Gods are allowed to do that.\n\r", ch);
		}
		pop_call();
		return;
	}
	else
	{
		if (arg[0] == '\0' || (area = lookup_area(arg)) == NULL)
		{
			area = ch->in_room->area;
		}

		if (!can_olc_modify(ch, area->low_r_vnum))
		{
			send_to_char("You cannot save this area.\n\r", ch);
			pop_call();
			return;
		}
	}

	save_area( area, forreal );

	if (ch != NULL)
	{
		if (!forreal)
		{
			log_god_printf("%s saving (backup format) area %s", ch->name, area->name);
			send_to_char("Area saved (backup format).\n\r", ch);
		}
		else
		{
			log_god_printf("%s saving (normal format) area %s", ch->name, area->name);
			send_to_char("Area saved (normal format).\n\r", ch);
		}
	}
	pop_call();
	return;
}

void save_area( AREA_DATA *area, bool forreal )
{
	char buf[200];
	FILE *fp;

	push_call("save_area(%p,%p)",area,forreal);

	sprintf( buf, "%s.bak", area->filename );

	close_reserve();

	fp = my_fopen(buf, "w", FALSE);

	REMOVE_BIT(area->flags, AFLAG_MODIFIED);

	save_header(	fp, area);
	if (area->first_help)
	{
		save_helps(	fp, area);
	}
	if (area->hi_m_vnum)
	{
		save_mobiles(	fp, area );
		save_shops(	fp, area );
		save_specials(	fp, area );
	}
	if (area->hi_o_vnum)
	{
		save_objects(	fp, area );
	}
	if (area->hi_r_vnum)
	{
		save_rooms(	fp, area );
	}
	if (area->first_reset)
	{
		save_resets(	fp, area );
	}

	fprintf( fp, "\n\n#$\n\nXXXXXXXXXX\n#Savearea\n" );

	my_fclose( fp );

	if (is_valid_save(buf, "Savearea"))
	{
		if (forreal)
		{
			remove( area->filename );
			rename(buf,area->filename);
		}
	}

	open_reserve();

	pop_call();
	return;
}

void do_goto( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *fch, *fch_next, *victim;
	CHAR_DATA *mount;
	ROOM_INDEX_DATA *in_room;
	int vnum;

	push_call("do_goto(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		log_printf( "[%d] Mob using goto: %s", ch->pIndexData->vnum, argument);
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char( "Goto where?\n\r", ch );
		pop_call();
		return;
	}

	mount = ch->mounting;
	vnum  = atol(argument);

	if (!is_number(argument) || get_room_index(vnum) == NULL)
	{
		if (!IS_GOD(ch))
		{
			if ((victim = get_char_area(ch, argument)) == NULL)
			{
				send_to_char("You cannot find that...\n\r", ch);
				pop_call();
				return;
			}
			vnum = victim->in_room->vnum;
		}
		else
		{
			if ((vnum = find_location(ch, argument)) == -1)
			{
				send_to_char( "You cannot find that...\n\r", ch );
				pop_call();
				return;
			}
		}
	}

	if (!can_olc_modify(ch, vnum))
	{
		send_to_char("That destination is not in your allocated vnum range.\n\r", ch);
		pop_call();
		return;
	}

	if ( room_is_private( room_index[vnum] ) )
	{
		if ( get_trust( ch ) < LEVEL_IMMORTAL )
		{
			send_to_char( "That room is private right now.\n\r", ch );
			pop_call();
			return;
		}
		else
		{
			send_to_char( "Overriding private flag!\n\r", ch );
		}
	}

	in_room = ch->in_room;

	if ( ch->fighting )
	{
		stop_fighting( ch, TRUE );
	}

	if (!IS_SET(ch->act, PLR_WIZINVIS))
	{
		act(ch->pcdata->bamfout[0] != '\0' ? ch->pcdata->bamfout : "$n leaves in a swirling mist.", ch, NULL, NULL, TO_ROOM);
	}

	char_from_room( ch );
	char_to_room( ch, vnum );

	if (!IS_SET(ch->act, PLR_WIZINVIS))
	{
		act(ch->pcdata->bamfin[0] != '\0' ? ch->pcdata->bamfin : "$n appears in a swirling mist.", ch, NULL, NULL, TO_ROOM);
	}

	do_look( ch, "auto" );

	ch->mounting = mount;

	if ( ch->in_room == in_room )
	{
		pop_call();
		return;
	}

	for (fch = in_room->first_person ; fch ; fch = fch_next)
	{
		fch_next = fch->next_in_room;

		if (IS_NPC(fch) && fch->master == ch)
		{
			char_from_room (fch);
			char_to_room( fch, vnum );
		}
		else if (fch->master == ch && IS_IMMORTAL(fch))
		{
			act( "You follow $N.", fch, NULL, ch, TO_CHAR );
			do_goto( fch, argument );
		}
	}
	pop_call();
	return;
}

extern int top_area;

void write_area_list( )
{
	AREA_DATA *tarea;
	FILE *fpout;

	push_call("write_area_list()");

	log_printf( "writing area.lst not allowed, use shell, aborting." );
	dump_stack();

	pop_call();
	return;

	fpout = my_fopen( AREA_LIST, "w",FALSE );

	if ( !fpout )
	{
		bug( "couldn't open area.lst for writing");
		pop_call();
		return;
	}

	for (tarea = mud->f_area ; tarea ; tarea = tarea->next)
	{
		fprintf( fpout, "%s\n", tarea->filename );
	}

	fprintf( fpout, "$\n" );
	my_fclose( fpout );
	pop_call();
	return;
}

void assign_area( CHAR_DATA *ch )
{
 	char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char taf[1024];
	AREA_DATA *pArea = NULL, *temp_area;

	push_call("assign_area(%p)",ch);

	if (ch->level >= LEVEL_IMMORTAL && ch->pcdata->a_range_lo && ch->pcdata->a_range_hi)
	{
		sprintf( taf, "newareas/%s.are", ch->name);

		log_printf( "Creating area entry for %s", ch->name );

		sprintf(buf1, "%s's Area", ch->name);
		sprintf(buf2, "%s", ch->name);

		ALLOCMEM( pArea, AREA_DATA, 1 );

		pArea->first_reset				= NULL;
		pArea->last_reset				= NULL;
		pArea->name         			= STRALLOC(buf1);
		pArea->filename				= STRALLOC(taf);
		pArea->authors					= STRALLOC(buf2);
		pArea->age					= 0;
		pArea->nplayer					= 0;
		pArea->flags					= 0;
		pArea->resetmsg				= STRDUPE(str_empty);
		ALLOCMEM(pArea->weather_info, WEATHER_DATA, 1);
		pArea->weather_info->temp_winter	= -10;
		pArea->weather_info->temp_summer	=  20;
		pArea->weather_info->temp_daily	=  10;
		pArea->weather_info->wet_scale	=   4;
		pArea->weather_info->wind_scale	=   4;
		pArea->olc_range_lo				= ch->pcdata->a_range_lo;
		pArea->olc_range_hi				= ch->pcdata->a_range_hi;
		pArea->low_r_vnum				= MAX_VNUM-1;
		pArea->low_m_vnum				= MAX_VNUM-1;
		pArea->low_o_vnum				= MAX_VNUM-1;
		pArea->hi_r_vnum				= 0;
		pArea->hi_m_vnum				= 0;
		pArea->hi_o_vnum				= 0;

		for (temp_area = mud->f_area ; temp_area ; temp_area = temp_area->next)
		{
			if (strcmp(pArea->name, temp_area->name) < 0)
			{
				INSERT_LEFT(pArea, temp_area, mud->f_area, next, prev);
				break;
			}
		}
		if (!temp_area)
		{
			LINK(pArea, mud->f_area, mud->l_area, next, prev);
		}
		mud->top_area++;
	}
	pop_call();
	return;
}

void do_medit( CHAR_DATA *ch, char *argument )
{
	char arg2 [MAX_INPUT_LENGTH];
	char arg3 [MAX_INPUT_LENGTH];
	char buf  [MAX_STRING_LENGTH];
	int  num, size, plus, value;
	char char1, char2;

	EXTRA_DESCR_DATA *ed;
	MOB_INDEX_DATA *mob;

	char *origarg = argument;

	char colw[10], colg[10], colW[10];

	push_call("do_medit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor\n\r", ch );
		pop_call();
		return;
	}

	if (ch->pcdata->edit_ptr && (mob = ch->pcdata->edit_ptr)->area == NULL)
	{
		ch_printf(ch, "The mob you were editing is no longer valid.\n\r");
		stop_olc_editing(ch, ch->in_room->area);
		pop_call();
		return;
	}

	switch (ch->pcdata->editmode)
	{
		case MODE_MOB_DESC:
			mob = ch->pcdata->edit_ptr;

			STRFREE(mob->description);
			mob->description = copy_buffer(ch);
			stop_editing(ch);

			pop_call();
			return;

		case MODE_MOB_EXTRA:
			ed  = ch->pcdata->temp_ptr;

			STRFREE( ed->description );
			ed->description = copy_buffer( ch );
			stop_editing( ch );

			pop_call();
			return;
	}

	smash_tilde( argument );

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		mob = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show medit status
			*/
			ch_printf(ch, "Syntax: <command> <value>\n\r\n\r");
			ch_printf(ch, "Command being one of:\n\r");
			ch_printf(ch, "  sex gold align race class affect act level pos body attack dam hit\n\r");
			ch_printf(ch, "  name short long desc ed rmed done ? < >\n\r");
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char( "Medit mode off.\n\r", ch );

			stop_olc_editing(ch, mob->area);

			pop_call();
			return;
		}

		if (!strcmp(argument, ">"))
		{
			if (get_mob_index(mob->vnum + 1) && can_olc_modify(ch, mob->vnum + 1))
			{
				stop_olc_editing(ch, mob->area);
				sprintf(buf, "%d", mob->vnum + 1);
				do_medit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Next index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcmp(argument, "<"))
		{
			if (get_mob_index(mob->vnum - 1) && can_olc_modify(ch, mob->vnum - 1))
			{
				stop_olc_editing(ch, mob->area);
				sprintf(buf, "%d", mob->vnum - 1);
				do_medit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Prev index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			ch_printf(ch, "%s  name%s:%s %s\n\r", colg, colw, colW, mob->player_name);
			ch_printf(ch, "%s short%s:%s %s\n\r", colg, colw, colW, mob->short_descr);
			ch_printf(ch, "%s  long%s:%s %s\n\r", colg, colw, colW, mob->long_descr);
			ch_printf(ch, "%s  desc%s:%s \n\r%s", colg, colw, colW, mob->description);
			ch_printf(ch, "%s    ed%s:%s %s\n\r", colg, colw, colW, get_ed(ch, mob->first_extradesc));
			ch_printf(ch, "%s   act%s:%s %s\n\r", colg, colw, colW, flag_string(mob->act, act_flags));
			ch_printf(ch, "%saffect%s:%s %s\n\r", colg, colw, colW, flag_string(mob->affected_by, a_flags));
			ch_printf(ch, "%s align%s:%s %d\n\r", colg, colw, colW, mob->alignment);
			ch_printf(ch, "%s level%s:%s %d\n\r", colg, colw, colW, mob->level);
			if (IS_SET(mob->act, ACT_BODY))
			{
				ch_printf(ch, "%s  body%s:%s %s\n\r", colg, colw, colW, flag_string(mob->body_parts, part_flags));
				ch_printf(ch, "%sattack%s:%s %s\n\r", colg, colw, colW, flag_string(mob->attack_parts, part_flags));
			}
			ch_printf(ch, "%s   hit%s:%s %d%sd%s%d%s+%s%d\n\r", colg, colw, colW, mob->hitnodice, colw, colW, mob->hitsizedice, colw, colW, mob->hitplus);
			ch_printf(ch, "%s   dam%s:%s %d%sd%s%d%s+%s%d\n\r", colg, colw, colW, mob->damnodice, colw, colW, mob->damsizedice, colw, colW, mob->damplus);
			ch_printf(ch, "%s  gold%s:%s %d\n\r", colg, colw, colW, mob->gold);
			if (IS_SET(mob->act, ACT_RACE))
			{
				ch_printf(ch, "%s  race%s:%s %s\n\r", colg, colw, colW, race_table[mob->race].race_name);
			}
			if (IS_SET(mob->act, ACT_CLASS))
			{
				ch_printf(ch, "%s class%s:%s %s\n\r", colg, colw, colW, class_types[mob->class]);
			}
			ch_printf(ch, "%s   pos%s:%s %s\n\r", colg, colw, colW, p_types[mob->position]);
			ch_printf(ch, "%s   sex%s:%s %s\n\r", colg, colw, colW, sex_types[mob->sex]);
			pop_call();
			return;
		}
	}
	else
	{
		if ((mob = get_mob_index(atoi(argument))) == NULL)
		{
			send_to_char("Syntax: edit mob <mobile vnum>\n\r", ch);
			pop_call();
			return;
		}
		else if (!can_olc_modify(ch, mob->vnum))
		{
			send_to_char("That vnum is not in your allocated range.\n\r", ch);
			pop_call();
			return;
		}

		ch->pcdata->edit_ptr = mob;
		ch->pcdata->edittype = EDIT_TYPE_MOB;

		ch_printf( ch, "[%d] Editing Mobile.\n\r", mob->vnum);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf( buf, "Editing Mobile Vnum: %d", mob->vnum );
			ch->pcdata->subprompt = STRALLOC( buf );
		}
		pop_call();
		return;
	}

	argument = one_argument(argument, arg2);
	strcpy(arg3, argument);

	value = is_number(argument) ? atoi(argument) : -1;

	if (!strcasecmp(arg2, "sex"))
	{
		if ((value = get_flag(arg3, sex_types)) == -1)
		{
			ch_printf(ch, "Syntax: sex <%s>\n\r", give_flags(sex_types));
			pop_call();
			return;
		}
		mob->sex = value;
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "race"))
	{
		if ((value = get_flag(arg3, r_types)) == -1)
		{
			sprintf(buf, "Syntax: race <%s>", give_flags(r_types));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
			pop_call();
			return;
		}
		mob->race = value;
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "class"))
	{
		if ((value = get_flag(arg3, class_types)) == -1)
		{
			sprintf(buf, "Syntax: class <%s>", give_flags(class_types));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
		}
		else
		{
			mob->class = value;
		}
		pop_call();
		return;
	}
	if ( !strcasecmp( arg2, "level" ) )
	{
		if ( value < 0 || value > 300 )
		{
			ch_printf( ch, "Level range is 0 to 300.\n\r");
			pop_call();
			return;
		}
		mob->level = value;

		mob->damnodice   = 1;
		mob->damsizedice = mob->level;
		mob->damplus     = 2 + mob->level * 0.005 * mob->level;

		mob->hitnodice   = 1;
		mob->hitsizedice = mob->level * mob->level / 4;
		mob->hitplus     = 10 + mob->level * mob->level * (0.40 + 0.0015 * mob->level);

		pop_call();
		return;
	}

	if ( !strcasecmp( arg2, "gold" ) )
	{
		if (value < 0 || value > 300000)
		{
			ch_printf(ch, "Gold range is 0 to 300000.\n\r");
			pop_call();
			return;
		}
		mob->gold = value;
		pop_call();
		return;
	}

	if ( !strcasecmp( arg2, "align" ) )
	{
		if ( value < -1000 || value > 1000 )
		{
			send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
			pop_call();
			return;
		}
		mob->alignment = value;
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "name"))
	{
		STRFREE(mob->player_name );
		mob->player_name = STRALLOC(lower_all(arg3));
		pop_call();
		return;
	}


	if (!strcasecmp(arg2, "short"))
	{
		STRFREE(mob->short_descr );
		mob->short_descr = STRALLOC(arg3);
		pop_call();
		return;
	}

	if (!strcasecmp( arg2, "long"))
	{
		STRFREE( mob->long_descr );
		mob->long_descr = STRALLOC(capitalize(arg3));
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "desc"))
	{
		ch->pcdata->editmode = MODE_MOB_DESC;
		ch->pcdata->tempmode  = MODE_REPEATCMD;
		start_editing(ch, mob->description);
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "ed"))
	{
		if (arg3[0] == '\0')
		{
			ch_printf(ch, "Syntax: ed <keywords>\n\r");
			pop_call();
			return;
		}
		ed            = set_ed_mob(mob, arg3);
		ch->pcdata->tempmode   = MODE_REPEATCMD;
		ch->pcdata->editmode  = MODE_MOB_EXTRA;
		ch->pcdata->temp_ptr = ed;

		start_editing(ch, ed->description);
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "rmed"))
	{
		if (!arg3 || arg3[0] == '\0')
		{
			ch_printf(ch, "Syntax: rmed <keyword>\n\r");
			pop_call();
			return;
		}

		if (!del_ed_mob(mob, argument))
		{
			ch_printf(ch, "Keyword: %s not found.\n\r", argument);
		}
		pop_call();
		return;
	}

/*
	if ( !strcasecmp( arg2, "spec" ) )
	{
		if ( !strcasecmp( arg3, "none" ) )
		{
			mob->spec_fun = 0;
			pop_call();
			return;
		}

		if (spec_lookup(arg3) == 0)
		{
			send_to_char( "No such spec fun.\n\r", ch );
			pop_call();
			return;
		}
		mob->spec_fun = spec_lookup(arg3);
		pop_call();
		return;
	}
*/

	if (!strcasecmp(arg2, "act"))
	{
		if (toggle_flags(&mob->act, arg3, act_flags) == FALSE)
		{
			sprintf(buf, "Syntax: act <%s>", give_flags(act_flags));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "affect"))
	{
		if (toggle_flags(&mob->affected_by, arg3, a_flags) == FALSE)
		{
			sprintf(buf, "Syntax: affect <%s>", give_flags(a_flags));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
		}
		pop_call();
		return;
	}

	if ( !strcasecmp( arg2, "pos" ) )
	{
		if ((value = get_flag(arg3, p_types)) == -1)
		{
			sprintf(buf, "Syntax: pos <%s>", give_flags(p_types));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
			pop_call();
			return;
		}
		mob->position = value;
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "body"))
	{
		if (toggle_flags(&mob->body_parts, arg3, part_flags) == FALSE)
		{
			sprintf(buf, "Syntax: body <%s>", give_flags(part_flags));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "attack"))
	{
		if (toggle_flags(&mob->attack_parts, arg3, part_flags) == FALSE)
		{
			sprintf(buf, "Syntax: attack <%s>", give_flags(part_flags));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "hit"))
	{
		value = sscanf(arg3, "%d %c %d %c %d", &num, &char1, &size, &char2, &plus);

		if (char1 != 'd' || char2 != '+' || value != 5)
		{
			ch_printf(ch, "Syntax: hit ?d?+?\n\r");
			pop_call();
			return;
		}

		if (num*size+plus < 1)
		{
			ch_printf(ch, "Max hitpoints dice must be greater than 1.\n\r");
			pop_call();
			return;
		}

		mob->hitnodice   = num;
		mob->hitsizedice = size;
		mob->hitplus     = plus;

		pop_call();
		return;
	}

	if (!strcasecmp(arg2, "dam"))
	{
		value = sscanf(arg3, "%d %c %d %c %d", &num, &char1, &size, &char2, &plus);

		if (char1 != 'd' || char2 != '+' || value != 5)
		{
			ch_printf(ch, "Syntax: dam ?d?+?\n\r");
			pop_call();
			return;
		}

		if (num*size+plus < 1 || num*size+plus > 1000)
		{
			ch_printf(ch, "Dam dice must be between 1 and 1000.\n\r");
			pop_call();
			return;
		}

		mob->damnodice   = num;
		mob->damsizedice = size;
		mob->damplus     = plus;

		pop_call();
		return;
	}

	/*
		Generate usage message.
	*/

	if ( ch->pcdata->editmode == MODE_REPEATCMD )
	{
		ch->pcdata->editmode = MODE_RESTRICTED;
		interpret( ch, origarg );
		ch->pcdata->editmode = MODE_REPEATCMD;
		ch->pcdata->last_cmd = do_edit;
	}
	else
	{
		do_medit( ch, "" );
	}
	pop_call();
	return;
}

void do_oedit( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];

	OBJ_INDEX_DATA *obj;
	EXTRA_DESCR_DATA *ed;
	AFFECT_DATA *aff;

	char *origarg = argument;

	int value;

	char colw[10], colg[10], colW[10], colM[10];

	push_call("do_oedit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));
	strcpy(colM, ansi_translate_text(ch, "{158}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor\n\r", ch );
		pop_call();
		return;
	}

	if (ch->pcdata->edit_ptr && (obj = ch->pcdata->edit_ptr)->area == NULL)
	{
		ch_printf(ch, "The object you were editing is no longer valid.\n\r");
		stop_olc_editing(ch, ch->in_room->area);
		pop_call();
		return;
	}

	switch(ch->pcdata->editmode)
	{
		case MODE_OBJ_DESC:
			obj = ch->pcdata->edit_ptr;

			STRFREE(obj->description);
			obj->description = copy_buffer(ch);
			stop_editing(ch);

			pop_call();
			return;

		case MODE_OBJ_EXTRA:
			ed  = ch->pcdata->temp_ptr;

			STRFREE( ed->description );
			ed->description = copy_buffer( ch );
			stop_editing( ch );

			pop_call();
			return;
	}

	smash_tilde( argument );

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		obj = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show oedit status
			*/
			ch_printf(ch, "Syntax: <command> <value>\n\r");
			ch_printf(ch, "Command being on of:\n\r");
			ch_printf(ch, "  type flag wear weight cost level ed rmed name short long desc done stat ? < >\n\r");
			switch (obj->item_type)
			{
				case ITEM_ARMOR:
					ch_printf(ch, "  affect armor\n\r");
					break;
				case ITEM_WEAPON:
					ch_printf(ch, "  affect weapon dice nounce dammsg\n\r");
					break;
				case ITEM_SCROLL:
				case ITEM_PILL:
				case ITEM_POTION:
				case ITEM_TOTEM:
					ch_printf(ch, "  affect spell\n\r");
					break;
				case ITEM_WAND:
				case ITEM_STAFF:
					ch_printf(ch, "  affect spell charge\n\r");
					break;
				case ITEM_CONTAINER:
					ch_printf(ch, "  affect lid carry key max\n\r");
					break;
				case ITEM_LIGHT:
					ch_printf(ch, "  affect light\n\r");
					break;
				case ITEM_FOUNTAIN:
					ch_printf(ch, "  affect liquid\n\r");
					break;
				case ITEM_DRINK_CON:
					ch_printf(ch, "  affect liquid charge poison\n\r");
					break;
				case ITEM_FOOD:
					ch_printf(ch, "  affect fill poison\n\r");
					break;
				case ITEM_MONEY:
					ch_printf(ch, "  affect\n\r");
					break;
				case ITEM_PORTAL:
					ch_printf(ch, "  affect charge enter toroom\n\r");
					break;
				case ITEM_FURNITURE:
					ch_printf(ch, "  affect size poses regen\n\r");
					break;
				case ITEM_LOCKPICK:
					ch_printf(ch, "  affect bonus charge pick\n\r");
					break;
				case ITEM_AMMO:
					ch_printf(ch, "  affect weapon damage speed range\n\r");
					break;
			}
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char( "Oedit mode off.\n\r", ch );

			stop_olc_editing(ch, obj->area);

			pop_call();
			return;
		}

		if (!strcmp(argument, ">"))
		{
			if (get_obj_index(obj->vnum + 1) && can_olc_modify(ch, obj->vnum + 1))
			{
				stop_olc_editing(ch, obj->area);
				sprintf(buf, "%d", obj->vnum + 1);
				do_oedit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Next index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcmp(argument, "<"))
		{
			if (get_obj_index(obj->vnum - 1) && can_olc_modify(ch, obj->vnum - 1))
			{
				stop_olc_editing(ch, obj->area);
				sprintf(buf, "%d", obj->vnum - 1);
				do_oedit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Prev index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			ch_printf(ch, "%s  name%s:%s %s\n\r", colg, colw, colW, obj->name);
			ch_printf(ch, "%s short%s:%s %s\n\r", colg, colw, colW, obj->short_descr);
			ch_printf(ch, "%s  long%s:%s %s\n\r", colg, colw, colW, obj->long_descr);
			ch_printf(ch, "%s  desc%s:%s \n\r%s", colg, colw, colW, obj->description);
			ch_printf(ch, "%s    ed%s:%s %s\n\r", colg, colw, colW, get_ed(ch, obj->first_extradesc));
			ch_printf(ch, "%s  type%s:%s %s\n\r", colg, colw, colW, type_string(obj->item_type, o_types));
			ch_printf(ch, "%s  flag%s:%s %s\n\r", colg, colw, colW, flag_string(obj->extra_flags, o_flags));
			ch_printf(ch, "%s  wear%s:%s %s\n\r", colg, colw, colW, flag_string(obj->wear_flags, w_flags));
			ch_printf(ch, "%sweight%s:%s %d\n\r", colg, colw, colW, obj->weight);
			ch_printf(ch, "%s  cost%s:%s %d\n\r", colg, colw, colW, obj->cost);
			ch_printf(ch, "%s level%s:%s %d %s(%d)\n\r", colg, colw, colW, obj->level, colM, obj_level_estimate(obj));
			ch_printf(ch, "%s class%s:%s %s\n\r", colg, colw, colW, flag_string(obj->class_flags, class_types));
			for (value = 0, aff = obj->first_affect ; aff ; aff = aff->next)
			{
				ch_printf(ch, "%saffect%s:%s %d%s %4d %s %s\n\r", colg, colw, colM, ++value, colW, aff->modifier, a_types[aff->location], aff->bitvector > 0 ? flag_string(aff->bitvector, oa_flags1) : flag_string(aff->bitvector * -1, oa_flags2));
			}
			if (value == 0)
			{
				ch_printf(ch, "%saffect%s:%s %d%s %s\n\r", colg, colw, colM, ++value, colW, "no affect set");
			}
			switch (obj->item_type)
			{
				case ITEM_ARMOR:
					ch_printf(ch, "%s armor%s:%s %d\n\r", colg, colw, colW, obj->value[0]);
					break;
				case ITEM_WEAPON:
					ch_printf(ch, "%sweapon%s:%s %s\n\r", colg, colw, colW, type_string(obj->value[0], weap_types));
					ch_printf(ch, "%s  dice%s:%s %d%sd%s%d\n\r", colg, colw, colW, obj->value[1], colw, colW, obj->value[2]);
					ch_printf(ch, "%snounce%s:%s %s\n\r", colg, colw, colW, type_string(obj->value[3], noun_types));
					ch_printf(ch, "%sdammsg%s:%s %s\n\r", colg, colw, colW, obj->attack_string);
					break;
				case ITEM_SCROLL:
				case ITEM_PILL:
				case ITEM_POTION:
					ch_printf(ch, "%s spell%s:%s %d %s%s,%s %s%s,%s %s\n\r", colg, colw, colW, obj->value[0],
						skill_table[URANGE(0, obj->value[1], MAX_SKILL-1)].name, colw, colW,
						skill_table[URANGE(0, obj->value[2], MAX_SKILL-1)].name, colw, colW,
						skill_table[URANGE(0, obj->value[3], MAX_SKILL-1)].name);
					break;
				case ITEM_WAND:
				case ITEM_STAFF:
					ch_printf(ch, "%s spell%s:%s %d %s\n\r", colg, colw, colW, obj->value[0], skill_table[URANGE(0, obj->value[3], MAX_SKILL-1)].name);
					ch_printf(ch, "%scharge%s:%s %d%s/%s%d\n\r", colg, colw, colW,
						obj->value[1], colw, colW, obj->value[2]);
					break;
				case ITEM_TOTEM:
					ch_printf(ch, "%s spell%s:%s %s%s,%s %s%s,%s %s%s,%s %s\n\r", colg, colw, colW,
						skill_table[URANGE(0, obj->value[0], MAX_SKILL-1)].name, colw, colW,
						skill_table[URANGE(0, obj->value[1], MAX_SKILL-1)].name, colw, colW,
						skill_table[URANGE(0, obj->value[2], MAX_SKILL-1)].name, colw, colW,
						skill_table[URANGE(0, obj->value[3], MAX_SKILL-1)].name);
					break;
				case ITEM_CONTAINER:
					ch_printf(ch, "%s carry%s:%s %d\n\r", colg, colw, colW, obj->value[0]);
					ch_printf(ch, "%s   lid%s:%s %s\n\r", colg, colw, colW, flag_string(obj->value[1], cont_flags));
					ch_printf(ch, "%s   key%s:%s %d\n\r", colg, colw, colW, obj->value[2]);
					ch_printf(ch, "%s   max%s;%s %d\n\r", colg, colw, colW, obj->value[3]);
					break;
				case ITEM_LIGHT:
					ch_printf(ch, "%s light%s:%s %d\n\r", colg, colw, colW, obj->value[2]);
					break;
				case ITEM_FOUNTAIN:
					ch_printf(ch, "%sliquid%s:%s %s\n\r", colg, colw, colW, type_string(obj->value[2], liq_types));
					break;
				case ITEM_DRINK_CON:
					ch_printf(ch, "%sliquid%s:%s %s\n\r", colg, colw, colW, type_string(obj->value[2], liq_types));
					ch_printf(ch, "%scharge%s:%s %d%s/%s%d\n\r", colg, colw, colW,
						obj->value[0], colw, colW, obj->value[1]);
					ch_printf(ch, "%spoison%s:%s %s\n\r", colg, colw, colW, type_string(obj->value[3], poison_types));
					break;
				case ITEM_FOOD:
					ch_printf(ch, "%s  fill%s:%s %d\n\r", colg, colw, colW, obj->value[0]);
					ch_printf(ch, "%spoison%s:%s %s\n\r", colg, colw, colW, type_string(obj->value[3], poison_types));
					break;
				case ITEM_PORTAL:
					ch_printf(ch, "%scharge%s:%s %d\n\r", colg, colw, colW, obj->value[0]);
					ch_printf(ch, "%s enter%s:%s %s\n\r", colg, colw, colW, flag_string(obj->value[2], portal_flags));
					ch_printf(ch, "%storoom%s:%s %d\n\r", colg, colw, colW, obj->value[3]);
					break;
				case ITEM_FURNITURE:
					ch_printf(ch, "%s  size%s:%s %d\n\r", colg, colw, colW, obj->value[0]);
					ch_printf(ch, "%s poses%s:%s %s\n\r", colg, colw, colW, flag_string(obj->value[2], f_flags));
					ch_printf(ch, "%s regen%s:%s %d\n\r", colg, colw, colW, obj->value[3]);
					break;
				case ITEM_LOCKPICK:
					ch_printf(ch, "%s bonus%s:%s %d\n\r", colg, colw, colW, obj->value[0]);
					ch_printf(ch, "%scharge%s:%s %d\n\r", colg, colw, colW, obj->value[1]);
					ch_printf(ch, "%s  pick%s:%s %s\n\r", colg, colw, colW, flag_string(obj->value[2], lp_flags));
					break;
				case ITEM_AMMO:
					ch_printf(ch, "%sweapon%s:%s %s\n\r", colg, colw, colW, type_string(obj->value[0], bow_types));
					ch_printf(ch, "%sdamage%s:%s %d\n\r", colg, colw, colW, obj->value[1]);
					ch_printf(ch, "%s speed%s:%s %d\n\r", colg, colw, colW, obj->value[2]);
					ch_printf(ch, "%s range%s:%s %d\n\r", colg, colw, colW, obj->value[3]);
					break;
			}
			pop_call();
			return;
		}
	}
	else if ((obj = get_obj_index(atoi(argument))) == NULL)
	{
		send_to_char("Syntax: edit obj <object vnum>\n\r", ch);
		pop_call();
		return;
	}
	else if (!can_olc_modify(ch, obj->vnum))
	{
		send_to_char("That vnum is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->pcdata->editmode != MODE_REPEATCMD)
	{
		ch->pcdata->edit_ptr = obj;
		ch->pcdata->edittype = EDIT_TYPE_OBJ;

		ch_printf( ch, "[%d] Editing Object.\n\r", obj->vnum);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf( buf, "Editing Object Vnum: %d", obj->vnum );
			ch->pcdata->subprompt = STRALLOC( buf );
		}
		pop_call();
		return;
	}

	argument = one_argument(argument, arg);

	value = is_number(argument) ? atoi(argument) : -1;

	if (!strcasecmp(arg, "name"))
	{
		cat_sprintf(argument, " i%d", obj->vnum);
		STRFREE(obj->name );
		obj->name = STRALLOC(lower_all(argument));
		pop_call();
		return;
	}


	if (!strcasecmp(arg, "short"))
	{
		STRFREE(obj->short_descr );
		obj->short_descr = STRALLOC(argument);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "long"))
	{
		STRFREE( obj->long_descr);
		obj->long_descr = STRALLOC(capitalize(argument));
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "desc"))
	{
		ch->pcdata->editmode = MODE_OBJ_DESC;
		ch->pcdata->tempmode  = MODE_REPEATCMD;
		start_editing(ch, obj->description);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "ed"))
	{
		if (argument[0] == '\0')
		{
			ch_printf(ch, "Syntax: ed <keywords>\n\r");
			pop_call();
			return;
		}
		ed            = set_ed_obj(obj, argument);
		ch->pcdata->tempmode   = MODE_REPEATCMD;
		ch->pcdata->editmode  = MODE_OBJ_EXTRA;
		ch->pcdata->temp_ptr = ed;

		start_editing(ch, ed->description);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "rmed"))
	{
		if (!argument || argument[0] == '\0')
		{
			ch_printf(ch, "Syntax: rmed <keyword>\n\r");
			pop_call();
			return;
		}

		if (!del_ed_obj(obj, argument))
		{
			ch_printf(ch, "Keyword: %s not found.\n\r", argument);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "type"))
	{
		if ((value = get_flag(argument, o_types)) == -1)
		{
			ch_printf(ch, "Syntax: type <%s>\n\r", give_flags(o_types));
			pop_call();
			return;
		}
		obj->item_type = value;

		/*
			Reset all object type related settings
		*/
		obj->value[0] = obj->value[1] = obj->value[2] = obj->value[3] = 0;
		RESTRING(obj->attack_string, "");
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "flag"))
	{
		if (toggle_flags(&obj->extra_flags, argument, o_flags) == FALSE)
		{
			ch_printf(ch, "Syntax: flag <%s>\n\r", give_flags(o_flags));
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "class"))
	{
		if (toggle_flags(&obj->class_flags, argument, class_types) == FALSE)
		{
			ch_printf(ch, "Syntax: class <%s>\n\r", give_flags(class_types));
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "wear"))
	{
		if (toggle_flags(&obj->wear_flags, argument, w_flags) == FALSE)
		{
			ch_printf(ch, "Syntax: wear <%s>\n\r", give_flags(w_flags));
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "weight"))
	{
		obj->weight = URANGE(1, value, 30000);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "level"))
	{
		obj->level = value;
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "cost"))
	{
		obj->cost = value;
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "affect"))
	{
		long long type;

		argument = one_argument(argument, arg);

		if (arg[0] == '\0' || atoi(arg) < 1 || atoi(arg) > 3)
		{
			ch_printf(ch, "Syntax: affect <1|2|3> <del|apply|mod|spell1|spell2> <argument>\n\r");
			pop_call();
			return;
		}
		value = atoi(arg);
		argument = one_argument(argument, arg);

		if (arg[0] == '\0')
		{
			ch_printf(ch, "Syntax: affect <%d> <apply|mod|spell1|spell2> <argument>\n\r", value);
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "del"))
		{
			set_obj_affect(obj, value, -1, -2, -1);
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "apply"))
		{
			if ((type = get_flag(argument, a_types)) == -1)
			{
				ch_printf(ch, "Syntax: affect <%d> apply <%s>\n\r", value, give_flags(a_types));
				pop_call();
				return;
			}
			set_obj_affect(obj, value, -1, type, -1);
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "spell1"))
		{
			if ((type = get_flag(argument, oa_flags1)) == -1)
			{
				ch_printf(ch, "Syntax: affect <%d> spell1 <%s>\n\r", value, give_flags(oa_flags1));
				pop_call();
				return;
			}
			set_obj_affect(obj, value, -1, -1, 1LL << type);
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "spell2"))
		{
			if ((type = get_flag(argument, oa_flags2)) == -1)
			{
				ch_printf(ch, "Syntax: affect <%d> spell1 <%s>\n\r", value, give_flags(oa_flags2));
				pop_call();
				return;
			}
			set_obj_affect(obj, value, -1, -1, (1LL << type) * -1);
			pop_call();
			return;
		}
		if (!strcasecmp(arg, "mod"))
		{
			set_obj_affect(obj, value, atol(argument), -1, -1);
			pop_call();
			return;
		}
		pop_call();
		return;
	}

	switch (obj->item_type)
	{
		case ITEM_ARMOR:
			if (!strcasecmp(arg, "armor"))
			{
				obj->value[0] = atoi(argument);
				pop_call();
				return;
			}
			break;
		case ITEM_WEAPON:
			if (!strcasecmp(arg, "weapon"))
			{
				if ((value = get_flag(argument, weap_types)) == -1)
				{
					ch_printf(ch, "Syntax: weapon <%s>\n\r", give_flags(weap_types));
				}
				else
				{
					obj->value[0] = value;
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "nounce"))
			{
				if ((value = get_flag(argument, noun_types)) == -1)
				{
					ch_printf(ch, "Syntax: nounce <%s>\n\r", give_flags(noun_types));
				}
				else
				{
					obj->value[3] = value;
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "dice"))
			{
				int num, size;
				char char1;

				value = sscanf(argument, "%d %c %d", &num, &char1, &size);

				if (char1 != 'd' || value != 3)
				{
					ch_printf(ch, "Syntax: dice ?d?\n\r");
				}
				else
				{
					obj->value[1] = num;
					obj->value[2] = size;
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "dammsg"))
			{
				RESTRING(obj->attack_string, argument);
				pop_call();
				return;
			}
			break;
		case ITEM_SCROLL:
		case ITEM_PILL:
		case ITEM_POTION:
			if (!strcasecmp(arg, "spell"))
			{
				if (argument[0] == '\0')
				{
					ch_printf(ch, "Syntax: spell <spell level> [spell1] [spell2] [spell3]\n\r");
					pop_call();
					return;
				}
				argument		= one_argument(argument, arg);
				obj->value[0]	= URANGE(1, atoi(arg), 100);
				argument		= one_argument(argument, arg);
				obj->value[1]	= UMAX(0, skill_lookup(arg));
				argument		= one_argument(argument, arg);
				obj->value[2]  = UMAX(0, skill_lookup(arg));
				argument		= one_argument(argument, arg);
				obj->value[3]  = UMAX(0, skill_lookup(arg));
				pop_call();
				return;
			}
			break;
		case ITEM_WAND:
		case ITEM_STAFF:
			if (!strcasecmp(arg, "spell"))
			{
				if (argument[0] == '\0')
				{
					ch_printf(ch, "Syntax: spell <spell level> [spell1]\n\r");
					pop_call();
					return;
				}
				argument		= one_argument(argument, arg);
				obj->value[0]	= URANGE(1, atoi(arg), 100);
				argument		= one_argument(argument, arg);
				obj->value[3]	= UMAX(0, skill_lookup(arg));
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "charge"))
			{
				int max, left;
				char char1;

				if (argument[0] == '\0')
				{
					ch_printf(ch, "Syntax: charge ?/?");
					pop_call();
					return;
				}

				value = sscanf(argument, "%d %c %d", &max, &char1, &left);

				if (char1 != '/' || value != 3)
				{
					ch_printf(ch, "Syntax: charge ?/?\n\r");
				}
				else
				{
					obj->value[1] = max;
					obj->value[2] = left;
				}
				pop_call();
				return;
			}
			break;
		case ITEM_TOTEM:
			if (!strcasecmp(arg, "spell"))
			{
				if (argument[0] == '\0')
				{
					ch_printf(ch, "Syntax: spell [spell1] [spell2] [spell3] [spell4]\n\r");
					pop_call();
					return;
				}
				argument		= one_argument(argument, arg);
				obj->value[0]	= UMAX(0, skill_lookup(arg));
				argument		= one_argument(argument, arg);
				obj->value[1]	= UMAX(0, skill_lookup(arg));
				argument		= one_argument(argument, arg);
				obj->value[2]  = UMAX(0, skill_lookup(arg));
				argument		= one_argument(argument, arg);
				obj->value[3]  = UMAX(0, skill_lookup(arg));
				pop_call();
				return;
			}
			break;
		case ITEM_CONTAINER:
			if (!strcasecmp(arg, "carry"))
			{
				obj->value[0] = URANGE(0, value, 2000);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "lid"))
			{
				if (toggle_flags(&obj->value[1], argument, cont_flags) == FALSE)
				{
					ch_printf(ch, "Syntax: lid <%s>\n\r", give_flags(cont_flags));
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "key"))
			{
				obj->value[2] = URANGE(-1, value, MAX_VNUM-1);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "max"))
			{
				obj->value[3] = URANGE(0, value, 150);
				pop_call();
				return;
			}
			break;
		case ITEM_LIGHT:
			if (!strcasecmp(arg, "light"))
			{
				obj->value[2] = URANGE(-1, value, 10000);
				pop_call();
				return;
			}
			break;
		case ITEM_FOUNTAIN:
			if (!strcasecmp(arg, "liquid"))
			{
				if ((value = get_flag(argument, liq_types)) == -1)
				{
					ch_printf(ch, "Syntax: liquid <%s>\n\r", give_flags(liq_types));
				}
				else
				{
					obj->value[2] = value;
				}
				pop_call();
				return;
			}
			break;
		case ITEM_DRINK_CON:
			if (!strcasecmp(arg, "liquid"))
			{
				if ((value = get_flag(argument, liq_types)) == -1)
				{
					ch_printf(ch, "Syntax: liquid <%s>\n\r", give_flags(liq_types));
				}
				else
				{
					obj->value[2] = value;
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "charge"))
			{
				int max, left;
				char char1;

				if (argument[0] == '\0')
				{
					ch_printf(ch, "Syntax: charge ?/?");
					pop_call();
					return;
				}

				value = sscanf(argument, "%d %c %d", &max, &char1, &left);

				if (char1 != '/' || value != 3)
				{
					ch_printf(ch, "Syntax: charge ?/?\n\r");
				}
				else
				{
					obj->value[0] = max;
					obj->value[1] = left;
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "poison"))
			{
				if ((value = get_flag(argument, poison_types)) == -1)
				{
					ch_printf(ch, "Syntax: poison <%s>\n\r", give_flags(poison_types));
				}
				else
				{
					obj->value[3] = value;
				}
				pop_call();
				return;
			}
			break;
		case ITEM_FOOD:
			if (!strcasecmp(arg, "fill"))
			{
				obj->value[0] = URANGE(-48, value, 48);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "poison"))
			{
				if ((value = get_flag(argument, poison_types)) == -1)
				{
					ch_printf(ch, "Syntax: poison <%s>\n\r", give_flags(poison_types));
				}
				else
				{
					obj->value[3] = value;
				}
				pop_call();
				return;
			}
			break;
		case ITEM_PORTAL:
			if (!strcasecmp(arg, "charge"))
			{
				obj->value[0] = URANGE(-1, value, 10000);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "enter"))
			{
				if (toggle_flags(&obj->value[2], argument, portal_flags) == FALSE)
				{
					ch_printf(ch, "Syntax: enter <%s>\n\r", give_flags(portal_flags));
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "toroom"))
			{
				if (!can_olc_modify(ch, value))
				{
					ch_printf(ch, "That vnum is not in your allocated range.\n\r");
				}
				else
				{
					obj->value[3] = value;
				}
				pop_call();
				return;
			}
			break;
		case ITEM_FURNITURE:
			if (!strcasecmp(arg, "size"))
			{
				obj->value[0] = URANGE(0, value, 100);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "poses"))
			{
				if (toggle_flags(&obj->value[2], argument, f_flags) == FALSE)
				{
					ch_printf(ch, "Syntax: poses <%s>\n\r", give_flags(f_flags));
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "regen"))
			{
				obj->value[3] = URANGE(0, value, 500);
				pop_call();
				return;
			}
			break;
		case ITEM_LOCKPICK:
			if (!strcasecmp(arg, "bonus"))
			{
				obj->value[0] = URANGE(0, value, 100);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "charge"))
			{
				obj->value[1] = URANGE(0, value, 1000);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "pick"))
			{
				if (toggle_flags(&obj->value[2], argument, lp_flags) == FALSE)
				{
					ch_printf(ch, "Syntax: pick <%s>\n\r", give_flags(lp_flags));
				}
				pop_call();
				return;
			}
			break;
		case ITEM_AMMO:
			if (!strcasecmp(arg, "weapon"))
			{
				if ((value = get_flag(argument, bow_types)) == -1)
				{
					ch_printf(ch, "Syntax: weapon <%s>\n\r", give_flags(bow_types));
				}
				else
				{
					obj->value[0] = value;
				}
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "damage"))
			{
				obj->value[1] = URANGE(1, value, 1000);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "speed"))
			{
				obj->value[2] = URANGE(1, value, 20);
				pop_call();
				return;
			}
			if (!strcasecmp(arg, "range"))
			{
				obj->value[3] = URANGE(1, value, 5);
				pop_call();
				return;
			}
	}

	ch->pcdata->editmode = MODE_RESTRICTED;
	interpret( ch, origarg );
	ch->pcdata->editmode = MODE_REPEATCMD;
	ch->pcdata->last_cmd = do_edit;

	pop_call();
	return;
}

void do_redit( CHAR_DATA *ch, char *argument )
{
	char arg [MAX_INPUT_LENGTH];
	char buf [MAX_STRING_LENGTH];

	ROOM_INDEX_DATA	*room;
	EXTRA_DESCR_DATA	*ed;
	EXIT_DATA           *exit;
	int				door, value;
	char				*origarg = argument;

	char colw[10], colg[10], colW[10], colM[10];

	push_call("do_redit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));
	strcpy(colM, ansi_translate_text(ch, "{158}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor.\n\r", ch );
		pop_call();
		return;
	}

	if (ch->pcdata->edit_ptr && (room = ch->pcdata->edit_ptr)->area == NULL)
	{
		ch_printf(ch, "The room you were editing is no longer valid.\n\r");
		stop_olc_editing(ch, ch->in_room->area);
		pop_call();
		return;
	}

	switch(ch->pcdata->editmode)
	{
		default:
			break;
		case MODE_ROOM_DESC:
			room = ch->pcdata->edit_ptr;

			STRFREE(room->description );
			room->description = copy_buffer(ch);
			stop_editing( ch );

			pop_call();
			return;
		case MODE_ROOM_EXTRA:
			ed = ch->pcdata->temp_ptr;
			STRFREE(ed->description);

			ed->description = copy_buffer(ch);
			stop_editing(ch);

			pop_call();
			return;
	}

	smash_tilde( argument );

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		room = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show redit status
			*/
			ch_printf(ch, "\n\r");
			ch_printf(ch, "%sSyntax%s: %s<%scommand%s> <%svalue%s>\n\r", colw, colW, colw, colg, colw, colW, colw);
			ch_printf(ch, "%s  flag sect name desc ed rmed done stat ? < >\n\r", colg);
			ch_printf(ch, "\n\r");
			ch_printf(ch, "%sSyntax%s: %s<%sdir%s> <%scommand%s> <%svalue%s>\n\r", colw, colW, colw, colM, colw, colg, colw, colW, colw);
			ch_printf(ch, "%s  link desc name flag\n\r", colg);
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char("Redit mode off.\n\r", ch);

			stop_olc_editing(ch, room->area);

			pop_call();
			return;
		}

		if (!strcmp(argument, ">"))
		{
			if (get_room_index(room->vnum + 1) && can_olc_modify(ch, room->vnum + 1))
			{
				stop_olc_editing(ch, room->area);
				sprintf(buf, "%d", room->vnum + 1);
				do_redit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Next index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcmp(argument, "<"))
		{
			if (get_room_index(room->vnum - 1) && can_olc_modify(ch, room->vnum - 1))
			{
				stop_olc_editing(ch, room->area);
				sprintf(buf, "%d", room->vnum - 1);
				do_redit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Prev index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			ch_printf(ch, "\n\r");
			ch_printf(ch, "%s  name%s:%s %s\n\r", colg, colw, colW, room->name);
			ch_printf(ch, "%s  desc%s:%s \n\r%s", colg, colw, colW, room->description);
			ch_printf(ch, "%s  flag%s:%s %s\n\r", colg, colw, colW, flag_string(room->room_flags, r_flags));
			ch_printf(ch, "%s  sect%s:%s %s\n\r", colg, colw, colW, sect_types[room->sector_type]);
			ch_printf(ch, "%s    ed%s:%s %s\n\r", colg, colw, colW, get_ed(ch, room->first_extradesc));
			pop_call();
			return;
		}
	}
	else if (argument[0] == '\0')
	{
		if (!can_olc_modify(ch, ch->in_room->vnum))
		{
			send_to_char("This room is not in your allocated range.\n\r", ch);
			pop_call();
			return;
		}
		room = ch->in_room;
	}
	else if ((room = get_room_index(atoi(argument))) == NULL)
	{
		send_to_char("Syntax: edit room <room vnum>\n\r", ch);
		pop_call();
		return;
	}
	else if (!can_olc_modify(ch, room->vnum))
	{
		send_to_char("That vnum is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->pcdata->editmode != MODE_REPEATCMD)
	{
		ch->pcdata->edit_ptr = room;
		ch->pcdata->edittype = EDIT_TYPE_ROOM;

		ch_printf( ch, "[%d] Editing Room.\n\r", room->vnum);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf( buf, "Editing Room Vnum: %d", room->vnum );
			ch->pcdata->subprompt = STRALLOC( buf );
		}
		pop_call();
		return;
	}

	argument = one_argument(argument, arg);

	value = is_number(argument) ? atoi(argument) : -1;

	if (!strcasecmp(arg, "name"))
	{
		STRFREE(room->name);
		room->name = STRALLOC(capitalize(argument));
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "desc"))
	{
		ch->pcdata->tempmode  = MODE_REPEATCMD;
		ch->pcdata->editmode = MODE_ROOM_DESC;
		start_editing( ch, room->description );
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "ed"))
	{
		if (argument[0] == '\0')
		{
			ch_printf(ch, "Syntax: ed <keywords>\n\r");
			pop_call();
			return;
		}
		ed            = set_ed_room(room, argument);
		ch->pcdata->tempmode   = MODE_REPEATCMD;
		ch->pcdata->editmode  = MODE_ROOM_EXTRA;
		ch->pcdata->temp_ptr = ed;

		start_editing(ch, ed->description);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "rmed"))
	{
		if (!argument || argument[0] == '\0')
		{
			ch_printf(ch, "Syntax: rmed <keyword>\n\r");
			pop_call();
			return;
		}

		if (!del_ed_room(room, argument))
		{
			ch_printf(ch, "Keyword: %s not found.\n\r", argument);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "flag"))
	{
		if (toggle_flags(&room->room_flags, argument, r_flags) == FALSE)
		{
			sprintf(buf, "Syntax: flag <%s>", give_flags(r_flags));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "sect"))
	{
		if ((value = get_flag(argument, sect_types)) == -1)
		{
			sprintf(buf, "Syntax: sect <%s>", give_flags(sect_types));
			ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
			pop_call();
			return;
		}
		room->sector_type = value;
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "n") || !strcasecmp(arg, "north")
	||  !strcasecmp(arg, "e") || !strcasecmp(arg,  "east")
	||  !strcasecmp(arg, "s") || !strcasecmp(arg, "south")
	||  !strcasecmp(arg, "w") || !strcasecmp(arg,  "west")
	||  !strcasecmp(arg, "u") || !strcasecmp(arg,  "down")
	||  !strcasecmp(arg, "d") || !strcasecmp(arg,    "up"))
	{
		door = direction_door(arg);

		if (argument[0] == '\0')
		{
			if ((exit = room->exit[door]) != NULL)
			{
				ch_printf(ch, "%s%6s%s:%s %d\n\r", colM, dir_name[door], colw, colW, exit->vnum);
				ch_printf(ch, "%s%6s%s:%s %s\n\r", colg, "desc", colw, colW, exit->description);
				ch_printf(ch, "%s%6s%s:%s %s\n\r", colg, "name", colw, colW, exit->keyword);
				ch_printf(ch, "%s%6s%s:%s %s\n\r", colg, "flag", colw, colW, flag_string(exit->exit_info, exit_flags));
				ch_printf(ch, "%s%6s%s:%s %d\n\r", colg,  "key", colw, colW, exit->key);
			}
			else
			{
				ch_printf(ch, "There is no connection %s.\n\r", dir_name[door]);
			}
			pop_call();
			return;
		}

		argument = one_argument(argument, arg);

		if (!strcasecmp(arg, "link"))
		{
			sprintf(buf, "%d %s", door, argument);
			do_link(ch, room, buf);
			pop_call();
			return;
		}

		if ((exit = room->exit[door]) == NULL)
		{
			ch_printf(ch, "There is no exit in that direction.\n\r");
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "desc"))
		{
			STRFREE(exit->description);
			exit->description = STRALLOC(capitalize(argument));
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "name"))
		{
			STRFREE(exit->keyword);
			exit->keyword = STRALLOC(argument);
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "flag"))
		{
			if (toggle_flags(&exit->exit_info, argument, exit_flags) == FALSE)
			{
				sprintf(buf, "Syntax: %s flag <%s>", dir_name[door], give_flags(exit_flags));
				ch_printf(ch, "%s\n\r", justify(buf, get_page_width(ch)));
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "key"))
		{
			exit->key = atoi(argument);
			pop_call();
			return;
		}		

		ch_printf(ch, "Syntax: <dir> <link|desc|name|flag> <argument>\n\r");
		pop_call();
		return;
	}

	/*
		Generate usage message.
	*/

	ch->pcdata->editmode = MODE_RESTRICTED;
	interpret( ch, origarg );
	ch->pcdata->editmode = MODE_REPEATCMD;
	ch->pcdata->last_cmd = do_edit;

	pop_call();
	return;
}

void do_hedit( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	HELP_DATA *help;
	HELP_MENU_DATA *menu;

	char *origarg = argument;

	char colg[10], colw[10], colW[10], colM[10];

	push_call("do_hedit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));
	strcpy(colM, ansi_translate_text(ch, "{158}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor.\n\r", ch );
		pop_call();
		return;
	}

	if (ch->pcdata->edit_ptr && (help = ch->pcdata->edit_ptr)->area == NULL)
	{
		ch_printf(ch, "The help file you were editing is no longer valid.\n\r");
		stop_olc_editing(ch, ch->in_room->area);
		pop_call();
		return;
	}

	switch( ch->pcdata->editmode )
	{
		case MODE_HELP_EDIT:
			help = ch->pcdata->edit_ptr;

			STRFREE(help->text);
			help->text = copy_buffer(ch);
			stop_editing( ch );

			pop_call();
			return;
	}

	smash_tilde(argument);

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		help = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show hedit status
			*/
			ch_printf(ch, "Syntax: <command> <value>\n\r\n\r");
			ch_printf(ch, "Command being one of:\n\r");
			ch_printf(ch, "  name desc level menu\n\r");
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char( "Hedit mode off.\n\r", ch );

			stop_olc_editing(ch, help->area);
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			ch_printf(ch, "%s  name%s:%s %s\n\r", colg, colw, colW, help->keyword);
			ch_printf(ch, "%s level%s:%s %d\n\r", colg, colw, colW, help->level);
			ch_printf(ch, "%s  desc%s:%s   \n\r", colg, colw, colW);
			for (menu = help->first_menu ; menu ; menu = menu->next)
			{
				ch_printf(ch, "%s  menu%s:%s %c %s%s\n\r", colg, colw, colM, menu->option, colW, menu->help->keyword);
			}
			pop_call();
			return;
		}
	}
	else if ((help = get_help(ch, argument)) == NULL)
	{
		send_to_char("Syntax: edit help <help keyword>\n\r", ch);
		pop_call();
		return;
	}
	else if (!can_olc_modify(ch, help->area->olc_range_lo))
	{
		send_to_char("That help file's area is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->pcdata->editmode != MODE_REPEATCMD)
	{
		ch->pcdata->edit_ptr = help;
		ch->pcdata->edittype = EDIT_TYPE_HELP;
		ch_printf(ch, "Editing Help: %s\n\r", help->keyword);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf(arg, "Editing Help: %s", help->keyword);
			ch->pcdata->subprompt = STRALLOC( arg );
		}
		pop_call();
		return;
	}

	argument = one_argument(argument, arg);

	if (!strcasecmp(arg, "desc"))
	{
		ch->pcdata->editmode = MODE_HELP_EDIT;
		ch->pcdata->tempmode  = MODE_REPEATCMD;
		start_editing(ch, help->text);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "level"))
	{
		help->level = URANGE(0, atoi(argument), MAX_LEVEL);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "name"))
	{
		if (argument[0] == '\0')
		{
			ch_printf(ch, "Syntax: name <help keywords>\n\r");
			pop_call();
			return;
		}
		STRFREE(help->keyword);
		help->keyword = STRALLOC(capitalize_all(argument));
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "menu"))
	{
		if (argument[0] == '\0')
		{
			ch_printf(ch, "Syntax: menu <option> [keyword]\n\r");
			pop_call();
			return;
		}
		argument = one_argument(argument, arg);

		for (menu = help->first_menu ; menu ; menu = menu->next)
		{
			if (menu->option == arg[0])
			{
				break;
			}
		}
		if (menu)
		{
			if (argument[0] == '\0')
			{
				ch_printf(ch, "menu %c removed.\n\r", menu->option);
				UNLINK(menu, help->first_menu, help->last_menu, next, prev);
			}
			else
			{
				if (get_help(ch, argument))
				{
					menu->help = get_help(ch, argument);
				}
				else
				{
					ch_printf(ch, "help file: %s not found.\n\r", argument);
				}
			}
			pop_call();
			return;
		}
		else
		{
			if (argument[0] == '\0')
			{
				ch_printf(ch, "Syntax: menu <option> <help file>\n\r");
			}
			else
			{
				if (get_help(ch, argument))
				{
					ALLOCMEM(menu, HELP_MENU_DATA, 1);
					menu->option = arg[0];
					menu->help   = get_help(ch, argument);
					LINK(menu, help->first_menu, help->last_menu, next, prev);
				}
				else
				{
					ch_printf(ch, "help file: %s not found.\n\r", argument);
				}
			}
		}
		pop_call();
		return;
	}

	ch->pcdata->editmode = MODE_RESTRICTED;
	interpret(ch, origarg);
	ch->pcdata->editmode = MODE_REPEATCMD;
	ch->pcdata->last_cmd = do_edit;
	pop_call();
	return;
}

void do_sedit( CHAR_DATA *ch, char *argument )
{
	char arg [MAX_INPUT_LENGTH];
	int  iTrade;

	MOB_INDEX_DATA *mob;
	SHOP_DATA		*shop;

	char *origarg = argument;

	char colw[10], colg[10], colW[10], colM[10];

	push_call("do_sedit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));
	strcpy(colM, ansi_translate_text(ch, "{158}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor\n\r", ch );
		pop_call();
		return;
	}

	smash_tilde( argument );

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		shop = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show medit status
			*/
			ch_printf(ch, "Syntax: <command> <value>\n\r\n\r");
			ch_printf(ch, "Command being one of:\n\r");
			ch_printf(ch, "  open close buy sell trade done ?\n\r");
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char( "Sedit mode off.\n\r", ch );

			stop_olc_editing(ch, get_mob_index(shop->keeper)->area);
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			ch_printf(ch, "%s  open%s:%s %2d\n\r", colg, colw, colW, shop->open_hour);
			ch_printf(ch, "%s close%s:%s %2d\n\r", colg, colw, colW, shop->close_hour);
			ch_printf(ch, "%s   buy%s:%s %2d\n\r", colg, colw, colW, shop->profit_buy);
			ch_printf(ch, "%s  sell%s:%s %2d\n\r", colg, colw, colW, shop->profit_sell);

			for (iTrade = 0 ; iTrade < MAX_TRADE ; iTrade++)
			{
				ch_printf(ch, "%s trade%s:%s %d %s%s\n\r", colg, colw, colM, iTrade+1, colW, type_string(shop->buy_type[iTrade], o_types));
			}
			pop_call();
			return;
		}
	}
	else
	{
		if ((mob = get_mob_index(atoi(argument))) == NULL)
		{
			send_to_char("Syntax: edit shop <mobile vnum>\n\r", ch);
			pop_call();
			return;
		}
		else if (!can_olc_modify(ch, mob->vnum))
		{
			send_to_char("That vnum is not in your allocated range.\n\r", ch);
			pop_call();
			return;
		}
		else if (mob->pShop == NULL)
		{
			ch_printf(ch, "Mobile %d is not a shop keeper.\n\r", mob->vnum);
			pop_call();
			return;
		}

		ch->pcdata->edit_ptr = mob->pShop;
		ch->pcdata->edittype = EDIT_TYPE_SHOP;

		ch_printf( ch, "[%d] Editing Shop.\n\r", mob->vnum);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf( arg, "Editing Shop Vnum: %d", mob->vnum );
			ch->pcdata->subprompt = STRALLOC( arg );
		}
		pop_call();
		return;
	}

	shop		= ch->pcdata->edit_ptr;
	argument	= one_argument(argument, arg);

	if (!strcasecmp(arg, "open"))
	{
		if (!is_number(argument))
		{
			ch_printf(ch, "Syntax: open <0-23>\n\r");
		}
		else
		{
			shop->open_hour = URANGE(0, atoi(argument), 23);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "close"))
	{
		if (!is_number(argument))
		{
			ch_printf(ch, "Syntax: close <0-23>\n\r");
		}
		else
		{
			shop->close_hour = URANGE(0, atoi(argument), 23);
		}
		pop_call();
		return;
	}


	if (!strcasecmp(arg, "buy"))
	{
		if (!is_number(argument))
		{
			ch_printf(ch, "Syntax: buy <100-200>\n\r");
		}
		else
		{
			shop->profit_buy = URANGE(100, atoi(argument), 200);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "sell"))
	{
		if (!is_number(argument))
		{
			ch_printf(ch, "Syntax: sell <1-50>\n\r");
		}
		else
		{
			shop->profit_sell = URANGE(1, atoi(argument), 50);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "trade"))
	{
		argument = one_argument(argument, arg);

		if (!is_number(arg) || atoi(arg) < 1 || atoi(arg) > 5)
		{
			ch_printf(ch, "Syntax: trade <1-5> <item type>\n\r");
			pop_call();
			return;
		}
		if (get_flag(argument, o_types) == -1)
		{
			sprintf(arg, "Syntax: trade <index> <%s>", give_flags(o_types));
			ch_printf(ch, "%s\n\r", justify(arg, get_page_width(ch)));
			pop_call();
			return;
		}
		shop->buy_type[atoi(arg)-1] = get_flag(argument, o_types);
		pop_call();
		return;
	}

	if ( ch->pcdata->editmode == MODE_REPEATCMD )
	{
		ch->pcdata->editmode = MODE_RESTRICTED;
		interpret( ch, origarg );
		ch->pcdata->editmode = MODE_REPEATCMD;
		ch->pcdata->last_cmd = do_edit;
	}
	else
	{
		do_medit( ch, "" );
	}
	pop_call();
	return;
}

void do_aedit( CHAR_DATA *ch, char *argument )
{
	AREA_DATA *area;
	char arg[MAX_INPUT_LENGTH];
	char *origarg = argument;

	char colw[10], colg[10], colW[10], colM[10];

	push_call("do_aedit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));
	strcpy(colM, ansi_translate_text(ch, "{158}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor\n\r", ch );
		pop_call();
		return;
	}

	area = NULL;
	smash_tilde( argument );

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		area = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show medit status
			*/
			ch_printf(ch, "Syntax: <command> <value>\n\r");
			ch_printf(ch, "Command being one of:\n\r");
			ch_printf(ch, "  name author popmsg flag softlo softhi hardlo hardhi done ?\n\r");
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char( "Aedit mode off.\n\r", ch );

			stop_olc_editing(ch, area);

			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			ch_printf(ch, "%s  name%s:%s %s\n\r", colg, colw, colW, area->name);
			ch_printf(ch, "%sauthor%s:%s %s\n\r", colg, colw, colW, area->authors);
			ch_printf(ch, "%spopmsg%s:%s %s\n\r", colg, colw, colW, area->resetmsg);
			ch_printf(ch, "%s  flag%s:%s %s\n\r", colg, colw, colW, flag_string(area->flags, area_flags));
			ch_printf(ch, "%ssoftlo%s:%s %d\n\r", colg, colw, colW, area->low_soft_range);
			ch_printf(ch, "%ssofthi%s:%s %d\n\r", colg, colw, colW, area->hi_soft_range);
			ch_printf(ch, "%shardlo%s:%s %d\n\r", colg, colw, colW, area->low_hard_range);
			ch_printf(ch, "%shardhi%s:%s %d\n\r", colg, colw, colW, area->hi_hard_range);
			if (IS_SET(area->flags, AFLAG_WEATHER))
			{
				ch_printf(ch, "%s  wind%s:%s %d\n\r", colg, colw, colW, area->weather_info->wind_scale);
				ch_printf(ch, "%s  rain%s:%s %d\n\r", colg, colw, colW, area->weather_info->wet_scale);
				ch_printf(ch, "%swinter%s:%s %d\n\r", colg, colw, colW, area->weather_info->temp_winter);
				ch_printf(ch, "%ssummer%s:%s %d\n\r", colg, colw, colW, area->weather_info->temp_summer);
				ch_printf(ch, "%s daily%s:%s %d\n\r", colg, colw, colW, area->weather_info->temp_daily);
			}
			if (IS_GOD(ch))
			{
				ch_printf(ch, "%s olclo%s:%s %d\n\r", colg, colw, colW, area->olc_range_lo);
				ch_printf(ch, "%s olchi%s:%s %d\n\r", colg, colw, colW, area->olc_range_hi);
			}
			pop_call();
			return;
		}
	}
	else
	{
		if (argument[0] == '\0')
		{
			area = ch->in_room->area;
		}
		else
		{
			area = lookup_area(argument);
		}

		if (area  == NULL)
		{
			ch_printf(ch, "Area: %s not found.\n\r", argument);
			pop_call();
			return;
		}
		else
		{
			if (!can_olc_modify(ch, area->olc_range_lo))
			{
				send_to_char("This area is not in your allocated vnum range.\n\r", ch);
				pop_call();
				return;
			}
		}
	}

	if (ch->pcdata->editmode != MODE_REPEATCMD)
	{
		ch->pcdata->edittype = EDIT_TYPE_AREA;
		ch->pcdata->edit_ptr = area;

		ch_printf( ch, "Editing Area: %s.\n\r", area->name);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf( arg, "Editing Area: %s", area->name);
			ch->pcdata->subprompt = STRALLOC( arg );
		}
		pop_call();
		return;
	}

	area		= ch->pcdata->edit_ptr;
	argument	= one_argument(argument, arg);

	if (!strcasecmp(arg, "name"))
	{
		RESTRING(area->name, argument);
	}
	else if (!strcasecmp(arg, "author"))
	{
		RESTRING(area->authors, argument);
	}
	else if (!strcasecmp(arg, "popmsg"))
	{
		RESTRING(area->resetmsg, argument);
	}
	else if (!strcasecmp(arg, "flag"))
	{
		if (toggle_flags(&area->flags, argument, area_flags) == FALSE)
		{
			ch_printf(ch, "Syntax: flag <%s>\n\r", give_flags(area_flags));
		}
	}
	else if (!strcasecmp(arg, "softlo"))
	{
		area->low_soft_range = URANGE(1, atoi(argument), 95);
	}
	else if (!strcasecmp(arg, "softhi"))
	{
		area->hi_soft_range = URANGE(1, atoi(argument), 95);
	}
	else if (!strcasecmp(arg, "hardlo"))
	{
		if (IS_GOD(ch))
		{
			area->low_hard_range = URANGE(1, atoi(argument), 100);
		}
		else
		{
			area->low_hard_range = URANGE(1, atoi(argument), 95);
		}
	}
	else if (!strcasecmp(arg, "hardhi"))
	{
		if (IS_GOD(ch))
		{
			area->hi_hard_range = URANGE(1, atoi(argument), 100);
		}
		else
		{
			area->hi_hard_range = URANGE(1, atoi(argument), 95);
		}
	}
	else if (!strcasecmp(arg, "wind"))
	{
		area->weather_info->wind_scale = URANGE(0, atoi(argument), 10);
	}
	else if (!strcasecmp(arg, "rain"))
	{
		area->weather_info->wet_scale = URANGE(0, atoi(argument), 10);
	}
	else if (!strcasecmp(arg, "winter"))
	{
		area->weather_info->temp_winter = URANGE(-100, atoi(argument), 100);
	}
	else if (!strcasecmp(arg, "summer"))
	{
		area->weather_info->temp_summer = URANGE(-100, atoi(argument), 100);
	}
	else if (!strcasecmp(arg, "daily"))
	{
		area->weather_info->temp_daily = URANGE(0, atoi(argument), 50);
	}
	else if (IS_GOD(ch) && !strcasecmp(arg, "olclo"))
	{
		area->olc_range_lo = URANGE(1, atoi(argument), MAX_VNUM-1);
	}
	else if (IS_GOD(ch) && !strcasecmp(arg, "olchi"))
	{
		area->olc_range_hi = URANGE(0, atoi(argument), MAX_VNUM-1);
	}
	else if (IS_GOD(ch) && !strcasecmp(arg, "setfilename"))
	{
		RESTRING(area->filename, argument);
	}
	else
	{
		ch->pcdata->editmode = MODE_RESTRICTED;
		interpret( ch, origarg );
		ch->pcdata->editmode = MODE_REPEATCMD;
		ch->pcdata->last_cmd = do_edit;
	}
	pop_call();
	return;
}


void do_opedit( CHAR_DATA *ch, char *argument )
{
	char arg [MAX_INPUT_LENGTH];
	char buf [MAX_INPUT_LENGTH];
	int  value, cnt, type;

	OBJ_INDEX_DATA *obj;
	OBJ_PROG		*prog,  *prog2;

	char *origarg = argument;

	char colw[10], colg[10], colW[10], colM[10];

	push_call("do_opedit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));
	strcpy(colM, ansi_translate_text(ch, "{158}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor\n\r", ch );
		pop_call();
		return;
	}

	obj = NULL;
	smash_tilde( argument );

	switch( ch->pcdata->editmode )
	{
		case MODE_OPROG_EDIT:
			if (ch->pcdata->edit_ptr && ch->pcdata->temp_ptr)
			{
				obj			= ch->pcdata->edit_ptr;
				prog			= ch->pcdata->temp_ptr;
				STRFREE(prog->argument);
				prog->argument	= copy_buffer(ch);
				stop_editing( ch );
			}
			else
			{
				log_printf("do_opedit: null edit_ptr or temp_ptr");
				ch->pcdata->tempmode = MODE_NONE;
				stop_editing(ch);
			}
			pop_call();
			return;
	}

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		obj = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show opedit status
			*/
			ch_printf(ch, "Syntax: <command> <value>\n\r\n\r");
			ch_printf(ch, "Command being one of:\n\r");
			ch_printf(ch, "  add del set edit done stat ? < >\n\r");
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char( "OPedit mode off.\n\r", ch );

			obj_prog_if_dest(obj);
			stop_olc_editing(ch, obj->area);

			pop_call();
			return;
		}

		if (!strcmp(argument, ">"))
		{
			if (get_obj_index(obj->vnum + 1) && can_olc_modify(ch, obj->vnum + 1))
			{
				obj_prog_if_dest(obj);
				stop_olc_editing(ch, obj->area);
				sprintf(buf, "%d", obj->vnum + 1);
				do_opedit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Next index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcmp(argument, "<"))
		{
			if (get_obj_index(obj->vnum - 1) && can_olc_modify(ch, obj->vnum - 1))
			{
				obj_prog_if_dest(obj);
				stop_olc_editing(ch, obj->area);
				sprintf(buf, "%d", obj->vnum - 1);
				do_opedit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Prev index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			for (cnt = 0, prog = obj->first_prog ; prog ; prog = prog->next)
			{
				cnt++;
				buf[0] = '\0';

				cat_sprintf(buf, "{178}%2d{078}] ", cnt);
				cat_sprintf(buf, "{078}[{128}%2d{078}]", prog->index);

				switch(prog->trigger)
				{
					case TRIG_VOID:
						cat_sprintf(buf, "[{178}----{078}] ");
						break;
					default:
						cat_sprintf(buf, "[{178}%3d%%{078}] ", prog->percentage);
						break;
				}

				cat_sprintf(buf, "{138}%-7s ", flag_string(prog->trigger, oprog_flags));

				switch (prog->trigger)
				{
					case TRIG_COMMAND:
					case TRIG_ROOM_COMMAND:
					case TRIG_WEAR_COMMAND:
						cat_sprintf(buf, "{178}%-8s", cmd_table[prog->cmd].name);
						break;
					case TRIG_UNKNOWN:
					case TRIG_ROOM_UNKNOWN:
					case TRIG_WEAR_UNKNOWN:
						cat_sprintf(buf, "{178}%-8s", prog->unknown);
						break;
					default:
						cat_sprintf(buf, "{178}%-8s", "");
						break;
				}
				cat_sprintf(buf, "{158} %-11s{078}", type_string(prog->obj_command, oprog_commands));

				switch (prog->obj_command)
				{
					case OPROG_QUEST_SET:
					case OPROG_QUEST_ADD:
						cat_sprintf(buf, " %d %d %d", prog->quest_offset, prog->quest_bits, prog->if_value);
						break;
					case OPROG_PLAYER_QUEST_IF:
					case OPROG_OBJECT_QUEST_IF:
						cat_sprintf(buf, " [{128}%2d {118}%2d{078}] %d %d %c %d", prog->if_true, prog->if_false, prog->quest_offset, prog->quest_bits, prog->if_symbol, prog->if_value);
						break;
					case OPROG_IF_HAS_OBJECT:
						cat_sprintf(buf, " [{128}%2d {118}%2d{078}] %d",  prog->if_true, prog->if_false, prog->if_value);
						break;
					case OPROG_APPLY:
						cat_sprintf(buf, "         %-4s %d", type_string(prog->if_check, oprog_applies), prog->if_value);
						break;
					case OPROG_IF:				
						cat_sprintf(buf, " [{128}%2d {118}%2d{078}] %s %c", prog->if_true, prog->if_false, type_string(prog->if_check, oprog_ifchecks), prog->if_symbol); 
						switch (prog->if_check)
						{
							case OIF_WEAR_LOC:
								cat_sprintf(buf, " %s", type_string(prog->if_value+1, oprog_wlocs));
								break;
							case OIF_USER_SECTOR:
								cat_sprintf(buf, " %s", type_string(prog->if_value, sect_types));
								break;
							case OIF_USER_CLASS:
								cat_sprintf(buf, " %s", type_string(prog->if_value, class_types));
								break;
							case OIF_USER_WHICH_GOD:
								cat_sprintf(buf, " %s", type_string(prog->if_value, god_types));
								break;
							case OIF_USER_POSITION:
								cat_sprintf(buf, " %s", type_string(prog->if_value, p_types));
								break;
							case OIF_USER_RACE:
								cat_sprintf(buf, " %s", type_string(prog->if_value, r_types));
								break;
							case OIF_USER_SEX:
								cat_sprintf(buf, " %s", type_string(prog->if_value, sex_types));
								break;
							case OIF_WEATHER:
								cat_sprintf(buf, " %s", type_string(prog->if_value, oprog_weathers));
								break;
							case OIF_USER_FIGHTING:
								cat_sprintf(buf, " %s", type_string(prog->if_value, boolean));
								break;
							default:
								cat_sprintf(buf, " %d", prog->if_value);
								break;
						}
				}
				ch_printf_color(ch, "%s\n\r", buf);
			}
			if (obj->first_prog == NULL)
			{
				ch_printf(ch, "This object does not have any defined programs.\n\r");
			}
			pop_call();
			return;
		}
	}
	else if ((obj = get_obj_index(atoi(argument))) == NULL)
	{
		send_to_char("Syntax: edit oprog <obj vnum>\n\r", ch);
		pop_call();
		return;
	}
	else if (!can_olc_modify(ch, obj->vnum))
	{
		send_to_char("That vnum is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	argument = one_argument(argument, arg);

	if (ch->pcdata->editmode != MODE_REPEATCMD)
	{
		ch->pcdata->edit_ptr = obj;
		ch->pcdata->edittype = EDIT_TYPE_OPROG;

		ch_printf( ch, "[%d] Editing Oprog.\n\r", obj->vnum);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf( arg, "Editing Oprog Vnum: %d", obj->vnum );
			ch->pcdata->subprompt = STRALLOC( arg );
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "del"))
	{
		if ((value = atoi(argument)) < 1)
		{
			ch_printf(ch, "Syntax: del <index>\n\r");
		}
		else
		{
			for (cnt = 0, prog = obj->first_prog ; prog ; prog = prog->next)
			{
				if (++cnt == value)
				{
					break;
				}
			}
			if (prog)
			{
				UNLINK(prog, obj->first_prog, obj->last_prog, next, prev);

				for (obj->progtypes = 0, prog = obj->first_prog ; prog ; prog = prog->next)
				{
					SET_BIT(obj->progtypes, prog->trigger);
				}
			}
			else
			{
				ch_printf(ch, "Index %d not found.\n\r", value);
			}
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "add"))
	{
		argument = one_argument(argument, arg);

		if ((value = atoi(arg)) < 1)
		{
			ch_printf(ch, "Syntax: add <index> <trigger>\n\r");
			pop_call();
			return;
		}

		if ((type = get_flag(argument, oprog_flags)) == -1)
		{
			ch_printf(ch, "add <index> <%s>\n\r", give_flags(oprog_flags));
			pop_call();
			return;
		}

		ALLOCMEM(prog, OBJ_PROG, 1);

		prog->trigger		= 1 << type;
		prog->obj_command	= OPROG_JUNK;
		prog->unknown		= STRALLOC("*");
		prog->argument		= STRALLOC("");
		prog->if_symbol	= '=';

		for (cnt = 0, prog2 = obj->first_prog ; prog2 ; prog2 = prog2->next)
		{
			if (++cnt == value)
			{
				break;
			}
		}

		if (prog2)
		{
			INSERT_LEFT(prog, prog2, obj->first_prog, next, prev);
		}
		else
		{
			LINK(prog, obj->first_prog, obj->last_prog, next, prev);
		}
		SET_BIT(obj->progtypes, prog->trigger);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "set"))
	{
		argument = one_argument(argument, arg);

		if ((value = atoi(arg)) < 1)
		{
			ch_printf(ch, "Syntax: set <index> <argument>\n\r");
			pop_call();
			return;
		}

		for (cnt = 0, prog = obj->first_prog ; prog ; prog = prog->next)
		{
			if (++cnt == value)
			{
				break;
			}
		}

		if (prog == NULL)
		{
			ch_printf(ch, "Index %d not found.\n\r", value);
			pop_call();
			return;
		}

		argument = one_argument(argument, arg);

		if (arg[0] == '\0')
		{
			ch_printf(ch, "Syntax: set %d <index|percent|trigger|argument|type|true|false>\n\r", value);

			switch (prog->obj_command)
			{
				case OPROG_IF_HAS_OBJECT:
					ch_printf(ch, "Syntax: set %d do <vnum> <true> <false>\n\r", value);
					break;
				case OPROG_IF:
					ch_printf(ch, "Syntax: set %d do <ifcheck> <symbol> <value> <true> <false>\n\r", value);
					break;
				case OPROG_QUEST_SET:
				case OPROG_QUEST_ADD:
					ch_printf(ch, "Syntax: set %d do <offset> <numbit> <value>\n\r", value);
					break;
				case OPROG_OBJECT_QUEST_IF:
				case OPROG_PLAYER_QUEST_IF:
					ch_printf(ch, "Syntax: set %d do <offset> <numbit> <symbol> <value> <true> <false>\n\r", value);
					break;
				case OPROG_APPLY:
					ch_printf(ch, "Syntax: set %d do <%s> <value>\n\r", value, give_flags(oprog_applies));
					break;
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "true"))
		{
			prog->if_true = atoi(argument);
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "false"))
		{
			prog->if_false = atoi(argument);
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "trigger"))
		{
			if ((type = get_flag(argument, oprog_flags)) == -1)
			{
				ch_printf(ch, "set %d trigger <%s>\n\r", value, give_flags(oprog_flags));
			}
			else
			{
				prog->trigger		= 1 << type;

				for (obj->progtypes = 0, prog = obj->first_prog ; prog ; prog = prog->next)
				{
					SET_BIT(obj->progtypes, prog->trigger);
				}
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "percent"))
		{
			switch (prog->trigger)
			{
				case TRIG_VOID:
					ch_printf(ch, "void triggers don't have a percentage.\n\r");
					break;
				default:
					prog->percentage = URANGE(0, atoi(argument), 100);
					break;
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "index"))
		{
			if (!is_number(argument))
			{
				ch_printf(ch, "Syntax: set %d index <number>\n\r", value);
			}
			else if (atoi(argument) < 1)
			{
				ch_printf(ch, "Index must be greater than zero.\n\r");
			}
			else if (prog->prev && atoi(argument) < prog->prev->index)
			{
				ch_printf(ch, "Index must be greater or equal to previous index.\n\r");
			}
			else if (prog->next && atoi(argument) > prog->next->index)
			{
				ch_printf(ch, "Index must be smaller or equal to next index.\n\r");
			}
			else
			{
				prog->index = atoi(argument);
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "argument"))
		{
			switch (prog->trigger)
			{
				case TRIG_COMMAND:
				case TRIG_ROOM_COMMAND:
				case TRIG_WEAR_COMMAND:
					if (find_command(argument, MAX_LEVEL - 1) == -1)
					{
						ch_printf(ch, "Syntax: set %d argument <command name>\n\r", value);
					}
					else
					{
						prog->cmd = find_command(argument, MAX_LEVEL - 1);
					}
					break;
				case TRIG_UNKNOWN:
				case TRIG_ROOM_UNKNOWN:
				case TRIG_WEAR_UNKNOWN:
					if (argument[0] == '\0')
					{
						ch_printf(ch, "Syntax: set %d argument <unknown name>\n\r", value);
					}
					else if (find_command(argument, MAX_LEVEL) != -1)
					{
						ch_printf(ch, "You cannot use an existing command for an unknown trigger.\n\r");
					}
					else
					{
						STRFREE(prog->unknown);
						prog->unknown = STRALLOC(argument);
					}
					break;
				default:
					ch_printf(ch, "You can only set arguments for command and unknown triggers.\n\r");
					break;
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "type"))
		{
			if ((type = get_flag(argument, oprog_commands)) == -1)
			{
				ch_printf(ch, "set %d type <%s>\n\r", value, give_flags(oprog_commands));
			}
			else
			{
				prog->obj_command = type;
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "do"))
		{
			int quest_offset, quest_bits, if_value, if_check;
			char if_symbol;

			switch (prog->obj_command)
			{
				case OPROG_IF_HAS_OBJECT:
					if (!is_number(argument))
					{
						ch_printf(ch, "Syntax: set %d do <vnum>\n\r", value);
						pop_call();
						return;
					}
					else
					{
						prog->if_value = atoi(argument);
					}
					break;
				case OPROG_IF:
					argument = one_argument(argument, arg);
					if ((if_check = get_flag(arg, oprog_ifchecks)) == -1)
					{
						ch_printf(ch, "Syntax: set %d do <%s> <symbol> <value>\n\r", value, give_flags(oprog_ifchecks));
						pop_call();
						return;
					}
					argument = one_argument(argument, arg);

					if_symbol = arg[0];

					switch (if_symbol)
					{
						case '<': case '>': case '!': case '=':
							break;
						default:
							ch_printf(ch, "Syntax: set %d do %s <symbol> <value>\n\r", value, type_string(if_check, oprog_ifchecks));
							ch_printf(ch, "Symbol must be one of: < > ! = \n\r");
							pop_call();
							return;
					}
					argument = one_argument(argument, arg);

					switch (if_check)
					{
						case OIF_WEAR_LOC:
							if ((if_value = get_flag(arg, oprog_wlocs)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(oprog_wlocs));
								pop_call();
								return;
							}
							if_value--;	/* cause table is 1 off */
							break;
						case OIF_USER_SECTOR:
							if ((if_value = get_flag(arg, sect_types)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(sect_types));
								pop_call();
								return;
							}
							break;
						case OIF_USER_CLASS:
							if ((if_value = get_flag(arg, class_types)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(class_types));
								pop_call();
								return;
							}
							break;
						case OIF_USER_WHICH_GOD:
							if ((if_value = get_flag(arg, god_types)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(god_types));
								pop_call();
								return;
							}
							break;
						case OIF_USER_POSITION:
							if ((if_value = get_flag(arg, p_types)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(p_types));
								pop_call();
								return;
							}
							break;
						case OIF_USER_RACE:
							if ((if_value = get_flag(arg, r_types)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(r_types));
								pop_call();
								return;
							}
							break;
						case OIF_USER_SEX:
							if ((if_value = get_flag(arg, sex_types)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(sex_types));
								pop_call();
								return;
							}
							break;
						case OIF_WEATHER:
							if ((if_value = get_flag(arg, oprog_weathers)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(oprog_weathers));
								pop_call();
								return;
							}
							break;
						case OIF_USER_FIGHTING:
							if ((if_value = get_flag(arg, boolean)) == -1)
							{
								ch_printf(ch, "Syntax: set %d do %s %c <%s>\n\r", value, type_string(if_check, oprog_ifchecks), if_symbol, give_flags(boolean));
								pop_call();
								return;
							}
							break;
						default:
							if_value = atoi(arg);
							break;
					}
					prog->if_check		= if_check;
					prog->if_symbol	= if_symbol;
					prog->if_value		= if_value;
					break;
				case OPROG_QUEST_SET:
				case OPROG_QUEST_ADD:
					if (sscanf(argument, "%d %d %d", &quest_offset, &quest_bits, &if_value) != 3)
					{
						ch_printf(ch, "Syntax: set %d do <offset> <numbit> <value>\n\r", value);
						pop_call();
						return;
					}
					else
					{
						prog->quest_offset	= quest_offset;
						prog->quest_bits	= quest_bits;
						prog->if_value		= if_value;
					}
					break;
				case OPROG_OBJECT_QUEST_IF:
				case OPROG_PLAYER_QUEST_IF:
					if (sscanf(argument, "%d %d %c %d", &quest_offset, &quest_bits, &if_symbol, &if_value) != 4)
					{
						ch_printf(ch, "Syntax: set %d do <offset> <numbit> <symbol> <value>\n\r", value);
						pop_call();
						return;
					}
					switch (if_symbol)
					{
						case '<': case '>': case '!': case '=':
							break;
						default:
							ch_printf(ch, "Syntax: set %d do <offset> <numbit> <symbol> <value> <true> <false>\n\r", value);
							ch_printf(ch, "Symbol must be one of: < > | = \n\r");
							pop_call();
							return;
					}
					prog->quest_offset	= quest_offset;
					prog->quest_bits	= quest_bits;
					prog->if_symbol	= if_symbol;
					prog->if_value		= if_value;
					break;
				case OPROG_APPLY:
					argument = one_argument(argument, arg);
					if ((if_check = get_flag(arg, oprog_applies)) == -1)
					{
						ch_printf(ch, "Syntax: set %d do %s <value>\n\r", value, give_flags(oprog_applies));
						pop_call();
						return;
					}
					prog->if_check = if_check;
					prog->if_value	= atoi(argument);
					break;
				default:
					ch_printf(ch, "This oprog type has no 'do' options, try 'edit'.\n\r");
					break;
			}
			pop_call();
			return;
		}
		do_opedit(ch, "set");
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "edit"))
	{
		if ((value = atoi(argument)) < 1)
		{
			ch_printf(ch, "Syntax: edit <index>\n\r");
			pop_call();
			return;
		}

		for (cnt = 0, prog = obj->first_prog ; prog ; prog = prog->next)
		{
			if (++cnt == value)
			{
				break;
			}
		}

		if (prog == NULL)
		{
			ch_printf(ch, "Index %d not found.\n\r", value);
			pop_call();
			return;
		}

		switch (prog->obj_command)
		{
			case OPROG_ECHO:
			case OPROG_GOD_COMMAND:
			case OPROG_GOD_ARGUMENT:
			case OPROG_COMMAND:
			case OPROG_ARGUMENT:
				break;
			default:
				ch_printf(ch, "You can only edit echo, godcommand, godargument, command and argument.\n\r");
				pop_call();
				return;
		}

		ch->pcdata->temp_ptr = prog;

		ch->pcdata->editmode = MODE_OPROG_EDIT;
		ch->pcdata->tempmode  = MODE_REPEATCMD;

		start_editing(ch, prog->argument);

		pop_call();
		return;
	}

	ch->pcdata->editmode = MODE_RESTRICTED;
	interpret( ch, origarg );
	ch->pcdata->editmode = MODE_REPEATCMD;
	ch->pcdata->last_cmd = do_edit;

	pop_call();
	return;
}

void do_mpedit( CHAR_DATA *ch, char *argument )
{
	char arg [MAX_INPUT_LENGTH];
	char buf [MAX_INPUT_LENGTH];

	int  value, cnt, type;

	MOB_INDEX_DATA *mob;
	MPROG_DATA	*prog,  *prog2;
	NPC_TOKEN		*token;

	char *origarg = argument;

	char colw[10], colg[10], colW[10], colM[10];

	push_call("do_mpedit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	strcpy(colw, ansi_translate_text(ch, "{078}"));
	strcpy(colg, ansi_translate_text(ch, "{028}"));
	strcpy(colW, ansi_translate_text(ch, "{178}"));
	strcpy(colM, ansi_translate_text(ch, "{158}"));

	if (!ch->desc)
	{
		send_to_char( "You have no descriptor\n\r", ch );
		pop_call();
		return;
	}

	mob = NULL;
	smash_tilde( argument );

	switch( ch->pcdata->editmode )
	{
		case MODE_MPROG_EDIT:
			if (ch->pcdata->edit_ptr && ch->pcdata->temp_ptr)
			{
				mob  = ch->pcdata->edit_ptr;
				prog = ch->pcdata->temp_ptr;
				STRFREE(prog->comlist);
				prog->comlist = copy_buffer(ch);
				stop_editing( ch );

				if (strstr(prog->comlist, "$r") != NULL)
				{
					SET_BIT(prog->flags, MPTRIGGER_RAND_PLR);
				}
				else
				{
					REMOVE_BIT(prog->flags, MPTRIGGER_RAND_PLR);
				}

				while ((token = prog->first_token) != NULL)
				{
					UNLINK(token, prog->first_token, prog->last_token, next, prev);
					STRFREE(token->string);
					FREEMEM(token);
				}
				expand_mob_prog(mob, prog);
			}
			else
			{
				log_printf("do_mpedit: null edit_ptr or temp_ptr");
				ch->pcdata->tempmode = MODE_NONE;
				stop_editing(ch);
			}
			pop_call();
			return;
	}

	if (ch->pcdata->editmode == MODE_REPEATCMD)
	{
		mob = ch->pcdata->edit_ptr;

		if (argument[0] == '?')
		{
			/*
				Show mpedit status
			*/
			ch_printf(ch, "Syntax: <command> <value>\n\r\n\r");
			ch_printf(ch, "Command being one of:\n\r");
			ch_printf(ch, "  add del set edit done stat ? < >\n\r");
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "done"))
		{
			send_to_char( "MPedit mode off.\n\r", ch );

			stop_olc_editing(ch, mob->area);

			pop_call();
			return;
		}

		if (!strcmp(argument, ">"))
		{
			if (get_mob_index(mob->vnum + 1) && can_olc_modify(ch, mob->vnum + 1))
			{
				stop_olc_editing(ch, mob->area);
				sprintf(buf, "%d", mob->vnum + 1);
				do_mpedit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Next index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcmp(argument, "<"))
		{
			if (get_mob_index(mob->vnum - 1) && can_olc_modify(ch, mob->vnum - 1))
			{
				stop_olc_editing(ch, mob->area);
				sprintf(buf, "%d", mob->vnum - 1);
				do_mpedit(ch, buf);
			}
			else
			{
				ch_printf(ch, "Prev index not found.\n\r");
			}
			pop_call();
			return;
		}

		if (!strcasecmp(argument, "stat") || argument[0] == ' ')
		{
			for (cnt = 0, prog = mob->first_prog ; prog ; prog = prog->next)
			{
				ch_printf_color(ch, "{178}%2d{078}] {138}%s {128}%s\n\r", ++cnt, mprog_type_to_name(prog->type), prog->arglist);
			}
			if (mob->first_prog == NULL)
			{
				ch_printf(ch, "This mobile does not have any programs defined.\n\r");
			}
			pop_call();
			return;
		}
	}	
	else if ((mob = get_mob_index(atoi(argument))) == NULL)
	{
		send_to_char("Syntax: edit mprog <mobile vnum>\n\r", ch);
		pop_call();
		return;
	}
	else if (!can_olc_modify(ch, mob->vnum))
	{
		send_to_char("That vnum is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	argument = one_argument(argument, arg);

	if (ch->pcdata->editmode != MODE_REPEATCMD)
	{
		ch->pcdata->edit_ptr = mob;
		ch->pcdata->edittype = EDIT_TYPE_MPROG;

		ch_printf( ch, "[%d] Editing Mprog.\n\r", mob->vnum);
		ch->pcdata->editmode = MODE_REPEATCMD;

		if (ch->pcdata)
		{
			if (ch->pcdata->subprompt)
			{
				STRFREE(ch->pcdata->subprompt);
			}
			sprintf( arg, "Editing Mprog Vnum: %d", mob->vnum );
			ch->pcdata->subprompt = STRALLOC( arg );
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "del"))
	{
		if ((value = atoi(argument)) < 1)
		{
			ch_printf(ch, "Syntax: del <index>\n\r");
		}
		else
		{
			for (cnt = 0, prog = mob->first_prog ; prog ; prog = prog->next)
			{
				if (++cnt == value)
				{
					break;
				}
			}
			if (prog)
			{
				UNLINK(prog, mob->first_prog, mob->last_prog, next, prev);

				for (mob->progtypes = 0, prog = mob->first_prog ; prog ; prog = prog->next)
				{
					SET_BIT(mob->progtypes, prog->type);
				}
			}
			else
			{
				ch_printf(ch, "Index %d not found.\n\r", value);
			}
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "add"))
	{
		argument = one_argument(argument, arg);

		if ((value = atoi(arg)) < 1)
		{
			ch_printf(ch, "Syntax: add <index> <trigger> [argument]\n\r");
			pop_call();
			return;
		}

		argument = one_argument(argument, arg);

		if ((type = get_flag(arg, mprog_flags)) == -1)
		{
			ch_printf(ch, "add <index> <%s>\n\r", give_flags(mprog_flags));
			pop_call();
			return;
		}

		ALLOCMEM(prog, MPROG_DATA, 1);

		prog->type	= 1 << type;
		prog->arglist	= STRALLOC(argument);
		prog->comlist  = STRALLOC("");

		for (cnt = 0, prog2 = mob->first_prog ; prog2 ; prog2 = prog2->next)
		{
			if (++cnt == value)
			{
				break;
			}
		}
		if (prog2)
		{
			INSERT_LEFT(prog, prog2, mob->first_prog, next, prev);
		}
		else
		{
			LINK(prog, mob->first_prog, mob->last_prog, next, prev);
		}
		SET_BIT(mob->progtypes, prog->type);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "set"))
	{
		argument = one_argument(argument, arg);

		if ((value = atoi(arg)) < 1)
		{
			ch_printf(ch, "Syntax: set <index> <trigger> <argument>\n\r");
			ch_printf(ch, "Syntax: set <index> <arglist> <argument>\n\r");
			pop_call();
			return;
		}

		for (cnt = 0, prog = mob->first_prog ; prog ; prog = prog->next)
		{
			if (++cnt == value)
			{
				break;
			}
		}

		if (prog == NULL)
		{
			ch_printf(ch, "Index %d not found.\n\r", value);
			pop_call();
			return;
		}

		argument = one_argument(argument, arg);

		if (!strcasecmp(arg, "trigger"))
		{
			if ((type = get_flag(argument, mprog_flags)) == -1)
			{
				ch_printf(ch, "set <index> trigger <%s>\n\r", give_flags(mprog_flags));
			}
			else
			{
				prog->type = 1 << type;

				for (mob->progtypes = 0, prog = mob->first_prog ; prog ; prog = prog->next)
				{
					SET_BIT(mob->progtypes, prog->type);
				}
			}
			pop_call();
			return;
		}

		if (!strcasecmp(arg, "arglist"))
		{
			RESTRING(prog->arglist, argument);
			pop_call();
			return;
		}
		ch_printf(ch, "Syntax: set <index> <trigger> <argument>\n\r");
		ch_printf(ch, "Syntax: set <index> <arglist> <argument>\n\r");
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "edit"))
	{
		if ((value = atoi(argument)) < 1)
		{
			ch_printf(ch, "Syntax: edit <index>\n\r");
			pop_call();
			return;
		}

		for (cnt = 0, prog = mob->first_prog ; prog ; prog = prog->next)
		{
			if (++cnt == value)
			{
				break;
			}
		}

		if (prog == NULL)
		{
			ch_printf(ch, "Index %d not found.\n\r", value);
			pop_call();
			return;
		}

		if (prog->comlist)
		{
			STRFREE(prog->comlist);
		}
		prog->comlist = STRALLOC(fixer_mprog(prog));

		ch->pcdata->temp_ptr = prog;

		ch->pcdata->editmode = MODE_MPROG_EDIT;
		ch->pcdata->tempmode = MODE_REPEATCMD;

		start_editing(ch, prog->comlist);

		pop_call();
		return;
	}

	ch->pcdata->editmode = MODE_RESTRICTED;
	interpret( ch, origarg );
	ch->pcdata->editmode = MODE_REPEATCMD;
	ch->pcdata->last_cmd = do_edit;

	pop_call();
	return;
}

bool bad_short_desc( char *str )
{
	push_call("bad_short_desc_check(%p)",str);

	if (!str_prefix("an ", str))
	{
		pop_call();
		return (str[0] == 'A');
	}

	if (!str_prefix("a ", str))
	{
		pop_call();
		return (str[0] == 'A');
	}

	if (!str_prefix("the ", str))
	{
		pop_call();
		return (str[0] == 'T');
	}

	pop_call();
	return FALSE;
}


void do_area_assign( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	int  a_lo, a_hi;
	CHAR_DATA *victim;

	push_call("do_area_assign(%p,%p)",ch,argument);

	CHECK_EDITMODE(ch)

	if (!IS_GOD(ch))
	{
		ch_printf(ch, "Alas, Gods only =]");
		pop_call();
		return;
	}

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );

	a_lo = atoi(arg2);
	a_hi = atoi(arg3);

	if (arg1[0] == '\0' || a_lo < 1 || a_hi < 1 || a_lo > a_hi)
	{
		send_to_char( "Syntax: edit area assign <player> <low vnum> <high vnum>\n\r", ch );
		pop_call();
		return;
	}

	if ((victim = get_player_world(ch, arg1)) == NULL)
	{
		send_to_char( "They don't seem to be around.\n\r", ch );
		pop_call();
		return;
	}

	if (IS_NPC(victim) || victim->level < LEVEL_IMMORTAL)
	{
		send_to_char( "They wouldn't know what to do with a vnum range.\n\r", ch);
		pop_call();
		return;
	}

	if (a_lo > a_hi)
	{
		send_to_char( "Unacceptable vnum range.\n\r", ch );
		pop_call();
		return;
	}

	if (a_lo == 0)
	{
		a_hi = 0;
	}

	victim->pcdata->a_range_lo = a_lo;
	victim->pcdata->a_range_hi = a_hi;

	ch_printf(ch,     "You assigned %s the vnum range %d - %d.\n\r", victim->name, a_lo, a_hi);
	ch_printf(victim, "%s has assigned you the vnum range %d - %d.\n\r", ch->name, a_lo, a_hi);

	if ((get_room_index(a_lo)) == NULL)
	{
		assign_area( victim );
		do_savearea(victim, "");
	}
	pop_call();
	return;
}


void do_area_check ( CHAR_DATA *ch, char *argument )
{
	char 			arg[MAX_STRING_LENGTH];
	bool				fix, world, door;
	AREA_DATA			*area;
	ROOM_INDEX_DATA	*room;
	MOB_INDEX_DATA		*mob;
	OBJ_INDEX_DATA		*obj;
	AFFECT_DATA		*aff;
	EXTRA_DESCR_DATA	*ed;

	int vnum, lvl_neg, lvl_pos, lvl, cnt, max_cnt, aff_cnt, x, lo_vnum, hi_vnum;

	push_call("do_area_check(%p,%p)",ch,argument);

	one_argument(argument, arg);

	if (!strcasecmp(arg, "mob"))
	{
		int hit, dam, low, max;

		argument = one_argument(argument, arg);
		argument = one_argument(argument, arg);

		low      = atoi(arg) ? atoi(arg) : 1;
		max      = atoi(argument) ? atoi(argument) : 300;

		low = URANGE(1, low, 300);
		max = URANGE(1, max, 300);

		for (arg[0] = '\0', cnt = low ; cnt <= max ; cnt++)
		{
			hit = (int) 10 + (cnt*cnt*(0.5 + 0.0015*cnt));
			dam = (int)  1 + (cnt    *(0.5 + 0.0050*cnt));

			cat_sprintf(arg, "{178}%3d) {128}hp: %5d/%5d    {118}dam: %2d/%2d\n\r", cnt, hit, hit*2, dam, dam*2);
		}
		send_to_char_color(arg, ch);
		pop_call();
		return;
	}

	world = fix = lo_vnum = hi_vnum = FALSE;

	if (IS_GOD(ch) && !strcasecmp(arg, "fixworld"))
	{
		fix		= TRUE;
		world	= TRUE;
		max_cnt	= 1000000;
		area		= ch->in_room->area;
		lo_vnum	= 1;
		hi_vnum	= MAX_VNUM-1;
	}
	else if (!strcasecmp(arg, "fix"))
	{
		fix 		= TRUE;
		max_cnt	= 10000;
		area 	= ch->in_room->area;
	}
	else
	{
		if (argument[0] != '\0')
		{
			area = lookup_area(argument);
		}
		else
		{
			area = ch->in_room->area;
		}
		max_cnt	= get_pager_breakpt(ch) - 2;
	}

	if (area == NULL)
	{
		ch_printf(ch, "Syntax: edit area check [name|mob|fix|fixworld]\n\r");
		pop_call();
		return;
	}

	if (!can_olc_modify(ch, area->low_r_vnum))
	{
		ch_printf(ch, "You can only check areas in your allocated vnum range\n\r");
		pop_call();
		return;
	}

	lvl = cnt	= 0;

	if (world == FALSE)
	{
		lo_vnum = area->low_r_vnum;
		hi_vnum = area->hi_r_vnum;
	}

	for (vnum = lo_vnum ; vnum <= hi_vnum ; vnum++)
	{
		if ((room = get_room_index(vnum)) == NULL)
		{
			continue;
		}

		if (IS_SET(room->room_flags, ROOM_NO_GOHOME) && IS_SET(area->flags, AFLAG_NOGOHOME) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(room->room_flags, ROOM_NO_GOHOME);
			}
			else
			{
				
			}
		}
		if (IS_SET(room->room_flags, ROOM_NO_RECALL) && IS_SET(area->flags, AFLAG_NORECALL) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(room->room_flags, ROOM_NO_RECALL);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {078}Both room and area have norecall flag set.\n\r", room->vnum);
			}
		}
		if (IS_SET(room->room_flags, ROOM_NO_CASTLE) && IS_SET(area->flags, AFLAG_NOCASTLE) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(room->room_flags, ROOM_NO_CASTLE);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {078}Both room and area have nocastle flag set.\n\r", room->vnum);
			}
		}
		if (IS_SET(room->room_flags, ROOM_NO_RIP) && IS_SET(area->flags, AFLAG_NORIP) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(room->room_flags, ROOM_NO_RIP);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {078}Both room and area have norip flag set.\n\r", room->vnum);
			}
		}
		if (room->description[0] && str_suffix("\n\r", room->description) && ++cnt < max_cnt)
		{
			if (fix)
			{
				sprintf(arg, "%s\n\r", room->description);
				RESTRING(room->description, arg);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Room has a bad description.\n\r", room->vnum);
			}
		}
		for (ed = room->first_extradesc ; ed ; ed = ed->next)
		{
			if (str_suffix("\n\r", ed->description) && ++cnt < max_cnt)
			{
				if (fix)
				{
					sprintf(arg, "%s\n\r", ed->description);
					RESTRING(ed->description, arg);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Room has a bad extra description.\n\r", room->vnum);
				}
			}
		}
		for (door = 0 ; door < 6 ; door++)
		{
			if (room->exit[door] && room->exit[door]->description[0])
			{
				if (!str_suffix("\n\r", room->exit[door]->description) && ++cnt < max_cnt)
				{
					if (fix)
					{
						strcpy(arg, room->exit[door]->description);
						arg[strlen(arg) - 2] = '\0';
						RESTRING(room->exit[door]->description, arg);
					}
					else
					{
						ch_printf_color(ch, "{118}[{178}%u{118}] {058}Room has a bad exit description.\n\r", room->vnum);
					}
				}
			}
		}
	}

	if (world == FALSE)
	{
		lo_vnum = area->low_o_vnum;
		hi_vnum = area->hi_o_vnum;
	}

	for (vnum = lo_vnum ; vnum <= hi_vnum ; vnum++)
	{
		if ((obj = get_obj_index(vnum)) == NULL)
		{
			continue;
		}

		lvl_neg = 0;
		lvl_pos = 1;

		if (fix == FALSE)
		{
			for (aff_cnt = 0, aff = obj->first_affect ; aff ; aff = aff->next, aff_cnt++)
			{
				switch (aff->location)
				{
					case APPLY_STR:
					case APPLY_WIS:
					case APPLY_CON:
					case APPLY_DEX:
					case APPLY_INT:
						if (aff->modifier > 0)
						{
							lvl_pos += aff->modifier * aff->modifier * 4/3;
						}
						else
						{
							lvl_neg += aff->modifier * aff->modifier * 3/4;
						}
						if (aff->modifier > obj->level/10 + 1 && ++cnt < max_cnt)
						{
							ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d     {128}Object's %s apply is too high.\n\r", obj->vnum, obj->level, a_types[aff->location]);
						}
						break;
					case APPLY_MANA:
						if (aff->modifier > 0)
						{
							lvl_pos += aff->modifier / 3;
						}
						if (aff->modifier > obj->level * 2 && ++cnt < max_cnt)
						{
							ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d     {128}Object's %s apply is too high.\n\r", obj->vnum, obj->level, a_types[aff->location]);
						}
						break;
					case APPLY_HIT:
						if (aff->modifier > 0)
						{
							lvl_pos += aff->modifier / 2;
						}
						else
						{
							lvl_neg -= aff->modifier / 5;
						}
						if (aff->modifier > obj->level + 5 && ++cnt < max_cnt)
						{
							ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d     {128}Object's %s apply is too high.\n\r", obj->vnum, obj->level, a_types[aff->location]);
						}
						break;
					case APPLY_MOVE:
						if (aff->modifier > 0)
						{
							lvl_pos += aff->modifier / 4;
						}
						if (aff->modifier > obj->level * 2 && ++cnt < max_cnt)
						{
							ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d     {128}Object's %s apply is too high.\n\r", obj->vnum, obj->level, a_types[aff->location]);
						}
						break;
					case APPLY_AC:
						if (aff->modifier < 0)
						{
							lvl_pos += aff->modifier * aff->modifier / 4;
						}
						else
						{
							lvl_neg += aff->modifier * aff->modifier / 8;
						}
						break;
					case APPLY_HITROLL:
						if (aff->modifier > 0)
						{
							lvl_pos += aff->modifier * aff->modifier / 2;
						}
						if (aff->modifier > obj->level / 5 + 1 && ++cnt < max_cnt)
						{
							ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d     {128}Object's %s apply is too high.\n\r", obj->vnum, obj->level, a_types[aff->location]);
						}
						break;
					case APPLY_DAMROLL:
						if (aff->modifier > 0)
						{
							lvl_pos += aff->modifier * aff->modifier;
						}
						if (aff->modifier > obj->level / 10 + 1 && ++cnt < max_cnt)
						{
							ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d     {128}Object's %s apply is too high.\n\r", obj->vnum, obj->level, a_types[aff->location]);
						}
						break;
					case APPLY_SAVING_PARA:
					case APPLY_SAVING_ROD:
					case APPLY_SAVING_PETRI:
					case APPLY_SAVING_BREATH:
					case APPLY_SAVING_SPELL:
						if (aff->modifier < 0)
						{
							lvl_pos += aff->modifier * aff->modifier / 3;
						}
						else
						{
							lvl_neg += aff->modifier * aff->modifier / 6;
						}
						if (aff->modifier > obj->level / 10 + 1 && ++cnt < max_cnt)
						{
							ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d     {128}Object's %s apply is too high.\n\r", obj->vnum, obj->level, a_types[aff->location]);
						}
						break;
					default:
						break;
				}
			}
	
			switch (obj->item_type)
			{
				case ITEM_LIGHT:
					break;
				case ITEM_SCROLL:
					if (obj->value[1] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					if (obj->value[2] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					if (obj->value[3] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					break;
				case ITEM_POTION:
					if (obj->value[1] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					if (obj->value[2] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					if (obj->value[3] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					break;
				case ITEM_PILL:
					if (obj->value[1] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					if (obj->value[2] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					if (obj->value[3] > 0 && obj->value[1] < MAX_SKILL)
					{
						lvl_pos += obj->value[0] / 2;
					}
					break;
				case ITEM_WAND:
					lvl_pos += obj->value[0]*obj->value[1] / 4;
					break;
				case ITEM_STAFF:
					lvl_pos += obj->value[0]*obj->value[1] / 6;
					break;
				case ITEM_WEAPON:
					lvl_pos += (5*(obj->value[1]*(1+(obj->value[2]-1)/2.0))/2) - 10;
					break;
				case ITEM_ARMOR:
					if (obj->value[0] > 0)
					{
						lvl_pos += obj->value[0] * obj->value[0] / 5 + 1;
					}
					else
					{
						lvl_neg += obj->value[0] * obj->value[0] / 5 + 0;
					}
					break;
				case ITEM_LOCKPICK:
					lvl_pos += obj->value[0] /  2;
					lvl_pos += obj->value[1] / 10;
					lvl_pos += IS_SET(obj->value[2], PICK_MAGICAL_LOCK)    ? 30 : 0;
					lvl_pos += IS_SET(obj->value[2], PICK_TRAPPED_LOCK)    ? 30 : 0;
					lvl_pos += IS_SET(obj->value[2], PICK_MECHANICAL_LOCK) ? 30 : 0;
					break;
				case ITEM_AMMO:
					lvl_pos += obj->value[1] * obj->value[3] / 20;
					break;
				case ITEM_CONTAINER:
					lvl_pos += obj->value[0] / 15;
					break;
				default:
					break;
			}
	
			if ((obj->extra_flags & ITEM_ANTI_GOOD) != 0)
			{
				lvl_neg += lvl_pos / 10;
			}
			if ((obj->extra_flags & ITEM_ANTI_EVIL) != 0)
			{
				lvl_neg += lvl_pos / 10;
			}
			if ((obj->extra_flags & ITEM_ANTI_NEUTRAL) != 0)
			{
				lvl_neg += lvl_pos / 10;
			}
			if ((obj->extra_flags & ITEM_AUTO_ENGRAVE) != 0)
			{
				lvl_neg += lvl_pos / 10;
			}

			if ((obj->extra_flags & ITEM_INVIS) != 0)
			{
				lvl_neg += lvl_pos / 20;
			}

			lvl = UMAX(1, lvl_pos - lvl_neg);

			if (obj->level + 5 < lvl - (lvl / 20) && ++cnt < max_cnt)
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object level too low.\n\r", obj->vnum, obj->level, lvl);
			}
			if (obj->level + 5 <= lvl_neg * 2 && ++cnt < max_cnt)
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has too many negative applies.\n\r", obj->vnum, obj->level, lvl);
			}
			if (aff_cnt > 3 && ++cnt < max_cnt)
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has more than 3 applies.\n\r", obj->vnum, obj->level, lvl);
			}
			if (aff_cnt > 0 && !IS_SET(obj->wear_flags, ITEM_TAKE) && ++cnt < max_cnt)
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has applies but cannot be worn.\n\r", obj->vnum, obj->level, lvl);
			}
		}

		if (obj->cost < 10 && ++cnt < max_cnt)
		{
			if (fix)
			{
				obj->cost = 10;
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object should cost at least 10 gold.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		if (obj->cost > 10000000 && ++cnt < max_cnt)
		{
			if (fix)
			{
				obj->cost = 10000000;
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object should cost less then 10 Million gold.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		if (obj->weight < 1 && ++cnt < max_cnt)
		{
			if (fix)
			{
				obj->weight = 1;
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object should have at least 1 weight.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		if (obj->wear_flags > 0 && !IS_SET(obj->wear_flags, ITEM_TAKE) && ++cnt < max_cnt)
		{
			if (fix)
			{
				obj->wear_flags = 0;
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object can be worn but doesn't have 'wear take' set.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		if (obj->wear_flags > 0 && obj->long_descr[0] == '\0' && ++cnt < max_cnt)
		{
			if (fix)
			{
				sprintf(arg, "%s is here.", capitalize(obj->short_descr));
				RESTRING(obj->long_descr, arg);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has wear flags but no long description set.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		if (fix == FALSE)
		{
			for (aff_cnt = x = 0 ; x < 32 ; x++)
			{
				aff_cnt += IS_SET(obj->wear_flags, 1 << x) ? 1 : 0;
			}
			if (aff_cnt > 3 && ++cnt < max_cnt)
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has more than 3 wear locations.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		if (bad_short_desc(obj->short_descr) && ++cnt < max_cnt)
		{
			if (fix)
			{
				strcpy(arg, obj->short_descr);
				arg[0] = tolower(arg[0]);
				RESTRING(obj->short_descr, arg);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has a bad short desc.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		if (obj->description[0] && str_suffix("\n\r", obj->description) && ++cnt < max_cnt)
		{
			if (fix)
			{
				sprintf(arg, "%s\n\r", obj->description);
				RESTRING(obj->description, arg);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has a bad description.\n\r", obj->vnum, obj->level, lvl);
			}
		}
		for (ed = obj->first_extradesc ; ed ; ed = ed->next)
		{
			if (str_suffix("\n\r", ed->description) && ++cnt < max_cnt)
			{
				if (fix)
				{
					sprintf(arg, "%s\n\r", ed->description);
					RESTRING(ed->description, arg);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {178}Lvl {078}%2d{178}/{078}%3d {128}Object has a bad extra description.\n\r", obj->vnum, obj->level, lvl);
				}
			}
		}
	}

	if (world == FALSE)
	{
		lo_vnum = area->low_m_vnum;
		hi_vnum = area->hi_m_vnum;
	}

	for (vnum = lo_vnum ; vnum <= hi_vnum ; vnum++)
	{
		int hit, dam, bothit, botdam;

		if ((mob = get_mob_index(vnum)) == NULL)
		{
			continue;
		}
		if (mob->gold > mob->level * 2000 && ++cnt < max_cnt)
		{
			if (fix)
			{
				mob->gold = number_fuzzy(mob->level * 1000);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile should have equal or less than %d gold.\n\r", mob->vnum, mob->level*2000);
			}
		}

		bothit = (int) 10 + (mob->level*mob->level*(0.5 + 0.0015*mob->level));
		botdam = (int)  1 + (mob->level           *(0.5 + 0.0050*mob->level));

		hit = mob->hitplus + (mob->hitnodice + (mob->hitnodice * mob->hitsizedice - mob->hitnodice) / 2);
		dam = mob->damplus + (mob->damnodice + (mob->damnodice * mob->damsizedice - mob->damnodice) / 2);

		if (hit < bothit && ++cnt < max_cnt)
		{
			if (fix)
			{
				mob->hitnodice		= 1;
				mob->hitsizedice	= mob->level * mob->level / 4;
				mob->hitplus		= 10 + mob->level * mob->level * (0.40 + 0.0015 * mob->level);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile should have equal or more than %d hitpoints.\n\r", mob->vnum, bothit * 1);
			}
		}
		else if (hit > bothit * 2 && ++cnt < max_cnt)
		{
			if (fix)
			{
				mob->hitnodice		= 1;
				mob->hitsizedice	= mob->level * mob->level / 4;
				mob->hitplus		= 10 + mob->level * mob->level * (0.70 + 0.0015 * mob->level);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile should have equal or less than %d hitpoints.\n\r", mob->vnum, bothit * 2);
			}
		}

		if (dam < botdam && ++cnt < max_cnt)
		{
			if (fix)
			{
				mob->damnodice   = 1;
				mob->damsizedice = mob->level;
				mob->damplus     = 1 + mob->level * 0.005 * mob->level;
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile should do equal or more than %d damage.\n\r", mob->vnum, botdam * 1);
			}
		}
		else if (dam > botdam * 2 && ++cnt < max_cnt)
		{
			if (fix)
			{
				mob->damnodice   = 1;
				mob->damsizedice = mob->level;
				mob->damplus     = 1 + mob->level * (0.5 + 0.005 * mob->level);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile should do equal or less than %d damage.\n\r", mob->vnum, botdam * 2);
			}
		}

		if (mob->attack_parts > 0 && !IS_SET(mob->act, ACT_BODY) && ++cnt < max_cnt)
		{
			if (fix)
			{
				SET_BIT(mob->act, ACT_BODY);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has attack parts set but no act_body.\n\r", mob->vnum);
			}
		}
		if (mob->body_parts   > 0 && !IS_SET(mob->act, ACT_BODY) && ++cnt < max_cnt)
		{
			if (fix)
			{
				SET_BIT(mob->act, ACT_BODY);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has body parts set but no act_body.\n\r", mob->vnum);
			}
		}
		if (!IS_SET(mob->act, ACT_SMART) && IS_SET(mob->affected_by, AFF_TONGUES) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->affected_by, AFF_TONGUES);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has tongues affect but no act_smart.\n\r", mob->vnum);
			}
		}
		if (!IS_SET(mob->act, ACT_SMART) && IS_SET(mob->affected_by, AFF_UNDERSTAND) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->affected_by, AFF_UNDERSTAND);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has understand affect but no act_smart.\n\r", mob->vnum);
			}
		}
		if (!IS_SET(mob->act, ACT_RACE) && IS_SET(mob->affected_by, AFF_TONGUES) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->affected_by, AFF_TONGUES);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has tongues affect but no act_race.\n\r", mob->vnum);
			}
		}
		if (!IS_SET(mob->act, ACT_RACE) && IS_SET(mob->affected_by, AFF_UNDERSTAND) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->affected_by, AFF_UNDERSTAND);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has understand affect but no act_race.\n\r", mob->vnum);
			}
		}
		if (IS_SET(mob->act, ACT_NO_ORDER) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->act, ACT_NO_ORDER);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has unused act_no_order set.\n\r", mob->vnum);
			}
		}
		if (IS_SET(mob->affected_by, AFF_TRUESIGHT) && IS_SET(mob->affected_by, AFF_DETECT_INVIS) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->affected_by, AFF_DETECT_INVIS);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both truesight and detect invis affect.\n\r", mob->vnum);
			}
		}
		if (IS_SET(mob->affected_by, AFF_TRUESIGHT) && IS_SET(mob->affected_by, AFF_DETECT_HIDDEN) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->affected_by, AFF_DETECT_HIDDEN);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both truesight and detect hidden affect.\n\r", mob->vnum);
			}
		}
		if (IS_SET(mob->affected_by, AFF_INFRARED) && ++cnt < max_cnt)
		{
			if (fix)
			{
				REMOVE_BIT(mob->affected_by, AFF_INFRARED);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has unneeded infrared affect.\n\r", mob->vnum);
			}
		}
		if (IS_SET(mob->affected_by, AFF_ETHEREAL))
		{
			if (IS_SET(mob->affected_by, AFF_DETECT_INVIS) && ++cnt < max_cnt)
			{
				if (fix)
				{
					REMOVE_BIT(mob->affected_by, AFF_DETECT_INVIS);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both ethereal and detect invis affect.\n\r", mob->vnum);
				}
			}
			if (IS_SET(mob->affected_by, AFF_DETECT_HIDDEN) && ++cnt < max_cnt)
			{
				if (fix)
				{
					REMOVE_BIT(mob->affected_by, AFF_DETECT_HIDDEN);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both ethereal and detect hidden affect.\n\r", mob->vnum);
				}
			}
			if (IS_SET(mob->affected_by, AFF_TRUESIGHT) && ++cnt < max_cnt)
			{
				if (fix)
				{
					REMOVE_BIT(mob->affected_by, AFF_TRUESIGHT);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both ethereal and truesight affect.\n\r", mob->vnum);
				}
			}
			if (IS_SET(mob->affected_by, AFF_STEALTH) && ++cnt < max_cnt)
			{
				if (fix)
				{
					REMOVE_BIT(mob->affected_by, AFF_STEALTH);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both ethereal and stealth affect.\n\r", mob->vnum);
				}
			}
			if (IS_SET(mob->affected_by, AFF_SNEAK) && ++cnt < max_cnt)
			{
				if (fix)
				{
					REMOVE_BIT(mob->affected_by, AFF_SNEAK);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both ethereal and sneak affect.\n\r", mob->vnum);
				}
			}
			if (IS_SET(mob->affected_by, AFF_INVISIBLE) && ++cnt < max_cnt)
			{
				if (fix)
				{
					REMOVE_BIT(mob->affected_by, AFF_INVISIBLE);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both ethereal and invis affect.\n\r", mob->vnum);
				}
			}
			if (IS_SET(mob->affected_by, AFF_IMP_INVISIBLE) && ++cnt < max_cnt)
			{
				if (fix)
				{
					REMOVE_BIT(mob->affected_by, AFF_IMP_INVISIBLE);
				}
				else
				{
					ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has both ethereal and invis affect.\n\r", mob->vnum);
				}
			}
		}
		if (bad_short_desc(mob->short_descr) && ++cnt < max_cnt)
		{
			if (fix)
			{
				strcpy(arg, mob->short_descr);
				arg[0] = tolower(arg[0]);
				STRFREE(mob->short_descr);
				mob->short_descr = STRALLOC(arg);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has a bad short desc.\n\r", mob->vnum);
			}
		}
		if (mob->description[0] && str_suffix("\n\r", mob->description) && ++cnt < max_cnt)
		{
			if (fix)
			{
				sprintf(arg, "%s\n\r", mob->description);
				RESTRING(mob->description, arg);
			}
			else
			{
				ch_printf_color(ch, "{118}[{178}%u{118}] {058}Mobile has a bad description.\n\r", mob->vnum);
			}
		}
	}
	if (fix)
	{
		ch_printf_color(ch, "{118}\n\r{118}Total of %d errors fixed.\n\r", cnt);
	}
	else
	{
		ch_printf_color(ch, "{118}\n\r{118}Total of %d errors found.\n\r", cnt);
	}
	pop_call();
	return;
}

void do_edit( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];

	push_call("do_edit(%p,%p)",ch,argument);

	CHECK_EDITMODE( ch );

	switch (ch->pcdata->edittype)
	{
		case EDIT_TYPE_HELP:	do_hedit (ch, argument);		pop_call();	return;
		case EDIT_TYPE_ROOM:	do_redit (ch, argument);		pop_call();	return;
		case EDIT_TYPE_MOB:		do_medit (ch, argument);		pop_call();	return;
		case EDIT_TYPE_OBJ:		do_oedit (ch, argument);		pop_call();	return;
		case EDIT_TYPE_RESET:	do_rredit(ch, argument);		pop_call();	return;
		case EDIT_TYPE_SHOP:	do_sedit (ch, argument);		pop_call();	return;
		case EDIT_TYPE_MPROG:	do_mpedit(ch, argument);		pop_call();	return;
		case EDIT_TYPE_OPROG:	do_opedit(ch, argument);		pop_call();	return;
		case EDIT_TYPE_AREA:	do_aedit (ch, argument);		pop_call();	return;
	}

	if (argument[0] == '\0')
	{
		ch_printf(ch, "Syntax: edit room  [create|delete|dupe] [vnum]\n\r");
		ch_printf(ch, "Syntax: edit mob   [create|delete]      <vnum>\n\r");
		ch_printf(ch, "Syntax: edit obj   [create|delete]      <vnum>\n\r");
		ch_printf(ch, "Syntax: edit shop  [create|delete]      <vnum>\n\r");
		ch_printf(ch, "Syntax: edit help  [create|delete]      <name>\n\r");
		ch_printf(ch, "Syntax: edit area  [check|assign]       [name]\n\r");
		ch_printf(ch, "Syntax: edit reset                      [vnum]\n\r");
		ch_printf(ch, "Syntax: edit mprog                      <vnum>\n\r");
		ch_printf(ch, "Syntax: edit oprog                      <vnum>\n\r");
		pop_call();
		return;
	}

	log_god_printf("(G) Log %s: edit %s", ch->name, argument);

	argument = one_argument(argument, arg1);
	strcpy(arg3, argument);
	argument = one_argument(argument, arg2);

	if (!strcasecmp(arg1, "room"))
	{
		if (!strcasecmp(arg2, "create"))
		{
			do_rcreate(ch, argument);
		}
		else if (!strcasecmp(arg2, "delete"))
		{
			do_rdelete(ch, argument);
		}
		else if (!strcasecmp(arg2, "dupe"))
		{
			do_rdupe(ch, argument);
		}
		else
		{
			do_redit(ch, arg2);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "mob"))
	{
		if (!strcasecmp(arg2, "create"))
		{
			do_mcreate(ch, argument);
		}
		else if (!strcasecmp(arg2, "delete"))
		{
			do_mdelete(ch, argument);
		}
		else
		{
			do_medit(ch, arg2);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "obj"))
	{
		if (!strcasecmp(arg2, "create"))
		{
			do_ocreate(ch, argument);
		}
		else if (!strcasecmp(arg2, "delete"))
		{
			do_odelete(ch, argument);
		}
		else
		{
			do_oedit(ch, arg2);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "help"))
	{
		if (!strcasecmp(arg2, "create"))
		{
			do_hcreate(ch, argument);
		}
		else if (!strcasecmp(arg2, "delete"))
		{
			do_hdelete(ch, argument);
		}
		else
		{
			do_hedit(ch, arg2);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "shop"))
	{
		if (!strcasecmp(arg2, "create"))
		{
			do_screate(ch, argument);
		}
		else if (!strcasecmp(arg2, "delete"))
		{
			do_sdelete(ch, argument);
		}
		else
		{
			do_sedit(ch, arg2);
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "mprog"))
	{
		do_mpedit(ch, arg2);

		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "oprog"))
	{
		do_opedit(ch, arg2);

		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "area"))
	{
		if (!strcasecmp(arg2, "check"))
		{
			do_area_check(ch, argument);
		}
		else if (!strcasecmp(arg2, "assign"))
		{
			do_area_assign(ch, argument);
		}
		else
		{
			do_aedit(ch, arg3);
		}

		pop_call();
		return;
	}

	if (!strcasecmp(arg1, "reset"))
	{
		do_rredit(ch, arg2);

		pop_call();
		return;
	}

	do_edit(ch, "");
	pop_call();
	return;
}
