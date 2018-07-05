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


char *	draw_room		args ( ( CHAR_DATA *ch, int vnum ) );
void		draw_map		args ( ( CHAR_DATA *ch ) );

int					area_map_x = 0;
int					area_map_y = 0;
char					area_map_buf[11];
int					area_map[201][91];
int					valid_exit;

sh_int		area_map_color[]	= {0,1,0,1,1,0,0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,
							   7,5,3,2,7,6,4,4,4,6,3,1,2,3,4,6,6,0,2,3,7,0};

sh_int		area_map_palet0[]	= { 42, 35, 35, 43, 35,124, 43, 43,
							    35, 43, 45, 43, 43, 43, 43, 43};

sh_int		area_map_palet1[]	= {126,247,247,109,247,120,108,116,
							   247,106,113,118,107,117,119,110};

typedef struct map_data		MAP_DATA;
typedef struct map_room		MAP_ROOM;

struct __attribute__((packed)) map_data
{
	char		id[4];
	short int	version;
	short int length;
	short int tileset;
	short int width;
	short int height;
	short int spare;
};

struct __attribute__((packed)) map_room
{
	short int	sector:6;
	short int exit:6;
	short int spare:4;
};

int valid_map_exit(CHAR_DATA *mob, int vnum, sh_int door)
{
	push_call("valid_map_exit");

	if (room_index[vnum]->exit[door] == NULL)
	{
		pop_call();
		return -1;
	}

	if (room_index[room_index[vnum]->exit[door]->to_room] == NULL)
	{
		pop_call();
		return -1;
	}

	if (IS_SET(room_index[room_index[vnum]->exit[door]->to_room]->room_flags, ROOM_IS_ENTRANCE))
	{
		pop_call();
		return -1;
	}

	if (IS_SET(room_index[vnum]->exit[door]->exit_info, EX_RIP))
	{
		pop_call();
		return -1;
	}
	pop_call();
	return room_index[vnum]->exit[door]->to_room;
}

void build_area_map(int vnum, CHAR_DATA *mob, sh_int x, sh_int y)
{
	bool door;
	sh_int xx = 0, yy = 0;

	area_map[x][y] = vnum;

	if (IS_SET(room_index[vnum]->room_flags, ROOM_MAZE))
	{
		return;
	}

	for (door = 0 ; door < 4 ; door++)
	{
		if ((valid_exit = valid_map_exit(mob, vnum, door)) != -1)
		{
			switch (door)
			{
				case 0: xx = x;	yy = y+1;	break;
				case 1: xx = x+1;	yy = y;	break;
				case 2: xx = x;	yy = y-1;	break;
				case 3: xx = x-1;	yy = y;	break;
			}
			if (xx < 0 || xx > area_map_x || yy < 0 || yy > area_map_y)
			{
				continue;
			}
			if (area_map[xx][yy])
			{
				continue;
			}
			build_area_map(valid_exit, mob, xx, yy);
		}
	}
}



void do_wizmap( CHAR_DATA *ch, char *argument )
{
	char tmp[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH * 2];
	int x, y, size;

	push_call("do_wizmap(%p,%p)",ch,argument);

	if (!can_olc_modify(ch, ch->in_room->vnum))
	{
		send_to_char("This room is not in your allocated range.\n\r", ch);
		pop_call();
		return;
	}

	memset(area_map, 0, sizeof(area_map));
	memset(buf, 0, MAX_STRING_LENGTH);

	if (!str_prefix("mode", argument))
	{
		ch->pcdata->vt100_mode = atol(one_argument(argument, tmp));
		pop_call();
		return;
	}

	if (!str_prefix("goto ", argument))
	{
		if (sscanf(argument, "%s %d %d", buf, &x, &y) != 3)
		{
			send_to_char("Syntax: wizmap goto <x> <y>\n\r", ch);
			pop_call();
			return;
		}
		area_map_y = 90;
		area_map_x = 200;

		build_area_map(ch->in_room->vnum, ch, area_map_x/2, area_map_y/2);

		if (area_map[URANGE(0, area_map_x / 2 + x, area_map_x)][URANGE(0, area_map_y / 2 + y, area_map_y)])
		{
			sprintf(buf, "%d", area_map[area_map_x / 2 + x][area_map_y / 2 + y]);
			do_goto(ch, buf);
		}
		else
		{
			send_to_char("No room found at given cooridinate.\n\r", ch);
		}
		pop_call();
		return;
	}
	else
	{
		size = atol(argument);
	}

	area_map_y = get_pager_breakpt(ch) -1;
	area_map_x = get_page_width(ch) -1;

	area_map_y = URANGE(12, size * 2, area_map_y);
	area_map_x = URANGE(28, size * 4, area_map_x);

	build_area_map(ch->in_room->vnum, ch, area_map_x/2, area_map_y/2);

	if (size == 666)
	{

	}

	if (ch->pcdata->vt100_mode == 9 && IS_GOD(ch))
	{
		draw_map(ch);
		pop_call();
		return;
	}

	for (buf[0] = '\0', y = area_map_y ; y >= 0 ; y--)
	{
		for (tmp[0] = '\0', x = 0 ; x <= area_map_x ; x++)
		{
			strcat(tmp, draw_room(ch, area_map[x][y]));
		}
		if (strlen(tmp) > area_map_x + 1)
		{
			cat_sprintf(buf, "%s\n\r", tmp);
		}
	}

	/*
		Toggle vt100 graphics mapping to write output - Scandum 03-03-2003
	*/

	switch (ch->pcdata->vt100_mode)
	{
		case 0:
			ch_printf(ch, "%s", ansi_compress(ch, buf, COLOR_TEXT, VT102_DIM));
			break;
		default:
			ch_printf(ch, "\033(0%s\033(B", ansi_compress(ch, buf, COLOR_TEXT, VT102_DIM));
			break;
	}
	pop_call();
	return;
}


char *draw_room(CHAR_DATA *ch, int vnum)
{
	static char buf[11];
	sh_int door, exits;

	push_call("draw_room(%p,%p)",ch,vnum);

	if (vnum == 0)
	{
		pop_call();
		return " ";
	}

	if (IS_SET(room_index[vnum]->room_flags, ROOM_MAZE))
	{
		switch (ch->pcdata->vt100_mode)
		{
			case 0:
				sprintf(buf, "{118}M");
				break;
			case 1:
				sprintf(buf, "{118}%c", 247);
				break;
			default:
				sprintf(buf, "{118}X");
				break;
		}
		pop_call();
		return buf;
	}

	if (vnum == ch->in_room->vnum)
	{
		switch (ch->pcdata->vt100_mode)
		{
			case 0:
				sprintf(buf, "{118}X");
				break;
			case 1:
				sprintf(buf, "{118}%c", 247);
				break;
			default:
				sprintf(buf, "{118}X");
				break;
		}
		pop_call();
		return buf;
	}

	sprintf(buf, "{%d%d8}", area_map_color[room_index[vnum]->sector_type], area_map_color[SECT_MAX + room_index[vnum]->sector_type]);

	for (exits = door = 0 ; door < 4 ; door++)
	{
		if (valid_map_exit(ch, room_index[vnum]->vnum, door) != -1)
		{
			SET_BIT(exits, 1 << door);
		}
	}
	switch (ch->pcdata->vt100_mode)
	{
		case 0:
			cat_sprintf(buf, "%c", area_map_palet0[exits]);
			break;
		default:
			cat_sprintf(buf, "%c", area_map_palet1[exits]);
			break;
	}
	pop_call();
	return buf;
}

/*
	Scandum - 28-05-2003
*/

void draw_map(CHAR_DATA *ch)
{
	FILE	*fp;
	MAP_DATA	*map;
	MAP_ROOM	*room;
	char		buf[100];
	int		x, y, door, exits;

	push_call("draw_map(%p)",ch);

	ALLOCMEM(map,  MAP_DATA, 1);
	ALLOCMEM(room, MAP_ROOM, 1);

	memcpy(map->id, "MAP$", 4);
	map->version	= 1 * 256;
	map->length	= sizeof(MAP_DATA);
	map->tileset	= 0;
	map->width	= area_map_x + 1;
	map->height	= area_map_y + 1;
	map->spare	= 0;

	sprintf(buf, "maps/%04d.map", ch->in_room->area->low_r_vnum / 100);

	if ((fp = my_fopen(buf, "w", FALSE)) == NULL)
	{
		ch_printf(ch, "Failed to open map file.\n\r");
		pop_call();
		return;
	}

	fwrite(map, sizeof(MAP_DATA), 1, fp);

	for (y = area_map_y ; y >= 0 ; y--)
	{
		for (x = 0 ; x <= area_map_x ; x++)
		{
			if (area_map[x][y])
			{
				for (exits = door = 0 ; door < 6 ; door++)
				{
					if (valid_map_exit(ch, area_map[x][y], door) != -1)
					{
						SET_BIT(exits, 1 << door);
					}
				}
				room->sector	= room_index[area_map[x][y]]->sector_type;
				room->exit	= exits;
			}
			else
			{
				room->sector	= 63; /* reserved for NULL rooms */
				room->exit	= 0;
			}
			fwrite(room, sizeof(MAP_ROOM), 1, fp);
		}
	}
	my_fclose(fp);

	FREEMEM(map);
	FREEMEM(room);

	pop_call();
	return;
}
