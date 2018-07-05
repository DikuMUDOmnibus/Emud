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

#include <stdarg.h>
#include "emud.h"

/*
	Local functions.
*/

char	* translate (char *);
bool is_note_to args ((CHAR_DATA * ch, NOTE_DATA * pnote));
void note_attach args ((CHAR_DATA * ch));
void note_remove args ((CHAR_DATA * ch, NOTE_DATA * pnote, bool remove_all));


bool is_note_to (CHAR_DATA * ch, NOTE_DATA * pnote)
{
	push_call("is_note_to (%p, %p)",ch,pnote);

	if (IS_NPC(ch) || IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_MUTED))
	{
		pop_call();
		return FALSE;
	}

	if (ch->pcdata->note_topic != pnote->topic && !IS_SET(ch->in_room->room_flags, ROOM_NOTE_BOARD))
	{
		pop_call();
		return FALSE;
	}

	if (ch->name[0] == pnote->sender[0] && !strcasecmp(ch->name, pnote->sender))
	{
		pop_call();
		return TRUE;
	}

	if (is_name(ch->name, pnote->to_list))
	{
		pop_call();
		return TRUE;
	}

	if (is_name ("all", pnote->to_list))
	{
		pop_call();
		return TRUE;
	}

	if (IS_IMMORTAL(ch) && is_name("immortal", pnote->to_list))
	{
		pop_call();
		return TRUE;
	}
	pop_call();
	return FALSE;
}

void note_attach (CHAR_DATA * ch)
{
	NOTE_DATA *pnote;

	push_call("note_attach(%p)",ch);

	if (ch->pcdata->pnote != NULL)
	{
		pop_call();
		return;
	}
	ALLOCMEM (pnote, NOTE_DATA, 1);

	pnote->next		= NULL;
	pnote->prev		= NULL;
	pnote->sender		= STRALLOC(ch->name);
	pnote->date		= STRALLOC ("");
	pnote->to_list 	= STRALLOC ("");
	pnote->subject 	= STRALLOC ("");
	pnote->text		= STRALLOC ("");
	pnote->topic		= -1;
	pnote->room_vnum	= 0;
	ch->pcdata->pnote	= pnote;
	pop_call();
	return;
}

void note_remove (CHAR_DATA * ch, NOTE_DATA * pnote, bool remove_all)
{
	char to_new[MAX_INPUT_LENGTH];
	char to_one[MAX_INPUT_LENGTH];
	FILE *fp;
	char *to_list;

	push_call("note_remove(%p,%p,%p)",ch,pnote,remove_all);

	/*
		Build a new to_list.
		Strip out this recipient.
	*/

	to_new[0] = '\0';
	to_list = pnote->to_list;
	while (*to_list != '\0')
	{
		to_list = one_argument (to_list, to_one);
		if (to_one[0] != '\0' && strcasecmp (ch->name, to_one))
		{
			strcat (to_new, " ");
			strcat (to_new, to_one);
		}
	}

	/*
		Just a simple recipient removal?
	*/

	if (!remove_all && strcasecmp (ch->name, pnote->sender) && to_new[0] != '\0')
	{
		STRFREE (pnote->to_list);
		pnote->to_list = STRALLOC (to_new + 1);
		pop_call();
		return;
	}

	/*
		Remove note from linked list.
	*/

	UNLINK(pnote, mud->f_note, mud->l_note, next, prev);

	STRFREE (pnote->text);
	STRFREE (pnote->subject);
	STRFREE (pnote->to_list);
	STRFREE (pnote->date);
	STRFREE (pnote->sender);
	FREEMEM (pnote);

	/*
		Rewrite entire list.
	*/

	close_reserve();

	fp = my_fopen (NOTE_FILE, "w",FALSE);

	for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
	{
		fprintf(fp, "Sender  %s~\nDate    %s~\nTime  %d\nTo      %s~\nSubject %s~\nTopic   %d\nText\n%s~\nRoom %u\n\n", pnote->sender, pnote->date, pnote->time, pnote->to_list, pnote->subject, pnote->topic, fixer(pnote->text), pnote->room_vnum);
	}
	my_fclose(fp);

	open_reserve();

	pop_call();
	return;
}

void do_refresh (CHAR_DATA * ch, char *argument)
{
	push_call("do_refresh(%p, %p)",ch,argument);

	if (!is_desc_valid(ch))
	{
		pop_call();
		return;
	}

	if (CH(ch->desc)->pcdata->vt100 == 0)
	{
		pop_call();
		return;
	}

	if (CH(ch->desc)->pcdata->tactical != NULL)
	{
		FREEMEM(CH(ch->desc)->pcdata->tactical);
		CH(ch->desc)->pcdata->tactical = NULL;
	}

	CH(ch->desc)->pcdata->vt100 = 2;

	vt100on(ch);

	pop_call();
	return;
}

bool new_notes (CHAR_DATA *ch)
{
	NOTE_DATA *pnote;
	int oldTopic;

	push_call("new_notes(%p)",ch);

	if (ch->level < 5)
	{
		pop_call();
		return FALSE;
	}

	oldTopic = ch->pcdata->note_topic;

	for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
	{
		ch->pcdata->note_topic = pnote->topic;

		if (is_note_to(ch, pnote) && pnote->time >= ch->pcdata->last_time)
		{
			if (pnote->room_vnum == 0 || pnote->room_vnum == ch->in_room->vnum)
			{
				ch->pcdata->note_topic = oldTopic;
				pop_call();
				return TRUE;
			}
		}
	}
	ch->pcdata->note_topic = oldTopic;
	pop_call();
	return FALSE;
}


void do_note (CHAR_DATA * ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	PLAYER_GAME *fch;
	NOTE_DATA *pnote;
	int vnum, anum, cnt, room_vnum;
	bool room_board;

	push_call("do_note(%p,%p)",ch,argument);

	if (IS_NPC (ch))
	{
		pop_call();
		return;
	}

	if (!ch->desc)
	{
		bug ("do_note: no descriptor", 0);
		pop_call();
		return;
	}

	if (ch->level < 5 && ch->pcdata->reincarnation == 0)
	{
		send_to_char ("You must be at least level 5 to use the Note Boards.\n\r", ch);
		pop_call();
		return;
	}

	switch (ch->pcdata->editmode)
	{
		default:
			break;
		case MODE_RESTRICTED:
			send_to_char ("You cannot use this command from while editing something else.\n\r", ch);
			pop_call();
			return;
			break;

		case MODE_WRITING_NOTE:
			STRFREE(ch->pcdata->pnote->text);
			ch->pcdata->pnote->text = copy_buffer (ch);
			stop_editing (ch);
			pop_call();
			return;
			break;
	}

	argument = one_argument (argument, arg);
	smash_tilde (argument);

	if (ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NOTE_BOARD) && ch->in_room->vnum > 0)
	{
		room_board = TRUE;
		room_vnum  = ch->in_room->vnum;
	}
	else
	{
		room_board = FALSE;
		room_vnum  = 0;
	}

	*buf = '\0';

	if (!strcasecmp(arg, "list"))
	{
		if (room_vnum)
		{
			strcpy (buf, "{078}Note Board:\n\r");
		}
		else
		{
			sprintf(buf, "{078}TOPIC: %s\n\r", topic_table[ch->pcdata->note_topic].name);
		}

		if (is_number (argument))
		{
			int curtime;

			send_to_char_color(buf, ch);

			curtime = mud->current_time - (24 * 60 * 60 * atol (argument));
			vnum = 0;

			for (buf[0] = '\0', pnote = mud->f_note ; pnote ; pnote = pnote->next)
			{
				if (is_note_to(ch, pnote) && pnote->time >= curtime && pnote->room_vnum == room_vnum)
				{
					if (room_vnum || pnote->time <= ch->pcdata->topic_stamp[pnote->topic])
					{
						cat_sprintf(buf, "{078}[%3u]{038} %s to %s:{078} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
					else
					{
						cat_sprintf(buf, "{178}[%3u]{128}*{138}%s to %s:{178} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
				}
				vnum++;
			}
			cat_sprintf(buf, "{078}The {128}*{078} denotes an unread note.\n\r");
			send_to_char_color(buf, ch );
		}
		else if (!strcasecmp (argument, "new"))
		{
			int oldTopic	= ch->pcdata->note_topic;

			vnum = 0;

			for (buf[0] = '\0', pnote = mud->f_note ; pnote ; pnote = pnote->next)
			{
				ch->pcdata->note_topic = pnote->topic;

				if (is_note_to(ch, pnote) && pnote->time >= ch->pcdata->last_time && pnote->room_vnum == room_vnum)
				{
					if (pnote->time <= ch->pcdata->topic_stamp[pnote->topic])
					{
						cat_sprintf(buf, "{078}[%3u]{038} %s to %s:{078} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
					else
					{
						cat_sprintf(buf, "{178}[%3u]{128}*{138}%s to %s:{178} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
				}
				vnum++;
			}
			if (buf[0] == '\0')
			{
				send_to_char ("There have been no new notes since you last logged out.\n\r", ch);
			}
			else
			{
				cat_sprintf(buf, "{078}The {128}*{078} denotes an unread note.\n\r");
				send_to_char_color(buf, ch);
			}
			ch->pcdata->note_topic = oldTopic;
		}
		else if (!strcasecmp (argument, "unread"))
		{
			int unreadNum[MAX_TOPIC]	= {0};
			int oldTopic			= ch->pcdata->note_topic;

			buf[0] = '\0';

			for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
			{
				ch->pcdata->note_topic = pnote->topic;

				if (is_note_to(ch, pnote) && pnote->room_vnum == room_vnum && pnote->time > ch->pcdata->topic_stamp[pnote->topic])
				{
					unreadNum[pnote->topic]++;
				}
			}

			for (cnt = 0 ; cnt < MAX_TOPIC - 1 ; cnt++)
			{
				cat_sprintf(buf, "{038}%2d  {078}%-22s {138}%d\n\r", cnt, topic_table[cnt].name, unreadNum[cnt]);
			}
			send_to_char_color(buf, ch);

			ch->pcdata->note_topic = oldTopic;
		}
		else if (argument[0] != '\0')
		{
			send_to_char_color (buf, ch);
			vnum = 0;

			for (buf[0] = '\0', pnote = mud->f_note ; pnote ; pnote = pnote->next)
			{
				if (is_note_to(ch, pnote) && pnote->room_vnum == room_vnum && ((!str_infix(argument, pnote->sender)) || (!str_infix(argument, pnote->subject)) || (!str_infix(argument, pnote->to_list))))
				{
					if (room_vnum || pnote->time <= ch->pcdata->topic_stamp[pnote->topic])
					{
						cat_sprintf(buf, "{078}[%3u]{038} %s to %s:{078} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
					else
					{
						cat_sprintf (buf, "{178}[%3u]{128}*{138}%s to %s:{178} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
				}
				vnum++;
			}
			cat_sprintf(buf, "{078}The {128}*{078} denotes an unread note.\n\r");
			send_to_char_color (buf, ch );
		}
		else
		{
			send_to_char_color (buf, ch);
			vnum = 0;

			for (buf[0] = '\0', pnote = mud->f_note ; pnote ; pnote = pnote->next)
			{
				if (is_note_to(ch, pnote) && pnote->room_vnum == room_vnum)
				{
					if (room_vnum || pnote->time <= ch->pcdata->topic_stamp[pnote->topic])
					{
						cat_sprintf(buf, "{078}[%3u]{038} %s to %s:{078} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
					else
					{
						cat_sprintf(buf, "{178}[%3u]{128}*{138}%s to %s:{178} %s\n\r", vnum, pnote->sender, pnote->to_list, pnote->subject);
					}
				}
				vnum++;
			}
			cat_sprintf(buf, "{078}The {128}*{078} denotes an unread note.\n\r");
			send_to_char_color (buf, ch );
		}
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "next"))
	{
		vnum = 0;

		for (pnote = mud->f_note ; pnote ; pnote = pnote->next, vnum++)
		{
			if (pnote->time <= ch->pcdata->last_note)
			{
				continue;
			}
			if (is_note_to(ch, pnote) && pnote->room_vnum == room_vnum)
			{
				sprintf(buf, "{178}[%3u]{138} %s:{178} %s\n\r%s\t\tTopic: %s\n\rTo: %s{078}\n\r", vnum, pnote->sender, pnote->subject, pnote->date, room_vnum ? "Private Note Board" : topic_table[pnote->topic].name, pnote->to_list);

				ch_printf_color(ch, "%s%s", buf, ansi_justify(pnote->text, 80));

				if (ch->pcdata->topic_stamp[pnote->topic] < pnote->time)
				{
					ch->pcdata->topic_stamp[ch->pcdata->note_topic] = pnote->time;
				}
				ch->pcdata->last_note = pnote->time;
				pop_call();
				return;
			}
		}
		send_to_char ("End of notes.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "unread") || arg[0] == '\0')
	{
		vnum = 0;

		if (room_vnum)
		{
			ch->pcdata->note_topic = 6;
		}

		for (pnote = mud->f_note ; pnote ; pnote = pnote->next, vnum++)
		{
			if (pnote->time <= ch->pcdata->topic_stamp[ch->pcdata->note_topic])
			{
				continue;
			}

			if (pnote->topic != ch->pcdata->note_topic)
			{
				continue;
			}

			if (is_note_to(ch, pnote) && pnote->room_vnum == room_vnum)
			{
				sprintf(buf, "{178}[%3u]{138} %s:{178} %s\n\r%s\t\tTopic: %s\n\rTo: %s{078}\n\r", vnum, pnote->sender, pnote->subject, pnote->date, room_vnum ? "Private Note Board" : topic_table[pnote->topic].name, pnote->to_list);

				ch_printf_color(ch, "%s%s", buf, ansi_justify(pnote->text, 80));

				if (ch->pcdata->topic_stamp[pnote->topic] < pnote->time)
				{
					ch->pcdata->topic_stamp[ch->pcdata->note_topic] = pnote->time;
				}
				ch->pcdata->last_note = pnote->time;
				pop_call();
				return;
			}
		}
		if (!room_vnum && ch->pcdata->note_topic < 5)
		{
			ch->pcdata->note_topic++;
			ch_printf(ch, "You've read all the notes in this topic. Proceeding to %s.\n\r", topic_table[ch->pcdata->note_topic].name);
			do_note(ch, "unread");
		}
		else
		{
			send_to_char ("End of notes.\n\r", ch);
			ch->pcdata->note_topic = 0;
		}
		pop_call();
		return;
	}

	if (!strcasecmp(arg, "catchup"))
	{
		for (vnum = 0 ; vnum < MAX_TOPIC ; vnum++)
		{
			ch->pcdata->topic_stamp[vnum] = mud->current_time;
		}
		ch->pcdata->last_note = mud->current_time;

		ch_printf(ch, "All notes have been marked as read.\n\r");

		pop_call();
		return;
	}

	if (!strcasecmp(arg, "read") || is_number(arg))
	{
		int old_topic = ch->pcdata->note_topic;

		if (is_number(arg))
		{
			anum = atol(arg);
		}
		else
		{
			if (is_number(argument))
			{
				anum = atol(argument);
			}
			else
			{
				send_to_char("Read what note number?\n\r", ch );
				pop_call();
				return;
			}
		}

		vnum = 0;

		for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
		{
			ch->pcdata->note_topic = pnote->topic;

			if (is_note_to(ch, pnote) && vnum == anum && pnote->room_vnum == room_vnum)
			{
				sprintf(buf, "{178}[%3u]{138} %s:{178} %s\n\r%s\t\tTopic: %s\n\rTo: %s{078}\n\r", vnum, pnote->sender, pnote->subject, pnote->date, room_vnum ? "Private Note Board" : topic_table[pnote->topic].name, pnote->to_list);

				ch_printf_color(ch, "%s%s", buf, ansi_justify(pnote->text, 80));

				ch->pcdata->last_note = pnote->time;
				if (ch->pcdata->topic_stamp[pnote->topic] < pnote->time)
				{
					ch->pcdata->topic_stamp[pnote->topic] = pnote->time;
				}
				ch->pcdata->note_topic = old_topic;
				pop_call();
				return;
			}
			vnum++;
		}
		ch->pcdata->note_topic = old_topic;

		send_to_char ("No such note.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "subject"))
	{
		note_attach (ch);
		STRFREE(ch->pcdata->pnote->subject);
		ch->pcdata->pnote->subject = STRALLOC (argument);
		send_to_char ("Ok.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "to"))
	{
		note_attach (ch);
		STRFREE (ch->pcdata->pnote->to_list);
		ch->pcdata->pnote->to_list = STRALLOC (argument);
		send_to_char ("Ok.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "write"))
	{
		note_attach (ch);
		ch->pcdata->editmode = MODE_WRITING_NOTE;
		ch->pcdata->tempmode = MODE_NONE;
		start_editing (ch, ch->pcdata->pnote->text);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "reply"))
	{
		NOTE_DATA *oldNote;

		if (!is_number(argument))
		{
			send_to_char ("Use: note reply <note number>.\n\r", ch);
			pop_call();
			return;
		}
		vnum = atol (argument);
		anum = 0;
		for (oldNote = mud->f_note ; oldNote ; oldNote = oldNote->next)
		{
			if (anum++ == vnum)
			{
				break;
			}
		}

		if (oldNote == NULL)
		{
			sprintf (buf, "There is no note #%u.\n\r", vnum);
			send_to_char (buf, ch);
			pop_call();
			return;
		}

		if (oldNote->topic == 0 && oldNote->room_vnum == 0 && !IS_GOD(ch))
		{
			ch_printf(ch, "You are not allowed to reply on the Announcements topic.\n\r");
			pop_call();
			return;
		}

		if (oldNote->room_vnum != room_vnum)
		{
			send_to_char ("There is no such note.\n\r", ch);
			pop_call();
			return;
		}

		note_attach (ch);
		STRFREE (ch->pcdata->pnote->to_list);
		STRFREE (ch->pcdata->pnote->subject);

		if (is_name ("all", oldNote->to_list) || is_name (oldNote->sender, oldNote->to_list))
		{
			strcpy (buf, oldNote->to_list);
		}
		else
		{
			sprintf (buf, "%s %s", oldNote->sender, oldNote->to_list);
		}
		ch->pcdata->pnote->to_list = STRALLOC (buf);
		ch->pcdata->pnote->topic   = oldNote->topic;

		if (oldNote->subject[0] == 'R' && oldNote->subject[1] == 'e' && oldNote->subject[2] == ':' && oldNote->subject[3] == ' ')
		{
			ch->pcdata->pnote->subject = STRALLOC (oldNote->subject);
		}
		else
		{
			sprintf (buf, "Re: %s", oldNote->subject);
			ch->pcdata->pnote->subject = STRALLOC (buf);
		}

		sprintf (buf, "%s: %s\n\rTo: %s\n\r", ch->pcdata->pnote->sender, ch->pcdata->pnote->subject, ch->pcdata->pnote->to_list);
		send_to_char (buf, ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "clear"))
	{
		if (ch->pcdata->pnote != NULL)
		{
			STRFREE (ch->pcdata->pnote->text);
			STRFREE (ch->pcdata->pnote->subject);
			STRFREE (ch->pcdata->pnote->to_list);
			STRFREE (ch->pcdata->pnote->date);
			STRFREE (ch->pcdata->pnote->sender);
			FREEMEM (ch->pcdata->pnote);
			ch->pcdata->pnote = NULL;
		}
		else
		{
			send_to_char("There was nothing to clear.\n\r",ch);
		}
		send_to_char ("Ok.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "show"))
	{
		if (ch->pcdata->pnote == NULL)
		{
			send_to_char ("You have no note in progress.\n\r", ch);
			pop_call();
			return;
		}
		ch_printf_color(ch, "{138}%s:{178} %s\n\rTo: %s\n\r", ch->pcdata->pnote->sender, ch->pcdata->pnote->subject, ch->pcdata->pnote->to_list);
		ch_printf_color(ch, "{078}%s", ansi_justify(ch->pcdata->pnote->text, 80));
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "post"))
	{
		FILE *fp;
		bool save_all = FALSE, can_announce = FALSE;
		NOTE_DATA *pnote_next;

		if (!IS_GOD(ch))
		{
			can_announce = FALSE;
		}
		else
		{
			can_announce = TRUE;
		}

		if (ch->pcdata->pnote == NULL)
		{
			send_to_char ("You have no note in progress.\n\r", ch);
			pop_call();
			return;
		}

		if (!strcasecmp(ch->pcdata->pnote->to_list, ""))
		{
			send_to_char ("This note is addressed to no one!\n\r", ch);
			pop_call();
			return;
		}

		if (!strcasecmp (ch->pcdata->pnote->subject, ""))
		{
			send_to_char ("This note has no subject.\n\r", ch);
			pop_call();
			return;
		}

		if (!strcasecmp (ch->pcdata->pnote->text, ""))
		{
			send_to_char ("There is nothing written on this note.\n\r", ch);
			pop_call();
			return;
		}

		if (ch->pcdata->pnote->topic == -1)
		{
			if (room_vnum)
			{
				ch->pcdata->pnote->topic = MAX_TOPIC -1;
			}
			else if (is_number(argument))
			{
				if (atol(argument) >= 0 && atol(argument) < MAX_TOPIC - 1)
				{
					ch->pcdata->pnote->topic = atol(argument);
				}
			}
		}

		if (ch->pcdata->pnote->topic == 0 && can_announce == FALSE)
		{
			send_to_char("You are not allowed to post on the Announcements topic.\n\r", ch);
			pop_call();
			return;
		}

		if (ch->pcdata->pnote->topic == -1)
		{
			ch_printf(ch, "You must specify a valid topic number for this note to be posted under.\n\rAvailable topics are:\n\r\n\r");

			for (cnt = !can_announce ; cnt < MAX_TOPIC - 1 ; cnt++)
			{
				if (ch->level >= topic_table[cnt].min_level)
				{
					ch_printf_color(ch, "{038}%2d  {078}%s\n\r", cnt, topic_table[cnt].name);
				}
			}
			pop_call();
			return;
		}

		ch->pcdata->pnote->date = STRALLOC(get_time_string(mud->current_time));

		if (IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_MUTED))
		{
			ch->pcdata->pnote->time = 0;
		}
		else
		{
			ch->pcdata->pnote->time = mud->current_time;
		}
		if (IS_SET(ch->in_room->room_flags, ROOM_NOTE_BOARD))
		{
			ch->pcdata->pnote->room_vnum = ch->in_room->vnum;
		}
		else
		{
			ch->pcdata->pnote->room_vnum = 0;
		}
		LINK(ch->pcdata->pnote, mud->f_note, mud->l_note, next, prev);

		for (pnote = mud->f_note ; pnote->next ; pnote = pnote_next)
		{
			pnote_next = pnote->next;

			if (pnote->time <= mud->current_time - 60 * 60 * 24 * 7 * 4)
			{
				UNLINK(pnote, mud->f_note, mud->l_note, next, prev);

				STRFREE (pnote->text);
				STRFREE (pnote->subject);
				STRFREE (pnote->to_list);
				STRFREE (pnote->date);
				STRFREE (pnote->sender);
				FREEMEM (pnote);
				save_all = TRUE;
			}
		}
		ch->pcdata->pnote = NULL;

		/*  Added for recieving mail while on the game.  Chaos 12/15/93  */

		if (!room_board)
		{
			for (fch = mud->f_player ; fch ; fch = fch->next)
			{
				if (is_name(fch->ch->name, pnote->to_list)
				|| (is_name("immortal", pnote->to_list) && IS_IMMORTAL(fch->ch)))
				{
					send_to_char_color("{088}{506}You just received a note.{088}\n\r", fch->ch);
				}
			}
		}

		close_reserve();

		if (!save_all)
		{
			fp = my_fopen(NOTE_FILE, "a", FALSE);
			fprintf (fp, "Sender  %s~\nDate    %s~\nTime   %d\nTo      %s~\nSubject %s~\nTopic   %d\nText\n%s~\nRoom %u\n\n", pnote->sender, pnote->date, pnote->time, pnote->to_list, pnote->subject, pnote->topic, fixer(pnote->text), pnote->room_vnum);
			my_fclose(fp);
		}
		else
		{
			fp = my_fopen (NOTE_FILE, "w", FALSE);

			for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
			{
				fprintf (fp, "Sender  %s~\nDate    %s~\nTime   %d\nTo      %s~\nSubject %s~\nTopic   %d\nText\n%s~\nRoom %u\n\n", pnote->sender, pnote->date, pnote->time, pnote->to_list, pnote->subject, pnote->topic, fixer(pnote->text), pnote->room_vnum);
			}
			my_fclose(fp);
		}

		open_reserve();

		if (!room_board)
		{
			send_to_char ("Ok.\n\r", ch);
		}
		else
		{
			send_to_char ("You post your note onto the noteboard.\n\r", ch);
		}
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "remove"))
	{
		if (!is_number (argument))
		{
			send_to_char ("Note remove which number?\n\r", ch);
			pop_call();
			return;
		}
		anum = atol (argument);
		vnum = 0;
		for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
		{
			if (is_note_to(ch, pnote) && vnum == anum)
			{
				note_remove (ch, pnote, FALSE);
				send_to_char ("Ok.\n\r", ch);
				pop_call();
				return;
			}
			vnum++;
		}
		send_to_char ("No such note.\n\r", ch);
		pop_call();
		return;
	}

	if (is_name(arg, "delete"))
	{
		if (!is_number(argument))
		{
			send_to_char ("Note delete which number?\n\r", ch);
			pop_call();
			return;
		}

		anum = atol (argument);
		vnum = 0;
		for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
		{
			if (vnum == anum)
			{
				if (IS_IMMORTAL(ch) || !strcmp(ch->name, pnote->sender))
				{
					note_remove(ch, pnote, TRUE);
					ch_printf(ch, "Note %d has been deleted.\n\r", anum);
				}
				else
				{
					ch_printf(ch, "You can only delete your own notes.\n\r");
				}
				pop_call();
				return;
			}
			vnum++;
		}
		send_to_char ("No such note.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "topic"))
	{
		if (room_board)
		{
			send_to_char ("You are in a room containing a note board.\n\r", ch);
			pop_call();
			return;
		}
		if (is_number (argument))
		{
			bool num = atol(argument);

			if (num >= MAX_TOPIC - 1)
			{
				send_to_char("There is no such topic.\n\r", ch);
				pop_call();
				return;
			}
			ch->pcdata->note_topic = num;
			ch_printf( ch, "Note topic changed to: %s\n\r", topic_table[ch->pcdata->note_topic].name);
			pop_call();
			return;
		}
		else
		{
			int unreadNum[MAX_TOPIC]	= {0};
			int oldTopic			= ch->pcdata->note_topic;

			for (pnote = mud->f_note ; pnote ; pnote = pnote->next)
			{
				ch->pcdata->note_topic = pnote->topic;

				if (is_note_to(ch, pnote) && pnote->room_vnum == room_vnum && pnote->time > ch->pcdata->topic_stamp[pnote->topic])
				{
					unreadNum[pnote->topic]++;
				}
			}

			ch->pcdata->note_topic = oldTopic;

			send_to_char_color ("{178}List of note topics:\n\r\n\r", ch);
			send_to_char_color ("{138}Reference Number    {178}Topic Name:                 {128}Unread Notes\n\r", ch);
			for (cnt = 0 ; cnt < MAX_TOPIC - 1 ; cnt++)
			{
				if (ch->level >= topic_table[cnt].min_level)
				{
					ch_printf_color(ch, "{038}%2d                  {078}%-25s  {028}%2d\n\r", cnt, topic_table[cnt].name, unreadNum[cnt]);
				}
			}
		}
		pop_call();
		return;
	}
	send_to_char ("Huh?  Type 'help note' for usage.\n\r", ch);
	pop_call();
	return;
}

void do_immtalk (CHAR_DATA * ch, char *argument)
{
	PLAYER_GAME *fpl;
	char buf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_immtalk(%p,%p)",ch,argument);

	if (argument[0] == '\0')
	{
		send_to_char ("Immtalk what?\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	sprintf(buf, "You immtalk '%s'", text2);
	ch_printf_color(ch, "{128}%s\n\r", ansi_justify(buf, get_page_width(ch)));

	sprintf(buf, "%s immtalks '%s'", ch->name, text2);

	for (fpl = mud->f_player ; fpl ; fpl = fpl->next)
	{
		if (ch == fpl->ch || fpl->ch->level < LEVEL_HERO)
		{
			continue;
		}
		ch_printf_color(fpl->ch, "{128}%s\n\r", ansi_justify(buf, get_page_width(fpl->ch)));
	}
	pop_call();
	return;
}

void do_plan (CHAR_DATA * ch, char *argument)
{
	PLAYER_GAME *fpl;
	char buf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_plan(%p,%p)",ch,argument);

	if (which_god(ch) == GOD_NEUTRAL)
	{
		send_to_char ("You are not a follower of a god.\n\r", ch);
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Plan what?\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && !IS_SET(ch->pcdata->channel, CHANNEL_PLAN))
	{
		send_to_char ("You decided not to plan.\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	sprintf(buf, "You plan '%s'", text2);
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_PLAN, VT102_BOLD), justify(buf, get_page_width(ch)));

	sprintf(buf, "%s plans '%s'", ch->name, text2);

	for (fpl = mud->f_player ; fpl ; fpl = fpl->next)
	{
		if (!IS_SET(fpl->ch->pcdata->channel, CHANNEL_PLAN))
		{
			continue;
		}

		if (ch == fpl->ch || which_god(ch) != which_god(fpl->ch))
		{
			continue;
		}
		ch_printf(fpl->ch, "%s%s\n\r", get_color_string(fpl->ch, COLOR_PLAN, VT102_BOLD), justify(buf, get_page_width(fpl->ch)));
	}
	pop_call();
	return;
}

void do_beep (CHAR_DATA * ch, char *argument)
{
	CHAR_DATA *fch;

	push_call("do_beep(%p,%p)",ch,argument);

	if (argument[0] == '\0')	/*  Beeping Room */
	{
		for (fch = ch->in_room->first_person ; fch ; fch = fch->next_in_room)
		{
			if (!IS_NPC(fch) && fch != ch)
			{
				if (blocking(fch, ch))
				{
					continue;
				}
				if (IS_SET(fch->pcdata->vt100_flags, VT100_BEEP))
				{
					ch_printf(fch, "%s beeped.\007\n\r", get_name(ch));
				}
				else
				{
					ch_printf(fch, "%s beeped.\n\r", get_name(ch));
				}
			}
		}
		send_to_char ("You beep the room.\n\r", ch);
		pop_call();
		return;
	}

	if ((fch = get_player_world(ch, argument)) == NULL)
	{
		send_to_char ("They aren't here.\n\r", ch);
		pop_call();
		return;
	}

	if (blocking (fch, ch))
	{
		ch_printf(ch, "%s refuses to hear you.\n\r", capitalize(get_name(fch)));
		pop_call();
		return;
	}
	if (IS_SET(fch->pcdata->vt100_flags, VT100_BEEP))
	{
		ch_printf(fch, "%s beeped.\007\n\r", get_name(ch));
	}
	else
	{
		ch_printf(fch, "%s beeped.\n\r", get_name(ch));
	}
	send_to_char ("You beep.\n\r", ch);
	pop_call();
	return;
}

void do_chat (CHAR_DATA * ch, char *argument)
{
	PLAYER_GAME *fpl;
	char jbuf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_chat(%p,%p)",ch,argument);

	if (argument[0] == '\0')
	{
		send_to_char ("Chat what?\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && !IS_SET(ch->pcdata->channel, CHANNEL_CHAT))
	{
		send_to_char ("You decided not to chat.\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}
	sprintf (jbuf, "You chat '%s'", text2);
	ch_printf (ch, "%s%s\n\r", get_color_string(ch, COLOR_CHAT, VT102_BOLD), justify(jbuf, get_page_width(ch)));

	sprintf(jbuf, "%s chats '%s'", capitalize(get_name(ch)), text2);

	for (fpl = mud->f_player ; fpl ; fpl = fpl->next)
	{
		if (ch == fpl->ch || !IS_SET(fpl->ch->pcdata->channel, CHANNEL_CHAT) || blocking(fpl->ch, ch))
		{
			continue;
		}
		ch_printf(fpl->ch, "%s%s\n\r", get_color_string(fpl->ch, COLOR_CHAT, VT102_BOLD), justify(jbuf, get_page_width(fpl->ch)));
	}
	pop_call();
	return;
}

void do_fos (CHAR_DATA * ch, char *argument)
{
	PLAYER_GAME *fpl;
	char jbuf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_fos(%p,%p)",ch,argument);

	if (!IS_NPC(ch) && !IS_SET(ch->pcdata->channel, CHANNEL_FOS))
	{
		send_to_char("You decided not to fos.\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Fos what?\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	sprintf (jbuf, "You babble '%s'", text2);
	ch_printf (ch, "%s%s\n\r", get_color_string(ch, COLOR_PLAN, VT102_BOLD), justify(jbuf, get_page_width(ch)));

	sprintf(jbuf, "%s babbles '%s'", capitalize(get_name(ch)), text2);

	for (fpl = mud->f_player ; fpl ; fpl = fpl->next)
	{
		if (ch == fpl->ch || !IS_SET(fpl->ch->pcdata->channel, CHANNEL_FOS) || blocking(fpl->ch, ch))
		{
			continue;
		}
		ch_printf(fpl->ch, "%s%s\n\r", get_color_string(fpl->ch, COLOR_PLAN, VT102_BOLD), justify(jbuf, get_page_width(fpl->ch)));
	}
	pop_call();
	return;
}

void do_battle (const char *fmt, ...)
{
	PLAYER_GAME *fpl;
	char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];
	va_list args;

	push_call("do_battle(%p)",fmt);

	va_start(args, fmt);
	vsprintf(buf1, fmt, args);
	va_end(args);

	for (fpl = mud->f_player ; fpl ; fpl = fpl->next)
	{
		if (IS_SET(fpl->ch->pcdata->channel, CHANNEL_BATTLE))
		{
			if (IS_GOD(fpl->ch))
			{
				snprintf(buf2, 20, "%s", get_time_string(mud->current_time));
				sprintf(buf3, "(%s) %s", &buf2[11], buf1);

				ch_printf(fpl->ch, "%s%s\n\r", get_color_string(fpl->ch, COLOR_YOU_ARE_HIT, VT102_BOLD), justify(buf3, get_page_width(fpl->ch)));
			}
			else
			{
				ch_printf(fpl->ch, "%s%s\n\r", get_color_string(fpl->ch, COLOR_YOU_ARE_HIT, VT102_BOLD), justify(buf1, get_page_width(fpl->ch)));
			}
		}
	}
	pop_call();
	return;
}

void do_channel_talk (CHAR_DATA * ch, char *argument)
{
	PLAYER_GAME *fpl;
	char jbuf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_channel_talk(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Talk what?\n\r", ch);
		pop_call();
		return;
	}

	if (ch->pcdata->clan == NULL)
	{
		send_to_char ("You are not a member of a clan.\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	sprintf (jbuf, "You talk '%s'", text2);
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_TALK, VT102_BOLD), justify(jbuf, get_page_width(ch)));

	sprintf(jbuf, "%s talks '%s'", get_name (ch), text2);

	for (fpl = mud->f_player ; fpl ; fpl = fpl->next)
	{
		if (ch == fpl->ch || ch->pcdata->clan != fpl->ch->pcdata->clan)
		{
			continue;
		}
		ch_printf(fpl->ch, "%s%s\n\r", get_color_string(fpl->ch, COLOR_TALK, VT102_BOLD), justify(jbuf, get_page_width(fpl->ch)));
	}
	pop_call();
	return;
}

void do_shout (CHAR_DATA * ch, char *argument)
{
	DESCRIPTOR_DATA *d;

	char buf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_shout(%p,%p)",ch,argument);

	if (ch->in_room->area->nplayer == 0)
	{
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Shout what?\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot shout.\n\r", ch);
		pop_call();
		return;
	}

	if (IS_NPC(ch) && !IS_SET(ch->act, ACT_SMART))
	{
		send_to_char ("You're too dumb to talk.\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	sprintf(buf, "You shout '%s'", text2);
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_SPEACH, VT102_BOLD), justify(buf, get_page_width(ch)));

	sprintf(buf, "%s shouts '%s'", capitalize(get_name(ch)), text2);

	for (d = mud->f_desc ; d ; d = d->next)
	{
		if (!d->character || d->connected < 0)
		{
			continue;
		}
		if (ch == d->character || d->character->in_room->area != ch->in_room->area || d->character->position <= POS_SLEEPING)
		{
			continue;
		}
		if (blocking(d->character, ch))
		{
			continue;
		}
		if (!can_understand(d->character, ch, FALSE))
		{
			continue;
		}
		ch_printf(d->character, "%s%s\n\r", get_color_string(d->character, COLOR_SPEACH, VT102_BOLD), justify(buf, get_page_width(d->character)));
	}
	wait_state(ch, PULSE_VIOLENCE);

	pop_call();
	return;
}

void do_buffer (CHAR_DATA * ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	int page, pt, cnt, ct, pages;
	char buf[MAX_STRING_LENGTH], buf2[MAX_INPUT_LENGTH];
	bool done;

	push_call("do_buffer(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	d = ch->desc;

	if (d->original != NULL || d->character != ch)
	{
		pop_call();
		return;
	}

	if (argument[0] != '\0')
	{
		pages = UMAX(0, atol(argument));
	}
	else
	{
		pages = 1;
	}

	done = FALSE;
	page = get_pager_breakpt(ch) -1;

	ct = 0 - page;

	if (pages > 99)
	{
		pages = 99;
	}

	for (pt = ch->pcdata->scroll_end ; !done ; pt--)
	{
		if (ch->pcdata->scroll_buf[pt] == '\n')
		{
			ct++;
		}
		if (ct > page * pages)
		{
			done = TRUE;
		}
		if ((ch->pcdata->scroll_start == 0 && pt == 0) || pt == ch->pcdata->scroll_end + 1 || (pt == 0 && ch->pcdata->scroll_end == MAX_BUFFER_LENGTH - 1))
		{
			send_to_char ("That is too far back.\n\r", ch);
			pop_call();
			return;
		}
		if (pt == 0)
		{
			pt = MAX_BUFFER_LENGTH;
		}
	}
	pt++;
	if (pt >= MAX_BUFFER_LENGTH)
	{
		pt = 0;
	}
	while (ch->pcdata->scroll_buf[pt] == '\n' || ch->pcdata->scroll_buf[pt] == '\r')
	{
		pt++;
		if (pt >= MAX_BUFFER_LENGTH)
		{
			pt = 0;
		}
	}

	sprintf(buf, "%s******************************** Pages back: %2d ********************************\n\r", get_color_string(ch, COLOR_PROMPT, VT102_BOLD), pages);
	strcat(buf, get_color_string(ch, COLOR_TEXT, VT102_DIM));

	done = FALSE;
	cnt  = strlen(buf);

	for (ct = 0 ; !done ; cnt++)
	{
		if (cnt >= MAX_STRING_LENGTH-300)
		{
			done = TRUE;
		}
		buf[cnt] = ch->pcdata->scroll_buf[pt];

		if (++pt >= MAX_BUFFER_LENGTH)
		{
			pt = 0;
		}

		if (buf[cnt] == '\n')
		{
			ct++;
		}
		if (ct >= page && buf[cnt] != '\n' && buf[cnt] != '\r')
		{
			done = TRUE;
		}
	}

	buf[cnt - 1] = '\0';

	sprintf(buf2, "%s******************************** Pages back: %2d ********************************\n\r", get_color_string(ch, COLOR_PROMPT, VT102_BOLD), pages);
	strcat(buf, buf2);

	SET_BIT(ch->pcdata->interp, INTERP_SCROLL);
	send_to_char(buf, ch);
	REMOVE_BIT(ch->pcdata->interp, INTERP_SCROLL);

	pop_call();
	return;
}

/*
	1) Find a block of text, this will be done by looking for \r+color or \n
	2) Search the block of text for the search key.
	3) Store the block of text
	4) Return at end of search or 10 blocks of text are found
	5) Buffer is searched downwards from scroll_end till scroll_end + 1
	   unless scroll_start == 0 (means buffer hasn't been filled yet)
	-) Rewritten by Scandum 24-05-2002
*/

void do_grep (CHAR_DATA * ch, char *argument)
{
	int pt, ct, st, cnt, fnd, fnd_max, lend, lstart;
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_INPUT_LENGTH];
	char buf3[MAX_INPUT_LENGTH];
	char lines[20][MAX_INPUT_LENGTH];

	push_call("do_grep(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Grep what?\n\r", ch);
		pop_call();
		return;
	}

	str_cpy_max(buf2, argument, 40);

	fnd_max = URANGE(10, get_pager_breakpt(ch) / 2, 20);

	for (fnd = 0, lend = pt = ch->pcdata->scroll_end ; pt > -2 ; pt--)
	{
		if (ch->pcdata->scroll_start == 0 && pt == -1)
		{
			break;
		}
		if (pt == ch->pcdata->scroll_end + 1)
		{
			break;
		}
		if (pt == -1)
		{
			pt = MAX_BUFFER_LENGTH-1;
		}

		if (ch->pcdata->ansi == FALSE && ch->pcdata->vt100 == 0)
		{
			if (ch->pcdata->scroll_buf[pt] == '\r')
			{
				lstart = pt+1;
			}
			else
			{
				continue;
			}
		}
		else
		{
			if (ch->pcdata->scroll_buf[pt] == '\033'
			&& (pt == 0 || ch->pcdata->scroll_buf[pt-1] == '\r'))
			{
				lstart = pt;
			}
			else
			{
				continue;
			}
		}
		for (ct = lstart ; ct < lend ; ct++)
		{
			if (ct == MAX_BUFFER_LENGTH)
			{
				ct = 0;
			}
			for (cnt = 0, st = ct ; buf2[cnt] != '\0' ; cnt++, st++)
			{
				if (st == MAX_BUFFER_LENGTH)
				{
					st = 0;
				}
				if (buf2[cnt] != ch->pcdata->scroll_buf[st])
				{
					break;
				}
			}
			if (buf2[cnt] == '\0')
			{
				for (cnt = lstart ; cnt < lend ; cnt++)
				{
					if (cnt == MAX_BUFFER_LENGTH)
					{
						cnt = 0;
					}
					lines[fnd][cnt-lstart] = ch->pcdata->scroll_buf[cnt];
				}
				lines[fnd][cnt - lstart] = '\0';

				if (++fnd >= fnd_max)
				{
					pt = -1;
				}
				lend = 0;
			}
		}
		lend = lstart;
	}

	sprintf (buf3, " Search for the word: %s ", buf2);

	if (strlen(buf3) % 2 == 1)
	{
		strcat(buf3, " ");
	}

	strcpy(buf, get_color_string(ch, COLOR_PROMPT, VT102_BOLD));

	for (cnt = 0 ; cnt < 40 - strlen(buf3) / 2 ; cnt++)
	{
		strcat (buf, "*");
	}
	strcat (buf, buf3);
	for (cnt = 0 ; cnt < 40 - strlen(buf3) / 2 ; cnt++)
	{
		strcat (buf, "*");
	}
	strcat(buf, get_color_string(ch, COLOR_TEXT, VT102_DIM));
	strcat(buf, "\n\r");

	if (fnd == 0)
	{
		strcat(buf, ansi_translate_text(ch, "{128}Search string not found.\n\r"));
	}
	else
	{
		for (cnt = fnd-1 ; cnt >= 0 ; cnt--)
		{
			strcat(buf, lines[cnt]);
		}
	}

	strcat(buf, get_color_string(ch, COLOR_PROMPT, VT102_BOLD));
	strcat(buf, "********************************************************************************\n\r");
	SET_BIT(ch->pcdata->interp, INTERP_SCROLL);
	send_to_char (buf, ch);
	REMOVE_BIT(ch->pcdata->interp, INTERP_SCROLL);
	pop_call();
	return;
}

void do_say (CHAR_DATA * ch, char *argument)
{
	CHAR_DATA *fch;
	char buf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_say(%p,%p)",ch,argument);

	if (argument[0] == '\0')
	{
		send_to_char ("Say what?\n\r", ch);
		pop_call();
		return;
	}

	if (IS_NPC(ch) && !IS_SET(ch->act, ACT_SMART))
	{
		if (MP_VALID_MOB(ch))
		{
			log_build_printf(ch->pIndexData->vnum, "mob is trying to talk, but doesn't have ACT_SMART set");
		}
		else
		{
			send_to_char ("You're too dumb to talk.\n\r", ch);
		}
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	if (!IS_NPC(ch) && strstr(text2, "!"))
	{
		sprintf(buf, "You exclaim '%s'", text2);
	}
	else if (!IS_NPC(ch) && strstr(text2, "?"))
	{
		sprintf(buf, "You ask '%s'", text2);
	}
	else
	{
		sprintf(buf, "You say '%s'", text2);
	}

	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_SPEACH, VT102_DIM), justify(buf, get_page_width(ch)));

	for (fch = ch->in_room->first_person ; fch ; fch = fch->next_in_room)
	{
		if (ch == fch || fch->position <= POS_SLEEPING)
		{
			continue;
		}

		if (blocking (fch, ch))
		{
			continue;
		}

		if (!can_understand(fch, ch, FALSE))
		{
			if (!IS_NPC(fch) && can_see(ch, fch))
			{
				can_understand(fch, ch, TRUE);
			}
			sprintf(buf, "%s says '%s'", capitalize(get_name(ch)), translate(text2));
			ch_printf(fch, "%s%s\n\r",  get_color_string(fch, COLOR_SPEACH, VT102_DIM), justify(buf, get_page_width(fch)));
			continue;
		}
		if (!IS_NPC(ch) && strstr(text2, "!"))
		{
			sprintf(buf, "%s exclaims '%s'", capitalize(get_name(ch)), text2);
		}
		else if (!IS_NPC(ch) && strstr(text2, "?"))
		{
			sprintf(buf, "%s asks '%s'", capitalize(get_name(ch)), text2);
		}
		else
		{
			sprintf(buf, "%s says '%s'", capitalize(get_name(ch)), text2);
		}
		ch_printf(fch, "%s%s\n\r", get_color_string(fch, COLOR_SPEACH, VT102_DIM), justify(buf, get_page_width(fch)));
	}

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	mprog_speech_trigger (argument, ch);
	pop_call();
	return;
}

void do_sing (CHAR_DATA * ch, char *argument)
{
	CHAR_DATA *fch;
	char buf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];

	push_call("do_sing(%p,%p)",ch,argument);

	if (ch == NULL)
	{
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Sing what?\n\r", ch);
		pop_call();
		return;
	}

	if (IS_NPC(ch) && !IS_SET(ch->act, ACT_SMART))
	{
		send_to_char ("You're too dumb to sing.\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot sing.\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	if (strstr(text2, "!"))
	{
		sprintf(buf, "You scream '%s'", text2);
	}
	else
	{
		sprintf(buf, "You sing '%s'", text2);
	}
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_SPEACH, VT102_DIM), justify(buf, get_page_width(ch)));

	for (fch = ch->in_room->first_person ; fch ; fch = fch->next_in_room)
	{
		if (ch == fch || fch->position <= POS_SLEEPING)
		{
			continue;
		}

		if (blocking (fch, ch))
		{
			continue;
		}

		if (!can_understand(fch, ch, FALSE))
		{
			if (!IS_NPC(fch) && can_see(ch, fch))
			{
				can_understand(fch, ch, TRUE);
			}
			sprintf(buf, "%s sings '%s'", capitalize(get_name(ch)), translate(text2));
			ch_printf(fch, "%s%s\n\r",  get_color_string(fch, COLOR_SPEACH, VT102_DIM), justify(buf, get_page_width(fch)));
			continue;
		}
		if (strstr(text2, "!"))
		{
			sprintf(buf, "%s screams '%s'", capitalize(get_name(ch)), text2);
		}
		else
		{
			sprintf(buf, "%s sings '%s'", capitalize(get_name(ch)), text2);
		}
		ch_printf(fch, "%s%s\n\r", get_color_string(fch, COLOR_SPEACH, VT102_DIM), justify(buf, get_page_width(fch)));
	}

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	mprog_speech_trigger (argument, ch);
	pop_call();
	return;
}

void do_dump (CHAR_DATA * ch, char *argument)
{
	push_call("do_dump(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp(argument, "buffer"))
	{
		FILE *dumpFile;
		char dump_dir[200], buf[MAX_BUFFER_LENGTH];

		if (ch->trust < 10)
		{
			ch_printf(ch, "You must be at least level 10 to dump your buffer.\n\r");
			pop_call();
			return;
		}
		ch_printf(ch, "Saving buffer..\n\r");

		if (ch->pcdata->scroll_start == 0)
		{
			strcpy(buf, ch->pcdata->scroll_buf);
		}
		else
		{
			strncpy(&buf[0], &ch->pcdata->scroll_buf[ch->pcdata->scroll_end], MAX_BUFFER_LENGTH - ch->pcdata->scroll_end);
			strncpy(&buf[MAX_BUFFER_LENGTH - ch->pcdata->scroll_end], &ch->pcdata->scroll_buf[0], ch->pcdata->scroll_end);
			buf[MAX_BUFFER_LENGTH-1] = '\0';
		}
		sprintf(dump_dir, "%s/%c/%c/dmp/%s.dmp", PLAYER_DIR, tolower(ch->name[0]), tolower(ch->name[1]), ch->name);

		close_reserve();

		if ((dumpFile = my_fopen(dump_dir, "wt", FALSE)) != NULL)
		{
			fprintf(dumpFile, "%s", fixer(ansi_strip(buf)));

			my_fclose(dumpFile);

			log_printf("%s has dumped their buffer", get_name(ch));

			wait_state(ch, PULSE_VIOLENCE);
		}

		open_reserve();

		pop_call();
		return;
	}

	send_to_char ("Waiting for Dump.  Issue 'stop' to cancel.\n\r", ch);

	SET_BIT(ch->pcdata->interp, INTERP_DUMP);

	pop_call();
	return;
}


void do_tell (CHAR_DATA * ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char jbuf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	push_call("do_tell(%p,%p)",ch,argument);

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("Your message didn't get through.\n\r", ch);
		pop_call();
		return;
	}

	if (IS_NPC(ch) && !IS_SET(ch->act, ACT_SMART))
	{
		if (MP_VALID_MOB(ch))
		{
			log_build_printf(ch->pIndexData->vnum, "mob is trying to talk, but doesn't have ACT_SMART set");
		}
		else
		{
			send_to_char ("You're too dumb to talk.\n\r", ch);
		}
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	argument = one_argument(argument, arg);

	if (arg[0] == '\0' || argument[0] == '\0')
	{
		send_to_char ("Tell whom what?\n\r", ch);
		pop_call();
		return;
	}

	if ((victim = get_player_world(ch, arg)) == NULL)
	{
		send_to_char ("They aren't here.\n\r", ch);
		pop_call();
		return;
	}

	if (victim->desc == NULL)
	{
		ch_printf(ch, "%s is link-dead.\n\r", get_name(victim));
		pop_call();
		return;
	}

	if (blocking(victim, ch) || IS_SET(pvnum_index[victim->pcdata->pvnum]->flags, PVNUM_MUTED))
	{
		ch_printf(ch, "%s refuses to hear you.\n\r", get_name(victim));
		pop_call();
		return;
	}

	if (IS_SET(victim->act, PLR_AFK))
	{
		ch_printf (ch, "%s is afk and may not see your message.\n\r", get_name(victim));
	}

	if (victim->desc && victim->desc->connected == CON_EDITING && get_trust (ch) < LEVEL_IMMORTAL)
	{
		ch_printf(ch, "%s is currently in a writing buffer.  Please try again in a few minutes.\n\r", get_name(victim));
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	if (!can_understand(victim, ch, TRUE))
	{
		sprintf(jbuf, "%s tells you '%s'", capitalize(get_name(ch)), translate(text2));
		ch_printf(victim, "%s%s\n\r", get_color_string(victim, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(victim)));

		pop_call();
		return;
	}
	sprintf(jbuf, "You tell %s '%s'", get_name(victim), text2);
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(ch)));

	sprintf(jbuf, "%s tells you '%s'", capitalize(get_name(ch)), text2);
	ch_printf(victim, "%s%s\n\r", get_color_string(victim, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(victim)));

	if (!IS_NPC(ch) && !IS_NPC(victim))
	{
		victim->pcdata->reply = ch;
	}
	pop_call();
	return;
}

void do_etell (CHAR_DATA * ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char jbuf[MAX_STRING_LENGTH];

	CHAR_DATA *victim;
	int expl;

	push_call("do_etell(%p,%p)",ch,argument);

	expl = 0;

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("Your message didn't get through.\n\r", ch);
		pop_call();
		return;
	}

	if (IS_NPC(ch) && !IS_SET (ch->act, ACT_SMART))
	{
		send_to_char ("You're too dumb to talk.\n\r", ch);
		pop_call();
		return;
	}

	argument = one_argument (argument, arg);

	if (arg[0] == '\0' || argument[0] == '\0')
	{
		send_to_char ("Tell whom what?\n\r", ch);
		pop_call();
		return;
	}

	if ((victim = get_player_world(ch, arg)) == NULL)
	{
		send_to_char ("They aren't here.\n\r", ch);
		pop_call();
		return;
	}

	/*
		poll the block list of the victim, make it cost more exp
	*/

	if (blocking(victim, ch))
	{
		ch_printf(ch, "%s refuses to hear you.\n\r", get_name(victim));
		pop_call();
		return;
	}

	sprintf(jbuf, "You etell %s '%s'", get_name(victim), argument);
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(ch)));

	sprintf(jbuf, "EMERGENCY: %s tells you '%s'", get_name(ch), argument);
	ch_printf(victim, "%s%s\n\r", get_color_string(victim, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(victim)));
	victim->pcdata->reply = ch;

	expl = 10 * (ch->level * ch->level * 4 + 10 * ch->level + 100);
	ch_printf (ch, "You lose %d experience for the emergency.\n\r", expl);
	gain_exp (ch, 0 - expl);

	pop_call();
	return;
}


void do_reply (CHAR_DATA * ch, char *argument)
{
	char jbuf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	push_call("do_reply(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		send_to_char("You cannot reply.\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("Your message didn't get through.\n\r", ch);
		pop_call();
		return;
	}

	if ((victim = ch->pcdata->reply) == NULL || IS_NPC(ch->pcdata->reply))
	{
		send_to_char ("They aren't here.\n\r", ch);
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Reply what?\n\r", ch);
		pop_call();
		return;
	}

	if (IS_SET(victim->act, PLR_AFK))
	{
		ch_printf(ch, "%s is afk and may not see your message.\n\r", get_name (victim));
	}

	if (victim->desc == NULL)
	{
		ch_printf (ch, "%s is link-dead.\n\r", get_name(victim));
		pop_call();
		return;
	}

	if (blocking(victim, ch))
	{
		ch_printf(ch, "%s refuses to hear you.\n\r", capitalize(get_name(victim)));
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	if (!can_understand(victim, ch, TRUE))
	{
		sprintf(jbuf, "%s replies to you '%s'", get_name(ch), translate(text2));
		ch_printf(victim, "%s%s\n\r", get_color_string(victim, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(victim)));
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}

	sprintf(jbuf, "You reply to %s '%s'", get_name(victim), text2);
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(ch)));

	sprintf(jbuf, "%s replies to you '%s'", capitalize(get_name(ch)), text2);
	ch_printf (victim, "%s%s\n\r", get_color_string(victim, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(victim)));

	victim->pcdata->reply = ch;

	pop_call();
	return;
}

void do_emote (CHAR_DATA * ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *fch;

	push_call("do_emote(%p,%p)",ch,argument);

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Emote what?\n\r", ch);
		pop_call();
		return;
	}

	sprintf(buf, "%s %s\n\r", get_name(ch), argument);

	for (fch = ch->in_room->first_person ; fch ; fch = fch->next_in_room)
	{
		if (IS_AWAKE(fch) && !blocking(fch, ch))
		{
			ch_printf(fch, "%s", capitalize(justify(buf, get_page_width(fch))));
		}
	}
	pop_call();
	return;
}

/*
	All the posing stuff.
*/

struct pose_table_type
{
	char *message[2 * MAX_CLASS];
};

/*
	Pose Table, updated by Scandum
*/

const struct pose_table_type pose_table[] =
{
	{
		{
			"You show your bulging muscles.",
			"$n shows $s bulging muscles.",
			"The townfolk cheer as you emerge victorious",
			"The townfolk cheer as $n emerges victorious",
			"You perform a small card trick.",
			"$n performs a small card trick.",
			"The shadows slide apart and you emerge.",
			"The shadows emerge and $n emerges.",
			"An angel consults you.",
			"An angel consults $n.",
			"You sizzle with energy.",
			"$n sizzles with energy.",
			"You feel very holy.",
			"$n looks very holy.",
			"Two little demons appear to polish your boots.",
			"Two little demons appear to polish $n's boots."
		}
	},

	{
		{
			"You crack nuts between your fingers.",
			"$n cracks nuts between $s fingers.",
			"The emperor's thumb goes up for you.",
			"The emperor's thumb goes up for $n.",
			"You wiggle your ears alternately.",
			"$n wiggles $s ears alternately.",
			"You carefully place a stopper on a bottle of lethal poison.",
			"$n looks at you as $e carefully stoppers a dirty looking bottle.",
			"You nonchalantly turn wine into water.",
			"$n nonchalantly turns wine into water.",
			"You turn into a butterfly, then return to your normal shape.",
			"$n turns into a butterfly, then returns to $s normal shape.",
			"Your bones and muscles make cracking noises as you clench your fists.",
			"You hear cracking noises as $n clenches $s fists.",
			"Blood spurts from the ground forming a pentagram around you.",
			"Blood spurts from the ground forming a pentagram around $n."
		}
	},

	{
		{
			"You grizzle your teeth and look mean.",
			"$n grizzles $s teeth and looks mean.",
			"You size up those around you and edge quietly through the room.",
			"Flexing $s weapon arm $n sizes you up with an icing glare.",
			"You nimbly tie yourself into a knot.",
			"$n nimbly ties $mself into a knot.",
			"You vanish and reappear without anyone's notice.",
			"*poof* $n just vanished and reappeared without your notice!",
			"Tongues of flame dance and twirl as you lose yourself in a trance.",
			"Tongues of flame dance and twirl as $n loses $mself in a trance.",
			"Blue sparks fly from your fingers.",
			"Blue sparks fly from $n's fingers.",
			"A halo appears over your head.",
			"A halo appears over $n's head.",
			"With a snap of your fingers the undead dance to your arcane harmony.",
			"The undead dance to $n's arcane harmony as $e snaps $s finger."
		}
	},

	{
		{
			"You hit your head, and your eyes roll.",
			"$n hits $s head, and $s eyes roll.",
			"You twirl your swords around in a remarkable display of swordsmanship.",
			"You watch $n twirl $s swords around in a remarkable display of swordsmanship.",
			"You juggle with daggers, apples, and eyeballs.",
			"$n juggles with daggers, apples, and eyeballs.",
			"You dazzle your audience executing triple back flip kicks.",
			"You get all dizzy watching $n executing triple back flip kicks.",
			"The heavens boom in thunderous clasps of lightning, just as rose petals fall gently around you.",
			"The heavens boom in thunderous clasps of lightning, just as rose petals fall gently around $n.",
			"Little green lights dance in your eyes.",
			"Little green lights dance in $n's eyes.",
			"You recite words of wisdom.",
			"$n recites words of wisdom.",
			"The mist gathers about you with screams of the undead.",
			"The mist gathers about $n with screams of the undead."
		}
	},

	{
		{
			"Crunch, crunch -- you munch a bottle.",
			"Crunch, crunch -- $n munches a bottle.",
			"You lift the dead lion over your head, proclaiming victory.",
			"$n lifts the dead lion over $s head, proclaiming victory.",
			"You steal the underwear off every person in the room.",
			"Your underwear is gone!  $n stole it!",
			"Wow!  You just caught a fly with chopsticks!",
			"Wow!  $n just caught a fly with chopsticks!",
			"Deep in prayer, you levitate.",
			"Deep in prayer, $n levitates.",
			"A slimy green monster appears before you and bows.",
			"A slimy green monster appears before $n and bows.",
			"*CRUSH* You just broke a big rock on your forehead!",
			"*CRUSH* $n just broke a big rock on $s forehead!",
			"You summon the grim reaper to stand by your side.",
			"$n summons the grim reaper to stand by $s side."
		}
	},

	{
		{
			"Four matched Percherons bring in your chariot.",
			"Four matched Percherons bring in $n's chariot.",
			"The crowd cheers in a standing ovation as you draw your blades.",
			"The crowd cheers in a standing ovation as $n draws $s blades.",
			"The dice roll ... and you win again.",
			"The dice roll ... and $n wins again.",
			"You shoot an arrow with great skill.",
			"An arrow from $n's bow just impaled the apple you were about to eat.",
			"An angel consults you.",
			"An angel consults $n.",
			"You turn everybody into a little pink elephant.",
			"You are turned into a little pink elephant by $n.",
			"A great light from the heavens enshroud you as you pray.",
			"A great light from the heavens enshrouds $n as $e prays.",
			"Dark clouds loom overhead as you chant a magic incarnation.",
			"Dark clouds loom overhead as $n chants a magic incarnation."
		}
	},

	{
		{
			"Arnold Schwarzenegger admires your physique.",
			"Arnold Schwarzenegger admires $n's physique.",
			"Blood splatters around as you wipe your weapon clean.",
			"Blood splatters around as $n wipes $s weapon clean.",
			"You count the money in everyone's pockets.",
			"Check your money, $n is counting it.",
			"You offer poisoned beer to everyone in the room.",
			"$n offers beer to everyone present.  It smells a little funny..",
			"Your body glows with an unearthly light.",
			"$n's body glows with an unearthly light.",
			"A small ball of light dances on your fingertips.",
			"A small ball of light dances on $n's fingertips.",
			"Your prayers have converted Lord Demise from his evil ways.",
			"The prayers of $n have converted Lord Demise from his evil ways.",
			"A skeletal hand bursts out of the ground to massage your back.",
			"A skeletal hand bursts out of the ground to massage $n's back."
		}
	},

	{
		{
			"Mercenaries arrive to do your bidding.",
			"Mercenaries arrive to do $n's bidding.",
			"With a sweep of your weapon, you knock down everyone in the room.",
			"$n sweeps with $s weapon, and you find yourself on the ground.",
			"You balance a pocket knife on your tongue.",
			"$n balances a pocket knife on $s tongue.",
			"You string up the hangman with $s own noose.",
			"$n strings up the hangman with $s own noose.",
			"Smoke and fumes leak from your nostrils.",
			"Smoke and fumes leak from $n's nostrils.",
			"A spot light hits you.",
			"A spot light hits $n.",
			"Your mighty upper cut sends your opponent flying.",
			"$n's mighty upper cut sends $s opponent flying.",
			"A strange glow emits from you as you start to chant.",
			"A strange glow emits from $n as $e starts to chant."
		}
	},

	{
		{
			"A fiery stallion kneels for you to mount.",
			"A fiery stallion kneels so that $n may mount.",
			"Swing!  You just decapitated your own shadow.",
			"Swing!  $n just decapitated $s own shadow.",
			"You produce a coin from everyone's ear.",
			"$n produces a coin from your ear.",
			"You step behind your own shadow.",
			"$n steps behind $s own shadow.",
			"Everyone levitates as you pray.",
			"You levitate as $n prays.",
			"The light flickers as you rap in magical languages.",
			"The light flickers as $n raps in magical languages.",
			"Oomph!  You squeeze water out of a granite boulder.",
			"Oomph!  $n squeezes water out of a granite boulder.",
			"Time collapses as you blink.",
			"Time collapses as $n blinks."
		}
	},

	{
		{
			"Elven Warhorses answer your call to battle.",
			"Elven Warhorses arrive as $n screams an ancient battlecry.",
			"You pick your teeth with a spear.",
			"$n picks $s teeth with a spear.",
			"You step behind your shadow.",
			"$n steps behind $s shadow.",
			"You appear out of nowhere.",
			"$n appears mysteriously from the gleam of your eye.",
			"A cool breeze refreshes you.",
			"A cool breeze refreshes $n.",
			"Your head disappears.",
			"$n's head disappears.",
			"The oracle asks you for your advice.",
			"The oracle asks for $n's advice.",
			"A demon host bows in submission to your might.",
			"A demon host bows in submission to $n's might."
		}
	},

	{
		{
			"You crack open a goblin's skull and drink the brains.",
			"$n cracks open a goblin's skull and drinks the brains.",
			"Your deathly stare sends shivers into your opponents hearts.",
			"$n's deathly stare sends shivers into your heart.",
			"Your eyes dance with greed.",
			"$n's eyes dance with greed.",
			"You flick your wrist and a throwing star appears from nowhere.",
			"With a flick of $s wrist a throwing star appears in $n's hand.",
			"A fire elemental singes your hair.",
			"A fire elemental singes $n's hair.",
			"The sun pierces through the clouds to illuminate you.",
			"The sun pierces through the clouds to illuminate $n.",
			"Everyone is swept off their feet by your hug.",
			"You are swept off your feet by $n's hug.",
			"The whispers of death descends on all as you emerge from the mists.",
			"The whispers of death descends on your soul as $n emerges from the mists."
		}
	},

	{
		{
			"Everyone moves out of your way as you grumble.",
			"You quickly move out of $n's way as $e grumbles.",
			"As you draw your blades, your enemies scatter before you.",
			"$n's enemies scatter as $e draws $s blades.",
			"You deftly steal everyone's weapon.",
			"$n deftly steals your weapon.",
			"Your mercurial dance leaves only your opponent's blood stains on the ground.",
			"$n's mercurial dance leaves only $s opponent's blood stains on the ground.",
			"The ocean parts before you.",
			"The ocean parts before $n.",
			"The sky changes color to match your eyes.",
			"The sky changes color to match $n's eyes.",
			"Your haymaker splits a tree.",
			"$n's haymaker splits a tree.",
			"Poisonous vines crack in whiplike fashion as you frown.",
			"Poisonous vines crack in whiplike fashion as $n frowns."
		}
	},

	{
		{
			"The sound of your battle cry rips havoc and fear to all around you.",
			"The sound of $n's battle cry rips havoc and fear all around $m.",
			"Your whirling blades massacre the titan.",
			"$n's whirling blades massacre the titan.",
			"The Grey Mouser buys you a beer.",
			"The Grey Mouser buys $n a beer.",
			"The shadows flicker as you descend and embed your knife into your victim.",
			"The shadows flicker as $n descends and embeds $s knife into $s victim.",
			"A thunder cloud kneels to you.",
			"A thunder cloud kneels to $n.",
			"The stones dance to your command.",
			"The stones dance to $n's command.",
			"A strap of your armor breaks over your mighty thews.",
			"A strap of $n's armor breaks over $s mighty thews.",
			"You order some eye of newt from Doogan's shop.",
			"$n orders some eye of newt from Doogan's shop."
		}
	},

	{
		{
			"You smile as you lick your opponent's blood from your blade.",
			"$n smiles as $e licks $s opponent's blood from $s blade.",
			"Valkyries sing praises to the legends of your victory",
			"Valkyries sing praises to the legends of $n's victories",
			"Everyone's pocket explodes with your fireworks.",
			"Your pocket explodes with $n's fireworks.",
			"Everyone discovers your dagger an inch from their eyes.",
			"You discover $n's dagger an inch from your eye.",
			"Meteors swoop down and circle you.",
			"Meteors sweep down and circle $n.",
			"The heavens and grass change color as you smile.",
			"The heavens and grass change color as $n smiles.",
			"A boulder cracks at your frown.",
			"A boulder cracks at $n's frown.",
			"You peer into everyone's soul, draining their life away.",
			"You feel your life drain away as $n's peers into your soul."
		}
	},

	{
		{
			"Your furious rage devours all that stand before you.",
			"$n's furious rage devours all that stand before $m.",
			"You blow the horn of Valhalla before entering the battle fields.",
			"You hear the horn of Valhalla as $n enters the battle fields.",
			"Everyone discovers your dagger a centimeter from their eye.",
			"You discover $n's dagger a centimeter from your eye.",
			"You emerge from a dark alley.",
			"$n emerges from the dark alley of your mind.",
			"You utter a few words and the earth glistens with new green life.",
			"$n utters a few words and the earth glistens with new green life.",
			"Everyone's clothes are transparent, and you are laughing.",
			"Your clothes are transparent, and $n is laughing.",
			"Watch your feet, you are juggling granite boulders.",
			"Watch your feet, $n is juggling granite boulders.",
			"The angel of death asks you for some pointers.",
			"The angel of death asks $n for some pointers."
		}
	},

	{
		{
			"The smell of rage leaks from your body as you go berserk.",
			"The smell of rage leaks from $n's body as $e goes berserk.",
			"You finish your job as the emperor's thumb goes down.",
			"The emperor gives the thumbs down, and $n finishes the job.",
			"Where did you go?",
			"Where did $n go?",
			"Millions cringe at the thought of your wrath.",
			"Millions cringe at the thought of $n's wrath.",
			"The great god Mota gives you a staff.",
			"The great god Mota gives $n a staff.",
			"A black hole swallows you.",
			"A black hole swallows $n.",
			"... 98, 99, 100 ... you do pushups.",
			"... 98, 99, 100 ... $n does pushups.",
			"An ill wind blows as you summon a demon steed to serve you.",
			"An ill wind blows as a demon steed approaches $n and offers its service."
		}
	},

	{
		{
			"Silence suddenly fills the area as you draw your sword and shield.",
			"Silence suddenly fills the area as $n draws $s sword and shield.",
			"Your slashing stance rips shreds in the wind.",
			"$n's slashing stance rips shreds in the wind.",
			"Click.",
			"Click.",
			"You post a 'Murder for Hire' sign.",
			"$n puts out $s 'Murder for Hire' sign.",
			"You spit on the ground and flowers sprout forth.",
			"$n spits on the ground and flowers sprout forth.",
			"The world shimmers in time with your whistling.",
			"The world shimmers in time with $n's whistling.",
			"A multitude of angels sing your praises for all the world to hear.",
			"A multitude of angels sing $n's praises for all the world to hear.",
			"Your evil aura drives stakes into the hearts of the holy.",
			"$n's evil aura drives stakes into the hearts of the holy."
		}
	}
};

void do_pose (CHAR_DATA * ch, char *argument)
{
	int pose;

	push_call("do_pose(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	pose = number_range(0, sizeof(pose_table) / sizeof(pose_table[0]) - 1);

	act (pose_table[pose].message[2 * ch->class + 0], ch, NULL, NULL, TO_CHAR);
	act (pose_table[pose].message[2 * ch->class + 1], ch, NULL, NULL, TO_ROOM);

	pop_call();
	return;
}


void do_qui (CHAR_DATA * ch, char *argument)
{
	push_call("do_qui(%p,%p)",ch,argument);

	send_to_char ("If you want to QUIT, you have to spell it out.\n\r", ch);
	pop_call();
	return;
}


void do_quit (CHAR_DATA * ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	PLAYER_GAME *gpl;

	push_call("do_quit(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		raw_kill(ch);
		pop_call();
		return;
	}

	if (argument && ch->position == POS_FIGHTING)
	{
		send_to_char ("No way! You are fighting.\n\r", ch);
		pop_call();
		return;
	}

	if (argument && ch->position < POS_STUNNED)
	{
		send_to_char ("You're not DEAD yet.\n\r", ch);
		pop_call();
		return;
	}

	if (argument && IS_SET(ch->in_room->room_flags, ROOM_NO_SAVE))
	{
		send_to_char("You can't save here, so you had better not quit.\n\r", ch);
		pop_call();
		return;
	}

	if (argument && ch->pcdata->last_combat + 20 > mud->current_time)
	{
		ch_printf(ch, "You must wait %d seconds before you can quit.\n\r", ch->pcdata->last_combat + 20 - mud->current_time);
		pop_call();
		return;
	}

	if (argument && ch->pcdata->pnote != NULL && strcasecmp(argument, "now"))
	{
		send_to_char ("You must specify 'quit now'.\n\rYou have a note in progress.\n\r", ch);
		pop_call();
		return;
	}

	for (d = mud->f_desc ; d ; d = d->next)
	{
		if (d->original == ch)
		{
			do_return(d->character, NULL);
		}
	}

	if (ch->in_room->vnum == ROOM_VNUM_LIMBO)
	{
		char_from_room(ch);
		char_to_room(ch, ch->pcdata->was_in_room);
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_SAVE))
	{
		char_from_room (ch);
		char_to_room(ch, ROOM_VNUM_TEMPLE);
	}

	if (ch->master != NULL)
	{
		stop_follower(ch);
	}

	if (ch->pcdata->editor)
	{
		stop_editing(ch);
	}

	save_char_obj(ch, NORMAL_SAVE);

	for (gpl = mud->f_player ; gpl ; gpl = gpl->next)
	{
		if (gpl->ch->pcdata->reply == ch)
		{
			gpl->ch->pcdata->reply = NULL;
		}
	}

	stop_fighting(ch, TRUE);

	show_who_can_see(ch, " has left the game.\n\r");

	log_god_printf("%s has quit!", get_name (ch));

	/*
		After extract_char the ch is no longer valid!
	*/

	if (ch->desc && ch->desc->snoop_by != NULL)
	{
		write_to_buffer(ch->desc->snoop_by, "Your victim has quit.\n\r", 0);
		ch->desc->snoop_by = NULL;
	}

	vt100off(ch);

	d = NULL;

	if (ch->desc != NULL)
	{
		d = ch->desc;
		send_goodbye(d);
		write_to_port(d);
		*d->inbuf = '\0';
	}

	extract_char(ch);

	if (d != NULL)
	{
		d->character = NULL;
	}
	if (d != NULL)
	{
		close_socket (d, TRUE);
	}
	pop_call();
	return;
}

void send_goodbye(DESCRIPTOR_DATA *d)
{
	CHAR_DATA *ch = CH(d);
	char buf1[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

	push_call("send_goodbye(%p)",d);

	strcpy(buf1, god_table[which_god(ch)].logout_msg);
	strcpy(buf2, god_table[which_god(ch)].color);

	cat_sprintf(buf2, "\n\r%s\n\r", center(buf1, get_page_width(ch)));
	cat_sprintf(buf2, "\n\r%s\n\r", center("Thank you for playing", get_page_width(ch)));
	cat_sprintf(buf2, "\n\r%s\n\r", center("Storms of Time", get_page_width(ch)));
	cat_sprintf(buf2, "\n\r%s\n\r", "{088}");

	write_to_descriptor(d, ansi_translate(buf2), 0);

	pop_call();
	return;
}

void do_relog (CHAR_DATA * ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	PLAYER_GAME *gpl;
	CHAR_DATA *fch;

	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char name[MAX_INPUT_LENGTH];

	bool loaded = FALSE;

	push_call("do_relog(%p,%p)",ch,argument);

	if (IS_NPC(ch) || ch->desc == NULL)
	{
		pop_call();
		return;
	}

	if (ch->position == POS_FIGHTING)
	{
		send_to_char ("No way! You are fighting.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->position < POS_STUNNED)
	{
		send_to_char ("You're not DEAD yet.\n\r", ch);
		pop_call();
		return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_SAVE))
	{
		send_to_char("You can't save here, so you had better not quit.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->pcdata->last_combat + 20 > mud->current_time)
	{
		ch_printf(ch, "You must wait %d seconds before you can quit.\n\r", ch->pcdata->last_combat + 20 - mud->current_time);
		pop_call();
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	strcpy(name, capitalize_name(arg1));

	remove_bad_desc_name(name);

	fch = lookup_char(name);

	if (fch == NULL)
	{
		fch = start_partial_load(ch, name);
		if (fch == NULL)
		{
			pop_call();
			return;
		}
		loaded = TRUE;
	}

	if (fch == ch)
	{
		send_to_char("You are already yourself.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->pcdata->password != fch->pcdata->password && encrypt64(arg2) != fch->pcdata->password)
	{
		send_to_char("Wrong password, please wait 5 seconds.\n\r", ch);
		wait_state(ch, PULSE_PER_SECOND * 5);

		if (loaded)
		{
			clear_partial_load(fch);
		}
		pop_call();
		return;
	}

	if (ch->in_room->vnum == ROOM_VNUM_LIMBO)
	{
		char_from_room(ch);
		char_to_room(ch, ch->pcdata->was_in_room);
	}

	if (ch->master != NULL)
	{
		stop_follower(ch);
	}

	if (ch->pcdata->editor)
	{
		stop_editing(ch);
	}

	save_char_obj(ch, NORMAL_SAVE);

	for (gpl = mud->f_player ; gpl ; gpl = gpl->next)
	{
		if (gpl->ch->pcdata->reply == ch)
		{
			gpl->ch->pcdata->reply = NULL;
		}
	}

	stop_fighting(ch, TRUE);

	show_who_can_see(ch, " has left the game.\n\r");

	d = ch->desc;

	if (loaded)
	{
		clear_partial_load(fch);

		load_char_obj(d, name);

		fch = d->character;

		add_char(fch);
		add_player(fch);

		char_to_room(fch, fch->pcdata->was_in_room);
	}
	else
	{
		if (fch->desc)
		{
			close_socket(fch->desc, TRUE);
		}
		d->character	= fch;
		fch->desc		= d;

		act("$n has relogged as $N.", ch, NULL, fch, TO_ROOM);
	}

	char_to_room(ch, ROOM_VNUM_LIMBO);

	fch->pcdata->vt100		= ch->pcdata->vt100;
	fch->pcdata->vt100_type	= ch->pcdata->vt100_type;

	do_refresh(fch, "");

	if (loaded)
	{
		enter_game(fch);
	}
	else
	{
		do_look(fch, "auto");
	}
	act("You relogged as $n.", fch, NULL, NULL, TO_CHAR);

	log_god_printf("%s has relogged as %s!", ch->name, fch->name);

	ch->desc = NULL;

	extract_char(ch);
	pop_call();
	return;
}


void do_save (CHAR_DATA * ch, char *argument)
{
	PLAYER_GAME *gch;
	CHAR_DATA   *tch;

	push_call("do_save(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (ch->level < 99)
	{
		send_to_char("Your character is saved every 10 minutes, and at important events.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp(argument, "all"))
	{
		send_to_char("Saving all players.\n\r", ch);
		for (gch = mud->f_player ; gch ; gch = gch->next)
		{
			save_char_obj(gch->ch, NORMAL_SAVE);
		}
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: save <all|player>\n\r", ch);
		pop_call();
		return;
	}
	else if ((tch = get_player_world(ch, argument)) == NULL)
	{
		do_save(ch, "");
		pop_call();
		return;
	}

	ch_printf(ch, "Saving %s.\n\r", get_name(tch));

	save_char_obj(tch, NORMAL_SAVE);

	pop_call();
	return;
}

void do_follow (CHAR_DATA * ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;

	push_call("do_follow(%p,%p)",ch,argument);

	one_argument (argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char ("Follow whom?\n\r", ch);
		pop_call();
		return;
	}

	if (ch->fighting)
	{
		send_to_char("No way!  You are still fighting!\n",ch);
		pop_call();
		return;
	}

	if ((victim = get_char_room (ch, arg)) == NULL)
	{
		send_to_char ("They aren't here.\n\r", ch);
		pop_call();
		return;
	}

	if (victim == ch)
	{
		if (ch->master == NULL)
		{
			send_to_char ("You already follow yourself.\n\r", ch);
			pop_call();
			return;
		}
		stop_follower(ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch))
	{
		if ((ch->level < (victim->level * 3 / 4) - 2 || ch->level > (victim->level * 5 / 4) + 2) && ch->level < MAX_LEVEL)
		{
			send_to_char ("You are not of the right caliber to follow.\n\r", ch);
			pop_call();
			return;
		}
		if (IS_AFFECTED (victim, AFF_CHARM) && IS_NPC (victim))
		{
			send_to_char ("You may not follow a pet.\n\r", ch);
			pop_call();
			return;
		}
	}

	/* poll the block list of the victim */

	if (blocking(victim, ch))
	{
		act ("$E eludes you at every step.", ch, 0, victim, TO_CHAR);
		pop_call();
		return;
	}

	if (ch->master != NULL)
	{
		stop_follower (ch);
	}
	add_follower (ch, victim);
	pop_call();
	return;
}

void add_follower (CHAR_DATA * ch, CHAR_DATA * master)
{
	PET_DATA *pet;

	push_call("add_follower(%p,%p)",ch,master);

	if (ch->master != NULL)
	{
		bug ("Add_follower: non-null master.", 0);
		pop_call();
		return;
	}

	ch->master = master;
	ch->leader = NULL;
	if (can_see(master, ch))
	{
		act ("$n now follows you.", ch, NULL, master, TO_VICT);
	}
	act ("You now follow $N.", ch, NULL, master, TO_CHAR);

	ALLOCMEM(pet, PET_DATA, 1);
	pet->ch = ch;
	LINK(pet, mud->f_pet, mud->l_pet, next, prev);

	pop_call();
	return;
}


void stop_follower (CHAR_DATA * ch)
{
	PET_DATA *pet;

	push_call("stop_follower(%p)",ch);

	if (ch == NULL)
	{
		bug( "stop_follower : ch == NULL.",0);
		pop_call();
		return;
	}

	if (ch->master == NULL)
	{
		bug( "Stop_follower: null master.", 0 );
		pop_call();
		return;
	}

	if (can_see(ch->master, ch))
	{
		act ("$n stops following you.", ch, NULL, ch->master, TO_VICT);
	}
	act ("You stop following $N.", ch, NULL, ch->master, TO_CHAR);

	if (ch->master->mounting == ch)
	{
		ch->master->mounting = NULL;
	}
	ch->master = NULL;
	ch->leader = NULL;

	for (pet = mud->f_pet ; pet ; pet = pet->next)
	{
		if (pet->ch == ch)
		{
			UNLINK(pet, mud->f_pet, mud->l_pet, next, prev);
			FREEMEM(pet);
			break;
		}
	}

	if (IS_AFFECTED(ch, AFF_CHARM))
	{
		raw_kill(ch);
	}

	if (IS_AFFECTED(ch, AFF2_POSSESS))
	{
		raw_kill(ch);
	}
	pop_call();
	return;
}


void die_follower (CHAR_DATA * ch)
{
	PET_DATA *pch, *pch_next;

	push_call("die_follower(%p)",ch);

	ch->position = POS_DEAD;

	if (ch->master != NULL)
	{
		stop_follower(ch);
	}
	ch->leader = NULL;

	for (pch = mud->f_pet ; pch ; pch = pch_next)
	{
		pch_next = pch->next;

		if (pch->ch->master == ch)
		{
			stop_follower(pch->ch);
		}
	}
	pop_call();
	return;
}


void do_shadow (CHAR_DATA * ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int door;
	ROOM_INDEX_DATA *old_room;

	push_call("do_shadow(%p,%p)",ch,argument);

	if (IS_NPC(ch) || learned(ch, gsn_shadow) == 0)
	{
		send_to_char("You failed.\n\r", ch);
		pop_call();
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if ((door = direction_door(arg1)) == -1)
	{
		if ((door = direction_door(arg2)) == -1)
		{
			send_to_char ("What direction?\n\r", ch);
			pop_call();
			return;
		}
		else
		{
			strcpy(arg2, arg1);
		}
	}

	if (arg2[0] == '\0')
	{
		send_to_char ("Shadow whom?\n\r", ch);
		pop_call();
		return;
	}

	if (!is_valid_exit(ch, ch->in_room, door))
	{
		send_to_char ("There is no exit there.\n\r", ch);
		pop_call();
		return;
	}

	old_room = ch->in_room;

	ch->in_room = room_index[get_exit(ch->in_room->vnum, door)->to_room];

	victim = get_char_room(ch, arg2);

	ch->in_room = old_room;

	if (victim == NULL || victim->in_room == ch->in_room)
	{
		send_to_char ("They aren't there.\n\r", ch);
		pop_call();
		return;
	}

	if (number_percent() >= learned(ch, gsn_shadow))
	{
		act ("You fail to shadow $m.", ch, NULL, victim, TO_CHAR);
		pop_call();
		return;
	}

	if (IS_NPC(victim) || victim->pcdata->shadowed_by || victim->pcdata->shadowing)
	{
		act ("You can't seem to shadow $m.", ch, NULL, victim, TO_CHAR);
		pop_call();
		return;
	}

	if (ch->master)
	{
		stop_follower (ch);
	}
	if (ch->pcdata->shadowing)
	{
		stop_shadow(ch);
	}
	add_shadow(ch, victim);
	pop_call();
	return;
}

void add_shadow (CHAR_DATA * ch, CHAR_DATA * master)
{
	push_call("add_shadow(%p,%p)",ch,master);

	if (ch->pcdata->shadowing)
	{
		bug("add_shadow : already shadowing.", 0);
		pop_call();
		return;
	}
	ch->pcdata->shadowing       = master;
	master->pcdata->shadowed_by = ch;
	act ("You are now shadowing $N.", ch, NULL, master, TO_CHAR);
	pop_call();
	return;
}


void stop_shadow (CHAR_DATA * ch)
{
	push_call("stop_shadow(%p)",ch);

	if (IS_NPC(ch) || ch->pcdata->shadowing == NULL)
	{
		pop_call();
		return;
	}
	act ("You stop shadowing $N.", ch, NULL, ch->pcdata->shadowing, TO_CHAR);

	ch->pcdata->shadowing->pcdata->shadowed_by = NULL;
	ch->pcdata->shadowing = NULL;

	pop_call();
	return;
}


void do_order (CHAR_DATA * ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	CHAR_DATA *och;
	CHAR_DATA *och_next;
	bool found;
	bool fAll;

	push_call("do_order(%p)",ch,argument);

	argument = one_argument (argument, arg);
	if (arg[0] == '\0' || argument[0] == '\0')
	{
		send_to_char ("Order whom to do what?\n\r", ch);
		pop_call();
		return;
	}

	if (IS_AFFECTED (ch, AFF_CHARM))
	{
		send_to_char ("You feel like taking, not giving, orders.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "all"))
	{
		fAll = TRUE;
		victim = NULL;
	}
	else
	{
		fAll = FALSE;
		if ((victim = get_char_room (ch, arg)) == NULL)
		{
			send_to_char ("They aren't here.\n\r", ch);
			pop_call();
			return;
		}

		if (victim == ch)
		{
			send_to_char ("Aye aye, right away!\n\r", ch);
			pop_call();
			return;
		}

		if (!IS_AFFECTED (victim, AFF_CHARM) || victim->master != ch)
		{
			send_to_char ("Do it yourself!\n\r", ch);
			pop_call();
			return;
		}
	}

	found = FALSE;
	for (och = ch->in_room->first_person; och != NULL; och = och_next)
	{
		och_next = och->next_in_room;
		if (IS_AFFECTED (och, AFF_CHARM) && och->master == ch && (fAll || och == victim))
		{
			found = TRUE;
			act ("$n orders you to '$t'.", ch, argument, och, TO_VICT);
			interpret (och, argument);
		}
	}

	if (found)
	{
		send_to_char ("Ok.\n\r", ch);
	}
	else
	{
		send_to_char ("You have no followers here.\n\r", ch);
	}
	pop_call();
	return;
}


void do_group (CHAR_DATA * ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	char dim[10], bld[10];
	int t_hp, t_current_hp, t_mana, t_current_mana, t_level, t_move, t_current_move;
	CHAR_DATA *victim, *leader;
	FIGHT_DATA *fight;
	PET_DATA	 *pch;

	push_call("do_group(%p,%p)",ch,argument);

	one_argument (argument, arg);

	t_hp = t_current_hp = t_current_mana = t_current_move = t_mana = t_level = t_move = 0;

	strcpy(dim, get_color_string(ch, COLOR_PROMPT, VT102_DIM));
	strcpy(bld, get_color_string(ch, COLOR_PROMPT, VT102_BOLD));

	leader = (ch->leader != NULL) ? ch->leader : ch;

	if (arg[0] == '\0')
	{
		ch_printf_color(ch, "{078}%s's group:\n\r", get_name(leader));

		ch_printf_color(ch, "{038}[%3d %s]{178} %-12s {128}%5d{028}/{128}%5d{028} Hp {128}%5d{028}/{128}%5d{028} Ma {128}%5d{028}/{128}%5d{028} Mv {128}%9d{028} Xp\n\r",
			leader->level,
			IS_NPC (leader) ? "Mob" : class_table[leader->class].who_name,
			str_resize(IS_NPC(leader) ? capitalize(leader->short_descr) : leader->name, buf, -12),
			leader->hit,  leader->max_hit,
			leader->mana, leader->max_mana,
			leader->move, leader->max_move,
			exp_level(leader->class, leader->level) - (IS_NPC(leader) ? 0 : leader->pcdata->exp));

			t_hp           += leader->max_hit;
			t_current_hp   += leader->hit;
			t_mana         += leader->max_mana;
			t_current_mana += leader->mana;
			t_move         += leader->max_move;
			t_current_move += leader->move;
			t_level        += leader->level;

		for (pch = mud->f_pet ; pch ; pch = pch->next)
		{
			if (pch->ch != leader && is_same_group(pch->ch, ch))
			{
				ch_printf_color(ch, "{038}[%3d %s]{178} %-12s {128}%5d{028}/{128}%5d{028} Hp {128}%5d{028}/{128}%5d{028} Ma {128}%5d{028}/{128}%5d{028} Mv {128}%9d{028} Xp\n\r",
					pch->ch->level,
					IS_NPC (pch->ch) ? "Mob" : class_table[pch->ch->class].who_name,
					str_resize(IS_NPC(pch->ch) ? capitalize(pch->ch->short_descr) : pch->ch->name, buf, -12),
					pch->ch->hit,  pch->ch->max_hit,
					pch->ch->mana, pch->ch->max_mana,
					pch->ch->move, pch->ch->max_move,
					exp_level(pch->ch->class, pch->ch->level) - (IS_NPC(pch->ch) ? 0 : pch->ch->pcdata->exp));

				t_hp           += pch->ch->max_hit;
				t_current_hp   += pch->ch->hit;
				t_mana         += pch->ch->max_mana;
				t_current_mana += pch->ch->mana;
				t_move         += pch->ch->max_move;
				t_current_move += pch->ch->move;
				t_level        += pch->ch->level;
			}
		}
		ch_printf_color(ch, "{038}[%3d    ]{178} %-12s {128}%5d{028}/{128}%5d{028} Hp {128}%5d{028}/{128}%5d{028} Ma {128}%5d{028}/{128}%5d{028} Mv\n\r",
			t_level,
			"Totals",
			t_current_hp,   t_hp,
			t_current_mana, t_mana,
			t_current_move, t_move);

		pop_call();
		return;
	}

	for (fight = mud->f_fight ; fight ; fight = fight->next)
	{
		if (is_same_group(ch, fight->ch))
		{
			send_to_char ("You cannot modify the group while a member is fighting.\n\r", ch);
			pop_call();
			return;
		}
	}

	if (!strcmp (arg, "disband"))
	{
		int count = 0;

		if (ch->master)
		{
			send_to_char ("You cannot disband a group if you're following someone.\n\r ", ch);
			pop_call();
			return;
		}

		for (pch = mud->f_pet ; pch ; pch = pch->next)
		{
			if (ch != pch->ch && is_same_group(ch, pch->ch))
			{
				pch->ch->leader = NULL;
				count++;
				send_to_char("Your group is disbanded.\n\r", pch->ch);
			}
		}

		if (count == 0)
		{
			send_to_char ("You have no group members to disband.\n\r", ch);
		}
		else
		{
			send_to_char ("You disband your group.\n\r", ch);
		}
		pop_call();
		return;
	}

	if (!strcmp (arg, "all"))
	{
		CHAR_DATA *rch;
		int count = 0;

		for (rch = ch->in_room->first_person ; rch ; rch = rch->next_in_room)
		{
			if (ch != rch && can_see(ch, rch) && rch->master == ch && !rch->fighting && rch->position != POS_FIGHTING && !ch->master && !ch->leader && !is_same_group(rch, ch))
			{
				rch->leader = ch;
				count++;
			}
		}

		if (count == 0)
		{
			send_to_char ("You have no eligible group members.\n\r", ch);
		}
		else
		{
			act ("$n groups $s followers.", ch, NULL, NULL, TO_ROOM);
			send_to_char ("You group your followers.\n\r", ch);
		}
		pop_call();
		return;
	}

	if (ch->master)
	{
		send_to_char ("But you are following someone else!\n\r", ch);
		pop_call();
		return;
	}

	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		if ((victim = get_player_world(ch, arg)) == NULL)
		{
			send_to_char ("Hmmm...where did they go?\n\r", ch);
			pop_call();
			return;
		}
	}

	if (victim->master != ch)
	{
		act ("$N isn't following you.", ch, NULL, victim, TO_CHAR);
		pop_call();
		return;
 	}

	if (is_same_group (victim, ch))
	{
		victim->leader = NULL;

		act ("$n removes $N from $s group.", ch, NULL, victim, TO_NOTVICT);
		act ("$n removes you from $s group.", ch, NULL, victim, TO_VICT);
		act ("You remove $N from your group.", ch, NULL, victim, TO_CHAR);

		pop_call();
		return;
	}

	if (victim->fighting != NULL || victim->position == POS_FIGHTING )
	{
		send_to_char ("You cannot group someone that is fighting.\n\r", ch);
		pop_call();
		return;
	}

	victim->leader = ch;

	act ("$N joins $n's group.", ch, NULL, victim, TO_NOTVICT);
	act ("You join $n's group.", ch, NULL, victim, TO_VICT);
	act ("$N joins your group.", ch, NULL, victim, TO_CHAR);

	pop_call();
	return;
}

/*
	'Split' originally by Gnort, God of Chaos.
*/

void do_split (CHAR_DATA * ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *gch;
	int members;
	int amount;
	int share;
	int extra;

	push_call("do_split(%p,%p)",ch,argument);

	one_argument (argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char ("How much gold do you want to split ?\n\r", ch);
		pop_call();
		return;
	}

	amount = atol (arg);

	if (amount < 0)
	{
		send_to_char ("Your group wouldn't like that.\n\r", ch);
		pop_call();
		return;
	}

	if (amount == 0)
	{
		send_to_char ("You hand out zero coins, but no one notices.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->gold < amount)
	{
		send_to_char ("You don't have that much gold.\n\r", ch);
		pop_call();
		return;
	}

	members = 0;
	for (gch = ch->in_room->first_person ; gch != NULL ; gch = gch->next_in_room)
	{
		if (is_same_group (gch, ch) && !IS_NPC (gch))
		{
			members++;
		}
	}

	if (members < 2)
	{
		send_to_char ("Just keep it all.\n\r", ch);
		pop_call();
		return;
	}

	share = amount / members;
	extra = amount % members;

	if (share == 0)
	{
		send_to_char ("Generousity is an art.\n\r", ch);
		pop_call();
		return;
	}

	ch->gold -= amount;
	ch->gold += share + extra;

	ch_printf(ch, "You split %d gold coins.  Your share is %d gold coins.\n\r", amount, share + extra);

	sprintf (buf, "$n splits %d gold coins.  Your share is %d gold coins.", amount, share);

	for (gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room)
	{
		if (gch != ch && is_same_group (gch, ch) && !IS_NPC (gch))
		{
			act (buf, ch, NULL, gch, TO_VICT);
			gch->gold += share;
		}
	}
	pop_call();
	return;
}

void do_gtell (CHAR_DATA * ch, char *argument)
{
	char jbuf[MAX_STRING_LENGTH];
	char text2[MAX_STRING_LENGTH];
	PLAYER_GAME *gpl;

	push_call("do_gtell(%p,%p)",ch,argument);

	if (argument[0] == '\0')
	{
		send_to_char ("Tell your group what?\n\r", ch);
		pop_call();
		return;
	}

	if (!IS_NPC(ch) && IS_SET(pvnum_index[ch->pcdata->pvnum]->flags, PVNUM_SILENCED))
	{
		send_to_char ("You cannot talk.\n\r", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}
	sprintf(jbuf, "You tell the group '%s'", text2);
	ch_printf(ch, "%s%s\n\r", get_color_string(ch, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(ch)));

	for (gpl = mud->f_player ; gpl ; gpl = gpl->next)
	{
		if (is_same_group(gpl->ch, ch) && ch != gpl->ch && !IS_NPC(gpl->ch))
		{
			if (!can_understand(gpl->ch, ch, TRUE))
			{
				continue;
			}
			sprintf(jbuf, "%s tells the group '%s'", capitalize(get_name(ch)), text2);
			ch_printf(gpl->ch, "%s%s\n\r", get_color_string(gpl->ch, COLOR_SPEACH, VT102_BOLD), justify(jbuf, get_page_width(gpl->ch)));
		}
	}
	pop_call();
	return;
}

/*
	It is very important that this be an equivalence relation:
	(1) A ~ A
	(2) if A ~ B then B ~ A
	(3) if A ~ B  and B ~ C, then A ~ C
*/

bool is_same_group (CHAR_DATA * ach, CHAR_DATA * bch)
{
	push_call("is_same_group(%p,%p)",ach,bch);
	/*
		the idea is this :
		if my leader is your leader, then we are in the same
		group, else we're in different groups, if at all in a group
	*/

	if (ach->leader != NULL)
	{
		ach = ach->leader;
	}
	if (bch->leader != NULL)
	{
		bch = bch->leader;
	}
	pop_call();
	return ach == bch;
}

bool vnum_in_group (CHAR_DATA * ach, int mobvnum)
{
	CHAR_DATA *gch;

	push_call("vnum_in_group(%p,%p)",ach,mobvnum);

	for (gch = ach->in_room->first_person ; gch ; gch = gch->next_in_room)
	{
		if (IS_NPC(gch) && gch->pIndexData->vnum == mobvnum && is_same_group(ach, gch))
		{
			pop_call();
			return TRUE;
		}
	}
	pop_call();
	return FALSE;
}

void vt100on (CHAR_DATA * ch)
{
	DESCRIPTOR_DATA *d;
	char buf[MAX_STRING_LENGTH];
	int n1, n2, n3;

	push_call("vt100on(%p)",ch);

	if (!is_desc_valid(ch))
	{
		pop_call();
		return;
	}

	d = ch->desc;

	write_to_port(d);

	if (CH(d)->pcdata->tactical == NULL)
	{
		ALLOCMEM(CH(d)->pcdata->tactical, TACTICAL_MAP, 1);
	}

	write_to_port(d);

	n1 = 1 + CH(d)->pcdata->tactical_mode % 100;

	n2 = CH(d)->pcdata->vt100_type % 100 - 2;

	n3 = CH(d)->pcdata->vt100_type % 100 - 1;

	sprintf(buf, "\033[2J\033[%d;%dr\033[%d;1H", n1, n2, n3);

	write_to_descriptor (d, buf, 0);

	sprintf(buf, "\033[%d;1H\033[2K\033[0m", CH(d)->pcdata->vt100_type % 100);
	write_to_descriptor(d, buf, 0);

	CH(d)->pcdata->vt100 = 1;

	clear_tactical_map(CH(d)->pcdata->tactical);

	vt100prompt(ch);

	write_to_port (d);

	pop_call();
	return;
}

void vt100off (CHAR_DATA * ch)
{
	DESCRIPTOR_DATA *d;
	char buf[MAX_STRING_LENGTH];

	push_call("vt100off(%p)",ch);

	if (!is_desc_valid (ch))
	{
		pop_call();
		return;
	}

	if (CH(ch->desc)->pcdata->vt100 == 0)
	{
		pop_call();
		return;
	}

	d = ch->desc;

	write_to_port (d);

	sprintf(buf, "\033[r\033[0m\033[2J");
	write_to_descriptor (d, buf, 0);
	d->outtop = 0;
	CH(d)->pcdata->vt100 = 0;

	REMOVE_BIT(CH(d)->pcdata->interp, INTERP_TACT_UPDATE);

	if (CH(d)->pcdata->tactical != NULL)
	{
		FREEMEM(CH(d)->pcdata->tactical);
		CH(d)->pcdata->tactical = NULL;
	}
	pop_call();
	return;
}

void vt100prompt(CHAR_DATA * ch)
{
	push_call("vt100prompt(%p)",ch);

	if (is_desc_valid(ch))
	{
		if (CH(ch->desc)->pcdata->vt100 == 3)
		{
			do_refresh(ch, "");
		}
		else if (CH(ch->desc)->pcdata->vt100 == 2)
		{
			vt100prompter(ch);
		}
		else if (CH(ch->desc)->pcdata->vt100 == 1)
		{
			SET_BIT(CH(ch->desc)->pcdata->interp, INTERP_TACT_UPDATE);
		}
	}
	pop_call();
	return;
}

void vt100prompter (CHAR_DATA * ch)
{
	DESCRIPTOR_DATA *d;
	char buf[MAX_STRING_LENGTH];
	TACTICAL_MAP *tact;

	push_call("vt100prompter(%p)",ch);

	tact = NULL;

	if (!is_desc_valid(ch))
	{
		pop_call();
		return;
	}

	d = ch->desc;

	if (CH(d)->pcdata->vt100 == 2)
	{
		vt100on(ch);
	}

	if (CH(d)->pcdata->vt100 == 3)
	{
		do_refresh(ch, "");
		pop_call();
		return;
	}

	if (d->connected != CON_EDITING)
	{
		write_to_buffer (d, "\033[0K", 1000000);
	}

	tact = get_diff_tactical(ch);

	REMOVE_BIT(CH(d)->pcdata->interp, INTERP_TACT_UPDATE);

	if (tact == NULL)
	{
		pop_call();
		return;
	}

	sprintf(buf, "\0337%s\0338%s\033[0K", get_tactical_string(ch, tact), get_color_string(ch, COLOR_PROMPT, VT102_DIM));

	write_to_buffer(d, buf, 1000000);

	pop_call();
	return;
}

void do_port (CHAR_DATA * ch, char *argument)
{
	int size;

	push_call("do_port(%p,%p)",ch,argument);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (ch->desc->original != NULL)
	{
		pop_call();
		return;
	}

	size = atol (argument);

	if (size == 0)
	{
		ch_printf(ch, "Current port settings:\n\rBlock size: %d\n\r", ch->desc->port_size);
		pop_call();
		return;
	}

	if (size < 100 || size > 10000)
	{
		send_to_char ("The range of sizes are 100 to 10000\n\r", ch);
	}

	size = URANGE(100, size, 10000);

	ch->desc->port_size		= size;
	ch->pcdata->port_size	= size;

	ch_printf(ch, "Port size set to %d.\n\r", size);
	pop_call();
	return;
}


void do_voice (CHAR_DATA * ch, char *argument)
{
	char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char speaker[MAX_INPUT_LENGTH];
	char text2[MAX_STRING_LENGTH];
	CHAR_DATA *vch;

	push_call("do_voice(%p,%p)",ch,argument);

	if (learned(ch, gsn_voice) == 0)
	{
		send_to_char("They'd just laugh at you.\n\r", ch);
		pop_call();
		return;
	}

	for (vch = ch->in_room->first_person ; vch != NULL ; vch = vch->next_in_room)
	{
		if (!IS_NPC(vch) && vch->level == MAX_LEVEL && ch->level < MAX_LEVEL && can_see(ch, vch))
		{
			ch_printf(ch, "You may not voice with %s in the room.\n\r", capitalize(get_name(vch)));
			pop_call();
			return;
		}
	}

	argument = one_argument_nolower(argument, speaker);

	if (argument[0] == '\0')
	{
		send_to_char ("What do you want them to say?", ch);
		pop_call();
		return;
	}

	if (is_drunk(ch))
	{
		strcpy(text2, drunkify(argument));
	}
	else
	{
		strcpy(text2, argument);
	}
	sprintf (buf1, "%s says '%s'", capitalize(speaker), text2);
	sprintf (buf2, "Someone say's '%s'", text2);

	for (vch = ch->in_room->first_person ; vch ; vch = vch->next_in_room)
	{
		if (vch->position >= POS_RESTING)
		{
			if (number_percent() > learned(ch, gsn_voice))
			{
				ch_printf(vch, "%s%s\n\r", get_color_string(vch, COLOR_SPEACH, VT102_DIM), justify(buf2, get_page_width(vch)));
			}
			else
			{
				ch_printf(vch, "%s%s\n\r", get_color_string(vch, COLOR_SPEACH, VT102_DIM), justify(buf1, get_page_width(vch)));
			}
		}
	}
	check_improve(ch, gsn_voice);

	pop_call();
	return;
}

char *translate (char *text)
{
	static char text2[MAX_STRING_LENGTH];
	char *pName;
	int iSyl;
	int length;

	struct syl_type
	{
		char *old;
		char *new;
	};

	static const struct syl_type syl_table[] =
	{
		{	" ",		" "		},
		{	"ar",	"ug"		},
		{	"au",	"ja"		},
		{	"le",	"fi"		},
		{	"li",	"ni"		},
		{	"ur",	"ir"		},
		{	"cu",	"je"		},
		{	"de",	"ca"		},
		{	"en",	"un"		},
		{	"li",	"fu"		},
		{	"lo",	"hi"		},
		{	"mo",	"za"		},
		{	"ma",	"do"		},
		{	"ne",	"la"		},
		{	"ni",	"ta"		},
		{	"pe",	"da"		},
		{	"ra",	"ru"		},
		{	"re",	"ca"		},
		{	"so",	"sa"		},
		{	"ec",	"in"		},
		{	"ri",	"lu"		},
		{	"en",	"of"		},
		{	"a",		"i"		},
		{	"b",		"t"		},
		{	"c",		"f"		},
		{	"d",		"p"		},
		{	"e",		"u"		},
		{	"f",		"l"		},
		{	"g",		"j"		},
		{	"h",		"s"		},
		{	"i",		"e"		},
		{	"j",		"n"		},
		{	"k",		"b"		},
		{	"l",		"k"		},
		{	"m",		"g"		},
		{	"n",		"r"		},
		{	"o",		"a"		},
		{	"p",		"y"		},
		{	"q",		"d"		},
		{	"r",		"m"		},
		{	"s",		"h"		},
		{	"t",		"w"		},
		{	"u",		"o"		},
		{	"v",		"x"		},
		{	"w",		"q"		},
		{	"x",		"z"		},
		{	"y",		"c"		},
		{	"z",		"v"		},
		{	"?",		"?"		},
		{	"!",		"!"		},
		{	".",		"."		},
		{	")",		")"		},
		{	"(",		"("		},
		{	":",		":"		},
		{	"'",		"'"		},
		{	"-",		"-"		},
		{	"=",		"="		},
		{	"*",		"*"		},
		{	"%",		"%"		},
		{	",",		","		},
		{	"<",		"<"		},
		{	">",		">"		},
		{	"",		""		}
	};

	push_call("translate(%p)",text);

	text2[0] = '\0';

	for (pName = text ; *pName != '\0' ; pName += length)
	{
		for (iSyl = 0 ; (length = strlen (syl_table[iSyl].old)) != 0 ; iSyl++)
		{
			if (!str_prefix(syl_table[iSyl].old, pName))
			{
				strcat(text2, syl_table[iSyl].new);
				break;
			}
		}
		if (length == 0)
		{
			length = 1;
		}
	}
	pop_call();
	return text2;
}


bool pvnum_in_group (CHAR_DATA * ch, int pvnum)
{
	CHAR_DATA *fch;

	push_call("pvnum_in_group(%p,%p)",ch,pvnum);

	if ((fch = get_char_pvnum(pvnum)) == NULL)
	{
		pop_call();
		return FALSE;
	}
	pop_call();
	return is_same_group(fch, ch);
}


void do_color (CHAR_DATA * ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_INPUT_LENGTH];
	int col1, col2, col3, cnt;

	push_call("do_color(%p,%p)",ch,argument);

	if (IS_NPC (ch) || !is_desc_valid (ch))
	{
		pop_call();
		return;
	}

	if (!strcasecmp ("on", argument))
	{
		if (ch->pcdata->ansi == TRUE)
		{
			send_to_char ("You already have color on.\n\r", ch);
			pop_call();
			return;
		}
		ch->pcdata->ansi = TRUE;
		do_refresh(ch, "");
		ch_printf_color(ch, "{300}Ansi color is ON!\n\r");
		pop_call();
		return;
	}

	if (!strcasecmp ("reset", argument))
	{
		/*
			Scandum's favorite colors
		*/
		reset_color (ch);
		do_refresh(ch, "");
		send_to_char ("Colors reset to defaults.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp ("off", argument))
	{
		ch->pcdata->ansi = FALSE;
		do_refresh(ch, "");
		send_to_char ("Ansi color is OFF.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp ("", argument))
	{
		char bufx[40];
		char text[20];
		sprintf(text, "%s%s", ansi_translate_text(ch, "{088}"), get_color_string (ch, COLOR_TEXT, VT102_DIM));

		for (buf[0] = '\0', cnt = 0 ; cnt < COLOR_MAX ; cnt++)
		{
			switch (cnt)
			{
				case 0:  strcpy (bufx, "Standard Text             "); break;
				case 1:  strcpy (bufx, "Top Status Bar            "); break;
				case 2:  strcpy (bufx, "Bottom Status Bar         "); break;
				case 3:  strcpy (bufx, "Score, Status, and Affects"); break;
				case 4:  strcpy (bufx, "You are hit               "); break;
				case 5:  strcpy (bufx, "You hit others            "); break;
				case 6:  strcpy (bufx, "Prompt line               "); break;
				case 7:  strcpy (bufx, "Exits                     "); break;
				case 8:  strcpy (bufx, "Party member is hit       "); break;
				case 9:  strcpy (bufx, "Speech commands           "); break;
				case 10: strcpy (bufx, "Objects                   "); break;
				case 11: strcpy (bufx, "Living things             "); break;
				case 12: strcpy (bufx, "General Tactical Map      "); break;
				case 13: strcpy (bufx, "Party members in Tactical "); break;
				case 14: strcpy (bufx, "Enemies in Tactical       "); break;
				case 15: strcpy (bufx, "Neutrals in Tactical      "); break;
				case 16: strcpy (bufx, "Chat Channel              "); break;
				case 17: strcpy (bufx, "Talk Channel              "); break;
				case 18: strcpy (bufx, "Plan Channel              "); break;
			}
			cat_sprintf(buf, "%s%2d - %s%s%s  foreground %2d, background %2d\n\r", text, cnt, get_color_string(ch, cnt, VT102_DIM), bufx, text, ch->pcdata->color[cnt] % 10, ch->pcdata->color[cnt] / 10);
		}
		cat_sprintf(buf, "%s\n\r%sSyntax:  COLOR <field number> <forground number> <background number>\n\r", text, text);
		send_to_char(buf, ch);
		pop_call();
		return;
	}

	argument = one_argument (argument, buf);
	argument = one_argument (argument, buf2);
	if (buf[0] < '0' || buf[0] > '9')
	{
		send_to_char ("Try using HELP COLOR.\n\r", ch);
		pop_call();
		return;
	}
	col1 = atol (buf) % COLOR_MAX;
	col2 = atol (buf2) % 10;
	col3 = atol (argument) % 10;
	ch->pcdata->color[col1] = col2 + col3 * 10;
	do_refresh(ch, "");
	ch_printf_color(ch, "{1%d%d}Color field #%d set to color forground %d, background %d.{088}\n\r", ch->pcdata->color[col1] % 10, ch->pcdata->color[col1] / 10, col1, col2, col3);

	pop_call();
	return;
}

void reset_color (CHAR_DATA * ch)
{
	push_call("reset_color(%p)",ch);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	ch->pcdata->color[ 0] = 6 + 8 * 10;
	ch->pcdata->color[ 1] = 7 + 1 * 10;
	ch->pcdata->color[ 2] = 3 + 4 * 10;
	ch->pcdata->color[ 3] = 3 + 8 * 10;
	ch->pcdata->color[ 4] = 1 + 8 * 10;
	ch->pcdata->color[ 5] = 2 + 8 * 10;
	ch->pcdata->color[ 6] = 7 + 8 * 10;
	ch->pcdata->color[ 7] = 5 + 8 * 10;
	ch->pcdata->color[ 8] = 4 + 8 * 10;
	ch->pcdata->color[ 9] = 3 + 8 * 10;
	ch->pcdata->color[10] = 2 + 8 * 10;
	ch->pcdata->color[11] = 5 + 8 * 10;
	ch->pcdata->color[12] = 7 + 4 * 10;
	ch->pcdata->color[13] = 3 + 4 * 10;
	ch->pcdata->color[14] = 1 + 4 * 10;
	ch->pcdata->color[15] = 5 + 4 * 10;
	ch->pcdata->color[16] = 3 + 8 * 10;
	ch->pcdata->color[17] = 5 + 8 * 10;
	ch->pcdata->color[18] = 7 + 8 * 10;

	pop_call();
	return;
}

void do_repeat (CHAR_DATA * ch, char *argument)
{
	char buf1[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];
	int amount, cnt;
	char bbuf[MAX_INPUT_LENGTH], *bptr;

	push_call("do_repeat(%p,%p)",ch,argument);

	if (ch->desc == NULL)
	{
		pop_call();
		return;
	}

	argument = one_argument(argument, buf1);

	amount = atol(buf1);

	if (amount < 2 || amount > 99 || isdigit(argument[0]))
	{
		send_to_char ("Syntax: <amount> <command string>\n\r", ch);
		send_to_char ("Amount range is 2 to 99, or whatever the input line can hold.\n\r", ch);
		pop_call();
		return;
	}

	if (argument[0] == '.' || argument[0] == '!')
	{
		send_to_char("That would be a bad idea.\n\r", ch);
		pop_call();
		return;
	}

	if ((bptr = strstr(argument, "&")) != NULL)
	{
		sprintf(bbuf, "%s\r", bptr+1);
		*bptr = '\0';
	}
	else
	{
		strcpy(bbuf, ch->desc->inbuf);
	}
	sprintf(buf2, "%s\r", argument);

	*ch->desc->inbuf = 0;
	 ch->desc->intop = 0;

	for (cnt = 0 ; cnt < amount ; cnt++)
	{
		if (ch->desc->intop >= MAX_INPUT_LENGTH - 6)
		{
			break;
		}
		ch->desc->intop = str_apd_max(ch->desc->inbuf, buf2, ch->desc->intop, MAX_INPUT_LENGTH - 5);
	}
	ch->desc->intop = str_apd_max(ch->desc->inbuf, bbuf, ch->desc->intop, MAX_INPUT_LENGTH - 6);

	pop_call();
	return;
}

void do_request (CHAR_DATA * ch, char *argument)
{
	push_call("do_request(%p,%p)",ch,argument);

	if (!is_desc_valid (ch) || IS_NPC (ch))
	{
		pop_call();
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: request < c+ | c- | v+ | v- >\n\r", ch);
	}
	else if (!str_prefix(argument, "v+") && IS_SET(ch->pcdata->request, REQUEST_VT_SAVE_ON))
	{
		if (ch->pcdata->vt100 == 0)
		{
			REMOVE_BIT(ch->pcdata->request, REQUEST_VT_SAVE_ON);
			vt100on (ch);
		}
	}
	else if (!str_prefix(argument, "v-") && !IS_SET(ch->pcdata->request, REQUEST_VT_SAVE_ON))
	{
		if (ch->pcdata->vt100 == 1)
		{
			SET_BIT (ch->pcdata->request, REQUEST_VT_SAVE_ON);
			vt100off (ch);
		}
	}
	else if (!str_prefix(argument, "c+") && IS_SET(ch->pcdata->request, REQUEST_ANSI_SAVE_ON))
	{
		if (ch->pcdata->ansi == FALSE)
		{
			REMOVE_BIT(ch->pcdata->request, REQUEST_ANSI_SAVE_ON);
			ch->pcdata->ansi = TRUE;
		}
	}
	else if (!str_prefix(argument, "c-") && !IS_SET(ch->pcdata->request, REQUEST_ANSI_SAVE_ON))
	{
		if (ch->pcdata->ansi == TRUE)
		{
			SET_BIT(ch->pcdata->request, REQUEST_ANSI_SAVE_ON);
			ch->pcdata->ansi = FALSE;
		}
	}
	pop_call();
	return;
}

void do_block (CHAR_DATA * ch, char *argument)
{
	char buf1[MAX_STRING_LENGTH];

	push_call("do_block(%p,%p)",ch,argument);

	if (!is_desc_valid (ch) || IS_NPC (ch))
	{
		pop_call();
		return;
	}


	argument = one_argument(argument, buf1);

	if (buf1[0] == '\0')
	{
		ch_printf(ch, "Your block-list is:\n\r > %s\n\r", ch->pcdata->block_list);
		pop_call();
		return;
	}
	smash_tilde (buf1);

	if (!strcasecmp ("clear", buf1))
	{
		STRFREE(ch->pcdata->block_list);
		ch->pcdata->block_list = STRALLOC("");
		send_to_char("Your block-list is now empty.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp ("del", buf1))
	{
		char *block_list,
			block_names	[MAX_INPUT_LENGTH],
			name_to_delete	[MAX_INPUT_LENGTH],
			cur_name		[MAX_INPUT_LENGTH],
			new_block		[MAX_INPUT_LENGTH];

		argument = one_argument(argument, name_to_delete);

		strcpy (block_names, ch->pcdata->block_list);

		block_list = &(block_names[0]);
		block_list = one_argument(block_list, cur_name);

		for (new_block[0] = '\0' ; cur_name[0] != '\0' ; block_list = one_argument(block_list, cur_name))
		{
			if (!strcasecmp (cur_name, name_to_delete))
			{
				strcat(new_block, block_list);
				STRFREE(ch->pcdata->block_list);
				ch->pcdata->block_list = STRALLOC (new_block);

				ch_printf(ch, "Deleted '%s' from your block-list.\n\r", name_to_delete);
				ch_printf(ch, "Your block-list is now:\n\r > %s\n\r", ch->pcdata->block_list);
				pop_call();
				return;
			}
			else
			{
				cat_sprintf(new_block, "%s ", cur_name);
			}
		}
		ch_printf(ch, "I was unable to find '%s' in your block-list.\n\r", name_to_delete);
		pop_call();
		return;
	}

	if (!strcasecmp ("add", buf1))
	{
		if (strlen(ch->pcdata->block_list) > 150)
		{
			send_to_char ("Your block-list is too long to add a new name.\n\r", ch);
			pop_call();
			return;
		}
		sprintf(buf1, "%s %s", ch->pcdata->block_list, argument);

		STRFREE (ch->pcdata->block_list);
		ch->pcdata->block_list = STRALLOC(buf1);

		ch_printf(ch, "Your block-list is now:\n\r > %s\n\r", ch->pcdata->block_list);
		pop_call();
		return;
	}
	send_to_char ("Incorrect blocking format.\n\rUse: block [<add/clear/del>] [namelist]\n\r", ch);
	pop_call();
	return;
}

void do_death (CHAR_DATA * ch, char *arg)
{
	push_call("do_death(%p,%p)",ch,arg);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_ALTAR))
	{
		ch->pcdata->death_room = ch->in_room->vnum;
		send_to_char ("Death room set.\n\r", ch);
		pop_call();
		return;
	}
	ch_printf(ch, "You may not set your death room here.\n\rYour death room is currently set at: %s\n\r", room_index[ch->pcdata->death_room]->name);

	pop_call();
	return;
}

/*
	Returns centered buffer - Scandum
*/

char *center (char *inp, int length)
{
	static char center_buf[MAX_STRING_LENGTH];
	int lead_cnt, cnt;

	push_call("center(%p,%p)",inp,length);

	lead_cnt = (length - strlen(inp)) / 2;

	for (center_buf[0] = cnt = 0 ; cnt < lead_cnt ; cnt++)
	{
		strcat(center_buf, " ");
	}

	strcat(center_buf, inp);

	pop_call();
	return center_buf;
}

/*
	Returns justified buffer for ansi translated text - Scandum 13-05-2002
*/

char *ansi_justify (char *inp, int length)
{
	static char justified_buf[MAX_STRING_LENGTH];
	char *pti, *pto, *last_i_space, *last_o_space;
	int cnt, skp;

	push_call("ansi_justify(%p)",inp);

	last_o_space = pto = (char *) justified_buf;
	last_i_space = pti = inp;
	cnt = skp = 0;

	while (TRUE)
	{
		*pto = *pti;
		switch (*pto)
		{
			case '\0':
				pop_call();
				return ((char *) justified_buf);
				break;
			case ' ':
				last_o_space = pto;
				last_i_space = pti;
				break;
			case '\n':
			case '\r':
				cnt = 0;
				skp = 0;
				pto++;
				pti++;
				continue;
				break;
			case '{':
				if (*(pti+1) >= '0'	&& *(pti+1) <= '8'
				&&  *(pti+2) >= '0' && *(pti+2) <= '8'
				&&  *(pti+3) >= '0' && *(pti+3) <= '8'
				&&  *(pti+4) == '}')
				{
					pto++; pti++; *pto = *pti;
					pto++; pti++; *pto = *pti;
					pto++; pti++; *pto = *pti;
					pto++; pti++; *pto = *pti;
					pto++;
					pti++;
					skp += 5;
					continue;
				}
				break;
		}
		pto++;
		pti++;
		cnt++;
		if (cnt >= length)
		{
			if (*pti == '\n' || *pti == '\0')
			{
				continue;
			}
			if (*pti == ' ')
			{
				last_o_space = pto;
				last_i_space = pti;
			}
			if (pto - last_o_space > 20 + skp)
			{
				*pto = '\n';
				pto++;
				*pto = '\r';
				pto++;
				cnt  = 0;
				skp  = 0;
			}
			else
			{
				pto  = last_o_space;
				*pto = '\n';
				pto++;
				*pto = '\r';
				pto++;
				pti  = last_i_space;
				pti++;
				cnt  = 0;
				skp  = 0;
			}
		}
	}
	pop_call();
	return ((char *)justified_buf);
}

/*
	Switched to flawless protocol - Scandum 13-05-2002
*/

char *justify (char *inp, int length)
{
	static char justified_buf[MAX_STRING_LENGTH];
	char *pti, *pto, *last_i_space, *last_o_space;
	int cnt;

	push_call("justify(%p)",inp);

	last_o_space = pto = (char *) justified_buf;
	last_i_space = pti = inp;
	cnt = 0;

	while (TRUE)
	{
		*pto = *pti;
		switch (*pto)
		{
			case '\0':
				pop_call();
				return ((char *) justified_buf);
				break;
			case ' ':
				last_o_space = pto;
				last_i_space = pti;
				break;
			case '\n':
			case '\r':
				cnt = 0;
				pto++;
				pti++;
				continue;
		}
		pto++;
		pti++;
		cnt++;
		if (cnt >= length)
		{
			if (*pti == '\n' || *pti == '\0')
			{
				continue;
			}
			if (*pti == ' ')
			{
				last_o_space = pto;
				last_i_space = pti;
			}
			if (pto - last_o_space > 20)
			{
				*pto = '\n';
				pto++;
				*pto = '\r';
				pto++;
				cnt  = 0;
			}
			else
			{
				pto  = last_o_space;
				*pto = '\n';
				pto++;
				*pto = '\r';
				pto++;
				pti  = last_i_space;
				pti++;
				cnt  = 0;
			}
		}
	}
	pop_call();
	return ((char *)justified_buf);
}

bool can_understand (CHAR_DATA * victim, CHAR_DATA * ch, bool fDisplay)
{
	int language, speak;

	push_call("can_understand(%p,%p)",victim,ch);

	if (IS_NPC(victim) && !IS_SET(victim->act, ACT_RACE))
	{
		pop_call();
		return TRUE;
	}

	if (IS_NPC(ch) && !IS_SET(ch->act, ACT_RACE))
	{
		pop_call();
		return TRUE;
	}

	if (IS_AFFECTED(ch, AFF_TONGUES) || IS_AFFECTED(victim, AFF_UNDERSTAND))
	{
		pop_call();
		return TRUE;
	}

	if (ch->level >= 90 || victim->level >= 90)
	{
		pop_call();
		return TRUE;
	}

	language	= IS_NPC(victim) ? SHIFT(victim->race) : victim->pcdata->language;
	speak	= IS_NPC(ch) ? SHIFT(ch->race) : ch->pcdata->speak;

	if (!IS_SET(language, speak))
	{
		if (fDisplay)
		{
			ch_printf(ch, "%s does not understand %s.\n\r", capitalize(get_name(victim)), race_table[UNSHIFT(speak)].race_name);
		}
		pop_call();
		return FALSE;
	}
	pop_call();
	return TRUE;
}


void do_tactical (CHAR_DATA * ch, char *arg)
{
	int val;
	int max;
	char buf[MAX_STRING_LENGTH];

	push_call("do_tactical(%p,%p)",ch,arg);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "dump"))
	{
		int cnt, size_v, size_h, old1, old2, old3, new1, new2, new3;

		old1 = old2 = old3 = -1;

		get_tactical_map(ch);

		size_v = ch->pcdata->tactical_mode % 100 + 1;
		size_h = ch->pcdata->vt100_type    / 100;

		for (buf[0] = '\0', cnt = 0 ; cnt < size_v * size_h ; cnt++)
		{
			new1 = mud->tactical->color[cnt] >= 128 ? 1 : mud->tactical->map[cnt] / 128;
			new2 = mud->tactical->color[cnt] % 8;
			new3 = (mud->tactical->color[cnt] % 128) / 8;

			cat_sprintf(buf, "%s%c",	get_color_diff(ch, old2, old3, old1, new2, new3, new1), mud->tactical->map[cnt] % 128);

			if ((cnt + 1) % size_h == 0)
			{
				old1 = old2 = old3 = -1;
				strcat(buf, "\033[0m\n\r");
				if (cnt + 1 == size_h * (ch->pcdata->tactical_mode % 100))
				{
					strcat(buf, "\n\r");
				}
			}
			else
			{
				old1 = new1;
				old2 = new2;
				old3 = new3;
			}
		}
		if (IS_SET(ch->act, PLR_WIZTIME))
		{
			cat_sprintf(buf, "Size: %d bytes.\n\r", strlen(buf));
		}
		ch_printf(ch, "%s", buf);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "top"))
	{
		if ((ch->pcdata->tactical_mode / 1000) % 10 == 0)
		{
			send_to_char ("Your tactical is on top.\n\r", ch);
			pop_call();
			return;
		}
		ch->pcdata->tactical_mode -= 1000;
		send_to_char ("Your tactical is set to the top.\n\r", ch);
		pop_call();
		return;
	}

	if (!strcasecmp (arg, "bottom"))
	{
		if ((ch->pcdata->tactical_mode / 1000) % 10 == 1)
		{
			send_to_char ("Your tactical is on bottom.\n\r", ch);
			pop_call();
			return;
		}
		ch->pcdata->tactical_mode += 1000;
		send_to_char ("Your tactical is set to the bottom.\n\r", ch);
		pop_call();
		return;
	}

	if (arg[0] == 'n' || arg[0] == 'N')
	{
		if ((ch->pcdata->tactical_mode / 100) % 10 == 0)
		{
			send_to_char ("You turn the tactical numbers off.\n\r", ch);
			ch->pcdata->tactical_mode += 100;
		}
		else
		{
			send_to_char ("You turn the tactical numbers on.\n\r", ch);
			ch->pcdata->tactical_mode -= 100;
		}
		pop_call();
		return;
	}

	if (arg[0] == 'i' || arg[0] == 'I')
	{
		char bufs[10];

		arg = one_argument (arg, buf);

		if (arg[0] <= ' ' || arg[0] > 'z' || arg[1] <= ' ' || arg[1] > 'z')
		{
			send_to_char ("You must specify a two character string for an index.\n\r", ch);
			pop_call();
			return;
		}

		if (arg[0] == '~')
		{
			arg[0] = '-';
		}

		if (arg[1] == '~')
		{
			arg[1] = '-';
		}

		bufs[0] = arg[0];
		bufs[1] = arg[1];
		bufs[2] = '\0';

		RESTRING(ch->pcdata->tactical_index, bufs);

		send_to_char ("Index set.\n\r", ch);

		pop_call();
		return;
	}

	if (arg[0] == 'c' || arg[0] == 'C')
	{
		arg = one_argument (arg, buf);

		if (arg[0] == '\0')
		{
			char buf1[MAX_STRING_LENGTH];
			char buf_sect[20];
			char col_tact[10];
			char col_tops[10];
			int cnt, col;

			strcpy(col_tact, get_color_code(ch, COLOR_TACTICAL, VT102_DIM));
			strcpy(col_tops, get_color_code(ch, COLOR_TOP_STAT, VT102_DIM));

			sprintf(buf1, "%s%s\n\r", col_tops, "Sector Types and their Compass colors.                                          {088}");

			for (cnt = 0 ; cnt < SECT_MAX ; cnt++)
			{
				col = sector_table[cnt].color;

				sprintf(buf_sect, "%-14s", capitalize(sector_table[cnt].sector_name));

				cat_sprintf(buf1, "%s {%d%d%d}%s%s ", col_tact, col / 128, col % 8, (col % 128) / 8, buf_sect, col_tact);
				if ((cnt+1) % 5 == 0)
				{
					strcat(buf1, "{088}\n\r");
				}
			}
			while (cnt % 5 != 0)
			{
				strcat(buf1, "                ");
				cnt++;
				if (cnt % 5 == 0)
				{
					strcat(buf1, "{088}\n\r");
				}
			}
			cat_sprintf(buf1, "%s                                                                                {088}\n\r", col_tops);
			send_to_char_color(buf1, ch);

			pop_call();
			return;
		}

		val = atol (arg);
		if (val == 0)
		{
			send_to_char ("The tactical compass is turned off.\n\r", ch);
			ch->pcdata->compass_width = 0;
			pop_call();
			return;
		}

		if (val > 15 || val < 5)
		{
			send_to_char ("The tactical compass size must be from 5 to 15, or 0 to turn off.\n\r", ch);
			pop_call();
			return;
		}

		ch->pcdata->compass_width = val;
		send_to_char ("The tactical compass size is set.\n\r", ch);
		pop_call();
		return;
	}

	max = URANGE(1, (ch->pcdata->vt100_type % 100) / 2, MAX_TACTICAL_ROW - 1);

	val = atol (arg);

	if (val < 1 || val > max)
	{
		ch_printf(ch, "The tactical size must be from 1 to %d.\n\r", max);
		pop_call();
		return;
	}

	ch->pcdata->tactical_mode = ch->pcdata->tactical_mode - ch->pcdata->tactical_mode % 100 + val;
	do_refresh (ch, "");
	pop_call();
	return;
}

void clear_tactical_map (TACTICAL_MAP *tact)
{
	static const TACTICAL_MAP clear_tact;

	*tact = clear_tact;
/*
	memset(tact->map,   0, MAX_TACTICAL_ROW * MAX_TACTICAL_COL);
	memset(tact->color, 0, MAX_TACTICAL_ROW * MAX_TACTICAL_COL);
*/
	return;
}

/*
	Chaos - 3/1/95
*/

char *get_tactical_string (CHAR_DATA * ch, TACTICAL_MAP * tact)
{
	DESCRIPTOR_DATA *d;

	static char buf[MAX_STRING_LENGTH];

	int cnt, lcnt;
	int new_bold, old_bold;
	int old_for,  old_bak;		/* last color selected */
	int new_for,  new_bak;
	int prev_h,   prev_v;		/* last known change in text */
	int size_h,   size_v;

	int ccnt, stbar;
	unsigned char *tm, *tc, *cm, *cc;	/* Pointers to tacticals */

	push_call("get_tactical_string(%p,%p)",ch,tact);

	d = ch->desc;

	if (!is_desc_valid(ch))
	{
		pop_call();
		return NULL;
	}

	prev_h   = -10;
	prev_v   = -10;
	old_bold = -1;
	old_for  = -1;
	old_bak  = -1;

	buf[0]   = '\0';

	stbar  = CH(d)->pcdata->vt100_type % 100 - 1;

	size_v = CH(d)->pcdata->tactical_mode % 100 + 1;
	size_h = CH(d)->pcdata->vt100_type    / 100;

	tm = (unsigned char *) tact->map;
	tc = (unsigned char *) tact->color;

	cm = (unsigned char *) CH(d)->pcdata->tactical->map;
	cc = (unsigned char *) CH(d)->pcdata->tactical->color;

	for (lcnt = 0 ; lcnt < size_v ; lcnt++)
	{
		for (cnt = 0 ; cnt < size_h ; cnt++, tm++, tc++, cm++, cc++)
		{
			if (*tm != *cm || *tc != *cc)
			{
				if (IS_SET(CH(d)->pcdata->vt100_flags, VT100_BOLD))
				{
					if (*tc >= 128 || *tm >= 128)
					{
						new_bold = 1;
					}
					else if (IS_SET(CH(d)->pcdata->vt100_flags, VT100_ECMA48))
					{
						new_bold = 22;
					}
					else
					{
						new_bold = 0;
					}
				}
				else
				{
					new_bold = 0;
				}

				if (CH(d)->pcdata->ansi == FALSE)
				{
					new_for = 0;
					new_bak = 1;
				}
				else
				{
					new_for = *tc % 8;
					new_bak = (*tc % 128) / 8;
				}
				/*
					Get color shifting
				*/

				strcat(buf, get_color_diff(ch, old_for, old_bak, old_bold, new_for, new_bak, new_bold));

				if (lcnt == CH(d)->pcdata->tactical_mode % 100)
				{
					ccnt = stbar - 1;
				}
				else
				{
					ccnt = lcnt;
				}

				if (prev_v == ccnt + 1 && prev_h == cnt)
				{
					cat_sprintf(buf, "%c", *tm % 128);
				}
				else
				{
					/*
						Check for slow VT102 processing
					*/

					if (!IS_SET(CH(d)->pcdata->vt100_flags, VT102_FAST))
					{
						cat_sprintf(buf, "\033[%d;%dH%c", ccnt + 1, cnt + 1, *tm % 128);
					}
					else
					{
						if (ccnt + 1 == prev_v)	/* Same row optimize */
						{
							cat_sprintf(buf, "\033[%dC%c", cnt - prev_h, *tm % 128);
						}
						else	/* Direct cursor positioning */
						{
							cat_sprintf(buf, "\033[%d;%dH%c", ccnt + 1, cnt + 1, *tm % 128);
						}
					}
				}
				prev_v = ccnt + 1;
				prev_h = cnt + 1;
				old_for = new_for;
				old_bak = new_bak;
				old_bold = new_bold;
			}
		}
	}

	/* Update the new map for player */

	memcpy(CH(d)->pcdata->tactical->map,	tact->map,	lcnt * size_h);
	memcpy(CH(d)->pcdata->tactical->color,	tact->color,	lcnt * size_h);

	pop_call();
	return(buf);
}

TACTICAL_MAP *get_tactical_map (CHAR_DATA * ch)
{
	register int val, cnt, lcnt, size_v, size_h, cuc, door;
	bool *pti, *pto;
	bool *ptoc;
	AFFECT_DATA *paf;
	PC_DATA *pcd;			/* Shortcutt one reference */
	char buf[MAX_STRING_LENGTH], buf2[MAX_INPUT_LENGTH], buf3[MAX_INPUT_LENGTH];
	bool *ptb;
	bool color1, color2, color3;
	int hr, mn, col, row, bld, colors, max_width, offset, vo, hour, cw;
	struct tm clk;
	bool *tm, *tc;
	EXIT_DATA *pexit;
	bool found;
	CHAR_DATA *fch;
	bool cval;

	push_call("get_tactical_map(%p)",ch);

	pcd  = CH(ch->desc)->pcdata;

	clk = *localtime (&mud->current_time);

	clear_tactical_map(mud->tactical);

	tm = (unsigned char *) mud->tactical->map;
	tc = (unsigned char *) mud->tactical->color;

	color1 = pcd->color[COLOR_TOP_STAT] % 10 + pcd->color[COLOR_TOP_STAT] / 10 * 8;
	color2 = pcd->color[COLOR_TACTICAL] % 10 + pcd->color[COLOR_TACTICAL] / 10 * 8;
	color3 = pcd->color[COLOR_BOT_STAT] % 10 + pcd->color[COLOR_BOT_STAT] / 10 * 8;

	val = pcd->tactical_mode;

	size_v = val % 100 + 1;
	size_h = pcd->vt100_type / 100;

	for (lcnt = 0 ; lcnt < size_v ; lcnt++)
	{
		for (cnt = 0 ; cnt < size_h ; cnt++, tm++, tc++)
		{
			*tm = 128 + ' ';

			if ((val / 1000 % 10 == 0 && lcnt == 0)
			||  (val / 1000 % 10 != 0 && lcnt == val-1))
			{
				*tc = color1;
			}
			else if (lcnt < val % 100)
			{
				*tc = color2;
			}
			else
			{
				*tc = color3;
			}
		}
	}

	/* Let's use these as a index pointer for speed */

	tm = (unsigned char *) mud->tactical->map;
	tc = (unsigned char *) mud->tactical->color;

	if ((val / 1000) % 10 == 0)
	{
		pto = tm;
	}
	else
	{
		pto = tm + size_h * (size_v - 2);
	}

	if (!IS_NPC(ch))
	{
		for (cnt = 0, pti = (bool *) ch->name ; *pti != '\0' ; pti++, pto++, cnt++)
		{
			*pto = *pti;
		}
	}
	else
	{
		for (cnt = 0, pti = (bool *) ch->short_descr ; *pti != '\0' && cnt < 73 ; pti++, pto++, cnt++)
		{
			*pto = *pti;
		}
	}

	for ( ; cnt < size_h ; cnt++, pto++)
	{
		*pto = ' ';
	}

	pto--;
	*pto = ch->level % 10 + 128 + '0';
	if (ch->level >= 10)
	{
		pto--;
		*pto = ch->level / 10 + 128 + '0';
	}
	pto--;
	*pto = ':';
	pto--;
	*pto = 'v';
	pto--;
	*pto = 'e';
	pto--;
	*pto = 'L';
	pto--;
	*pto = ' ';

	memset(buf, 0, 5);
	sprintf(buf, "%d", ch->alignment);

	for (cnt = 4 ; cnt >= 0 ; cnt--)
	{
		if (buf[cnt] != '\0')
		{
			pto--;
			*pto = 128 + buf[cnt];
		}
	}

	pto--;	*pto = ':';
	pto--;	*pto = 'l';
	pto--;	*pto = 'A';
	pto--;	*pto = ' ';

	switch (ch->speed)
	{
		case 0:	pto--;	*pto = 128 + 'W'; break;
		case 1:	pto--;	*pto = 128 + 'N'; break;
		case 2:	pto--;	*pto = 128 + 'J'; break;
		case 3:	pto--;	*pto = 128 + 'R'; break;
		case 4:	pto--;	*pto = 128 + 'H'; break;
		case 5:	pto--;	*pto = 128 + 'B'; break;
	}

	pto--;	*pto = ' ';

	col = 0;

	if (!IS_NPC(ch))
	{
		if (IS_SET (ch->act, PLR_HOLYLIGHT))
		{
			pto--;	*pto = 'L' + 128;
			pto--;	*pto = 'H' + 128;
		}
		if (IS_SET (ch->act, PLR_WIZINVIS))
		{
			pto--;	*pto = 'I' + 128;
			pto--;	*pto = 'W' + 128;
		}
		if (IS_SET (ch->act, PLR_WIZCLOAK))
		{
			pto--;	*pto = 'C' + 128;
			pto--;	*pto = 'W' + 128;
		}
	}

	if (ch->first_affect != NULL)
	{
		for (cnt = 0, paf = ch->first_affect ; paf && cnt < (size_h - 40) / 2 ; paf = paf->next, cnt++)
		{
			pto--;	*pto = skill_table[paf->type].name[1] + 128;
			pto--;	*pto = skill_table[paf->type].name[0] + 128 - 32;
		}
	}

	if (ch->first_affect == NULL
	&& !IS_SET(CH(ch->desc)->act, PLR_WIZINVIS)
	&& !IS_SET(CH(ch->desc)->act, PLR_HOLYLIGHT)
	&& !IS_SET(CH(ch->desc)->act, PLR_WIZCLOAK))
	{
		pto--;	*pto = 128 + 'e';
		pto--;	*pto = 128 + 'n';
		pto--;	*pto = 128 + 'o';
		pto--;	*pto = 128 + 'N';
	}
	pto--;	*pto = ':';
	pto--;	*pto = 'X';
	pto--;	*pto = 'F';
	pto--;	*pto = ' ';

	pto--; *pto = 128 + race_table[UNSHIFT(CH(ch->desc)->pcdata->speak)].race_name[1];
	pto--; *pto = 128 + race_table[UNSHIFT(CH(ch->desc)->pcdata->speak)].race_name[0];
	pto--;

	*pto = ':';		pto--;
	*pto = 'p';		pto--;
	*pto = 'S';		pto--;
	*pto = ' ';		pto--;

	{
		hour = mud->time_info->hour % 12;

		*pto = 'm' + 128;
		if (mud->time_info->hour >= 12)
		{
			pto--;	*pto = 'p' + 128;
		}
		else
		{
			pto--;	*pto = 'a' + 128;
		}

		if (hour == 0)
		{
			hour = 12;
		}
		pto--;	*pto = '0' + (hour % 10) + 128;
		if (hour >= 10)
		{
			pto--;	*pto = '1' + 128;
		}
		pto--;	*pto = ' ';
	}

	/*
		Add the bottom stat bar
	*/

	pto	= tm + (size_h * (size_v - 1));
	ptoc = tc + (size_h * (size_v - 1));

	cuc = color3;

	if (!IS_NPC(ch) && IS_IMMORTAL(ch) && ch->pcdata->editmode != MODE_NONE && pcd->subprompt && pcd->subprompt[0] != '\0')
	{
		sprintf(buf, "%-61s", ch->pcdata->subprompt);
		for (ptb = (bool *) buf ; *ptb != '\0' ; ptb++)
		{
			*pto = *ptb + 128;
			pto++;
			*ptoc = cuc;
			ptoc++;
		}
		for (cnt = 61 ; cnt < size_h - 19 ; cnt++)
		{
			*pto = 128 + ' ';	pto++;
		}
	}
	else
	{
		*pto		= 128 + ' ';	pto++;
		*ptoc	= cuc;		ptoc++;
		*pto		= 'H';		pto++;
		*ptoc	= cuc;		ptoc++;
		*pto		= ':';		pto++;
		*ptoc	= cuc;		ptoc++;

		val = UMAX(0, ch->hit);

		switch (val * 10 / UMAX(1, ch->max_hit) / 4)
		{
			case 0:	cuc = 1 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
			case 1:	cuc = 3 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
			case 2:	cuc = 2 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
		}

		sprintf(buf, "%5d", ch->hit);

		*pto		= 128 + buf[0];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[1];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[2];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[3];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[4];	pto++;
		*ptoc	= cuc;			ptoc++;

		cuc = color3;

		sprintf(buf, "/%-5d", ch->max_hit);

		*pto		=   0 + buf[0];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[1];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[2];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[3];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[4];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[5];	pto++;
		*ptoc	= cuc;			ptoc++;

		*pto		= 128 + ' ';		pto++;
		*ptoc	= cuc;			ptoc++;

		*pto		= 'M';			pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 'n';			pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= ':';			pto++;
		*ptoc	= cuc;			ptoc++;

		val = UMAX(0, ch->mana);

		switch (val * 10 / UMAX(1, ch->max_mana) / 4)
		{
			case 0:	cuc = 1 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
			case 1:	cuc = 3 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
			case 2:	cuc = 2 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
		}

		sprintf(buf, "%-5d", ch->mana);

		*pto		= 128 + buf[0];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[1];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[2];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[3];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[4];	pto++;
		*ptoc	= cuc;			ptoc++;

		cuc		= color3;

		*pto		= 128 + ' ';	pto++;
		*ptoc	= cuc;		ptoc++;
		*pto		= 'M';		pto++;
		*ptoc	= cuc;		ptoc++;
		*pto		= 'v';		pto++;
		*ptoc	= cuc;		ptoc++;
		*pto		= ':';		pto++;
		*ptoc	= cuc;		ptoc++;

		val = UMAX(0, ch->move);

		switch (val * 10 / UMAX(1, ch->max_move) / 2)
		{
			case 0:	case 1:	cuc = 1 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
			case 2:	case 3:	cuc = 3 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
			case 4:	case 5:	cuc = 2 + pcd->color[COLOR_BOT_STAT] / 10 * 8;	break;
		}

		sprintf(buf, "%-5d", ch->move);

		*pto		= 128 + buf[0];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[1];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[2];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[3];	pto++;
		*ptoc	= cuc;			ptoc++;
		*pto		= 128 + buf[4];	pto++;
		*ptoc	= cuc;			ptoc++;

		for (cnt = 32 ; cnt < size_h - 48 ; cnt++)
		{
			*pto	= 128 + ' ';	pto++;	ptoc++;
		}

		*pto		= 128 + ' ';	pto++;

		cuc = color3;

		if (IS_SET(CH(ch->desc)->act, PLR_EXP_TO_LEVEL))
		{
			*pto		= 'e';	pto++;
			*pto		= 'X';	pto++;
		}
		else
		{
			*pto		= 'E';	pto++;
			*pto		= 'x';	pto++;
		}
		*pto		= ':';	pto++;

		if (IS_SET(CH(ch->desc)->act, PLR_EXP_TO_LEVEL))
		{
			if (ch->level >= 95)
			{
				strcpy (buf, " max      ");
			}
			else
			{
				sprintf (buf, "%-10d", exp_level(ch->class, ch->level) - (IS_NPC(ch) ? 0 : ch->pcdata->exp));
			}
		}
		else
		{
			sprintf (buf, "%-10d", IS_NPC(ch) ? 0 : ch->pcdata->exp);
		}

		for (ptb = (bool *) buf ; *ptb != '\0' ; ptb++)
		{
			*pto		= *ptb + 128;	pto++;
		}
		*pto = 'G';	pto++;
		*pto = 'd';	pto++;
		*pto = ':';	pto++;

		sprintf (buf, "%-10d", ch->gold);
		for (ptb = (bool *) buf ; *ptb != '\0' ; ptb++)
		{
			*pto = *ptb + 128;	pto++;
		}
	}
	*pto = 'E';	pto++;
	*pto = 'x';	pto++;
	*pto = 'i';	pto++;
	*pto = 't';	pto++;
	*pto = ':';	pto++;
	*pto = ' ';	pto++;

	cnt = 0;

	if (can_see_in_room(ch, ch->in_room))
	{
		for (door = 0; door < 6; door++)
		{
			if ((pexit = get_exit(ch->in_room->vnum, door)) != NULL
			&&   !IS_SET(pexit->exit_info, EX_CLOSED)
			&&  (!IS_SET(ch->in_room->room_flags, ROOM_SMOKE)	|| can_see_smoke(ch))
			&&  (!IS_SET(pexit->exit_info, EX_HIDDEN) || can_see_hidden(ch))
			&&   can_use_exit(ch, pexit))
			{
				cnt++;
				switch (door)
				{
					case 0:	*pto = 'N' + 128;	pto++;	break;
					case 1:	*pto = 'E' + 128;	pto++;	break;
					case 2:	*pto = 'S' + 128;	pto++;	break;
					case 3:	*pto = 'W' + 128;	pto++;	break;
					case 4:	*pto = 'U' + 128;	pto++;	break;
					case 5:	*pto = 'D' + 128;	pto++;	break;
				}
			}
		}
	}
	for ( ; cnt < 7 ; cnt++)
	{
		*pto = 128 + ' ';	pto++;
	}

	ptoc += 40;	*ptoc = cuc - 2;

	if (ch->wait != 0)
	{
		*pto		= '*' + 128;	pto++;
	}
	else
	{
		*pto 	= ' ' + 128;	pto++;
	}

	mn = clk.tm_min  + pcd->clock / 10000;
	hr = clk.tm_hour + pcd->clock % 100 + mn / 60;
	mn = mn % 60;
	hr = hr % 24;

	if (pcd->clock % 10000 / 1000 == 1)
	{
		hr = URANGE(0, (mud->current_time - pcd->last_connect) / 3600, 999);

		sprintf(buf, "%3d", hr);

		buf[3] = ':';
		buf[4] = '0' + ((mud->current_time - pcd->last_connect) % 3600) / 600;
		buf[5] = '0' + ((mud->current_time - pcd->last_connect) % 600) / 60;
		buf[6] = ' ';
		buf[7] = '\0';
	}
	else if (pcd->clock % 1000 / 100 == 1)
	{
		buf[0] = (hr / 10) + '0';
		buf[1] = (hr % 10) + '0';
		buf[2] = ':';
		buf[3] = (mn / 10) + '0';
		buf[4] = (mn % 10) + '0';
		buf[5] = ' ';
		buf[6] = ' ';
		buf[7] = '\0';
	}
	else
	{
		buf[0] = hr == 12 ? '1' : hr % 12 >= 10 ? '1' : ' ';
		buf[1] = hr == 12 ? '2' : '0' + hr % 12 % 10;
		buf[2] = ':';
		buf[3] = '0' + (mn / 10);
		buf[4] = '0' + (mn % 10);
		buf[5] = hr >= 12 ? 'p' : 'a';
		buf[6] = 'm';
		buf[7] = '\0';
	}

	for (ptb = (bool *) buf ; *ptb != '\0' ; ptb++)
	{
		*pto = *ptb + 128;	pto++;
	}

	/* Do the room mobiles  */

	if (size_v - 1 > 1)
	{
		colors = 7;
		cw = pcd->compass_width;

		if (cw == 0)
			max_width = size_h - 17;
		else
			max_width = size_h - 17 - 3 - 3 * cw;

		col = 2;
		row = 1;

		if (can_see_in_room(ch, ch->in_room) && ch->position > POS_SLEEPING)
		{
			for (fch = ch->in_room->first_person ; fch ; fch = fch->next_in_room)
			{
				if (col < max_width && can_see(ch, fch) && !(IS_AFFECTED(fch, AFF_HIDE) || IS_AFFECTED(fch, AFF_STEALTH) || IS_AFFECTED(fch, AFF_ETHEREAL)))
				{
					if (IS_NPC (fch))
					{
						pti = (bool *) capitalize (fch->short_descr);	/* Don't use again in loop */
					}
					else
					{
						pti = (bool *) fch->name;
					}

					if (fch->fighting && (who_fighting(fch) == NULL || fch->fighting->who->in_room != fch->in_room))
					{
						stop_fighting (fch, FALSE);
					}

					if (fch->fighting != NULL)	/* Bold for those fighting */
						bld = 128;
					else
						bld = 0;

					if (fch->hit >= fch->max_hit)
						cval = 'F';
					else
						cval = '0' + (10 * fch->hit / UMAX(1, fch->max_hit));

					if ((pcd->tactical_mode / 1000) % 10 == 0)
					{
						pto = tm + size_h * row + col;
						ptoc = tc + size_h * row + col;
					}
					else
					{
						pto = tm + size_h * (row - 1) + col;
						ptoc = tc + size_h * (row - 1) + col;
					}
					cnt = 0;

					*pto = cval;
					pto++;
					if ((10 * fch->hit / UMAX(1, fch->max_hit)) > 7)
					{
						*ptoc = 2 + pcd->color[COLOR_TACTICAL] / 10 * 8;
					}
					else if ((10 * fch->hit / UMAX(1, fch->max_hit)) > 3)
					{
						*ptoc = 3 + pcd->color[COLOR_TACTICAL] / 10 * 8;
					}
					else
					{
						*ptoc = 1 + pcd->color[COLOR_TACTICAL] / 10 * 8;
					}
					ptoc++;
					if ((pcd->tactical_mode / 100) % 10 == 0)
					{
						if (IS_NPC(fch))
						{
							*pto = *fch->name;
							if (*pto >= 'a' && *pto <= 'z')
							{
								*pto -= ('a' - 'A');
							}
							pto++;
							ptoc++;
							*pto = 'a' + ((int) fch % 25);
							pto++;
							ptoc++;
						}
						else if (fch->pcdata->tactical_index[0] == '\0')
						{
							*pto = '0' + ((fch->pcdata->pvnum % 100) / 10);
							pto++;
							ptoc++;
							*pto = '0' + (fch->pcdata->pvnum % 10);
							pto++;
							ptoc++;
						}
						else
						{
							*pto = fch->pcdata->tactical_index[0];
							pto++;
							ptoc++;
							*pto = fch->pcdata->tactical_index[1];
							pto++;
							ptoc++;
						}
					}
					else
					{
						*pto = '-';
						pto++;
						ptoc++;
					}

					if (who_fighting(fch) && is_same_group(fch->fighting->who, ch))
					{
						colors = pcd->color[COLOR_TACT_ENEMY] % 10   + pcd->color[COLOR_TACT_ENEMY] / 10 * 8;
					}
					else if (is_same_group (fch, ch))
					{
						colors = pcd->color[COLOR_TACT_PARTY] % 10   + pcd->color[COLOR_TACT_PARTY] / 10 * 8;
					}
					else
					{
						colors = pcd->color[COLOR_TACT_NEUTRAL] % 10 + pcd->color[COLOR_TACT_NEUTRAL] / 10 * 8;
					}

					if (who_fighting(fch) == NULL || (pcd->tactical_mode / 100) % 10 == 1)
					{
						for (; cnt < 12 && *pti != '\0'; cnt++, pti++, pto++, ptoc++)
						{
							*pto = *pti + bld;
							*ptoc = colors;
						}
					}
					else
					{
						for (; cnt < 9 && *pti != '\0'; cnt++, pti++, pto++, ptoc++)
						{
							*pto = *pti + bld;
							*ptoc = colors;
						}
						for (; cnt < 9; cnt++, pto++, ptoc++)
						{
							*pto = '-' + bld;
							*ptoc = colors;
						}
						*pto = '>';
						pto++;
						ptoc++;

						if (IS_NPC(fch->fighting->who))
						{
							*pto = *fch->fighting->who->name;
							if (*pto >= 'a' && *pto <= 'z')
							{
								*pto -= ('a' - 'A');
							}
							pto++;
							ptoc++;
							*pto = 'a' + ((int) fch->fighting->who % 25);
							pto++;
							ptoc++;
						}
						else if (fch->fighting->who->pcdata->tactical_index[0] == '\0')
						{
							*pto = '0' + ((fch->fighting->who->pcdata->pvnum % 100) / 10);
							pto++;
							ptoc++;
							*pto = '0' + (fch->fighting->who->pcdata->pvnum % 10);
							pto++;
							ptoc++;
						}
						else
						{
							*pto = fch->fighting->who->pcdata->tactical_index[0];
							pto++;
							ptoc++;
							*pto = fch->fighting->who->pcdata->tactical_index[1];
							pto++;
							ptoc++;
						}
					}
					row++;
					if (row > size_v - 2)
					{
						row = 1;
						col += 16;
					}
				}
			}
		}
		/* Compass code  */
		if (can_see_in_room(ch, ch->in_room))
		{
			if (size_v - 1 > 3 && cw > 0)
			{
				ROOM_INDEX_DATA *room = ch->in_room;
				for (door = -1 ; door <= 5 ; door++)
				{
					if (door == -1)
					{
						if (!can_see_in_room(ch, room))
						{
							strcpy(buf3, "Too dark to tell");
						}
						else
						{
							strcpy(buf3, room->name);
						}
						colors = sector_table[room->sector_type].color;
					}
					else
					{
						if ((pexit = get_exit(room->vnum, door)) == NULL)
						{
							continue;
						}
						if (IS_SET(ch->in_room->room_flags, ROOM_SMOKE) && !can_see_smoke(ch))
						{
							continue;
						}
						if (IS_SET(pexit->exit_info, EX_HIDDEN) && !can_see_hidden(ch))
						{
							continue;
						}
						if (!can_use_exit(ch, pexit))
						{
							continue;
						}
						colors = sector_table[room_index[pexit->to_room]->sector_type].color;
						if (!IS_SET(pexit->exit_info, EX_CLOSED))
						{
							if (!can_see_in_room(ch, room_index[pexit->to_room]))
							{
								strcpy (buf3, "Too dark to tell");
							}
							else
							{
								strcpy (buf3, room_index[pexit->to_room]->name);
							}
						}
						else
						{
							if (pexit->keyword[0] != '\0')
							{
								strcpy(buf3, capitalize(pexit->keyword));
							}
							else
							{
								strcpy(buf3, "Door");
							}
						}
					}

					if ((pcd->tactical_mode / 1000) % 10 == 0)
					{
						vo = 0;
					}
					else
					{
						vo = -1;
					}
					offset = 0;

					if (buf3[0] == 't' || buf3[0] == 'T')
					{
						if (buf3[1] == 'h' || buf3[1] == 'H')
						{
							if (buf3[2] == 'e' || buf3[2] == 'E')
							{
								if (buf3[3] == ' ')
								{
									offset = 4;
								}
							}
						}
					}
					if (buf3[0] == 'a' || buf3[0] == 'A')
					{
						if (buf3[1] == ' ')
						{
							offset = 2;
						}
					}
					str_cpy_max (buf2, capitalize (buf3 + offset), cw + 1);
					if (size_v - 1 > 5)
					{
						switch (door)
						{
							case -1:
								pto  = tm + size_h * (3 + vo) + size_h - 1 - 2 * cw;
								ptoc = tc + size_h * (3 + vo) + size_h - 1 - 2 * cw;
								break;
							case 0:
								pto  = tm + size_h * (2 + vo) + size_h - 2 - 3 * cw / 2;
								*pto = '|';
								pto  = tm + size_h * (1 + vo) + size_h - 1 - 2 * cw;
								ptoc = tc + size_h * (1 + vo) + size_h - 1 - 2 * cw;
								break;
							case 1:
								pto  = tm + size_h * (3 + vo) + size_h - 1 - cw;
								*pto = '-';
								pto  = tm + size_h * (3 + vo) + size_h - cw;
								ptoc = tc + size_h * (3 + vo) + size_h - cw;
								break;
							case 2:
								pto  = tm + size_h * (4 + vo) + size_h - 2 - 3 * cw / 2;
								*pto = '|';
								pto  = tm + size_h * (5 + vo) + size_h - 1 - 2 * cw;
								ptoc = tc + size_h * (5 + vo) + size_h - 1 - 2 * cw;
								break;
							case 3:
								pto  = tm + size_h * (3 + vo) + size_h - 2 - 2 * cw;
								*pto = '-';
								pto  = tm + size_h * (3 + vo) + size_h - 2 - 3 * cw;
								ptoc = tc + size_h * (3 + vo) + size_h - 2 - 3 * cw;
								break;
							case 4:
								pto  = tm + size_h * (2 + vo) + size_h - 1 - cw;
								*pto = '/';
								pto  = tm + size_h * (1 + vo) + size_h - cw;
								ptoc = tc + size_h * (1 + vo) + size_h - cw;
								break;
							case 5:
								pto  = tm + size_h * (4 + vo) + size_h - 2 - 2 * cw;
								*pto = '/';
								pto  = tm + size_h * (5 + vo) + size_h - 2 - 3 * cw;
								ptoc = tc + size_h * (5 + vo) + size_h - 2 - 3 * cw;
								break;
						}
						for (cnt = 0, found = FALSE, pti = (bool *)buf2 ; cnt < cw ; ptoc++, pto++, cnt++)
						{
							if (found)
							{
								*pto  = ' ';
								*ptoc = (bool) colors;
							}
							else if (*pti == '\n' || *pti == '\r' || *pti == '\0')
							{
								found = TRUE;
								*pto  = ' ';
								*ptoc = (bool) colors;
							}
							else
							{
								*pto  = *pti;
								*ptoc = (bool) colors;
								pti++;
							}
						}
					}
					else
					{
						switch (door)
						{
							case -1:
								pto  = tm + size_h * (2 + vo) + size_h - 1 - 2 * cw;
								ptoc = tc + size_h * (2 + vo) + size_h - 1 - 2 * cw;
								break;
							case 0:
								pto  = tm + size_h * (1 + vo) + size_h - 1 - 2 * cw;
								ptoc = tc + size_h * (1 + vo) + size_h - 1 - 2 * cw;
								break;
							case 1:
								pto  = tm + size_h * (2 + vo) + size_h - 1 - cw;
								*pto = '-';
								pto  = tm + size_h * (2 + vo) + size_h - cw;
								ptoc = tc + size_h * (2 + vo) + size_h - cw;
								break;
							case 2:
								pto  = tm + size_h * (3 + vo) + size_h - 1 - 2 * cw;
								ptoc = tc + size_h * (3 + vo) + size_h - 1 - 2 * cw;
								break;
							case 3:
								pto  = tm + size_h * (2 + vo) + size_h - 2 - 2 * cw;
								*pto = '-';
								pto  = tm + size_h * (2 + vo) + size_h - 2 - 3 * cw;
								ptoc = tc + size_h * (2 + vo) + size_h - 2 - 3 * cw;
								break;
							case 4:
								pto  = tm + size_h * (1 + vo) + size_h - 1 - cw;
								*pto = '/';
								pto  = tm + size_h * (1 + vo) + size_h - cw;
								ptoc = tc + size_h * (1 + vo) + size_h - cw;
								break;
							case 5:
								pto  = tm + size_h * (3 + vo) + size_h - 2 - 2 * cw;
								*pto = '/';
								pto  = tm + size_h * (3 + vo) + size_h - 2 - 3 * cw;
								ptoc = tc + size_h * (3 + vo) + size_h - 2 - 3 * cw;
								break;
						}
						for (cnt = 0, found = FALSE, pti = (bool *) buf2 ; cnt < cw; ptoc++, pto++, cnt++)
						{
							if (found)
							{
								*pto  = ' ';
								*ptoc = (bool) colors;
							}
							else if (*pti == '\n' || *pti == '\r' || *pti == '\0')
							{
								found = TRUE;
								*pto  = ' ';
								*ptoc = (bool) colors;
							}
							else
							{
								*pto  = *pti;
								*ptoc = (bool) colors;
								pti++;
							}
						}
					}
				}
			}
		}
	}
	pop_call();
	return (mud->tactical);
}

TACTICAL_MAP *get_diff_tactical (CHAR_DATA * ch)
{
	int size_v, size_h;

	push_call("get_diff_tactical(%p)",ch);

	size_v = CH(ch->desc)->pcdata->tactical_mode % 100 + 1;
	size_h = CH(ch->desc)->pcdata->vt100_type    / 100;

	get_tactical_map(ch);

	if (CH(ch->desc)->pcdata->tactical == NULL)
	{
		pop_call();
		return (mud->tactical);
	}

	if (memcmp(mud->tactical->map, CH(ch->desc)->pcdata->tactical->map, size_v * size_h))
	{
		pop_call();
		return mud->tactical;
	}

	if (memcmp(mud->tactical->color, CH(ch->desc)->pcdata->tactical->color, size_v * size_h))
	{
		pop_call();
		return mud->tactical;
	}
	pop_call();
	return NULL;
}


char *get_color_diff (CHAR_DATA * ch, bool old_for, bool old_bak, bool old_bold, bool new_for, bool new_bak, bool new_bold)
{
	static char buf[20];

	push_call("get_color_diff(%p,%p,%p,%p,%p,%p,%p)",ch,old_for,old_bak,old_bold,new_for,new_bak,new_bold);

	buf[0] = '\0';

	if (ch->desc == NULL)
	{
		pop_call();
		return buf;
	}

	if (CH(ch->desc)->pcdata->ansi == FALSE && !IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_BOLD) && !IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_REVERSE))
	{
		pop_call();
		return buf;
	}

	/* supporting bold and background color */

	if (CH(ch->desc)->pcdata->ansi == FALSE)
	{
		if (new_bak == old_bak && new_bold == old_bold)
		{
			pop_call();
			return buf;
		}

		if (new_bak > 0 && new_bak != 8 && IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_REVERSE))
		{
			if (new_bak != old_bak)
			{
				strcpy(buf, "\033[7m");
			}
		}
		else
		{
			switch (new_bold)
			{
				case 0:
					if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_ECMA48))
					{
						strcpy(buf, "\033[m");
					}
					else
					{
						strcpy(buf, "\033[0m");
					}
					break;
				case 1:
					if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_BOLD))
					{
						strcpy(buf, "\033[1m");
					}
					break;
				case 4:
					if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_UNDERLINE))
					{
						strcpy(buf, "\033[4m");
					}
					break;
				case 5:
					if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_FLASH))
					{
						strcpy(buf, "\033[5m");
					}
					break;
				case 7:
					if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_REVERSE))
					{
						strcpy(buf, "\033[7m");
					}
					break;
				case 22:
					if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_ECMA48))
					{
						strcpy(buf, "\033[22m");
					}
					break;
				default:
					pop_call();
					return buf;
			}
		}
		pop_call();
		return buf;
	}

	if (old_for == new_for && old_bak == new_bak && old_bold == new_bold)
	{
		pop_call();
		return buf;
	}

	/*
		VT100 and minor ECMA-48 support - Scandum 27-02-2002
	*/

	strcpy(buf, "\033[");

	if (new_bold != old_bold)
	{
		switch (new_bold)
		{
			case 0:
				strcat(buf, "0");
				break;
			case 1:
				if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_BOLD))
				{
					strcat(buf, "1");
				}
				break;
			case 4:
				if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_UNDERLINE))
				{
					strcat(buf, "4");
				}
				break;
			case 5:
				if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_FLASH))
				{
					strcat(buf, "5");
				}
				break;
			case 7:
				if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_REVERSE))
				{
					strcat(buf, "7");
				}
				break;
			case 22:
				if (IS_SET(CH(ch->desc)->pcdata->vt100_flags, VT100_ECMA48))
				{
					strcat(buf, "22");
				}
				break;
		}
	}
	if (new_for != 8 && ((new_bold != old_bold && new_bold == 0) || new_for != old_for))
	{
		cat_sprintf(buf, "%s%d", (buf[2] != '\0') ? ";" : "", 30 + new_for);
	}

	if (new_bak != 8 && ((new_bold != old_bold && new_bold == 0) || new_bak != old_bak))
	{
		cat_sprintf(buf, "%s%d", (buf[2] != '\0') ? ";" : "", 40 + new_bak);
	}
	strcat(buf, "m");

	pop_call();
	return buf;
}


void do_reincarnate (CHAR_DATA * ch, char *arg)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	sh_int future_reincarnate, future_class, future_race, future_sex, reinc;
	int cnt, sn, future_hours;
	bool was_95;

	push_call("do_reincarnate(%p,%p)",ch,arg);

	if (IS_NPC(ch))
	{
		pop_call();
		return;
	}

	if (ch->level == 95 && ch->pcdata->reincarnation == 10)
	{
		send_to_char ("You have reached the pinnacle of mortality. Rejoice and be at peace.\n\r", ch);
		pop_call();
		return;
	}

	if (arg[0] == '\0')
	{
		if ((ch->pcdata->reincarnation != 0 && ch->level == 2) || ch->level == 95)
		{
			send_to_char ("Syntax: reincarnate <class> <race> <sex> <password>\n\r", ch);
		}
		else
		{
			send_to_char ("You may not reincarnate yet.\n\r",ch);
		}
		pop_call();
		return;
	}

	if ((ch->pcdata->reincarnation == 0 || ch->level != 2) && ch->level != 95)
	{
		send_to_char ("You may only reincarnate yourself at 95th level or as a new reincarnate.\n\r", ch);
		pop_call();
		return;
	}

	if (ch->level == 2)
	{
		was_95 = FALSE;
	}
	else
	{
		was_95 = TRUE;
	}

	arg = one_argument (arg, arg1);
	arg = one_argument (arg, arg2);
	arg = one_argument (arg, arg3);

	if (was_95)
	{
		reinc = ch->pcdata->reincarnation+1;
	}
	else
	{
		reinc = ch->pcdata->reincarnation;
	}

	future_class = -1;
	future_race  = -1;
	future_sex   = -1;

	for (cnt = 0 ; cnt < MAX_CLASS ; cnt++)
	{
		if (is_name_short(arg1, (char *) class_table[cnt].who_name_long))
		{
			future_class = cnt;
			break;
		}
	}
	for (cnt = 0 ; cnt < MAX_RACE ; cnt++)
	{
		if (is_name_short(arg2, (char *) race_table[cnt].race_name))
		{
			if (reinc >= race_table[cnt].enter && reinc <= race_table[cnt].leave)
			{
				future_race = cnt;
				break;
			}
			else
			{
				ch_printf(ch, "%ss are for other reincarnations.\n\r", race_table[cnt].race_name);
				pop_call();
				return;
			}
		}
	}
	if (is_name_short(arg3, "neutral"))
	{
		future_sex = 0;
	}
	else if (is_name_short(arg3, "male"))
	{
		future_sex = 1;
	}
	else if (is_name_short(arg3, "female"))
	{
		future_sex = 2;
	}
	if (future_class == -1)
	{
		ch_printf(ch, "%s is not a valid class.\n\r", capitalize(arg1));
		pop_call();
		return;
	}
	if (future_race == -1)
	{
		ch_printf(ch, "%s is not a valid class.\n\r", capitalize(arg2));
		pop_call();
		return;
	}
	if (future_sex == -1)
	{
		ch_printf(ch, "%s is not a valid gender.\n\r", capitalize(arg3));
	}
	if (race_table[future_race].race_class[future_class] == 1)
	{
		send_to_char ("That combination of class and race is not allowed.\n\r", ch);
		pop_call();
		return;
	}
	if (encrypt64(arg) != ch->pcdata->password)
	{
		send_to_char ("You must include your password to reincarnate.\n\r", ch);
		pop_call();
		return;
	}

	if (was_95)
	{
		future_reincarnate = ch->pcdata->reincarnation + 1;
		future_hours = ch->pcdata->played / 3600;
		ch->pcdata->played = 0;
	}
	else
	{
		future_reincarnate = ch->pcdata->reincarnation;
		future_hours = 0;
	}

	/*
		Reset all the character stats here
	*/

	die_follower(ch);
	stop_fighting(ch, TRUE);

	char_from_room(ch);
	char_to_room(ch, ROOM_VNUM_SCHOOL);

	while (ch->first_carrying)
	{
		junk_obj(ch->first_carrying);
	}

	while (ch->first_affect)
	{
		affect_from_char(ch, ch->first_affect);
	}
	ch->affected_by  = 0;
	ch->affected2_by = 0;

	REMOVE_BIT(ch->act, PLR_KILLER);
	REMOVE_BIT(ch->act, PLR_THIEF);

	ch->level						= 2;
	ch->trust						= 2;
	ch->class						= future_class;
	ch->pcdata->honorific			= future_class * 8;
	ch->race						= future_race;
	ch->sex						= future_sex;
	ch->pcdata->speak				= 0;
	ch->pcdata->language			= 0;
	ch->leader					= NULL;

	ch->hit						= future_reincarnate * 15 + 30;
	ch->max_hit					= future_reincarnate * 15 + 30;
	ch->pcdata->actual_max_hit		= future_reincarnate * 15 + 30;
	ch->mana						= future_reincarnate * 15 + 100;
	ch->max_mana					= future_reincarnate * 15 + 100;
	ch->pcdata->actual_max_mana		= future_reincarnate * 15 + 100;
	ch->move						= future_reincarnate * 15 + 100;
	ch->max_move					= future_reincarnate * 15 + 100;
	ch->pcdata->actual_max_move		= future_reincarnate * 15 + 100;

	ch->carry_weight				= 0;
	ch->carry_number				= 0;
	ch->saving_throw				= 0;
	ch->gold						= 0;
	ch->alignment					= 0;
	ch->speed						= 1;
	ch->pcdata->exp				= exp_level (future_class, 1) + 1;
	ch->pcdata->recall				= ROOM_VNUM_TEMPLE;
	ch->pcdata->death_room			= ROOM_VNUM_TEMPLE;
	ch->pcdata->practice			= 10 + 3 * future_reincarnate;
	ch->pcdata->wimpy				= 10;
	ch->pcdata->condition[COND_THIRST]	= 48;
	ch->pcdata->condition[COND_FULL]	= 48;
	ch->pcdata->condition[COND_DRUNK]	= 0;
	ch->pcdata->god				= GOD_NEUTRAL;
	ch->pcdata->eqsaves				= 0;
	ch->pcdata->eqhitroll			= 0;
	ch->pcdata->eqdamroll			= 0;
	ch->pcdata->played				= 0;

	STRFREE(ch->long_descr);
	ch->long_descr					= STRALLOC("");
	STRFREE(ch->pcdata->auto_command);
	ch->pcdata->auto_command			= STRALLOC("");
	ch->pcdata->auto_flags			= AUTO_OFF;

	for (cnt = 0 ; cnt < MAX_CLASS; cnt++)
	{
		ch->pcdata->mclass[cnt] = (future_class != cnt) ? 0 : 2;
	}

	for (cnt = 0 ; cnt < MAX_AREA ; cnt++)
	{
		if (ch->pcdata->quest[cnt] != NULL)
		{
			FREEMEM(ch->pcdata->quest[cnt]);
			ch->pcdata->quest[cnt]	= NULL;
		}
	}

	for (sn = 0 ; sn < MAX_SKILL ; sn++)
	{
		ch->pcdata->learned[sn] = 0;
	}
	ch->pcdata->corpse_room			= 0;
	ch->pcdata->corpse				= NULL;
	ch->pcdata->account				= 0;
	ch->pcdata->asn_obj				= NULL;
	ch->furniture					= NULL;
	ch->mounting					= NULL;

	if (ch->pcdata->shadowed_by)
	{
		stop_shadow(ch->pcdata->shadowed_by);
	}
	stop_shadow (ch);

	sub_player (ch);
	add_player (ch);

	ch->pcdata->reincarnation		= future_reincarnate;
	ch->pcdata->previous_hours		+= future_hours;

	roll_race(ch);

	char_reset (ch);

	do_status (ch, "r");

	pop_call();
	return;
}

