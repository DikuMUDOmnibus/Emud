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

/*
	race specialties,
	written by Manwe
	08/10/1999

	Changed to requirements :
	rspec_req : race specialty
	cspec_req : class specialty
	sspec_req : spell requirement (does the spell come from a god ?)
*/

bool rspec_req( CHAR_DATA *ch, unsigned long int attr)
{
	push_call("rspec_req(%p,%p)",ch,attr);

	if (ch == NULL)
	{
		pop_call();
		return FALSE;
	}

	if (race_table[ch->race].flags & attr)
	{
		pop_call();
		return TRUE;
	}
	else
	{
		pop_call();
		return FALSE;
	}
}

bool cspec_req( CHAR_DATA *ch, unsigned long int attr)
{
	push_call("cspec_req(%p,%p)",ch,attr);

	if (IS_NPC(ch))
	{
		pop_call();
		return FALSE;
	}

	if ((class_table[ch->class].spec) & attr)
	{
		pop_call();
		return TRUE;
	}
	else
	{
		pop_call();
		return FALSE;
	}

	pop_call();
	return FALSE;
}

int get_age_bonus(CHAR_DATA *ch, int type)
{
	int bonus;

	push_call("get_age_bonus(%p,%p)",ch,type);

	bonus = 0;

	if (IS_SET(race_table[ch->race].flags, type))
	{
		switch (type)
		{
			case RSPEC_AGE_AC:
				bonus -= ch->level/6  + ch->pcdata->reincarnation*3/2;
				break;
			case RSPEC_AGE_HR:
				bonus += ch->level/6  + ch->pcdata->reincarnation*2/1;
				break;
			case RSPEC_AGE_DR:
				bonus += ch->level/18 + ch->pcdata->reincarnation*1/2;
				break;
		}
	}
	pop_call();
	return bonus;
}
