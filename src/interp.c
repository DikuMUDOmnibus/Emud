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

#include <sys/time.h>
#include "emud.h"

bool check_social args( ( CHAR_DATA *ch, char *command, char *argument ) );

void    process_command( CHAR_DATA *, char *);
void preprocess_command( CHAR_DATA *, char *);

/*
	Command logging types.
*/

#define LOG_NORMAL      0
#define LOG_ALWAYS      1
#define LOG_NEVER       2

/*
	Command table.
*/

sh_int cmd_gsn[512];

const struct cmd_type cmd_table [] =
{
	{ "/",			do_recall,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "?",			do_grep,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_HIDE|CMD_BERSERK},
	{ ".",			do_chat,			POS_SLEEPING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ ",",			do_emote,			POS_RESTING,	-9,			LOG_NORMAL,	CMD_HIDE},
	{ ";",			do_gtell,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_HIDE},
	{ "'",			do_say,			POS_RESTING,	-9,			LOG_NORMAL,	CMD_HIDE},
	{ ":",			do_channel_talk,	POS_DEAD,		 1,			LOG_NORMAL,	CMD_HIDE},
	{ "]",			do_immtalk,		POS_SLEEPING,	96,			LOG_NORMAL,	CMD_HIDE},

	/*
		alphabetical ordered for indexing - Scandum
	*/

	{ "a",			do_a,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_HIDE},
	{ "acupunch",		do_acupunch,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "advance",		do_advance,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "affects",		do_affects,		POS_DEAD,		-8,			LOG_NORMAL,	CMD_BERSERK},
	{ "afk",			do_afk,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "alias",		do_alias,			POS_DEAD,		-3,			LOG_NORMAL,	CMD_NONE},
	{ "ambush",		do_ambush,		POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "areas",		do_areas,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "arrest",		do_arrest,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "assassin",		do_assassin,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "assassinate",	do_assassinate,	POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "at",			do_at,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "attack",		do_attack,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "auto",			do_auto,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_BERSERK},


	{ "buy",			do_buy,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "bank",			do_bank,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "banish",		do_ban,			POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "backstab",		do_backstab,		POS_STANDING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "bash",			do_bash,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "beep",			do_beep,			POS_SLEEPING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "berserk",		do_berserk,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "block",		do_block,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "bloodfrenzy",	do_bloodfrenzy,	POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "bounty",		do_bounty,		POS_RESTING,	 1,			LOG_NORMAL, 	CMD_NONE},
	{ "brandish",		do_brandish,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "buffer",		do_buffer,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "buildlight",	do_buildlight, 	POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},


	{ "cast",			do_cast,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_NONE},
	{ "castle",		do_castle,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "camp",			do_camp,			POS_RESTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "chat",			do_chat,			POS_SLEEPING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "clans",		do_clans,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "clanmessage",	do_clan_message,	POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "clanset",		do_clanset,		POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},
	{ "clanwhere",		do_clanwhere,		POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},
	{ "class",		do_class,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "clearpath",		do_clear_path,		POS_STANDING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "cloak",		do_cloak,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "close",		do_close,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "clock",		do_clock,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "circle",		do_circle,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "coffer",		do_coffer,		POS_RESTING,	10,  		LOG_ALWAYS,	CMD_HIDE},
	{ "color",		do_color,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "commands",		do_commands,		POS_DEAD,		-8,			LOG_NORMAL,	CMD_HIDE},
	{ "compare",		do_compare,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "connect",		do_connect,		POS_DEAD,		MAX_LEVEL-1,	LOG_NORMAL,	CMD_NONE},
	{ "consider",		do_consider,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "constitution",	do_constitution,	POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "config",		do_config,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "copyover",		do_copyover,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_QUIT},
	{ "credits",		do_credits,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "cripple",		do_cripple,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},


	{ "down",			do_down,			POS_STANDING,	-8,			LOG_NORMAL,	CMD_IDLE|CMD_BERSERK},
	{ "death",		do_death,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "delete",		do_delete,		POS_DEAD,		 1,			LOG_NEVER,	CMD_NONE},
	{ "deny",			do_deny,			POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "description",	do_description,	POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "destroy",		do_destroy,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "dexterity",		do_dexterity,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "disarm",		do_disarm,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "disconnect",	do_disconnect,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "disguise",		do_disguise,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "dismount",		do_dismount,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "distract",		do_distract,		POS_STANDING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "divert",		do_divert,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "donate",		do_donate,		POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},
	{ "doorbash",		do_bashdoor,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "doorset",		do_door,			POS_DEAD,		MAX_LEVEL-1,	LOG_NORMAL,	CMD_NONE},
	{ "drink",		do_drink,			POS_RESTING,	~ 1,			LOG_NORMAL,	CMD_NONE},
	{ "drain",		do_drain,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "drop",			do_drop,			POS_RESTING,	-9,			LOG_NORMAL,	CMD_NONE},
	{ "dump",			do_dump,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},


	{ "east",			do_east,			POS_STANDING,	-8,			LOG_NORMAL,	CMD_IDLE|CMD_BERSERK},
	{ "eat",			do_eat,			POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "echo",			do_echo,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "edit",			do_edit,			POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "email",		do_email,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "emote",		do_emote,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "engrave",		do_engrave,		POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "enter",		do_enter,			POS_STANDING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "equipment",		do_equipment,		POS_DEAD,		-8,			LOG_NORMAL,	CMD_NONE},
	{ "etell",		do_etell,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "evaluate",		do_evaluate,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "examine",		do_examine,		POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "exits",		do_exits,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},


	{ "flee",			do_flee,			POS_FIGHTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "fill",			do_fill,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "finger",		do_finger,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "find",			do_find,			POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "fixpass",		do_fixpass,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "follow",		do_follow,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "forage",		do_forage,		POS_STANDING,	1,			LOG_NORMAL,	CMD_HIDE},
	{ "force",		do_force,			POS_DEAD,		MAX_LEVEL-3,	LOG_ALWAYS,	CMD_NONE},
	{ "forceren",		do_forceren,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_HIDE},
	{ "forcerent",		do_forcerent,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "forge",		do_forge,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "fos",			do_fos,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "freeze",		do_freeze,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},


	{ "get",			do_get,			POS_RESTING,	-9,			LOG_NORMAL,	CMD_NONE},
	{ "give",			do_give,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "glance",		do_glance,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "gohome",		do_gohome,		POS_RESTING,	10,  		LOG_NORMAL,	CMD_HIDE},
	{ "goto",			do_goto,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "gouge",		do_gouge,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "grep",			do_grep,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE|CMD_BERSERK},
	{ "group",		do_group,			POS_SLEEPING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "gtell",		do_gtell,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},


	{ "help",			do_help,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "headbutt",		do_head_butt,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "heal",			do_heal,			POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},
	{ "hearlog",		do_hearlog,		POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "hide",			do_hide,			POS_RESTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "history",		do_history,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "hold",			do_wear,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_HIDE},
	{ "holylight",		do_holylight,		POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "honorific",		do_honorific,		POS_SLEEPING,  90,			LOG_NORMAL,	CMD_NONE},
	{ "html",			do_html,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "hunt",			do_hunt,			POS_STANDING,	-3,			LOG_NORMAL,	CMD_HIDE},


	{ "inventory",		do_inventory,		POS_DEAD,		-8,			LOG_NORMAL,	CMD_NONE},
	{ "identify",		do_identify,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "immtalk",		do_immtalk,		POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "initiate",		do_initiate,		POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},
	{ "intelligence",	do_intelligence,	POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "invis",		do_invis,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},


	{ "kill",           do_kill,			POS_FIGHTING,	-9,			LOG_NORMAL,	CMD_BERSERK},
	{ "kick",			do_kick,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE|CMD_BERSERK},
	{ "knife",		do_knife,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "know",			do_know,			POS_RESTING,	1,			LOG_NORMAL,	CMD_HIDE},


	{ "look",			do_look,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE|CMD_BERSERK},
	{ "language",		do_language,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "level",		do_level,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "list",			do_list,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "load",			do_load,			POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "lock",			do_lock,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "log",			do_log,			POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},


	{ "map",			do_map,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "makeclan",		do_makeclan,		POS_RESTING,	10,  		LOG_NORMAL,	CMD_HIDE},
	{ "makeflash",		do_make_flash,		POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "makepoison",	do_make_poison,	POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "mana",			do_mana,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "mass",			do_mass,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "maze",			do_maze,			POS_DEAD,		MAX_LEVEL-1,	LOG_ALWAYS,	CMD_NONE},
	{ "morph",		do_morph,			POS_DEAD,		-7,			LOG_NORMAL,	CMD_NONE},
	{ "most",			do_most,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "mount",		do_mount,			POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "move",			do_move,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "multiclass",	do_multi,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "murde",		do_murde,			POS_STANDING,	 5,			LOG_NORMAL,	CMD_HIDE},
	{ "murder",		do_murder,		POS_STANDING,	 5,			LOG_NORMAL,	CMD_BERSERK},
	{ "mute",			do_mute,			POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "mpaset",		do_mpaset,		POS_DEAD,		97,			LOG_ALWAYS,	CMD_HIDE},
	{ "mpasound",		do_mpasound,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpareaecho",	do_mpareaecho,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpdamage",		do_mpdamage,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpdelay",		do_mpdelay,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpjunk",		do_mpjunk,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpjunkperson",	do_mpjunk_person,	POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpecho",		do_mpecho,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpechoat",		do_mpechoat,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpechoaround",	do_mpechoaround,	POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpgorand",		do_mpgorand,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpkill",		do_mpkill,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpzset",		do_mpzset,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpcalculate",	do_mpcalculate,	POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpdo",			do_mpdo,			POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpmload",		do_mpmload, 		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpoload",		do_mpoload, 		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mppurge",		do_mppurge, 		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpquiet",		do_mpquiet, 		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpgoto",		do_mpgoto,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpat",			do_mpat,			POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mptransfer",	do_mptransfer,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mptrigger",		do_mptrigger,		POS_DEAD,		97,			LOG_NORMAL,	CMD_HIDE},
	{ "mpforce",		do_mpforce, 		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mplog",		do_mplog,			POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpmset",		do_mpmset,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mposet",		do_mposet,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpmadd",		do_mpmadd,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpoadd",		do_mpoadd,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},
	{ "mpswap",		do_mpswap,		POS_DEAD,		98,			LOG_NORMAL,	CMD_HIDE},


	{ "north",		do_north,			POS_STANDING,	-8,			LOG_NORMAL,	CMD_IDLE|CMD_BERSERK},
	{ "notice",		do_notice,		POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "nbanish",		do_nban,			POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "nominate",		do_nominate,		POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},
	{ "notag",		do_notag,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "note",			do_note,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},


	{ "open",			do_open,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "order",		do_order,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "outcast",		do_outcast,		POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},


	{ "put",			do_put,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "pardon",		do_pardon,		POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "password",		do_password,		POS_DEAD,		 1,			LOG_NEVER, 	CMD_NONE},
	{ "peace",		do_peace,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "peek",			do_peek,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "pick",			do_pick,			POS_RESTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "plan",			do_plan,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "plant",		do_plant,	 		POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "pledge",		do_pledge,		POS_RESTING,	20,  		LOG_NORMAL,	CMD_HIDE},
	{ "pload",		do_pload,			POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "pquit",		do_pquit,			POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "polymorph",		do_polymorph,		POS_STANDING,  -3,			LOG_NORMAL,	CMD_HIDE},
	{ "poof",			do_poof,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "port",			do_port,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "pose",			do_pose,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "practice",		do_practice,		POS_SLEEPING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "prog",			do_prog,			POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "prompt",		do_prompt,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "purge",		do_purge,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},


	{ "quaff",		do_quaff,			POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "qui",			do_qui,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_HIDE},
	{ "quit",			do_quit,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_BERSERK|CMD_QUIT},


	{ "reply",		do_reply,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "rangecast",		do_range_cast,		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "reboo",		do_reboo,			POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_HIDE},
	{ "reboot",		do_reboot,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_QUIT},
	{ "recall",		do_recall,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "recite",		do_recite,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "refresh",		do_refresh,		POS_DEAD,		-8,			LOG_NORMAL,	CMD_BERSERK},
	{ "reincarnate",	do_reincarnate,	POS_STANDING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "relog",		do_relog,			POS_STANDING,   1,  		LOG_NORMAL,	CMD_BERSERK|CMD_QUIT},
	{ "release",		do_release,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "rename",		do_rename,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "renounce",		do_renounce,		POS_RESTING,	10,			LOG_NORMAL,	CMD_HIDE},
	{ "rent",			do_rent,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "remove",		do_remove,		POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "report",		do_report,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "request",		do_request,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "rescale",		do_rescale,		POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "rescue",		do_rescue,		POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "resetarea",		do_resetarea,		POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "rest",			do_rest,			POS_SLEEPING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "restore",		do_restore,		POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "return",		do_return,		POS_DEAD,		-8,			LOG_NORMAL,	CMD_NONE},
	{ "revert",		do_revert,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},


	{ "south",		do_south,			POS_STANDING,	-8,			LOG_NORMAL,	CMD_IDLE|CMD_BERSERK},
	{ "say",			do_say,			POS_RESTING,	-9,			LOG_NORMAL,	CMD_NONE},
	{ "sacrifice",		do_sacrifice,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "save",			do_save,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_HIDE},
	{ "savearea",		do_savearea,		POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "scan",			do_scan,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_BERSERK},
	{ "score",		do_score,			POS_DEAD,		-7,			LOG_NORMAL,	CMD_BERSERK},
	{ "search",		do_search,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "sell",			do_sell,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "set",			do_set,			POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "shadow",		do_shadow,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "shoot",		do_shoot,	 		POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "shout",		do_shout,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "show",			do_show,			POS_DEAD,      MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "shutdow",		do_shutdow,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_HIDE},
	{ "shutdown",		do_shutdown,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_QUIT},
	{ "silence",		do_silence,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "sing",			do_sing,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "sit",			do_sit,			POS_SLEEPING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "skills",		do_skills,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "sla",			do_sla,			POS_DEAD,		MAX_LEVEL-3,	LOG_NORMAL,	CMD_HIDE},
	{ "slaughter",      do_slaughter,		POS_DEAD,		MAX_LEVEL-1,	LOG_NORMAL,	CMD_NONE},
	{ "slay",			do_slay,			POS_DEAD,		MAX_LEVEL-3,	LOG_ALWAYS,	CMD_NONE},
	{ "sleep",		do_sleep,			POS_SLEEPING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "socials",		do_socials,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "snatch",		do_snatch,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "sneak",		do_sneak,			POS_STANDING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "snoop",		do_snoop,			POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "spam",			do_spam,			POS_DEAD,		-7,			LOG_NORMAL,	CMD_NONE},
	{ "speak",		do_speak,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "speed",		do_speed,			POS_DEAD,		-9,			LOG_NORMAL,	CMD_NONE},
	{ "split",		do_split,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "spy",			do_spy,			POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "stand",		do_stand,			POS_SLEEPING,	-9,			LOG_NORMAL,	CMD_NONE},
	{ "stat",			do_stat,			POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "status",		do_status,		POS_DEAD,		-7,			LOG_NORMAL,	CMD_BERSERK},
	{ "steal",		do_steal,			POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "stealth",		do_stealth,		POS_STANDING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "stop",			do_visible,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "strength",		do_strength,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "strangle",		do_strangle,		POS_STANDING,  -3,			LOG_NORMAL,	CMD_HIDE},
	{ "suicide",		do_suicide,		POS_DEAD,		 1,			LOG_NEVER,	CMD_NONE},
	{ "switch",		do_switch,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},


	{ "tell",			do_tell,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "tag", 			do_tag,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "take",			do_get,			POS_RESTING,	-9,			LOG_NORMAL,	CMD_HIDE},
	{ "talk",			do_channel_talk,	POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "tame",			do_tame,			POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "tactical",		do_tactical,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "test",			do_test,			POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_HIDE},
	{ "throw",		do_throw,			POS_FIGHTING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "tick",			do_tick,			POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "title",		do_title,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "time",			do_time,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "timemode",		do_timemode,		POS_DEAD,		MAX_LEVEL,	LOG_NORMAL,	CMD_NONE},
	{ "train",		do_train,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "track",		do_track,			POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},
	{ "trip",			do_trip,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "transfer",		do_transfer,		POS_DEAD,		MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},
	{ "travel",		do_travel,		POS_STANDING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "trivia",		do_trivia,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},


	{ "up",			do_up,			POS_STANDING,	-8,			LOG_NORMAL,	CMD_IDLE|CMD_BERSERK},
	{ "unalias",		do_unalias,		POS_DEAD,		-3,			LOG_NORMAL,	CMD_NONE},
	{ "unlock",		do_unlock,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "usage",		do_usage,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},


	{ "vt102",		do_vt100,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "value",		do_value,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "version",		do_version,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "victory",		do_victory_list,	POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "voice",		do_voice,			POS_RESTING,	 1,			LOG_NORMAL,	CMD_HIDE},


	{ "west",			do_west,			POS_STANDING,	-8,			LOG_NORMAL,	CMD_IDLE|CMD_BERSERK},
	{ "wake",			do_wake,			POS_SLEEPING,	-9,			LOG_NORMAL,	CMD_NONE},
	{ "wear",			do_wear,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "weather",		do_weather,		POS_RESTING,	 1,			LOG_NORMAL,	CMD_NONE},
	{ "where",		do_where,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_NONE},
	{ "whirl",		do_whirl,			POS_FIGHTING,	-3,			LOG_NORMAL,	CMD_HIDE},
	{ "who",			do_who,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "wield",		do_wear,			POS_RESTING,	-8,			LOG_NORMAL,	CMD_HIDE},
	{ "wimpy",		do_wimpy,			POS_DEAD,		 1,			LOG_NORMAL,	CMD_BERSERK},
	{ "wisdom",		do_wisdom,		POS_DEAD,		 1,			LOG_NORMAL,	CMD_NONE},
	{ "wizhelp",		do_wizhelp,		POS_DEAD,      MAX_LEVEL-3,	LOG_NORMAL,	CMD_NONE},
	{ "wizlock",		do_wizlock,		POS_DEAD,		MAX_LEVEL,	LOG_ALWAYS,	CMD_NONE},
	{ "wizmap",		do_wizmap,		POS_DEAD,      MAX_LEVEL-2,	LOG_NORMAL,	CMD_NONE},


	{ "zap",			do_zap,			POS_STANDING,	 1,			LOG_NORMAL,	CMD_HIDE},


	{ "",			NULL,			POS_DEAD,	 	 0,			LOG_NORMAL,	CMD_HIDE}
};

/*
	The social table.
	Add new socials here.
	Alphabetical order is not required.
*/

const struct social_type social_table [] =
{
	{
		"accuse",
		"Accuse whom?",
		"$n is in an accusing mood.",
		"You look accusingly at $M.",
		"$n looks accusingly at $N.",
		"$n looks accusingly at you.",
		"You accuse yourself.",
		"$n seems to have a bad conscience."
	},

	{
		"ack",
		"You gasp and say 'ACK!' at your mistake.",
		"$n ACKS at $s big mistake.",
		"You ACK $M.",
		"$n ACKS $N.",
		"$n ACKS you.",
		"You ACK yourself.",
		"$n ACKS $mself.  Must be a bad day."
	},

	{
		"addict",
		"You stand and admit to all in the room 'Hi, I'm $n, and I'm a mud addict'",
		"$n stands and says 'Hi, I'm $n, and I'm a mud addict'",
		"You tell $N that you are addicted to $S love.",
		"$n tells $N that $e is addicted to $S love.",
		"$n tells you that $e is addicted to your love.",
		"You stand and admit to yourself that you are a mud addict. Maybe you should tell everyone else now?",
		"$n is mumbling to $mself, probably $e's been mudding too long."
	},

	{
		"admire",
		"Admire whom?",
		 NULL,
		"You admire $N's beauty.",
		"$n admires $N's beauty.",
		"$n admires your beauty.",
		"You admire yourself in a mirror.",
		"$n admires $mself in a mirror."
	},

	{
		"airguitar",
		"You grab your air guitar and play for all you're worth!",
		"$n bangs $s head, air-guitaring like a mad man!",
		"You play a most excellent tune for $N on your air-guitar.",
		"$n plays a most excellent tune for $N on $s air-guitar.",
		"$n plays a most excellent tune for you on $s air-guitar.",
		"You spin around in circles, air-guitaring in a total frenzy!",
		"$n spins around in circles, air-guitaring in a total frenzy!",
	},

	{
		"applaud",
		"Clap, clap, clap.",
		"$n gives a round of applause.",
		"You clap at $S actions.",
		"$n claps at $N's actions.",
		"$n gives you a round of applause.  You MUST have done something good!",
		"You applaud at yourself.  Boy, are we conceited!",
		"$n applauds at $mself.  Boy, are we conceited!"
	},

	{
		"backpat",
		"Backpat whome?",
		 NULL,
		"You pat $M on the back for a job well done.",
		"$n pats $N on the back.",
		"$n pats you on the back for a job well done.",
		"You pat yourself on the back vainly.",
		"$n vainly pats $mself on the back."
	},

	{
		"ballad",
		"You raise your clear voice towards the sky.",
		"$n has begun to sing a bit offkey.",
		"You sing a ballad to $M.",
		"$n sings a ballad to $N.",
		"$n sings a ballad to you!  How sweet!",
		"You sing a little ditty to yourself.",
		"$n sings a little ditty to $mself."
	},

	{
		"bark",
		"Woof!  Woof!",
		"$n barks like a dog.",
		"You bark at $M.",
		"$n barks at $N.",
		"$n barks at you.",
		"You bark at yourself.  Woof!  Woof!",
		"$n barks at $mself.  Woof!  Woof!"
	},

	{
		"bat",
		"You bat your long, dark lashes and smile innocently.",
		"$n bats $s long, dark lashes and smiles innocently.",
		"You bat your long, dark lashes at $M.",
		"$n bats $s long, dark lashes at $N, and smiles innocently at $M.",
		"$n bats $s long, dark lashes and smiles innocently at you.",
		"Awww.. You're such an adorable being.",
		"$n looks cross-eyed while batting $s long, dark lashes."
	},

	{
		"beam",
		"You beam delightedly at nothing in particular.",
		"$n beams broadly at nothing in particular.",
		"You dazzle $N with your smile.",
		"$n beams at $N broadly, $e must like $M a great deal.",
		"$n beams at you broadly, $e must like you a great deal.",
		"Well, aren't you special?",
		"$n beams a $mself, obviously pleased."
	},

	{
		"bearhug",
		"Whom do you wish to bearhug?",
		 NULL,
		"You overwhelm $N with a great, big, hulking bearhug.",
		"$n encompasses $N in an ENORMOUS bearhug!",
		"$n clutches you tightly in an overwhelming bearhug!",
		"You vainly clutch yourself in a bearhug for consolation.",
		"$n hugs $mself as tightly as possible.  Looks like $e could use a hand.",
	},

	{
		"beckon",
		"You beckon for everyone to follow.",
		"$n beckons for everyone to follow.",
		"You beckon for $M to follow - sure hope they do!",
		"$n beckons $N to follow... hmmm what's going on?",
		"$n beckons for you to follow.",
		"You beckon to your shadow to follow.",
		"$n beckons to $s shadow to follow."
	},

	{
		"beer",
		"You down a cold, frosty beer.",
		"$n downs a cold, frosty beer.",
		"You draw a cold, frosty beer for $N.",
		"$n draws a cold, frosty beer for $N.",
		"$n draws a cold, frosty beer for you.",
		"You draw yourself a beer.",
		"$n draws $mself a beer."
	},

	{
		"bleed",
		"You bleed all over the room!",
		"$n bleeds all over the room!  Get out of $s way!",
		"You bleed all over $M!",
		"$n bleeds all over $N.  Better leave, you may be next!",
		"$n bleeds all over you!  YUCK!",
		"You bleed all over yourself!",
		"$n bleeds all over $mself."
	},

	{
		"beg",
		"You beg the gods for mercy.",
		"The gods fall down laughing at $n's request for mercy.",
		"You desperately beg $M for help.",
		"$n begs $N for assistance!",
		"$n begs you for help.",
		"Begging yourself for help wouldn't be very smart.",
		"$n begs $mself for help, how ingenious."
	},

	{
		"blink",
		"You blink in utter disbelief.",
		"$n blinks in utter disbelief.",
		"You blink at $M in confusion.",
		"$n blinks at $N in confusion.",
		"$n blinks at you in confusion.",
		"You are sooooooooooooo confused.",
		"$n blinks at $mself in complete confusion."
	},

	{
		"blush",
		"Your cheeks are burning.",
		"$n blushes.",
		"Your cheeks turn red seeing $M here.",
		"$n blushes as $e sees $N here.",
		"$n blushes as $e sees you here.  Such an effect on people!",
		"You blush at your actions.",
		"$n blushes at $s actions."
	},

	{
		"bong",
		"Are you just gonna stare at it?",
		 NULL,
		"You pass the bong to $M.",
		"$n passes the bong to $N.",
		"$n passes you the bong.",
		"You light the bong and take a hit.",
		"$n lights the bong and takes a hit."
	},

	{
		"bonk",
		"*** !!BONK!! ***",
		"$n charges up and goes *** !!BONK!! ***",
		"You bonk $M on the head for being such a moron.",
		"$n bonks $N on the head for being such an UTTER moron.",
		"$n bonks you on the head for being so foolish.",
		"You bonk yourself on the head, fool that you are!",
		"$n bonks $mself and grimaces in pain."
	},

	{
		"bounce",
		"BOIINNNNNNGG!",
		"$n bounces around happily.",
		"You bounce onto $S lap.",
		"$n bounces onto $N's lap.",
		"$n bounces onto your lap.",
		"You bounce your head like a basketball.",
		"$n plays basketball with $s head."
	},

	{
		"bow",
		"You bow deeply.",
		"$n bows deeply flourishing $s cape.",
		"You bow before $M.",
		"$n bows before $N.",
		"$n bows before you, flourishing $s cape.",
		"You kiss your toes; how flexable.",
		"$n folds up like a jack knife and kisses $s own toes."
	},

	{
		"bungy",
		"You tie a bungy cord to the mud and jump into the internet.",
		"$n ties a bungy cord to the mud and jumps into the internet.",
		"You tie a bungy cord to $N and throw $M off the mud.",
		"$n ties a bungy cord to $N and throws $M off the mud.",
		"$n ties a bungy cord to you and throws you off the mud.",
		"You tie a bungy cord to yourself and jump off the mud .",
		"$n ties a bungy cord to $mself and jumps off the mud."
	},

	{
		"burp",
		"Excuse yourself.",
		"$n burps loudly.",
		"You burp loudly to $M in response.",
		"$n burps loudly in response to $N's remark.",
		"$n burps loudly in response to your remark.",
		"You burp at yourself.",
		"$n burps at $mself. How rude."
	},

	{
		"bye",
		"You say goodbye to all in the room.",
		"$n says goodbye to everyone in the room, including you!",
		"You say goodbye to all in the room, and especially to $N.",
		"$n says goodbye to everyone in the room, including you!",
		"$n says goodbye to everyone in the room, and especially you!",
		"You say goodbye to all in the room.",
		"$n says goodbye to everyone in the room, including you!"
	},

	{
		"cackle",
		"You throw back your head and cackle with insane glee!",
		"$n throws back $s head and cackles with insane glee!",
		"You cackle gleefully at $N",
		"$n cackles gleefully at $N.",
		"$n cackles gleefully at you.  Better keep your distance from $m.",
		"You cackle at yourself.  Now, THAT'S strange!",
		"$n is really crazy now!  $e cackles at $mself!"
	},

	{
		"cap",
		"You tip your cap gallantly.",
		"$n tips $s cap gallantly.",
		"You tip your cap to $N gallantly.",
		"$n tips $s cap gallantly for $N.",
		"$n tips $s cap to you gallantly.",
		"You tip your cap over your head for a nap.",
		"$n tips $s cap over $s head for a nap."
	},

	{
		"cheek",
		"Kiss whose cheek?",
		 NULL,
		"You lean forward and kiss $M gently on the cheek.",
		"$n leans forward and kisses $N gently on the cheek.",
		"$n leans forward and kisses you gently on the cheek.",
		"People might start to talk..",
		 NULL
	},

	{
		"cheer",
		"You cheer with great gusto.",
		"$n cheers with great gusto.",
		"You cheer at $M with great gusto.",
		"$n cheers for $N with great gusto.",
		"$n cheers for you with great gusto.",
		"You cheer for yourself since no one else does.",
		"$n cheers for $mself since no one else does."
	},

	{
		"choke",
		"Choke whom?",
		 NULL,
		"You grab $M by the neck and shake violently.",
		"$n grabs $N by the neck and shakes violently.",
		"$n grabs YOU by the neck and shakes violently.  Ouch!",
		"You grab your own neck, and start shaking violently.",
		"$n grabs $s own neck and shakes violently."
	},

	{
		"chuckle",
		"You chuckle politely.",
		"$n chuckles politely.",
		"You chuckle at $S actions.",
		"$n chuckles at $N's actions.",
		"$n chuckles at your actions.",
		"You chuckle at your own joke, since no one else would.",
		"$n chuckles at $s own joke, since none of you would."
	},

	{
		"clap",
		"You clap your hands together.",
		"$n shows $s approval by clapping $s hands together.",
		"You clap at $S performance.",
		"$n claps at $N's performance.",
		"$n claps at your performance.",
		"You clap at your own performance.",
		"$n claps at $s own performance."
	},

	{
		"clueless",
		"You chastise everyone in the room for being clueless.",
		"$n chastises everyone for being clueless.",
		"You chastise $M for being clueless.",
		"$n chastises $N for being clueless.",
		"$n chastises you for being clueless.",
		"You chastise yourself for being clueless.  What a dweeb you are!",
		"$n chastises $mslef for being clueless.  What a dweeb!"
	},

	{
		"comb",
		"You comb your hair - perfect.",
		"$n combs $s hair, how dashing!",
		"You comb $S hair and make it shine.",
		"$n patiently combs $N's hair and makes it shine.",
		"$n combs your hair and makes it shine.",
		"You pull your hair attempting to comb it.",
		"$n tries to comb $s tangled hair."
	},

	{
		"comfort",
		"Aww, what's wrong?",
		"$n needs someone to comfort $m.",
		"You comfort $M.",
		"$n comforts $N.",
		"$n comforts you.",
		"You make a vain attempt to comfort yourself.",
		"$n has no one to comfort $m but $mself."
	},

	{
		"cough",
		"You cough to clear your throat and eyes and nose and....",
		"$n coughs loudly.",
		"You cough loudly.  It must be $S fault, $E gave you this cold.",
		"$n coughs loudly, and glares at $N, like it is $S fault.",
		"$n coughs loudly, and glares at you.  Did you give $M that cold?",
		"You cough loudly.  Why don't you take better care of yourself?",
		"$n coughs loudly.  $n should take better care of $mself."
	},

	{
		"cringe",
		"You cringe in terror.",
		"$n cringes in terror!",
		"You cringe away from $M.",
		"$n cringes away from $N in mortal terror.",
		"$n cringes away from you.",
		"I beg your pardon?",
		 NULL
	},

	{
		"cry",
		"Poor baby.",
		"$n bursts into tears.",
		"You cry on $S shoulder.",
		"$n cries on $N's shoulder.",
		"$n cries on your shoulder.",
		"You cry to yourself.",
		"$n sobs quietly to $mself."
	},

	{
		"cuddle",
		"Whom do you feel like cuddling today?",
		NULL,
		"You cuddle $M warmly.",
		"$n cuddles $N warmly, you wish someone would cuddle you.",
		"$n cuddles you warmly.",
		"Get a life.",
		 NULL
	},

	{
		"curse",
		"You swear loudly.",
		"$n swears: @*&^%@*&!",
		"You swear at $M.",
		"$n swears at $N.",
		"$n swears at you! What did you do?",
		"You swear at your own mistakes.",
		"$n starts swearing at $mself for making a mistake."
	},

	{
		"curtsey",
		"You curtsey to your audience.",
		"$n curtseys gracefully.",
		"You curtsey to $M.",
		"$n curtseys gracefully to $N.",
		"$n curtseys gracefully for you.",
		"You curtsey to your audience (yourself).",
		"$n curtseys to $mself, since no one is paying attention to $m."
	},

	{
		"dance",
		"Feels silly, doesn't it?",
		"$n tries to break dance, but nearly breaks $s neck!",
		"You stick a rose between your teeth and sweep $M into a tango!",
		"$n sticks a rose between $s teeth and sweeps $N into a tango!",
		"$n sticks a rose between $s teeth and sweeps you into a tango!",
		"You skip and dance around by yourself.",
		"$n dances a pas-de-une."
	},

	{
		"doh",
		"You say, 'Doh!!'  and slap yourself.  What an idiot you are!",
		"$n slaps $mself and says, 'Doh!!!'",
		"You say, 'Doh!!'  and slap yourself.  What an idiot you are!",
		"$n slaps $mself and says, 'Doh!!!'",
		"$n slaps $mself and says, 'Doh!!!'  You made $m feel dumb.  Good job!",
		"You slap yourself and say, 'Doh!!!'",
		"$n slaps $mself and says, 'Doh!!!'"
	},

	{
		"drool",
		"You drool on yourself.",
		"$n drools on $mself.",
		"You drool all over $N.",
		"$n drools all over $N.",
		"$n drools all over you.",
		"You drool on yourself.",
		"$n drools on $mself."
	},

	{
		"dutch",
		"Going Dutch on whom?",
		 NULL,
		"You give $N a Dutch kiss.",
		"$n gives $N a Dutch kiss.",
		"$n gives you a Dutch kiss.",
		"You go Dutch on yourself.",
		"$n goes Dutch on $mself."
	},

	{
		"embrace",
		"Who do you want to hold?",
		"$n looks around for someone to hold close to $m.",
		"You hold $N in a warm and loving embrace.",
		"$n holds $N in a warm and loving embrace.",
		"$n holds you in a warm and loving embrace.",
		"You hold yourself in a warm and loving embrace.  Feels silly doesn't it?",
		"$n holds $mself in a warm and loving embrace ($e looks pretty silly, doesn't $e?)"
	},

	{
		"eskimo",
		"Who do you want to eskimo kiss with?",
		"$n is looking for someone to rub noses with.  Any volunteers?",
		"You rub noses with $M.",
		"$n rubs noses with $N.  You didn't know they were eskimos!",
		"$n rubs noses with you.",
		"You become cross-eyed from your attempts at rubbing your nose on yourself!",
		"$n is trying to rub $s nose on $mself.  That must hurt!"
	},

	{
		"eyebrow",
		"You raise an eyebrow at the notion.",
		"$n raises an eyebrow.",
		"You raise your eyebrow at $M.",
		"$n raises an eyebrow at $N's weird actions.",
		"$n raises an eyebrow at you.",
		"You make weird faces, practising your eyebrow raise skills.",
		"$n makes weird faces while practising eyebrow raising."
	},

	{
		"faint",
		"You faint from the aromas that linger within.",
		"$n faints from the aromas that linger within.",
		"You faint as you smell $N.",
		"$n faints as $e smells $N.",
		"$n faints as $e smells you.",
		"You faint as you smell your stench.",
		"$n faints as $e smells $mself."
	},

	{
		"fakerep",
		"You report: 7367/7367 hp 1049/1049 mana 2595/2595 mv 497189537 xp.",
		"$n reports: 7367/7367 hp 1049/1049 mana 2595/2595 mv 497189537 xp.",
		"You report: 7367/7367 hp 1049/1049 mana 2595/2595 mv 497189537 xp.",
		"$n reports: 7367/7367 hp 1049/1049 mana 2595/2595 mv 497189537 xp.",
		"$n reports: 7367/7367 hp 1049/1049 mana 2595/2595 mv 497189537 xp.",
		"You report: 7367/7367 hp 1049/1049 mana 2595/2595 mv 497189537 xp.",
		"$n reports: 7367/7367 hp 1049/1049 mana 2595/2595 mv 497189537 xp."
	},

	{
		"fart",
		"Where are your manners?",
		"$n lets off a real rip-roarer ... a greenish cloud envelops $n!",
		"You fart at $M.  Boy, you are sick.",
		"$n farts in $N's direction.  Better flee before $e turns to you!",
		"$n farts in your direction.  You gasp for air.",
		"You fart at yourself.  You deserve it.",
		"$n farts at $mself.  Better $m than you."
	},

	{
		"flash",
		"You grin and strut as you open your coat.",
		"$n rips $s clothes off and parades naked around the room!",
		"Swiftly you jerk open your jacket, trying to attract $S notice!",
		"Cover your eyes! $n is exposing $mself to $N! EWWWW!",
		"You gasp! $n has just revealed $s \"equipment\" to you!",
		"You sick lil bunny.",
		"(PERVERT ALERT) $n is trying to flash $mself!"
	},

	{
		"flip",
		"You flip head over heels.",
		"$n flips head over heels.",
		"You flip $M over your shoulder.",
		"$n flips $N over $s shoulder.",
		"$n flips you over $s shoulder.  Hmmmm.",
		"You tumble all over the room.",
		"$n does some nice tumbling and gymnastics."
	},

	{
		"flirt",
		"Wink wink!",
		"$n flirts -- probably needs a date, huh?",
		"You flirt with $M trying to get $S attention.",
		"$n flirts with $N trying to get $S attention.",
		"$n wants you to show some interest and is flirting with you.",
		"You flirt with yourself.",
		"$n flirts with $mself.  Hoo boy."
	},

	{
		"fondle",
		"Who needs to be fondled?",
		 NULL,
		"You fondly fondle $M.",
		"$n fondly fondles $N.",
		"$n fondly fondles you.",
		"You fondly fondle yourself, feels funny doesn't it ?",
		"$n fondly fondles $mself - this is going too far !!"
	},

	{
		"french",
		"Kiss whom?",
		 NULL,
		"You give $N a long and passionate kiss.",
		"$n kisses $N passionately.",
		"$n gives you a long and passionate kiss.",
		"You gather yourself in your arms and try to kiss yourself.",
		"$n makes an attempt at kissing $mself."
	},

	{
		"frown",
		"What's bothering you ?",
		"$n frowns.",
		"You frown at what $E did.",
		"$n frowns at what $N did.",
		"$n frowns at what you did.",
		"You frown at yourself.  Poor baby.",
		"$n frowns at $mself.  Poor baby."
	},

	{
    		"fuck",
	    	"Mind your language!",
		 NULL,
		"You swear at $N 'FUCK YOU!'",
		"$n swears at $N, rather rough language is used there!",
		"$n swears at you 'FUCK YOU!'",
		"You move in mysterious ways to make love with yourself.",
		"$n moves in mysterious ways to make love with $mself."
	},

	{
		"fume",
		"You grit your teeth and fume with rage.",
		"$n grits $s teeth and fumes with rage.",
		"You stare at $M, fuming.",
		"$n stares at $N, fuming with rage.",
		"$n stares at you, fuming with rage!",
		"That's right - hate yourself!",
		"$n clenches $s fists and stomps $s feet, fuming with anger."
	},

	{
		"gasp",
		"You gasp in astonishment.",
		"$n gasps in astonishment.",
		"You gasp as you realize what $e did.",
		"$n gasps as $e realizes what $N did.",
		"$n gasps as $e realizes what you did.",
		"You look at yourself and gasp!",
		"$n takes one look at $mself and gasps in astonishment!"
	},

	{
		"giggle",
		"You giggle.",
		"$n giggles.",
		"You giggle in $N's presence.",
		"$n giggles at $N's actions.",
		"$n giggles at you.  Hope it's not contagious!",
		"You giggle at yourself.  You must be nervous or something.",
		"$n giggles at $mself looking rather jittery."
	},

	{
		"glare",
		"You glare at nothing in particular.",
		"$n glares around $m.",
		"You glare icily at $M.",
		"$n glares at $N.",
		"$n glares icily at you, you feel cold to your bones.",
		"You glare icily at your feet, they are suddenly very cold.",
		"$n glares at $s feet, what is bothering $m?"
	},

	{
		"girn",
		"You try to grin, but somehow get it slightly wrong.",
		"$n tries to grin, but somehow gets it slightly wrong.",
		"You try to grin at $N, but $E gives you a funny look.",
		"$n tries to grin at $N, but screws up badly.",
		"$n turns $s lips in a sad attempt at a lopsided grin.",
		"Your face becomes a ghastly mask as you fail to grin.",
		"$n's face becomes a strange death mask as $s tries to grin."
	},

	{
		"grin",
		"You grin evilly.",
		"$n grins evilly.",
		"You grin evilly at $M.",
		"$n grins evilly at $N.",
		"$n grins evilly at you with a twinkle in $s eyes.",
		"You grin at yourself.  You must be getting very bad thoughts.",
		"$n grins at $mself.  You must wonder what $e is thinking."
	},

	{
		"groan",
		"You groan loudly.",
		"$n groans loudly.",
		"You groan at the sight of $M.",
		"$n groans at the sight of $N.",
		"$n groans at the sight of you.",
		"You groan as you realize what you have done.",
		"$n groans as $e realizes what $e has done."
	},

	{
		"grope",
		"Whom do you wish to grope?",
		 NULL,
		"Well, what sort of noise do you expect here?",
		"$n gropes $N.",
		"$n gropes you.",
		"You grope yourself - YUCK.",
		"$n gropes $mself - YUCK."
	},

	{
		"grovel",
		"You grovel in the dirt.",
		"$n grovels in the dirt.",
		"You grovel before $M.",
		"$n grovels at $N's feet.",
		"$n grovels in the dirt at your feet.",
		"That seems a little silly to me.",
		 NULL
	},

	{
		"growl",
		"Grrrrrrrrrr ...",
		"$n growls.",
		"Grrrrrrrrrr ... take that, $N!",
		"$n growls at $N.  Better leave the room before the fighting starts.",
		"$n growls at you.  Hey, two can play it that way!",
		"You growl at yourself.  Boy, do you feel bitter!",
		"$n growls at $mself.  This could get interesting..."
	},

	{
		"grumble",
		"You grumble.",
		"$n grumbles.",
		"You grumble to $M.",
		"$n grumbles to $N.",
		"$n grumbles to you.",
		"You grumble under your breath.",
		"$n grumbles under $s breath."
	},

	{
		"grunt",
		"Oink, oink.",
		"$n grunts like a pig.",
		"Oink, oink.",
		"$n grunts to $N.  What a pig!",
		"$n grunts to you.  What a pig!",
		"Oink, oink.",
		"$n grunts to nobody in particular.  What a pig!"
	},

	{
		"gulp",
		"ggggggguuuuuuulllllppppp!!!!!",
		"$n says 'That drink was nice.'",
		"$N thanks you for the drink.",
		"$N comsumes a drink from $n.",
		"$n gives you a refreshing drink which you gulp down.",
		"You gulp down a refreshing drink.",
		"$n gulps down a refreshing looking drink."
	},

	{
		"hand",
		"Kiss whose hand?",
		 NULL,
		"You kiss $S hand.",
		"$n kisses $N's hand.  How sweet!",
		"$n kisses your hand.  What a sweetie!",
		"You kiss your own hand.",
		"$n kisses $s own hand."
	},

	{
		"hello",
		"You say hello to everyone in the room.",
		"$n says hello to everyone in the room.",
		"You say tell $M how truly glad you are to see $M.",
		"$n tells $N 'Hi!'",
		"$n tells you how truly glad $e is you are here.",
		"You greet yourself enthusiastically.",
		"$n greets $mself enthusiastically.  How odd."
	},

	{
		"hangover",
		"You pop a few aspirin and put on your sunglasses.  Ow, your head hurts!",
		"$n holds $s head and says 'Quit breathing so loud!'",
		"Won't $N be quiet?  Your head is gonna split in two if $E keeps talking!",
		"$n complains to $N 'Be quiet!  I have a hangover!",
		"$n complains to you 'Be quiet!  I have a hangover!",
		"You shoosh yourself.  Be quiet!  You have a hangover!",
		"$n shooshes $mself.  Wow, what a hangover that must be!"
	},

 	{
		"highfive",
		"You jump in the air...oops, better get someone else to join you.",
		"$n jumps in the air by $mself.  Is $e a cheerleader, or just daft?",
		"You jump in the air and give $M a big highfive!",
		"$n jumps in the air and gives $N a big highfive!",
		"$n jumps in the air and gives you a big highfive!",
		"You jump in the air and congratulate yourself!",
		"$n jumps in the air and gives $mself a big highfive!  Wonder what $e did?"
	},

	{
		"hooter",
		"You light up a big, fat hooter.",
		"$n lights up a big, fat hooter.",
		"You roll a big, fat hooter for $N.",
		"$n rolls a big, fat hooter for $N.",
		"$n rolls a big, fat hooter for YOU!",
		"You roll yourself a big, fat hooter.",
		"$n rolls $mself a big, fat hooter."
	},

	{
		"hop",
		"You hop around like a little kid.",
		"$n hops around like a little kid.",
		"You hop at $N.",
		"$n hops at $N.",
		"$n hops at you.",
		"Maybe in your next life.",
		 NULL
	},

	{
		"howl",
		"You howl at the moon.",
		"$n howls at the moon.",
		"You howl soulfully at $N.",
		"$n howls at $N.",
		"$n howls soulfully at you.",
		"You throw back your head and howl in profound frustration!",
		"$n throws back $s head and howls in profound frustration!"
	},

	{
		"hug",
		"Hug whom?",
		 NULL,
		"You hug $M tightly.",
		"$n hugs $N tightly.",
		"$n hugs you tightly.",
		"You hug yourself.",
		"$n hugs $mself in a vain attempt to get friendship."
	},

	{
		"huggle",
		"Huggle whom?",
		 NULL,
		"You huggle $M.",
		"The world seems to brighten as $n huggles $N.",
		"$n mesmerises you with love as $e envelopes you in a huggle.",
		"Get a life.",
		 NULL
	},

	{
		"hum",
		"Hmm Hmm Hmm Hmmmmmmm.",
		"$n hums like a bee with a chest cold.",
		"You hum a little ditty for $M.  Hmm Hmm Hmm Hmmmmmm.",
		"$n hums a little ditty for $N.  Hmm Hmm Hmm Hmmmmmm.",
		"$n hums a little ditty for you.  Hmm Hmm Hmm Hmmmmmm.",
		"Hmm Hmm Hmmmmmmm.",
		"$n hums like a bee with a chest cold."
	},

	{
		"kiss",
		"Isn't there someone you want to kiss?",
		NULL,
		"You kiss $M.",
		"$n kisses $N.",
		"$n kisses you.",
		"All the lonely people :(",
		 NULL
	},

	{
		"knee",
		"You raise one knee, preparing to do justice.",
		"$n cocks one knee threateningly.",
		"You knee $M hard in the groin.",
		"$N doubles over in pain as $n knees $M in the groin.",
		"$n suddenly knees you in the groin, sending you down, writhing in pain!",
		"What ARE you thinking !?!?!?!?!?!?",
		"You turn around, shaking your head at $n's attempts at kneeing $mself."
	},

	{
		"laces",
		"Tie whose laces?",
		 NULL,
		"With the greatest of stealth, you tie $S shoelaces together.",
		"$n sneaks up to $N and ties $S shoelaces together.",
		"You try to take a step, and you hit the world facedown!  Someone must have tied your shoelaces together!",
		"You tie your own shoelaces together, try to walk, and promptly fall.",
		"$n cleverly ties $s own shoelaces together, tries to walk, and sprawls flat on the floor."
	},

	{
		"laugh",
		"You laugh.",
		"$n laughs.",
		"You laugh at $N mercilessly.",
		"$n laughs at $N mercilessly.",
		"$n laughs at you mercilessly.  Hmmmmph.",
		"You laugh at yourself.  I would, too.",
		"$n laughs at $mself.  Let's all join in!!!"
	},

	{
		"lag",
		"You complain about the terrible lag.",
		"$n starts complaining about the terrible lag.",
		"You complain to $N about the terrible lag.",
		"$n complains to $N about the terrible lag.",
		"$n complains to you about the terrible lag.",
		"You start muttering about the awful lag.",
		"$n starts muttering about the awful lag."
	},

	{
		"lick",
		"You lick your lips and smile.",
		"$n licks $s lips and smiles.",
		"You lick $M.",
		"$n licks $N.",
		"$n licks you.",
		"You lick yourself.",
		"$n licks $mself - YUCK."
	},

	{
		"liver",
		"You look angrily for someone to disembowel.",
		"$n angrily looks around for someone to molest.",
		"You rip out $S liver and eat it.",
		"$n rips out $N's liver and eats it.",
		"$n rips out your liver and eats it.",
		"You rip out your own liver and eat it.",
		"$n rips out $s liver and eats it, how fascinating!"
	},

	{
		"love",
		"You love the whole world.",
		"$n loves everybody in the world.",
		"You tell your true feelings to $N.",
		"$n whispers softly to $N.",
		"$n whispers to you sweet words of love.",
		"Well, we already know you love yourself (lucky someone does!)",
		"$n loves $mself, can you believe it ?"
	},

	{
		"maim",
		"Who do you want to maim?",
		"$n is looking for someone to maim.",
		"You maim $N with your dull fingernails.",
		"$n raises $s hand and tries to maim $N to pieces.",
		"$n raises $s hand and paws at you.  You've been maimed!",
		"You maim yourself with your dull fingernails.",
		"$n raises $s hand and maims $mself to pieces."
	},

	{
		"makelove",
		"Whom do you wish to make love with ?",
		NULL,
		"You make love with $N.",
		"$n makes love with $N.",
		"$n is making love with you.",
		"You fold up weirdly and start making love to yourself.",
		"$n folds up weirdly and starts making love to $mself."
	},

	{
		"massage",
		"Massage what?  Thin air?",
		 NULL,
		"You gently massage $N's shoulders.",
		"$n massages $N's shoulders.",
		"$n gently massages your shoulders.",
		"You practice yoga as you try to massage yourself.",
		"$n gives a show on yoga positions, trying to massage $mself."
	},

	{
		"mischievous",
		"You give a mischievous smile at the trouble you have in mind.",
		"$n grins mischievously at $s nasty ideas.",
		"You grin mischievously at $N, thinking at your plans with $M.",
		"$n grins mischievously at $N, what is $n up to with $M.",
		"$n grins mischievously at you. You inch toward the door.",
		"You grin mischievously at yourself.",
		"$n grins mischievously at $mself, that could be something interesting!"
	},

	{
		"moan",
		"You start to moan.",
		"$n starts moaning.",
		"You moan for the loss of $m.",
		"$n moans for the loss of $N.",
		"$n moans at the sight of you.  Hmmmm.",
		"You moan at yourself.",
		"$n makes $mself moan."
	},

	{
		"moon",
		"You drop your drawers and moon everyone in the room.",
		"$n drops $s pants and gives you the vertical smile.",
		"You bend over and moon $M.",
		"$n drops $s pants and rudely moons $N.",
		"$n bends over and gives you the vertical smile.",
		"You try to moon yourself, but you end up in a big knot.",
		"$n nearly goes into convulsions while trying to moon $mself."
	},

	{
		"muhaha",
		"MUHAHAHAHAHA",
		"$n laughs diabolically.  MUHAHAHAHAHA!",
		"You laugh at $M diabolically.",
		"$n laughs at $N diabolically.  MUHAHAHAHAHA!",
		"$n laughs at you diabolically.  MUHAHAHAHAHA!",
		"You must have funny thoughts.",
		 NULL
	},

	{
		"navel",
		"Whom would you like to victimize?",
		 NULL,
		"You lift $N's shirt and blow loudly into $S navel!",
		"$n lifts $N's shirt and blows loudly into $S navel!",
		"Yeek!  $n lifts your shirt and blows loudly into your navel!",
		"Careful, people are staring...",
		"$n looks around furtively, then bends over and blows loudly into $s own navel!!"
	},

	{
		"nibble",
		"Nibble on whom?",
		 NULL,
		"You nibble on $N's ear.",
		"$n nibbles on $N's ear.",
		"$n nibbles on your ear.",
		"You nibble on your OWN ear.",
		"$n nibbles on $s OWN ear."
	},

	{
		"nod",
		"You nod.",
		"$n nods.",
		"You nod at $N.",
		"$n nods at $N.",
		"$n nods at you in agreement.",
		"You attempt to nod at yourself and get dizzy instead.",
		"$n nods quietly to $mself. What a wacko."
	},

	{
		"nudge",
		"Nudge whom?",
		 NULL,
		"You nudge $M.",
		"$n nudges $N.",
		"$n nudges you.",
		"You nudge yourself, for some strange reason.",
		"$n nudges $mself, to keep $mself awake."
	},

	{
		"nuzzle",
		"Nuzzle whom?",
		 NULL,
		"You nuzzle $S neck softly.",
		"$n softly nuzzles $N's neck.",
		"$n softly nuzzles your neck.",
		"I'm sorry, friend, but that's impossible.",
		 NULL
	},

	{
		"ogle",
		"Whom do you want to ogle?",
		 NULL,
		"You ogle $M like $E was a piece of meat.",
		"$n ogles $N.  Maybe you should leave them alone for awhile?",
		"$n ogles you.  Guess what $e is thinking about?",
		"You ogle yourself.  You may just be too wierd for this mud.",
		"$n ogles $mself.  Better hope that $e stops there."
	},

	{
		"ohno",
		"Oh no!  You did it again!",
		"Oh no!  $n did it again!",
		"You exclaim, 'Oh no!  I did it again!'",
		"$n exclaims to $N, 'Oh no!  I did it again!'",
		"$n exclaims to you, 'Oh no!  I did it again!'",
		"You exclaim to yourself, 'Oh no!  I did it again!'",
		"$n mumbles to $mself, 'Oh no!  I did it again!'"
	},

	{
		"pat",
		"Pat whom?",
		 NULL,
		"You pat $N on $S head.",
		"$n pats $N on $S head.",
		"$n pats you on your head.",
		"You pat yourself on your back, great job!",
		"$n pats $mself on the back for $s accomplishment."
	},

	{
    		"peck",
	    	"Wouldn't you like to do that to a person, or do you prefer air?",
		"$n looks around with a naughty look in $s eyes.",
		"Risking a slap to the face, you give $M a peck on the cheek.",
		"$n smiles and gives $N a chaste peck on the cheek.",
		"$n pecks you on the cheek, how sweet!",
		"Wouldn't you like to do that to a person, or do you prefer air?",
		 NULL,
	},

	{
		"peer",
		"You peer intently about your surroundings.",
		"$n peers intently about the area, looking for thieves no doubt.",
		"You peer at $M quizzically.",
		"$n peers at $N quizzically.",
		"$n peers at you quizzically.",
		"You peer intently about your surroundings.",
		"$n peers intently about the area, looking for thieves no doubt."
	},

	{
		"point",
		"Point at whom?",
		 NULL,
		"You point at $M meaningfully.",
		"$n points at $N meaningfully.",
		"$n points at you meaningfully.",
		"You point meaningfully at yourself.",
		"$n points meaningfully at $mself."
	},

	{
		"poke",
		"Poke whom?",
		 NULL,
		"You poke $M in the ribs.",
		"$n pokes $N in the ribs.",
		"$n pokes you in the ribs.",
		"You poke yourself in the ribs, feeling very silly.",
		"$n pokes $mself in the ribs, looking very sheepish."
	},

	{
		"ponder",
		"You sit down and think deeply.",
		"$n sits down and thinks deeply.",
		"You ponder what $E is thinking.",
		"$n tries to ponder what $N is thinking.",
		"$n ponders what you are thinking, want to help?",
		"If you don't know, who does?",
		"$n stares in the distance, pondering a ponderful thought."
	},
	{
		"possum",
		"You do your best imitation of a corpse.",
		"$n hits the ground... DEAD.",
		"You do your best imitation of a corpse.",
		"$n hits the ground... DEAD.",
		"$n hits the ground... DEAD.",
		"You do your best imitation of a corpse.",
		"$n hits the ground... DEAD."
	},

	{
		"pout",
		"Ah, don't take it so hard.",
		"$n pouts.",
		"You pout at $M through lowered lashes.",
		"$n pouts at $N.",
		"$n pouts at you through lowered lashes.",
		"If you want to POUTONIZE spell it out!",
		 NULL
	},

	{
		"pray",
		"You feel righteous, and maybe a little foolish.",
		"$n begs and grovels to the powers that be.",
		"You crawl in the dust before $M.",
		"$n falls down and grovels in the dirt before $N.",
		"$n kisses the dirt at your feet.",
		"Talk about narcissism ...",
		"$n mumbles a prayer to $mself."
	},

	{
		"pretend",
		"You pretend you are the Imp, and slay everyone in sight!",
		"$n is pretending $e is the implementor again.  *sigh*",
		"You pretend you are Zolstead, and you demote $N to level 1.",
		"$n pretends $e is Zolstead, and says, '$N, you're demoted to level 1!'",
		"$n pretends $e is Zolstead, and says, 'You are demoted to level 1!'",
		"You pretend you are the implementor, and you demote yourself to level 1.",
		"$n pretends $e is Zolstead, and demotes $mself to level 1."
	},


	{
		"puke",
		"You puke ... chunks everywhere!",
		"$n pukes.",
		"You puke on $M.",
		"$n pukes on $N.",
		"$n spews vomit and pukes all over your clothing!",
		"You puke on yourself.",
		"$n pukes on $s clothes."
	},

	{
		"punch",
		"Punch whom?",
		 NULL,
		"You punch $M playfully.",
		"$n punches $N playfully.",
		"$n punches you playfully.  OUCH!",
		"You punch yourself.  You deserve it.",
		"$n punches $mself.  Why don't you join in?"
	},

	{
		"purr",
		"MMMMEEEEEEEEOOOOOOOOOWWWWWWWWWWWW.",
		"$n purrs contentedly.",
		"You purr contentedly rubbing against $N's leg.",
		"$n purrs contentedly brushing against $N's leg.",
		"$n purrs contentedly rubbing against your leg.",
		"You purr at yourself.",
		"$n purrs at $mself.  Must be a cat thing."
	},

	{
		"raspberry",
	    	"You stick your tongue out at ... well, no one.",
		"$n sticks $s tongue out, catching flies in the process.",
	    	"You give $M the raspberry ... and spit all over yourself.",
		"$n gives $N the raspberry... PHBT! What a mess... spit EVERYWHERE.",
	    	"You are splattered with spit as $n gives you the raspberry.",
		"You try to stick your tongue out at yourself... Somehow it isn't the same.",
		 NULL
	},

	{
		"renandstimpy",
		"You say, 'Oh Happy Happy, Joy Joy!'",
		"$n exclaims, 'Oh Happy Happy, Joy Joy!'",
		"You exclaim, 'Oh Happy Happy, Joy Joy!' at the mere thought of $M.",
		"$n exclaims, 'Oh Happy Happy, Joy Joy!' as $e sees $N enter the room.",
		"$n exclaims, 'Oh Happy Happy, Joy Joy!' when $e sees you approach.",
		"You exclaim, 'Oh Happy Happy, Joy Joy!' at the thought of yourself.",
		"$n exclaims, 'Oh Happy Happy, Joy Joy!' at the thought of $mself."
	},

	{
    		"roar",
	    	"You take a deep breath and roar loadly!",
    		"$n lets out a deafening roar!",
	    	"You roar loudly in $N's ear!",
    		"$n roars loudly in $N's ears!",
	    	"$n roars loudly at you! Your poor ears, you're all deaf now!",
	    	"You roar in frustration.",
    		"$n roars in frustration."
	},

	{
		"rofl",
		"You roll around on the floor laughing hysterically.",
		"$n rolls around on the floor laughing hysterically.",
		"You roll around on the floor laughing hysterically in front of $M.",
		"$n rolls around on the floor laughing hysterically in front of $N.",
		"$n rolls around on the floor laughing hysterically in front of you.",
		"You roll around on the floor laughing hysterically.",
		"$n rolls around on the floor laughing hysterically."
	},

	{
		"roll",
		"You roll your eyes.",
		"$n rolls $s eyes.",
		"You roll your eyes at $M.",
		"$n rolls $s eyes at $N.",
		"$n rolls $s eyes at you.",
		"You roll your eyes at yourself.",
		"$n rolls $s eyes at $mself."
	},

	{
		"rose",
		"Who would you like to give a rose to ?",
		"$n is looking for someone to give a rose to.",
		"You give $M a pretty rose {118}@{128}-{028},{128}-{028}`{128}-",
		"$n gives $N a pretty rose {118}@{128}-{028},{128}-{028}`{128}-",
		"$n gives you a pretty rose {118}@{128}-{028},{128}-{028}`{128}-",
		"You lament that no-one has given you any roses recently.",
		"$n is hinting that $E would like to be given a rose."
	},

	{
		"rub",
		"You rub your eyes.  How long have you been at this?",
		"$n rubs $s eyes.  $n must have been playing all day.",
		"You rub your eyes.  Has $N been playing as long as you have?",
		"$n rubs $s eyes.  $n must have been playing all day.",
		"$n rubs $s eyes.  Have you been playing as long as $m?",
		"You rub your eyes.  How long have you been at this?",
		"$n rubs $s eyes.  $n must have been playing all day."
	},

	{
		"ruffle",
		"You've got to ruffle SOMEONE.",
		 NULL,
		"You ruffle $N's hair playfully.",
		"$n ruffles $N's hair playfully.",
		"$n ruffles your hair playfully.",
		"You ruffle your hair.",
		"$n ruffles $s hair."
	},

	{
		"scream",
		"ARRRRRRRRRRGH!!!!!",
		"$n screams loudly!",
		"ARRRRRRRRRRGH!!!!!  Yes, it MUST have been $S fault!!!",
		"$n screams loudly at $N.  Better leave before $n blames you, too!!!",
		"$n screams at you!  That's not nice!  *sniff*",
		"You scream at yourself.  Yes, that's ONE way of relieving tension!",
		"$n screams loudly at $mself!  Is there a full moon up?"
	},

	{
    		"scuff",
	    	"You scuff your feet sheepishly.",
    		"$n scuffs $s feet in the dirt, looking very sheepish.",
	    	"You scuff your foot, avoiding $S gaze.",
    		"$n scuffs $s foot, shyly avoiding $N's gaze.",
	    	"$n scuffs $s foot, shyly avoiding your gaze.",
    		"You scuff your foot sheepishly.",
	    	"$n scuffs $s foot in the dirt, looking very sheepish."
	},

	{
		"seduce",
		"Whom did you wish to seduce?",
		 NULL,
		"Well, it's worth a try...",
		"$n tries to seduce $N with a smile.",
		"$n smiles seductively at you with darkened eyes.",
		"Umm, are you sure about that?",
		 NULL
	},

	{
		"shake",
		"You shake your head.",
		"$n shakes $s head.",
		"You shake $S hand.",
		"$n shakes $N's hand.",
		"$n shakes your hand.",
		"You are shaken by yourself.",
		"$n shakes and quivers like a bowl full of jelly."
	},

	{
    		"shawl",
	    	"You slam your head against the wall, laughing.",
    		"$n slams $s head against the wall, laughing.",
	    	"You slam your head against the wall, laughing at $N.",
    		"$n slams $s head against the wall, laughing at $N.",
    		"$n slams $s head against the wall, laughing at you.",
	    	"You slam your head against the wall, laughing at your own errors.",
    		"$n slams $s head against the wall, laughing at $s own errors."
	},

	{
		"shiver",
		"Brrrrrrrrr.",
		"$n shivers contentedly.",
		"You shiver at the thought of fighting $M.",
		"$n shivers at the thought of fighting $N.",
		"$n shivers at the suicidal thought of fighting you.",
		"You shiver to yourself?",
		"$n shivers because $e's cold."
	},

	{
		"shrug",
		"You shrug.",
		"$n shrugs helplessly.",
		"You shrug in response to $s question.",
		"$n shrugs in response to $N's question.",
		"$n shrugs in response to your question.",
		"You shrug to yourself.",
		"$n shrugs to $mself.  What a strange person."
	},

	{
		"sigh",
		"You sigh.",
		"$n sighs loudly.",
		"You sigh as you think of $M.",
		"$n sighs at the sight of $N.",
		"$n sighs as $e thinks of you.  Touching, huh?",
		"You sigh at yourself.  You MUST be lonely.",
		"$n sighs at $mself.  What a sorry sight."
	},


	{
		"slap",
		"Slap whom?",
		 NULL,
		"You slap $N on the face.",
		"$n slaps $N on the face.",
		"$n slaps you on your face.",
		"You slap yourself HARD!",
		"$n slaps $mself repeatedly, $e seems to enjoy it!"
	},

	{
		"slither",
		"You act like a worm and jiggle around.",
		"You see $n slither around the floor.",
		"You crawl down and slide up $N's pants.",
		"You see $n sliding around $N's leg.",
		"You see $n slithering toward YOU!",
		"Curling up you slide onto the ground.",
		"$n slides onto the floor and curls up."
	},

	{
		"smack",
		"Smack whom?",
		 NULL,
		"You smack $N upside the head.",
		"$n smacks $N upside the head.",
		"$n smacks you upside the head.",
		"You smack yourself HARD!",
		"$n smacks $mself upside the head, what an idiot!"
	},

	{
		"smile",
		"You smile happily.",
		"$n smiles happily.",
		"You smile at $M.",
		"$n beams a smile at $N.",
		"$n smiles at you.",
		"You smile at yourself.",
		"$n smiles at $mself."
	},

	{
		"smirk",
		"You smirk.",
		"$n smirks.",
		"You smirk at $S saying.",
		"$n smirks at $N's saying.",
		"$n smirks at your saying.",
		"You smirk at yourself.  Okay ...",
		"$n smirks at $s own 'wisdom'."
	},

	{
		"smoke",
		"You light a cigarette.",
		"$n lights a cigarette.",
		"You offer $M a smoke.",
		"$n offers $M a smoke.",
		"$n offers you a smoke.",
		"Stressed?",
		"$n is very stressed and lights yet another cigaret."
	},

	{
		"snap",
		"PRONTO ! You snap your fingers.",
		"$n snaps $s fingers.",
		"You snap you fingers at $M.",
		"$n snaps $s fingers at $N impatiently.",
		"$n snaps $s fingers at you impatiently. C'mon already!",
		"You snap yourself to attention.",
		"$n snaps $mself to attention."
	},

	{
		"snarl",
		"You grizzle your teeth and look mean.",
		"$n snarls angrily.",
		"You snarl at $M.",
		"$n snarls at $N.",
		"$n snarls at you in anger.",
		"You snarl at yourself.",
		"$n snarls at $mself."
	},

	{
		"sneeze",
		"Gesundheit!",
		"$n sneezes.",
		"You sneeze in $N's face.  YACK!",
		"$n sneezes in $N's face.  YACK!",
		"$n sneezes in your face.  YACK!",
		"You sneeze all over yourself.",
		"$n sneezes all over $mself.",
	},

	{
		"snicker",
		"You snicker softly.",
		"$n snickers softly.",
		"You snicker with $M about your shared secret.",
		"$n snickers with $N about their shared secret.",
		"$n snickers with you about your shared secret.",
		"You snicker at your own evil thoughts.",
		"$n snickers at $s own evil thoughts."
	},

	{
		"sniff",
		"You sniff sadly. *SNIFF*",
		"$n sniffs sadly.",
		"You sniff sadly at the way $E is treating you.",
		"$n sniffs sadly at the way $N is treating $m.",
		"$n sniffs sadly at the way you are treating $m.",
		"You sniff sadly at your lost opportunities.",
		"$n sniffs sadly at $mself.  Something MUST be bothering $m."
	},

	{
		"snore",
		"Zzzzzzzzzzzzzzzzz.",
		"$n snores loudly.",
		 NULL,
		 NULL,
  		 NULL,
		 NULL,
		 NULL
	},

	{
		"snowball",
		"Whom do you want to throw a snowball at?",
		 NULL,
		"You throw a snowball in $N's face.",
		"$n throws a snowball at $N.",
		"$n throws a snowball at you.  Splat!",
		"You throw a snowball at yourself.",
		"$n throws a snowball at $mself."
	},

	{
		"snuggle",
		"Who?",
		 NULL,
		"You snuggle $M.",
		"$n snuggles upon to $N.",
		"$n snuggles upon to you affectionately.",
		"You snuggle upon, getting ready to sleep.",
		"$n snuggles upon, getting ready to sleep."
	},

	{
		"spank",
		"Spank whom?",
		 NULL,
		"You spank $M playfully.",
		"$n spanks $N playfully.",
		"$n spanks you playfully.  OUCH!",
		"You spank yourself.  Kinky!",
		"$n spanks $mself.  Kinky!"
	},

	{
		"spit",
		"You spit.",
		"$n spits.",
		"You spit on $N.",
		"$n spits on $N.",
		"$n spits on you.",
		"You spit on yourself.  Always knew you were a drooler.",
		"$n spits on $mself."
	},


	{
		"squeeze",
		"Where, what, how, whom?",
		 NULL,
		"You squeeze $M fondly.",
		"$n squeezes $N fondly.",
		"$n squeezes you fondly.",
		"You squeeze yourself - try to relax a little!",
		"$n squeezes $mself."
	},

	{
		"stare",
		"You stare at the sky.",
		"$n stares at the sky.",
		"You stare dreamily at $N, completely lost in $S eyes..",
		"$n stares dreamily at $N.",
		"$n stares dreamily at you, completely lost in your eyes.",
		"You stare dreamily at yourself - enough narcissism for now.",
		"$n stares dreamily at $mself - NARCISSIST!"
	},

	{
		"stomp",
		"***STOMP*** ***STOMP***",
		"$n stomps insanely around the room!",
		"You stomp $S ass into a bloody mess on the floor.",
		"$n begins stomping $N's ass into the floor.",
		"$n must not like you because $e's stomping your ass into the floor.",
		"You strain your back, reach back, and ram your foot up your ass.",
		"$n strains $s back, reaches back, and rams $s foot up $s ass."
	},

	{
    		"stroke",
	    	"You stroke the air vaguely.",
    		"$n makes vague stroking motions.",
	    	"You gently stroke $S inner thigh. $E inhales sharply.",
    		"$N exhales very sharply suddenly.",
	    	"$n gently strokes your inner thigh, you inhale sharply.",
	    	"You are about to do something you'd rather not be caught doing.",
		NULL
	},

	{
		"strut",
		"Strut your stuff.",
		"$n struts proudly.",
		"You strut to get $S attention.",
		"$n struts, hoping to get $N's attention.",
		"$n struts, hoping to get your attention.",
		"You strut to yourself, lost in your own world.",
		"$n struts to $mself, lost in $s own world."
	},

	{
		"suffer",
		"No xp again?  You suffer at the hands of fate.",
		"$n is suffering.  Looks like $e can't seem to level.",
		"You tell $N how you suffer whenever you're away from $M.",
		"$n tells $N that $e suffers whenever they're apart.",
		"$n tells you that $e suffers whenever you're apart.",
		"No xp again?  You suffer at the hands of fate.",
		"$n is suffering.  Looks like $e can't seem to level."
	},

	{
		"sulk",
		"You sulk.",
		"$n sulks in the corner.",
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL
	},

	{
    		"sweep",
	    	"You look at your empty arms and sigh.",
    		"$n looks at $s empty arms and sighs.",
	    	"You sweep $N in your arms and kiss $M long and deeply.",
    		"$n sweeps $N in $s arms and kisses $M long and deeply.",
	    	"$n sweeps you in $s arms and kisses you long and deeply.",
    		"You look at your empty arms and sweep yourself for the kiss.",
	    	"$n looks at $s empty arms, sighs, and sweeps $mself in a long kiss."
	},

	{
		"tap",
		"You tap your foot impatiently.",
		"$n taps $s foot impatiently.",
		"You tap your foot impatiently.  Will $N ever be ready?",
		"$n taps $s foot impatiently as $e waits for $N.",
		"$n taps $s foot impatiently as $e waits for you.",
		"You tap yourself on the head.  Ouch!",
		"$n taps $mself on the head."
	},

	{
		"thank",
		"Thank you too.",
		 NULL,
		"You thank $N heartily.",
		"$n thanks $N heartily.",
		"$n thanks you heartily.",
		"You thank yourself since nobody else wants to !",
		"$n thanks $mself since you won't."
	},

	{
		"tender",
		"You pout your lips, trying to lure someone into a kiss.",
		"$n is pouting $s lips, trying to lure someone into a kiss.",
		"You give $M a soft, tender kiss.",
		"$n gives $N a soft, tender kiss.",
		"$n gives you a soft, tender kiss.",
		"You'd better not, people may start to talk!",
		 NULL
	},

	{
		"tickle",
		"Whom do you want to tickle?",
		 NULL,
		"You tickle $N.",
		"$n tickles $N.",
		"$n tickles you - hee hee hee.",
		"You tickle yourself, how funny!",
		"$n tickles $mself."
	},

	{
    		"tongue",
	    	"You slowly give your tongue a nice workout.",
    		"$n gives $s tongue a nice workout... hrrmmm...",
	    	"Mmmm... You give $M a good workover with your tongue.",
    		"$n slides $s tongue over $N's body...",
	    	"$n slides $s tongue all over your body.",
    		"You slowly slide your tongue all over your body.",
	    	"$n slowly slides $s tongue all over $s body... ICK!"
	},

	{
		"tummy",
		"You rub your tummy and wish you'd bought a pie at the bakery.",
		"$n rubs $s tummy and wishes $e'd bought a pie at the bakery.",
		"You rub your tummy and ask $M for some food.",
		"$n rubs $s tummy and asks $N for some food.",
		"$n rubs $s tummy and asks you for some food.  Please?",
		"You rub your tummy and wish you'd bought a pie at the bakery.",
		"$n rubs $s tummy and wishes $e'd bought a pie at the bakery."
	},

	{
		"twiddle",
		"You patiently twiddle your thumbs.",
		"$n patiently twiddles $s thumbs.",
		"You twiddle $S ears.",
		"$n twiddles $N's ears.",
		"$n twiddles your ears.",
		"You twiddle your ears like Dumbo.",
		"$n twiddles $s own ears like Dumbo."
	},

	{
		"undress",
		"You start to undress in the shadows, waiting for somebody to notice.",
		"$n begins to undress in the shadows... Wow! that looks SEXY!",
		"You undress $M with your eyes... Kinky!",
		"You feel a raise in temperature as $n lusts after $N.",
		"Feeling exposed, you sense $n undressing you with $s eyes.",
		"A mirror would help better!",
		"You feel a raise in temerature as $n lusts on $mself."
	},

	{
		"voodoo",
		"Bad juju like this has to be directed at a person.",
		"$n plays with $s voodoo dolls.",
		"You stare daggers at $N, jabbing pins into $S effigy.",
		"$N writhes in agony as $n jabs pins into a doll.",
		"You see $n jab pins in a doll, and suddenly you're in agony!",
		"You jab a pin into your own voodoo doll! AGONY!!!!!!",
		"$n jabs a pin into a doll, and $e suddenly writhes in agony!"
	},

	{
		"wave",
		"You wave.",
		"$n waves happily.",
		"You wave goodbye to $N.",
		"$n waves goodbye to $N.",
		"$n waves goodbye to you.  Have a good journey.",
		"Are you going on adventures as well?",
		"$n waves goodbye to $mself."
	},

	{
		"whistle",
		"You whistle appreciatively.",
		"$n whistles appreciatively.",
		"You whistle at the sight of $M.",
		"$n whistles at the sight of $N.",
		"$n whistles at the sight of you.",
		"You whistle a little tune to yourself.",
		"$n whistles a little tune to $mself."
	},

	{
		"whip",
		"You crack your bullwhip menacingly.",
		"$n cracks a bullwhip menacingly.",
		"You flick your whip across $N's bare back.",
		"$n flicks $s whip across $N's bare back.",
		"$n flicks a whip across your back... the pain... the pleasure!",
		"You whip yourself into a frenzy!",
		"$n whips $mself into a frenzy!"
	},

	{
		"whew",
		"What a relief.",
		"$n wipes $s brow and exclaims, 'Whew, that was close.'",
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL
	},

	{
		"wiggle",
		"Your wiggle your bottom.",
		"$n wiggles $s bottom.",
		"You wiggle your bottom toward $M.",
		"$n wiggles $s bottom toward $N.",
		"$n wiggles $s bottom toward you.",
		"You wiggle about like a fish.",
		"$n wiggles about like a fish."
	},

	{
		"wince",
		"You wince.  Ouch!",
		"$n winces.  Ouch!",
		"You wince at $M.",
		"$n winces at $N.",
		"$n winces at you.",
		"You wince at yourself.  Ouch!",
		"$n winces at $mself.  Ouch!"
	},

	{
		"wink",
		"You wink suggestively.",
		"$n winks suggestively.",
		"You wink suggestively at $N.",
		"$n winks at $N.",
		"$n winks suggestively at you.",
		"You wink at yourself ?? - what are you up to ?",
		"$n winks at $mself - something strange is going on..."
	},

	{
    		"wrestle",
	    	"You look around for someone to wrestle with.",
    		"$n looks around for someone to wrestle with.",
	    	"You wrestle $N to the floor and pin $M there.",
    		"$n wrestles $N to the floor, pinning $M there.",
	    	"$n wrestles you to the floor and pins you there!",
    		"You wrestle with your conciense.",
	    	"$n makes funny faces while wrestling with $s conscience."
	},

	{
		"yawn",
		"You must be tired.",
		"$n yawns tiredly.",
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL
	},


	{
		"",
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL
	}
};


long long get_game_usec( void )
{
	struct timeval last_time;

	push_call("get_game_usec()");

	gettimeofday(&last_time, NULL);

	pop_call();
	return((long long) last_time.tv_usec + 1000000LL * (long long) last_time.tv_sec);
}

/*
	The main entry point for executing commands.
	Can be recursively called from 'at', 'order', 'force'.
*/


void interpret( CHAR_DATA *ch, char *argue )
{
	char command[MAX_INPUT_LENGTH];
	char logline[MAX_INPUT_LENGTH];
	char pag_buf[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	char argument_buf[MAX_INPUT_LENGTH];
	char *argument;

	int cmd = -1;
	int leng;
	int trust;
	bool found = FALSE;
	long long last_time, delta;
	char *pt, *pti, *pto;
	bool auto_command;

	push_call_format("interpret(%s,%s)", ch->name, argue);

	last_time = get_game_usec();

	if (!IS_NPC(ch) && IS_SET(ch->pcdata->interp, INTERP_AUTO))
	{
		REMOVE_BIT(ch->pcdata->interp, INTERP_AUTO);
		auto_command = TRUE;
	}
	else
	{
		auto_command = FALSE;
	}

	strcpy(argument_buf, argue);

	/*
		Strip off non printables  - Chaos 5/13/96
	*/

	for (pt = argument_buf ; *pt != '\0' ; pt++)
	{
		if (*pt != '\r' && *pt != '\n' && (*pt < 32 || *pt > 126))
		{
			*pt = ' ';
		}
	}
	argument = argument_buf;

	/*
		OLC stuff
	*/

	if (!IS_NPC(ch) && (ch->pcdata->editmode == MODE_REPEATCMD || ch->pcdata->tempmode == MODE_REPEATCMD))
	{
		DO_FUN *fun;

		if ((fun = ch->pcdata->last_cmd) == NULL)
		{
			ch->pcdata->editmode = MODE_NONE;
			bug( "interpret: bad last_cmd mode");

			pop_call();
			return;
		}
		else
		{
			for (cmd = 0 ; *cmd_table[cmd].name != '\0' ; cmd++)
			{
				if (cmd_table[cmd].do_fun == fun)
				{
					found = TRUE;
					break;
				}
			}
			if (!found)
			{
				cmd = 0;
				bug( "interpret: invalid last_cmd");

				pop_call();
				return;
			}
			sprintf( logline, "(%s) %s", cmd_table[cmd].name, argument );
		}
	}

	if (cmd == -1)
	{
		if (ch->desc)
		{
			if (ch->desc->character == ch)
			{
				*ch->desc->incomm = '\0';
			}

			/*
				Let's not display commands in obj_progs, and doesn't hurt either
			*/

			if (!IS_NPC(ch) && IS_SET(ch->act, PLR_REPEAT))
			{
				if (ch->trust <= ch->level && !IS_SET(ch->pcdata->interp, INTERP_FORCE))
				{
					ch_printf_color(ch, "{078}[{178}%s{078}]\n\r", argument);
				}
			}

			if (!IS_NPC(ch) && IS_SET(ch->pcdata->interp, INTERP_DUMP) && auto_command == FALSE && !IS_SET(ch->pcdata->interp, INTERP_FORCE))
			{
				char jbuf[MAX_STRING_LENGTH];
				CHAR_DATA *fch;

				if (!strcasecmp(argument, "stop" ) )
				{
					send_to_char( "Dump stopped.\n\r", ch );
					REMOVE_BIT(ch->pcdata->interp, INTERP_DUMP);

					pop_call();
					return;
				}
				for (fch = ch->in_room->first_person ; fch ; fch = fch->next_in_room)
				{
					if (fch->position <= POS_SLEEPING)
					{
						continue;
					}
					if (ch != fch && blocking(fch, ch))
					{
						continue;
					}
					if (!can_understand(fch, ch, FALSE))
					{
						continue;
					}

					sprintf( jbuf, "%s dumps '%s'", ch->name, argument);
					ch_printf(fch, "%s%s\n\r", get_color_string(fch, COLOR_SPEACH, VT102_DIM), jbuf);
				}
				pop_call();
				return;
			}
		}

		/*
			Strip leading/tailing spaces.
		*/

		while (isspace(*argument) && *argument != '\0')
		{
			argument++;
		}

		if (*argument != '\0')
		{
			for (pt = argument + strlen(argument) - 1 ; *pt == ' ' ; pt--)
			{
				*pt = '\0';
			}
		}

		/*
			Send the scroll prompter out only if no inputs  -  Chaos 8/22/97
		*/

		if (ch->desc && IS_SET(CH(ch->desc)->pcdata->interp, INTERP_PAGE) && CH(ch->desc)->pcdata->page_buf != NULL)
		{
			REMOVE_BIT(CH(ch->desc)->pcdata->interp, INTERP_PAGE);

			leng = str_cpy_max( pag_buf, ansi_translate_text( ch, "{300}" ), MAX_BUFFER_LENGTH );
			leng = str_apd_max( pag_buf, CH(ch->desc)->pcdata->page_buf, leng, MAX_BUFFER_LENGTH);

			STRFREE (CH(ch->desc)->pcdata->page_buf );
			if (*argument == '\0')
			{
				send_to_char(pag_buf, ch);
			}
			else
			{
				send_to_char((const char*)ansi_translate_text(ch, "{088}{128}-----------------------------[Page Buffer Canceled]-----------------------------\n\r"), ch);
			}
		}

		if (*argument == '\0')
		{
			pop_call();
			return;
		}

		/*
			Add links to hyper-text helps
		*/

		if (!IS_NPC(ch) && IS_SET(ch->pcdata->interp, INTERP_HELP))
		{
			REMOVE_BIT(ch->pcdata->interp, INTERP_HELP);

			if (argument[1] == '\0' && argument[0] != '\0')
			{
				HELP_MENU_DATA *menu;
				HELP_MENU_DATA *prev_menu=NULL;
				HELP_MENU_DATA *menu2;
				HELP_DATA      *help;
				bool   foundh, foundh2;

				foundh	= FALSE;
				help		= ch->pcdata->last_help;
				for (menu = help->first_menu ; menu ; menu = menu->next)
				{
					if (menu->option == '-')
					{
						foundh = TRUE;
					}
					prev_menu = menu;
					if (prev_menu == NULL)
					{
						break;
					}
				}
				if (argument[0] == '-' && !foundh && ch->pcdata->prev_help != NULL)
				{
					send_to_char_color(ch->pcdata->prev_help->text, ch);
					ch->pcdata->last_help = ch->pcdata->prev_help;
					ch->pcdata->prev_help = NULL;
					SET_BIT(ch->pcdata->interp, INTERP_HELP);
					pop_call();
					return;
				}
				for (menu = help->first_menu ; menu ; menu = menu->next)
				{
					if (tolower(argument[0]) == menu->option)
					{
						send_to_char_color( menu->help->text, ch);
						foundh2	= FALSE;

						for (menu2 = menu->help->first_menu ; menu2 ; menu2 = menu2->next)
						{
							if (menu2->option == '-')
							{
								foundh2 = TRUE;
							}
						}
						if (!foundh2 && argument[0] != '-')
						{
							if (ch->pcdata->page_buf == NULL)
							{
								send_to_char_color("\n\r  {128}(-) {058}Return\n\r", ch);
							}
							else
							{
								leng = str_cpy_max(pag_buf, ch->pcdata->page_buf, MAX_BUFFER_LENGTH);
								leng = str_apd_max(pag_buf, ansi_translate_text(ch, "\n\r  {128}(-) {058}Return\n\r"), leng, MAX_BUFFER_LENGTH);
								ch->pcdata->page_buf = STRALLOC(pag_buf);
							}
						}
						if (argument[0] == '-')
						{
							ch->pcdata->prev_help = NULL;
						}
						else
						{
							ch->pcdata->prev_help = ch->pcdata->last_help;
						}
						ch->pcdata->last_help = menu->help;
						SET_BIT(ch->pcdata->interp, INTERP_HELP);

						pop_call();
						return;
					}
				}
				/* assume a menu option not listed is normal command and continue */
			}
		}

		/*
			Last Command  - Chaos 5/6/99
		*/

		if (!IS_NPC(ch) && !IS_SET(ch->pcdata->interp, INTERP_FORCE) && auto_command == FALSE && ch->pcdata->last_command != NULL)
		{
			char jbuf[MAX_STRING_LENGTH];

			if (!strcasecmp(argument, "stop") || !strcasecmp(argument, "quit"))
			{
				send_to_char( "Command stopped.\n\r", ch );
				STRFREE( ch->pcdata->last_command );
				ch->pcdata->last_command = NULL;

				pop_call();
				return;
			}
			strcpy( jbuf, ch->pcdata->last_command);
			strcat( jbuf, argument );
			strcpy( argument, jbuf );
			STRFREE( ch->pcdata->last_command );
			ch->pcdata->last_command = NULL;
		}
 
		/*
			Any command starting with numbers is a repeat command
		*/

		if (*argument >= '0' && *argument <= '9')
		{
			do_repeat( ch, argument);

			pop_call();
			return;
		}

		/*
			Added the '&' to multi-line commands  Chaos 12/7/93
		*/
		pti = argument;
		pto = command;

		for (pti = argument, pto = command ; *pti != ' ' && *pti != '\0' ; pto++, pti++)
		{
			*pto = *pti;
		}
		*pto = '\0';

		if (ch->desc && ch->desc->intop < MAX_INPUT_LENGTH-2)
		{
			if (strchr(argument, '&') != NULL && strcasecmp(command, "alias"))
			{
				process_command(ch, argument);

				pop_call();
				return;
			}
		}

		/*
			Implement freeze command.
		*/

		if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_FROZEN))
		{
			send_to_char("You're totally frozen!\n\r", ch);
			pop_call();
			return;
		}

		/*
			Grab the command word.
			Special parsing so ' can be a command,
			also no spaces needed after punctuation.
		*/

		strcpy(logline, argument );

		if (!isalpha(*argument) && !isdigit(*argument))
		{
			*command = *argument;
			*(command+1) = '\0';
			argument++;
			while (isspace(*argument))
			{
				argument++;
			}
		}
		else
		{
			argument = one_argument(argument, command);
		}

		/*
			Look for command in command table.
		*/

		found = FALSE;
		trust = get_trust( ch );

		/*
			Look for alias first - Chaos 10/11/93
		*/

		if (ch->desc && auto_command == FALSE)
		{
			if (!IS_SET(CH(ch->desc)->pcdata->interp, INTERP_ALIAS)
			&&  !IS_SET(CH(ch->desc)->pcdata->interp, INTERP_OPROG)
			&&  !IS_SET(CH(ch->desc)->pcdata->interp, INTERP_FORCE))
			{
				for (cmd = 0 ; cmd < MAX_ALIAS ; cmd++)
				{
					if (CH(ch->desc)->pcdata->alias[cmd] != NULL && CH(ch->desc)->pcdata->alias[cmd][0] != '\0')
					{
						if (!strcasecmp(CH(ch->desc)->pcdata->alias_c[cmd], command))
						{
							SET_BIT(CH(ch->desc)->pcdata->interp, INTERP_ALIAS);
							buf[0]	= '\0';
							pti		= CH(ch->desc)->pcdata->alias[cmd];

							for (pto = buf ; *pti != '\0' ; pti++)
							{
								if (*pti == '%')
								{
									pto = &buf[0] + str_cat_max(buf, argument, MAX_INPUT_LENGTH);
								}
								else
								{
									if ((pto-&buf[0]) < MAX_INPUT_LENGTH-1)
									{
										*pto = (char) (*pti);
										*(++pto)= '\0';
									}
								}
							}
							str_cat_max(buf, "\0", MAX_INPUT_LENGTH);

							if (ch->desc && ch->desc->character == ch)
							{
								if (ch->desc->back_buf != NULL)
								{
									char buf2[MAX_INPUT_LENGTH];

									str_cpy_max( buf2, ch->desc->back_buf, MAX_INPUT_LENGTH );
									STRFREE(ch->desc->back_buf );
									str_cat_max( buf2, ch->desc->inbuf, MAX_INPUT_LENGTH );
									ch->desc->back_buf = STRALLOC( buf2 );
								}
								else
								{
									ch->desc->back_buf = STRALLOC(ch->desc->inbuf);
								}
								ch->desc->intop = 0;
								*ch->desc->inbuf = '\0';
							}
							preprocess_command(ch, buf);

							pop_call();
							return;
						}
					}
				}
			}
		}

		if ((cmd = find_command(command, trust)) != -1)
		{
			found = TRUE;
		}

		/*
			Berserk players can only "kill" or "murder"
		*/

		if (IS_AFFECTED(ch, AFF2_BERSERK) && !IS_IMMORTAL(ch))
		{
			if (!found || !IS_SET(cmd_table[cmd].flags, CMD_BERSERK))
			{
				send_to_char("You're Berserk!  Kill!  Kill!  Kill!\n\r", ch);
				pop_call();
				return;
			}
		}

		/*
			Arrested players are very limited as well. (Presto 02/97)
		*/

		if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_ARRESTED))
		{
			if ((ch->pcdata->jailtime - (mud->current_time - ch->pcdata->jaildate)) <= 0)
			{
				auto_release(ch);
				pop_call();
				return;
			}

			if (!found)
			{
				send_to_char("You're in jail!\n\r", ch);
				pop_call();
				return;
			}

			if (strcasecmp(cmd_table[cmd].name, "say")
			&&  strcasecmp(cmd_table[cmd].name, "look")
			&&  strcasecmp(cmd_table[cmd].name, "quit")
			&&  strcasecmp(cmd_table[cmd].name, "refresh"))
			{
				send_to_char("You're in jail!\n\r", ch);
				pop_call();
				return;
			}
		}

		if (!IS_NPC(ch))
		{
			if (IS_SET(ch->act, PLR_AFK))
			{
				REMOVE_BIT(ch->act, PLR_AFK);
				act( "$n is no longer afk.", ch, NULL, NULL, TO_ROOM );
			}
		}
	}

	/*
		Log and snoop.
	*/

	if (cmd_table[cmd].log == LOG_NEVER)
	{
		strcpy(logline, cmd_table[cmd].name);
	}

	sprintf(mud->last_player_cmd, "%s: %s", ch->name, logline);

	if ((!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_LOGGED)) || IS_SET(mud->flags, MUD_LOGALL) || cmd_table[cmd].log == LOG_ALWAYS)
	{
		log_printf("Log %s: %s", IS_NPC(ch) ? ch->short_descr : ch->name, logline);
	}

	if (!found)
	{
		/* Check for object programs of unknown commands*/

		if (!IS_NPC(ch) && ch->desc && !IS_SET(ch->pcdata->interp, INTERP_OPROG))
		{
			OBJ_DATA *obj;
			OBJ_PROG *prg;

			for (obj = ch->in_room->first_content ; obj ; obj = obj->next_content)
			{
				if (IS_SET(obj->pIndexData->progtypes, TRIG_ROOM_UNKNOWN))
				{
					for (prg = obj->pIndexData->first_prog ; prg ; prg = prg->next)
					{
						if (prg->trigger == TRIG_ROOM_UNKNOWN  && !str_prefix(command, prg->unknown))
						{
							if (number_percent() <= prg->percentage)
							{
								start_object_program( ch, obj, prg, argument);
								pop_call();
								return;
							}
						}
					}
				}
			}

			for (obj = ch->first_carrying ; obj ; obj = obj->next_content)
			{
				if (obj->wear_loc != WEAR_NONE && IS_SET(obj->pIndexData->progtypes, TRIG_WEAR_UNKNOWN))
				{
					for (prg = obj->pIndexData->first_prog ; prg ; prg = prg->next)
					{
						if (prg->trigger == TRIG_WEAR_UNKNOWN && !str_prefix(command, prg->unknown))
						{
							if (number_percent() <= prg->percentage)
							{
								start_object_program( ch, obj, prg, argument);
								pop_call();
								return;
							}
						}
					}
				}
				if (IS_SET(obj->pIndexData->progtypes, TRIG_UNKNOWN))
				{
					for (prg = obj->pIndexData->first_prog ; prg ; prg = prg->next)
					{
						if (prg->trigger == TRIG_UNKNOWN && !str_prefix(command, prg->unknown))
						{
							if (number_percent() <= prg->percentage)
							{
								start_object_program( ch, obj, prg, argument);
								pop_call();
								return;
							}
						}
					}
				}
			}
		}

		if (!check_social(ch, command, argument) && strcasecmp(command, "!") && strcasecmp( command, "."))
		{
			ch_printf(ch, "Huh?  '%s' is not a command.\n\r", command);

			/*
				Pets limitations - Scandum
			*/

			if (IS_NPC(ch) && IS_AFFECTED(ch, AFF_CHARM) && ch->master)
			{
				send_to_char( "Your pet can't do that.\n\r", ch->master);
				pop_call();
				return;
			}
		}
		pop_call();
		return;
	}

	/*
		Check for invalid combat
	*/

	if (ch->fighting != NULL)
	{
		if (ch->fighting->who->in_room != ch->in_room)
		{
			stop_fighting(ch,FALSE);
			update_pos( ch );
		}
	}

	/*
		Character not in position for command?
	*/

	if (ch->position < cmd_table[cmd].position)
	{
		switch( ch->position )
		{
			case POS_DEAD:
				send_to_char( "Lie still; you are DEAD.\n\r", ch );
				break;

			case POS_MORTAL:
			case POS_INCAP:
				send_to_char( "You are hurt far too bad for that.\n\r", ch );
				break;

			case POS_STUNNED:
				send_to_char( "You are too stunned to do that.\n\r", ch );
				break;

			case POS_SLEEPING:
				send_to_char( "In your dreams, or what?\n\r", ch );
				break;

			case POS_RESTING:
				send_to_char( "Nah... You feel too relaxed...\n\r", ch);
				break;

			case POS_SITTING:
				send_to_char( "Nah... You are too comfortable...\n\r", ch);
				break;

			case POS_FIGHTING:
				send_to_char( "No way!  You are still fighting!\n\r", ch);
				break;
		}
		pop_call();
		return;
	}

	/*
		Check for objects to respond to command in room
	*/

	if (!IS_NPC(ch) && ch->desc && !IS_SET(ch->pcdata->interp, INTERP_OPROG) && !IS_SET(ch->pcdata->interp, INTERP_FORCE))
	{
		OBJ_DATA *obj;
		OBJ_PROG *prg;

		for (obj = ch->in_room->first_content ; obj ; obj = obj->next_content)
		{
			if (IS_SET(obj->pIndexData->progtypes, TRIG_ROOM_COMMAND))
			{
				for (prg = obj->pIndexData->first_prog ; prg ; prg = prg->next)
				{
					if (prg->trigger == TRIG_ROOM_COMMAND && prg->cmd == cmd)
					{
						if (number_percent() <= prg->percentage)
						{
							start_object_program(ch, obj, prg, argument);
							pop_call();
							return;
						}
					}
				}
			}
		}
	}

	/*
		Decrement movement for exertion cmds
	*/

	if (cmd_gsn[cmd] != -1 && !IS_IMMORTAL(ch))
	{
		if (ch->move < skill_table[cmd_gsn[cmd]].min_mana)
		{
			send_to_char( "You are too tired to do that!\n\r", ch );

			pop_call();
			return;
		}
		else
		{
			ch->move -= skill_table[cmd_gsn[cmd]].min_mana;
		}
	}

	/* Check for object programs */

	if (!IS_NPC(ch) && ch->desc && !IS_SET(ch->pcdata->interp, INTERP_OPROG) && !IS_SET(ch->pcdata->interp, INTERP_FORCE))
	{
		OBJ_DATA *obj;
		OBJ_PROG *prg;

		for (obj = ch->first_carrying ; obj ; obj = obj->next_content)
		{
			if (obj->wear_loc != WEAR_NONE && IS_SET(obj->pIndexData->progtypes, TRIG_WEAR_COMMAND))
			{
				for (prg = obj->pIndexData->first_prog ; prg ; prg = prg->next)
				{
					if (prg->trigger == TRIG_WEAR_COMMAND && prg->cmd == cmd)
					{
						if (number_percent() <= prg->percentage)
						{
							start_object_program( ch, obj, prg, argument);
							pop_call();
							return;
						}
					}
				}
			}

			if (IS_SET(obj->pIndexData->progtypes, TRIG_COMMAND))
			{
				for (prg = obj->pIndexData->first_prog ; prg ; prg = prg->next)
				{
					if (prg->trigger == TRIG_COMMAND && prg->cmd == cmd)
					{
						if (number_percent() <= prg->percentage)
						{
							start_object_program( ch, obj, prg, argument);
							pop_call();
							return;
						}
					}
				}
			}
		}
	}

	if (!IS_NPC(ch))
	{
		ch->pcdata->last_cmd = *cmd_table[cmd].do_fun;

		if (IS_SET(cmd_table[cmd].flags, CMD_IDLE))
		{
			ch->pcdata->idle = 0;
		}

		if (ch->pcdata->editmode == MODE_RESTRICTED && IS_SET(cmd_table[cmd].flags, CMD_QUIT))
		{
			ch_printf(ch, "You cannot use quit commands while editing.\n\r");
			pop_call();
			return;
		}
	}

	(*cmd_table[cmd].do_fun) ( ch, argument );

	if (!IS_SET(cmd_table[cmd].flags, CMD_QUIT) && ch != NULL && !IS_NPC(ch))
	{
		if (IS_SET(ch->act, PLR_WIZTIME))
		{
			delta = get_game_usec() - last_time;
			ch_printf(ch, "(%lld usec to execute command)\n\r", delta);
		}
	}

	pop_call();
	return;
}


bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
	int cmd;
	bool retval;

	push_call("check_social(%p,%p,%p)",ch,command,argument);

	if ((cmd = find_social(command)) == -1)
	{
		pop_call();
		return FALSE;
	}
	retval = check_social_fast(ch, cmd, argument);
	pop_call();
	return( retval );
}

bool check_social_fast( CHAR_DATA *ch, int cmd, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;

	push_call("check_social_fast(%p,%p,%p)",ch,cmd,argument);

	switch ( ch->position )
	{
		case POS_DEAD:
			send_to_char( "Lie still; you are DEAD.\n\r", ch );
			pop_call();
			return TRUE;

		case POS_INCAP:
		case POS_MORTAL:
			send_to_char( "You are hurt far too bad for that.\n\r", ch );
			pop_call();
			return TRUE;

		case POS_STUNNED:
			send_to_char( "You are too stunned to do that.\n\r", ch );
			pop_call();
			return TRUE;

		case POS_SLEEPING:
			send_to_char( "In your dreams, or what?\n\r", ch );
			pop_call();
			return TRUE;
	}

	if ( argument == NULL)
	{
		act( social_table[cmd].others_no_arg, ch, NULL, NULL, TO_ROOM    );
		act( social_table[cmd].char_no_arg,   ch, NULL, NULL, TO_CHAR    );

		pop_call();
		return TRUE;
	}

	one_argument( argument, arg );
	victim = NULL;

	if (arg == NULL || arg[0] == '\0')
	{
		act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
		act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );

		pop_call();
		return TRUE;
	}
	else if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
	}
	else if (victim == ch)
	{
		act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
		act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
	}
	else
	{
		act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
		if (!blocking(victim, ch))
		{
			act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
			act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );
		}

		if (!mprog_social_trigger(social_table[cmd].name, victim, ch))
		{
			if (!MP_VALID_MOB(ch) && MP_VALID_MOB(victim))
			{
				switch (number_bits(2))
				{
					case 0:
						check_social_fast(victim, cmd, ch->name);
						break;
					case 1:
						check_social(victim, "slap", ch->name);
						break;
					case 2:
						check_social(victim, "ponder", ch->name);
						break;
				}
			}
		}
	}
	pop_call();
	return TRUE;
}

/*
	Return true if an argument is completely numeric.
*/

bool is_number( char *argument )
{
	push_call("is_number(%p)",argument);

	if (*argument == '\0')
	{
		pop_call();
		return FALSE;
	}

	for ( ; *argument != '\0'; argument++ )
	{
		if (!isdigit(*argument) && (*argument != '-') && (*argument != '+'))
		{
			pop_call();
			return FALSE;
		}
	}
	pop_call();
	return TRUE;
}


bool is_number_argument( char *argument )
{
	push_call("is_number_argument(%p)",argument);

	if (*argument != '\0')
	{
		for ( ; *argument != '\0'; argument++ )
		{
			if (!isdigit(*argument) && *argument != '-' && *argument != '+' && *argument != '.')
			{
				pop_call();
				return FALSE;
			}
		}
	}
	pop_call();
	return TRUE;
}


/*
	Given a string like 14.foo, Return 14 and 'foo'
*/

int number_argument( char *argument, char *arg )
{
	char *pdot;
	int number;

	push_call("number_argument(%p,%p)",argument,arg);

	if ((pdot = strstr(argument, ".")) != NULL)
	{
		*pdot	= '\0';
		number	= atol(argument);
		*pdot	= '.';
		strcpy(arg, pdot+1);

		pop_call();
		return number;
	}
	strcpy(arg, argument);

	pop_call();
	return 1;
}

/*
	Pick off one argument from a string and Return the rest.
	Understands quotes, lowercases.
*/

char *one_argument( char *argument, char *arg_first )
{
	char cEnd;

	push_call("one_argument(%p,%p)",argument,arg_first);

	if (argument == NULL)
	{
		log_string("one_argument: argument == NULL");
		dump_stack();
		pop_call();
		return NULL;
	}

	while (*argument == ' ' || *argument == '\n' || *argument == '\r' || *argument == 27)
	{
    		argument++;
	}

	cEnd = ' ';

	if (*argument == '\'' || *argument == '"')
	{
		cEnd = *argument++;
	}

	while (*argument != '\0' && *argument != '\n' && *argument != '\r')
	{
		if (*argument == cEnd)
		{
			argument++;
			break;
		}

		*arg_first = tolower(*argument);

		arg_first++;
		argument++;
	}
	*arg_first = '\0';

	while (*argument == ' ' || *argument == '\n' || *argument == '\r' || *argument == 27)
	{
		argument++;
	}
	pop_call();
	return argument;
}


char *one_argument_nolower( char *argument, char *arg_first )
{
	char cEnd;

	push_call("one_argument(%p,%p)",argument,arg_first);

	if (argument == NULL)
	{
		log_string("one_argument_nolower(), argument is NULL");
		pop_call();
		return NULL;
	}

	while ( *argument==' ' || *argument=='\n' || *argument=='\r' || *argument==27)
	{
    		argument++;
	}
	cEnd = ' ';
	if ( *argument == '\'' || *argument == '"' )
	{
		cEnd = *argument++;
	}

	while ( *argument != '\0' && *argument != '\n' && *argument != '\r' )
	{
		if ( *argument == cEnd )
		{
			argument++;
			break;
		}

		*arg_first = *argument;

		arg_first++;
		argument++;
	}
	*arg_first = '\0';

	while ( *argument==' ' || *argument=='\n' || *argument=='\r' || *argument==27)
	{
		argument++;
	}
	pop_call();
	return argument;
}

void process_command( CHAR_DATA *ch, char *argument)
{
	char buf2[MAX_INPUT_LENGTH], buf3[MAX_INPUT_LENGTH];
	char *pti, *pto;
	bool found;

	push_call("process_command(%p,%p)",ch,argument);

	found = FALSE;

	for (pti = argument, pto = buf3 ; *pti != '\0' ; pti++, pto++)
	{
		if (*pti == '&')
		{
			if (!found)
			{
				*pto = '\0';
				pto = buf2-1;   /* Switch recording medium */
				found = TRUE;
			}
			else
			{
				*pto = '\r';
			}
		}
		else
		{
			*pto = *pti;
		}
	}

	if (found)
	{
		*pto = '\r';
		pto++;
	}
	*pto = '\0';

	if (found)
	{
		/* Add buf2 to the inbuf */

		if (*ch->desc->inbuf == '\0')
		{
			ch->desc->intop = str_cpy_max( ch->desc->inbuf, buf2, MAX_INPUT_LENGTH);
		}
		else
		{
			if (ch->desc->inbuf[ch->desc->intop-1] != '\r'
			&&  ch->desc->inbuf[ch->desc->intop-1] != '\n')
			{
				ch->desc->intop = str_apd_max(ch->desc->inbuf, "\r", ch->desc->intop, MAX_INPUT_LENGTH);
				ch->desc->intop = str_apd_max(ch->desc->inbuf, buf2, ch->desc->intop, MAX_INPUT_LENGTH);
			}
			else
			{
				ch->desc->intop = str_apd_max(ch->desc->inbuf, buf2, ch->desc->intop,	MAX_INPUT_LENGTH);
			}
		}
	}
	interpret(ch, buf3);

	pop_call();
	return;
}

void preprocess_command( CHAR_DATA *ch, char *argument)
{
	char buf2[MAX_INPUT_LENGTH], buf3[MAX_INPUT_LENGTH];
	char buf1[MAX_INPUT_LENGTH];
	char *pti, *pto;
	bool found;

	push_call("preprocess_command(%p,%p)",ch,argument);

	found = FALSE;

	for (pti = argument, pto = buf3 ; *pti != '\0' ; pti++, pto++)
	{
		if (*pti == '&')
		{
			if (!found)
			{
				*pto = '\0';
				pto = buf2-1;   /* Switch recording medium */
				found = TRUE;
			}
			else
			{
				*pto = '\r';
			}
		}
		else
		{
			*pto = *pti;
		}
	}
	if (found)
	{
		*pto = '\r';
		pto++;
	}
	*pto = '\0';

	if (found)
	{
		if (*ch->desc->inbuf == '\0')
		{
			ch->desc->intop = str_cpy_max( ch->desc->inbuf, buf2, MAX_INPUT_LENGTH);
		}
		else
		{
			strcpy( buf1, ch->desc->inbuf);
			ch->desc->intop = str_cpy_max( ch->desc->inbuf, buf2, MAX_INPUT_LENGTH);
			ch->desc->intop = str_apd_max(ch->desc->inbuf, "\r", ch->desc->intop, MAX_INPUT_LENGTH);
			ch->desc->intop = str_apd_max(ch->desc->inbuf, buf1, ch->desc->intop, MAX_INPUT_LENGTH);
		}
	}
	interpret(ch, buf3);

	pop_call();
	return;
}


void start_object_program ( CHAR_DATA * ch, OBJ_DATA *obj, OBJ_PROG *prg, char *argument)
{
	push_call("start_object_program(%p,%p,%p,%p)",ch,obj,prg,argument);

	if (ch->desc == NULL || ch->desc->character != ch || IS_NPC(ch))
	{
		pop_call();
		return;
	}

	/*
		No lock-ups here including wait oriented char lock-ups
	*/

	if (IS_SET(ch->pcdata->interp, INTERP_OPROG) || ch->wait > 0)
	{
		pop_call();
		return;
	}

	if (ch->desc->connected == CON_EDITING || IS_SET(ch->pcdata->interp, INTERP_DUMP))
	{
		pop_call();
		return;
	}

	SET_BIT(ch->pcdata->interp, INTERP_OPROG);

	object_program( ch, obj, prg, argument );

	REMOVE_BIT(ch->pcdata->interp, INTERP_OPROG);

	pop_call();
	return;
}

void check_object_prog( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_PROG *prg, char *argument, int if_val )
{
	push_call("check_object_prog(%p,%p,%p,%p,%p)",ch,obj,prg,argument,if_val);

	switch( prg->if_symbol )
	{
		case '<':
			if (if_val < prg->if_value)
			{
				object_program( ch, obj, prg->true, argument );
			}
			else
			{
				object_program( ch, obj, prg->false , argument);
			}
			pop_call();
			return;

		case '>':
			if (if_val > prg->if_value)
			{
				object_program( ch, obj, prg->true, argument );
			}
			else
			{
				object_program( ch, obj, prg->false , argument);
			}
			pop_call();
			return;

		case '=':
			if (if_val == prg->if_value)
			{
				object_program( ch, obj, prg->true, argument );
			}
			else
			{
				object_program( ch, obj, prg->false , argument);
			}
			pop_call();
			return;

		case '!':
			if (if_val != prg->if_value)
			{
				object_program( ch, obj, prg->true, argument );
			}
			else
			{
				object_program( ch, obj, prg->false , argument);
			}
			pop_call();
			return;
	}
	pop_call();
	return;
}

void object_program ( CHAR_DATA * ch, OBJ_DATA *obj, OBJ_PROG *prg, char *argument)
{
	OBJ_DATA *obj2;
	int if_val;
	int cnt;
	char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
	char *pt, pb[2];

	char * const he_she		[] = { "it",  "he",  "she" };
	char * const him_her	[] = { "it",  "him", "her" };
	char * const his_her	[] = { "its", "his", "her" };

	push_call("object_program(%p,%p,%p,%p)",ch,obj,prg,argument);

	if (prg == NULL || obj == NULL || ch == NULL)
	{
		pop_call();
		return;
	}

	if (ch->desc == NULL || ch->desc->character != ch)
	{
		pop_call();
		return;
	}

	if_val = 0;

	switch (prg->obj_command)
	{
		case OPROG_ECHO:
			if (!str_suffix("\n\r", prg->argument))
			{
				ch_printf_color(ch, "%s", ansi_justify(prg->argument, get_page_width(ch)));
			}
			else
			{
				ch_printf_color(ch, "%s\n\r", ansi_justify(prg->argument, get_page_width(ch)));
			}
			object_program(ch, obj, prg->true , argument);
			pop_call();
			return;

		case OPROG_IF_HAS_OBJECT:
			for (obj2 = ch->first_carrying ; obj2 ; obj2 = obj2->next_content)
			{
				if (obj2->pIndexData->vnum == prg->if_value)
				{
					break;
				}
			}
			if (obj2)
			{
				object_program(ch, obj, prg->true, argument);
			}
			else
			{
				object_program(ch, obj, prg->false, argument);
			}
			pop_call();
			return;

		case OPROG_JUNK:
			junk_obj(obj);
			pop_call();
			return;

		case OPROG_GOD_COMMAND:
		case OPROG_GOD_ARGUMENT:
		case OPROG_COMMAND:
		case OPROG_ARGUMENT:
			pb[1]	= '\0';
			buf[0]	= '\0';

			for (pt = prg->argument ; *pt != '\0' ; pt++)
			{
				if (*pt != '$')
				{
					*pb = *pt;
					strcat( buf, pb );
				}
				else
				{
					pt++;
					switch (*pt)
					{
						case 'f':
							if (who_fighting(ch))
							{
								one_argument_nolower(ch->fighting->who->name, buf2);
								strcat(buf, buf2);
							}
							break;
						case 'F':
							if (who_fighting(ch))
							{
								strcat(buf, get_name(ch->fighting->who));
							}
							break;
						case 'i': strcat(buf, ch->name);					break;
						case ' ':	cat_sprintf(buf, "%s ", ch->name);			break;
						case 'I':	strcat(buf, get_name(ch));				break;
						case 'e':	strcat(buf, he_she [URANGE(0, ch->sex, 2)]);	break;
						case 'm': strcat(buf, him_her[URANGE(0, ch->sex, 2)]);	break;
						case 's':	strcat(buf, his_her[URANGE(0, ch->sex, 2)]);	break;
						case 'o': strcat(buf, obj->short_descr);			break;
						case '/': strcat(buf, "\n\r");					break;
						case '$':	strcat(buf, "$" );						break;
						default:
							cat_sprintf(buf, "%s ", ch->name);
							log_printf( "[%u] Bad Oprog $var: %c", obj->pIndexData->vnum, *pt);
							break;
					}
				}
 			}

			switch (prg->obj_command)
			{
				case OPROG_GOD_COMMAND:
				case OPROG_GOD_ARGUMENT:
					ch->trust = MAX_LEVEL-1;
					break;
			}

			for (buf2[0] = '\0', pt = buf ; *pt != '\0' ; pt++)
			{
				if (*pt == '\n')
				{
					continue;
				}
				if (*pt != '&' && *pt != '\r')
				{
					*pb = *pt;
					strcat( buf2, pb);
				}
				else if (*(pt+1) != '\0')
				{
					interpret(ch, buf2);
					buf2[0] = '\0';
				}
			}

			switch (prg->obj_command)
			{
				case OPROG_GOD_ARGUMENT:
				case OPROG_ARGUMENT:
					cat_sprintf(buf2, " %s", argument);
					break;
			}

			interpret(ch, buf2);

			ch->trust = ch->level;

			object_program(ch, obj, prg->true, argument);

			pop_call();
			return;

		case OPROG_APPLY:
			switch (prg->if_check)
			{
				case 1:
					ch->hit = UMIN(ch->max_hit, ch->hit + prg->if_value);

					if (ch->hit < 0 && ch->fighting == NULL)
					{
						ch->trust = ch->level;
						raw_kill( ch );
					}
					break;
				case 2:
					ch->move = URANGE(0, ch->move + prg->if_value, ch->max_move);
					break;
				case 3:
					ch->mana = URANGE(0, ch->mana + prg->if_value, ch->max_mana);
					break;
				case 4:
					ch->alignment = URANGE(-1000, ch->alignment + prg->if_value, 1000);

					check_zap(ch, TRUE);
			}
			object_program(ch, obj, prg->true, argument);
			break;

		case OPROG_QUEST_ADD:
			cnt  = get_quest_bits( obj->obj_quest, prg->quest_offset, prg->quest_bits);
			cnt += prg->if_value;
			set_quest_bits( &obj->obj_quest, prg->quest_offset, prg->quest_bits, cnt);

			SET_BIT(obj->extra_flags, ITEM_MODIFIED);

			object_program( ch, obj, prg->true, argument );
			break;

		case OPROG_QUEST_SET:
			set_quest_bits( &obj->obj_quest, prg->quest_offset, prg->quest_bits, prg->if_value);

			SET_BIT(obj->extra_flags, ITEM_MODIFIED);

			object_program( ch, obj, prg->true, argument );
			break;

		case OPROG_IF:
			switch( prg->if_check )
			{
				case OIF_USER_CLASS:
					if_val = ch->class;
					break;
				case OIF_USER_POSITION:
					if_val = ch->position;
					break;
				case OIF_USER_GOLD:
					if_val = ch->gold;
					break;
				case OIF_USER_AREA:
					if_val = ch->in_room->area->low_r_vnum;
					break;
				case OIF_USER_ROOM_NUM:
					if_val = ch->in_room->vnum;
					break;
				case OIF_RANDOM_PERCENT:
					if_val = number_percent();
					break;
				case OIF_USER_WHICH_GOD:
					if_val = which_god(ch);
					break;
				case OIF_USER_ALIGNMENT:
					if_val = ch->alignment;
					break;
				case OIF_USER_LEVEL:
					if_val = ch->level;
					break;
				case OIF_USER_RACE:
					if_val = ch->race;
					break;
				case OIF_USER_SEX:
					if_val = URANGE(0, ch->sex, 2);
					break;
				case OIF_WEAR_LOC:
					if_val = obj->wear_loc;
					break;
				case OIF_USER_PERCENT_MOVE:
					if_val = ch->move*100/UMAX(1, ch->max_move);
					break;
				case OIF_USER_PERCENT_HITPT:
					if_val = ch->hit*100/UMAX(1, ch->max_hit);
					break;
				case OIF_USER_PERCENT_MANA:
					if_val = ch->mana*100/UMAX(1, ch->max_mana);
					break;
				case OIF_MCLASS_WARRIOR:
				case OIF_MCLASS_GLADIATOR:
				case OIF_MCLASS_MARAUDER:
				case OIF_MCLASS_NINJA:
				case OIF_MCLASS_DRUID:
				case OIF_MCLASS_SORCERER:
				case OIF_MCLASS_SHAMAN:
				case OIF_MCLASS_WARLOCK:
					if_val = ch->pcdata->mclass[prg->if_check];
					break;
				case OIF_USER_SECTOR:
					if_val = ch->in_room->sector_type;
					break;
				case OIF_TIME_OF_DAY:
					if_val = mud->time_info->hour;
					break;
				case OIF_WEATHER:
					if_val = (!IS_OUTSIDE(ch) || NO_WEATHER_SECT(ch->in_room->sector_type)) ? -1 : ch->in_room->area->weather_info->sky;
					break;
				case OIF_USER_FIGHTING:
					if_val = (who_fighting(ch) != NULL);
					break;
			}
			check_object_prog(ch, obj, prg, argument, if_val);
			break;

		case OPROG_OBJECT_QUEST_IF:
			if_val = get_quest_bits( obj->obj_quest, prg->quest_offset, prg->quest_bits );

			check_object_prog(ch, obj, prg, argument, if_val);
			break;

		case OPROG_PLAYER_QUEST_IF:
			if_val = get_quest_bits(ch->pcdata->quest[obj->pIndexData->area->low_r_vnum/100], prg->quest_offset, prg->quest_bits );

			check_object_prog(ch, obj, prg, argument, if_val);
			break;

		default:
			log_printf("object_progam, unknown trigger: %d", prg->obj_command);
			break;
	}
	pop_call();
	return;
}


void do_a( CHAR_DATA *ch, char *argument )
{
	push_call("do_a(%p,%p)",ch,argument);

	send_to_char( "Ah, what?\n\r", ch );

	pop_call();
	return;
}

