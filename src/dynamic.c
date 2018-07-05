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

void process_dyn_position();
void process_dyn_sector();
void process_dyn_test();

typedef void DO_DYN	args( ( ) );

struct dyn_type
{
	char			*	const name;
	DO_DYN		*	do_dyn;
};

const struct dyn_type dyn_table[] =
{
	{	"position",	process_dyn_position	},
	{	"sector",		process_dyn_sector		},
	{	"test",		process_dyn_test		},
	{	"",			NULL					}
};

ROOM_INDEX_DATA *dyn_room;
CHAR_DATA *dyn_char;
char dyn_out[MAX_STRING_LENGTH];
long long dyn_arg[4];

void get_dyn_argument(char *input)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int cnt, arg_cnt;

	push_call("get_dyn_argument(%p)",input);

	arg1[0] = '\0';
	memset(dyn_arg, 0, 4 * sizeof(long long));

	for (cnt = arg_cnt = 0 ; input[cnt] != '\0' && input[cnt] != ']' ; cnt++)
	{
		switch (input[cnt])
		{
			case ' ':
				arg2[0] = '\0';
				while (++cnt && input[cnt] != '\0' && input[cnt] != ' ' && input[cnt] != ']')
				{
					cat_sprintf(arg2, "%c", input[cnt]);
				}
				if (arg_cnt < 4)
				{
					dyn_arg[arg_cnt] = UMAX(0, atol(arg2));
				}
				arg_cnt++;
				cnt--;
				break;
			default:
				cat_sprintf(arg1, "%c", input[cnt]);
				break;
		}
	}

	for (cnt = 0 ; *dyn_table[cnt].name != '\0' ; cnt++)
	{
		if (!strcmp(dyn_table[cnt].name, arg1))
		{
			(*dyn_table[cnt].do_dyn) ();
			break;
		}
	}
	pop_call();
	return;
}

void get_dyn_output(char *input)
{
	int cnt;

	push_call("get_dyn_output(%p)",input);

	dyn_out[0] = 0;

	for (cnt = 0 ; input[cnt] != '\0' ; cnt++)
	{
		switch (input[cnt])
		{
			case '[':
				get_dyn_argument(&input[cnt + 1]);
				while (input[cnt] != '\0' && input[cnt] != ']')
				{
					cnt++;
				}
				break;
			default:
				cat_sprintf(dyn_out, "%c", input[cnt]);
				break;
		}
		if (input[cnt] == '\0')
		{
			pop_call();
			return;
		}
	}
	pop_call();
	return;
}

void process_dyn_position()
{
	char * const dynamic_pos_earth [] =
	{
		"dead",
		"mortaly wounded",
		"incapitated",
		"stunned",
		"sleeping",
		"resting on [sector 1]",
		"sitting on",
		"fighting on",
		"standing on"
	};

	char * const dynamic_pos_water [] =
	{
		"floating above",
		"sailing on",
		"swimming in"
	};

	if (CAN_FLY(dyn_char))
	{
		strcat(dyn_out, dynamic_pos_water[0]);
		return;
	}

	if (IS_SET(sector_table[dyn_room->sector_type].flags, SFLAG_SWIM))
	{
		OBJ_DATA *boat;

		for (boat = dyn_char->first_carrying ; boat ; boat = boat->next_content)
		{
			if (boat->item_type == ITEM_BOAT)
			{
				strcat(dyn_out, dynamic_pos_water[1]);
				return;
			}
		}
		strcat(dyn_out, dynamic_pos_water[2]);
	}
	else
	{
		strcat(dyn_out, dynamic_pos_earth[dyn_char->position]);
	}
}

void process_dyn_sector()
{
	strcat(dyn_out, sector_table[dyn_room->sector_type].sector_name);
}

void process_dyn_test()
{
	cat_sprintf(dyn_out, "%lld %lld %lld %lld", dyn_arg[0], dyn_arg[1], dyn_arg[2], dyn_arg[3]);
}

void process_dyn_sector_climate()
{
	char * const dynamic_sector_climate[SECT_MAX][9] =
	{
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},

		{
			"frozen wasteland",
			"desolated wasteland",
			"cracked wasteland",
			"misty plain",
			"green plain",
			"arid plain",
			"damp field",
			"sodden field",
			"verdant field"
		},

		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"frigid rock hill",
			"dusty rock hill",
			"arid rock hill",
			"frosty hill",
			"lush hill",
			"flourishing hill",
			"frigid sand slope",
			"boggy sand slope",
			"muddy sand slope"
		},
		{
			"ice-capped mountain",
			"towering mountain",
			"rocky mountain",
			"windswept mountain",
			"soaring mountain",
			"balmy mountain",
			"waterlogged mountain",
			"slippery mountain",
			"steamy mountain"
		},
		{
			"icebound lake",
			"shallow lake",
			"tepid lake",
			"iceblue lake",
			"azure lake",
			"clouded lake",
			"foggy lake",
			"billowing lake",
			"murky lake"
		},
		{
			"glacial river",
			"sluggish river",
			"murky river",
			"steel grey river",
			"winding river",
			"swampy river",
			"swollen stream",
			"swirling stream",
			"hazy stream"
		},
		{
			"arctic ocean",
			"cold ocean",
			"azure ocean",
			"leaden ocean",
			"rippling ocean",
			"silken ocean",
			"chilly ocean",
			"floroush ocean",
			"torrid ocean"
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"frigid sand plain",
			"barren sand plain",
			"parched sand plain",
			"frozen desert",
			"sandy desert",
			"sultry desert",
			"damp desert",
			"fertile desert",
			"sweltering desert"
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		}
	};
	if (dynamic_sector_climate[0][0])
	{
		return;
	}
}

char *	const	dyn_road_exits			[]	=
{
	"at the end of a dirt road",
	"on a dirt road",
	"on a dirt intersection",
	"on a dirt crossroads"
};

/*
	SKY 0...3		TEMPERATURE 0...1
*/

char *	const	dyn_road_sky_wind_temp	[]	=
{
	"dust blowing softly in the breeze",
	"small puddles muddying the ground",
	"soft raindrops creating muddy puddles",
	"the pouring rain overflowing the puddles muddying the ground",

	"a strong wind swirling dust around",
	"a strong wind rippling small puddles on the ground",
	"soft raindrops creating muddy puddles",
	"the pouring rain overflowing the puddles muddying the ground",

	"feathery strokes of frost covering the ground",
	"powdery snow blowing white in the harsh wind",
	"the falling snow covering the ground in a white blanket",
	"the falling snow creating enormous drifts on the ground",

	"feathery strokes of frost covering the ground",
	"powdery snow blowing softly in the breeze",
	"the falling snow covering the ground in a white blanket",
	"the falling snow creating enormous drifts on the ground"
};

/*
	SUN 0...3		SKY 0...3		TEMPERATURE 0...1
*/

char *	const	dyn_sun_sky_temp		[]	=
{
	"your surroundings lit only by stars",
	"the rising sun revealing a perfect sky",
	"a canopy of azure blue sky above you, the sun shining warmly",
	"the colors of the sky melting into deep crimson and gold as the sun sets",

	"a few stars barely peeking through the clouded skies above",
	"the sun painting the clouded sky in shades of deep blue and pink as it rises",
	"the clouds lowering over your head in shades of grey and lavender",
	"the sun sinking into a band of red and amber clouds as it sets",

	"the rain falling inky and black in the night",
	"the rain greeting the sun as it peeks above the horizon",
	"silvery rain falling from a dark grey sky",
	"the rain growing purple as the sun sets for the day",

	"the dark velvet of the sky only illuminated by the sudden flash of lightning",
	"a brilliant blue bolt of lightning greeting the sun as it cautiously rises",
	"the leaden light surrounding you brightened by occasional flashes of lightning",
	"an enormous bolt of lightning striking as the sun lowers its head for the night",


	"the stars sparkling like diamonds in a midnight blue sky",
	"the light of the stars giving way to the arrival of the sun",
	"the sky a brilliant, cloudless blue",
	"the peacock blue sky giving way to the pinks and golds of the setting sun",

	"the sky clouded, with only the barest light from a few rogue stars lighting your way",
	"the sky painted in broad stripes of red and orange as the sun struggles to be seen through the clouds",
	"the clouds obscuring the warmth of the sun, reducing it to a mere aura of light",
	"the sun gratefully dipping below the horizon, leaving the clouds to the night",

	"whispers of snowflakes falling from the night sky",
	"the sun rising from its sleep to reveal the lacy beauty of falling snow",
	"the silence surrounding you broken only by the soft fall of snow from the clouds above",
	"the sun setting, pulling a blanket of pink over the snow falling silently around you",

	"the blackness of the sky illuminated by the swiftly falling snow",
	"the gold of the sun obscured by the rapidly falling snow",
	"the world transformed into a startling blue white, the only sound the constant fall of snow",
	"the heavy snow falling in shades of pink and orange as the sun lowers its golden curtain"
};


char *	const	dyn_season			[]	=
{
	"The cool, crisp smell of winter gives the air a chilling bite",
	"The scent of tender plants opening to the spring fills the air",
	"The heady scent of larkspur and hyacinth perfume the air",
	"The secret, nostalgic scent of fallen leaves surrounds you"
};

char *	const	dyn_ocean_season		[]	=
{
	"Lonely icebergs pierce through the depths, keeping watch on the distant horizon",
	"A pair of dolphins leap from the water before diving back again to safety",
	"Seals twist and wriggle before diving beneath the surface for more fish",
	"Whales sing their mournful songs as they spray tall geysers of white water into the air"
};

char *	const	dyn_road_sky_temp		[]	=
{
	"Well worn wagon tracks are dug into the cracked ground.",
	"Well worn wagon tracks are dug into the damp ground.",
	"Well worn wagon tracks are dug into the muddy ground.",
	"Well worn wagon tracks are dug into the mud.",

	"Well worn wagon tracks are frozen into the ground.",
	"Wagon tracks are ground into the snowy slush.",
	"Frozen wagon tracks are almost obscured by the new fallen snow.",
	"A thick blanket of unblemished snow covers the ground."
};

char *	const	dyn_ocean_sun_sky_temp	[]	=
{
	"a silver ocean, stars reflected in its black depths",
	"an indigo ocean, sunrise dancing over the waves",
	"a cerulean blue ocean, its surface sparkling with a thousand diamonds",
	"a crimson ocean, the sun setting beneath the horizon",

	"a black ocean, its inky depths untouched by light",
	"a dark grey ocean, the soft pink of sunrise barely showing behind thick clouds",
	"a steel grey ocean, the clouds lowering over its onyx flecked surface",
	"a darkening ocean, the orange haze of sunset coloring the clouds bove",

	"a coal black ocean, dark blue rain piercing its inky surface",
	"a leaden ocean, the sunrise coloring the silvery rain a soft pink",
	"a windswept ocean of teal green, dark silver rain falling from heavy clouds above",
	"a burnt orange ocean, the rain transmuting from silver to dark gold, dancing on its surface",

	"a churning black ocean, heavy gusts of wind blowing the rain in frenzied circles",
	"a blood red ocean, the sunrise revealing angry clouds unleashing their fury over the dark surface",
	"a dark grey ocean, mirroring darker grey clouds above as they mercilessly release their pent up fury",
	"a jade colored ocean, the sunset hiding the storm's malevolence",


	"a silver ocean, stars reflected in its black depths",
	"an indigo ocean, sunrise dancing over the waves",
	"a cerulean blue ocean, its surface sparkling with a thousand diamonds",
	"a crimson ocean, the sun setting beneath the horizon",

	"a black ocean, its inky depths untouched by light",
	"a dark grey ocean, the soft pink of sunrise barely showing behind thick clouds",
	"a steel grey ocean, the clouds lowering over its onyx flecked surface",
	"a darkening ocean, the orange haze of sunset coloring the clouds bove",

	"an ebony ocean, argent flakes of snow whispering down from the night skies",
	"a slate ocean, the obscured sunrise turning the downy flakes of snow into alabaster",
	"a pale grey ocean, pearl white snowflakes casting themselves down upon its surface",
	"a deep purple ocean, flakes of snow falling from a scarlet sky",

	"an ocean the color of jet, heavy, ice-blue snow falling from the skies",
	"a jet black ocean, ice blue snow heavily falling from the skies",
	"a shadowy grey ocean, heavy snows flying down from leaden skies",
	"a deep amethyst ocean, snow falling in a blinding tempest",
};

char *    const     dyn_ocean_wind_1	[]   =
{
	"A barely noticable",
	"A soft",
	"A light",
	"A slight",
	"A",
	"A steady",
	"A strong",
	"A sudden",
	"An angry",
	"A raging",
};

char *	const	dyn_ocean_wind_2	[]	=
{
	"northerly",	"northeasterly",	"easterly",	"southeasterly",
	"southerly",	"southwesterly",	"westerly",	"northwesterly"
};

char *	const	dyn_ocean_wind_3	[]	=
{
	"wind leaves the glass-like surface of the water untouched",
	"wind blows across the waters, leaving gentle ripples in its wake",
	"breeze causes the waters to flutter gently",
	"zephyr blows across the ocean, causing the water to foam",
	"flow of air gusts across the ocean creating small whitecaps",
	"wind kisses the surrounding ocean causing waves to rise",
	"wind blows across the ocean as the waves roll",
	"squall blows in as the waters churn in response",
	"gale flares up as waves violently surge upward",
	"typhoon scours the ocean as unforgiving titanic waves savagely reign"
};

int get_sky( ROOM_INDEX_DATA *room )
{
	return (room->area->weather_info->sky);
}

int get_sun( ROOM_INDEX_DATA *room )
{
	return (mud->sunlight);
}

int get_wind( ROOM_INDEX_DATA *room )
{
	return (room->area->weather_info->wind_speed < 6 ? 0 : 1);
}

int get_wind_speed( ROOM_INDEX_DATA *room )
{
	return URANGE(0, room->area->weather_info->wind_speed, 9);
}

int get_wind_dir (ROOM_INDEX_DATA *room )
{
	return room->area->weather_info->wind_dir;
}

int get_frost( ROOM_INDEX_DATA *room )
{
	return (room->area->weather_info->temperature > 0 ? 0 : 1);
}

int get_season()
{
	return (mud->time_info->month / 4);
}

int count_exits( ROOM_INDEX_DATA *room )
{
	int door, cnt;

	for (door = cnt = 0 ; door < 6 ; door++)
	{
		if (get_exit(room->vnum, door))
		{
			cnt++;
		}
	}
	return URANGE(0, cnt - 1, 3);
}

char *get_dyn_road_exits( ROOM_INDEX_DATA *room )
{
	return dyn_road_exits[count_exits(room)];
}

char *get_dyn_road_sky_wind_temp( ROOM_INDEX_DATA *room )
{
	return dyn_road_sky_wind_temp[get_sky(room) + get_wind(room) * 4 + get_frost(room) * 8];
}

char *get_dyn_sun_sky_temp( ROOM_INDEX_DATA *room )
{
	return dyn_sun_sky_temp[get_sun(room) + get_sky(room) * 4 + get_frost(room) * 16];
}

char *get_dyn_ocean_sun_sky_temp( ROOM_INDEX_DATA *room )
{
	return dyn_ocean_sun_sky_temp[get_sun(room) + get_sky(room) * 4 + get_frost(room) * 16];
}

char *get_dyn_ocean_wind_1( ROOM_INDEX_DATA *room )
{
	return dyn_ocean_wind_1[get_wind_speed(room)];
}

char *get_dyn_ocean_wind_2( ROOM_INDEX_DATA *room )
{
	return dyn_ocean_wind_2[get_wind_dir(room)];
}

char *get_dyn_ocean_wind_3( ROOM_INDEX_DATA *room )
{
	return dyn_ocean_wind_3[get_wind_speed(room)];
}

char *get_dyn_season( )
{
	return dyn_season[get_season()];
}

char *get_dyn_ocean_season( )
{
	return dyn_ocean_season[get_season()];
}

char *get_dyn_road_sky_temp( ROOM_INDEX_DATA *room )
{
	return dyn_road_sky_temp[get_sky(room) + get_frost(room) * 4];
}

char *get_dynamic_description( CHAR_DATA *ch )
{
	static char dyn_buf[MAX_INPUT_LENGTH];

	push_call("get_dynamic_description(%p)",ch);

	dyn_char = ch;
	dyn_room = ch->in_room;


	switch (ch->in_room->sector_type)
	{
		case SECT_ROAD:
			sprintf(dyn_buf, "You are standing %s, %s, %s. %s %s.%s%s",
				get_dyn_road_exits(ch->in_room),
				get_dyn_road_sky_wind_temp(ch->in_room),
				get_dyn_sun_sky_temp(ch->in_room),
				get_dyn_road_sky_temp(ch->in_room),
				get_dyn_season(),
				ch->in_room->description[0] != '\0' ? " " : "\n\r",
				ch->in_room->description);
			pop_call();
			return dyn_buf;

		case SECT_OCEAN:
			sprintf(dyn_buf, "You are looking out over %s. %s %s %s. %s.%s%s",
				get_dyn_ocean_sun_sky_temp(ch->in_room),
				get_dyn_ocean_wind_1(ch->in_room),
				get_dyn_ocean_wind_2(ch->in_room),
				get_dyn_ocean_wind_3(ch->in_room),
				get_dyn_ocean_season(),
				ch->in_room->description[0] != '\0' ? " " : "\n\r",
				ch->in_room->description);
			pop_call();
			return dyn_buf;
		default:
			get_dyn_output(ch->in_room->description);
			strcpy(dyn_buf, dyn_out);
			get_dyn_output(dyn_buf);
			pop_call();
			return dyn_out;
	}
	pop_call();
	return "Incorrect sector type for dynamic descriptions.";
}
