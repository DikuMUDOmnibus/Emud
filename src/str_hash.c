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

#define MAX_STR_HASH	100000

STR_HASH_DATA *str_hash[MAX_STR_HASH];

unsigned short str_leng_alloc = 0;

/*
	Generate a kinky hash index - Scandum
*/

unsigned int get_hash( char *str)
{
	unsigned int h;

	for (h = str_leng_alloc = 0 ; *str != 0 ; str++, str_leng_alloc++)
	{
		h -= (4 - (h >> 7)) * *str;
	}

	h = (h + str_leng_alloc) % MAX_STR_HASH;

	return h;
}


char *str_alloc( char *str )
{
	unsigned int hash;
	STR_HASH_DATA *hash_ptr;

	push_call("str_alloc(%p)",str);

	hash  = get_hash(str);

	for (hash_ptr = str_hash[hash] ; hash_ptr ; hash_ptr = hash_ptr->next)
	{
		if (*str == *((char *) hash_ptr+mud->str_hash_size) && !strcmp(str, (char *) hash_ptr+mud->str_hash_size))
		{
			hash_ptr->links++;

			pop_call();
			return (char *) hash_ptr+mud->str_hash_size;
		}
	}

	ALLOCMEM(hash_ptr, unsigned char, str_leng_alloc + mud->str_hash_size + 1);

	hash_ptr->links  = 1;
	hash_ptr->hash   = hash;

	strcpy( (char *) hash_ptr+mud->str_hash_size, str );

	if (str_hash[hash] != NULL)
	{
		hash_ptr->next = str_hash[hash];
		str_hash[hash]->prev = hash_ptr;
	}
	str_hash[hash] = hash_ptr;

	pop_call();
	return (char *) hash_ptr+mud->str_hash_size;
}


char *str_dupe( char *str )
{
	STR_HASH_DATA *str_ptr;

	push_call("str_dupe(%p)",str);

	str_ptr = (STR_HASH_DATA *) (str - mud->str_hash_size);

	str_ptr->links++;

	pop_call();
	return str;
}


void str_free( char *str )
{
	STR_HASH_DATA *str_ptr;

	push_call("str_free(%p)",str);

	if (str == NULL)
	{
		pop_call();
		return;
	}

	str_ptr = (STR_HASH_DATA *) (str - mud->str_hash_size);

	if (--str_ptr->links == 0)
	{
		if (str_ptr->prev)
		{
			str_ptr->prev->next = str_ptr->next;
		}
		else
		{
			str_hash[str_ptr->hash] = str_ptr->next;
		}
		if (str_ptr->next)
		{
			str_ptr->next->prev = str_ptr->prev;
		}
		FREEMEM(str_ptr);
	}
	pop_call();
	return;
}


char *str_hash_status( char *argument )
{
	STR_HASH_DATA *hash_ptr;
	static char buf[MAX_STRING_LENGTH];
	bool hashamounts[MAX_STR_HASH];
	int hihash, hihashcnt, usedhash, cnt, strings, links, ulinks, memoryused, memorysaved, maxlink, maxlinkcnt;

	push_call("str_hash_status(%p)",argument);

	memorysaved = maxlink = maxlinkcnt = strings = ulinks = links = memoryused = hihashcnt = hihash = usedhash = 0;

	memset(hashamounts, 0, MAX_STR_HASH);

	if (is_number(argument))
	{
		for (buf[0] = '\0', hash_ptr = str_hash[URANGE(0, atol(argument), MAX_STR_HASH - 1)] ; hash_ptr ; hash_ptr = hash_ptr->next)
		{
			cat_sprintf(buf, "- %s\n", (char *) hash_ptr+mud->str_hash_size);
		}
		pop_call();
		return buf;
	}

	for (buf[0] = cnt = 0 ; cnt < MAX_STR_HASH ; cnt++)
	{
		for (hash_ptr = str_hash[cnt] ; hash_ptr ; hash_ptr = hash_ptr->next)
		{
			hashamounts[cnt]++;

			if (hash_ptr->links == 1)
			{
				ulinks++;
			}

			if (*argument == 'd')
			{
				if (hash_ptr->links > 500)
				{
					cat_sprintf(buf, "{078}[%6d] links: %s\n\r", hash_ptr->links, (char *) hash_ptr+mud->str_hash_size);
				}

				if (get_hash((char *) hash_ptr+mud->str_hash_size) != cnt)
				{
					log_printf("Found a bad string in hash table index: %d", cnt);
					log_printf("-- current string  --");
					log_printf("%s", (char *) hash_ptr+mud->str_hash_size);
					log_printf("-- previous string --");
					log_printf("%s", hash_ptr->prev ? (char *) hash_ptr->prev+mud->str_hash_size : "(null");
					log_printf("-- adjacent string --");
					log_printf("%s", hash_ptr->next ? (char *) hash_ptr->next+mud->str_hash_size : "(null");
				}
			}

			if (hash_ptr->links > maxlink)
			{
				maxlink    = hash_ptr->links;
				maxlinkcnt = cnt;
			}

			links       += hash_ptr->links;
			memoryused  += strlen((char *) hash_ptr + mud->str_hash_size) + mud->str_hash_size + 1;
			memorysaved += hash_ptr->links * (1 + strlen((char *) hash_ptr + mud->str_hash_size));
		}
	}

	for (cnt = 0 ; cnt < MAX_STR_HASH ; cnt++)
	{
		strings += hashamounts[cnt];

		if (hashamounts[cnt] > hihash)
		{
			hihash    = hashamounts[cnt];
			hihashcnt = cnt;
		}
		if (hashamounts[cnt] > 0)
		{
			usedhash++;
		}
	}

	cat_sprintf(buf,
		"{078}Strings allocated :%10d   Total links     :%10d\n\r"
		"{078}Memory allocated  :%10d   Memory saved    :%10d\n\r"
		"{078}Unique links      :%10d   Average hash    :%10.3f\n\r"
		"{078}Maximum links     :%10d   Max links index :%10d\n\r"
		"{078}Maximum Hash      :%10d   Max hash index  :%10d\n\r"
		"{078}Used Hash:        :%10d   Hash size       :%10d\n\r",
		strings,
		links,
		memoryused,
		memorysaved - memoryused,
		ulinks,
		(float) strings / (float) usedhash,
		maxlink,
		maxlinkcnt,
		hihash,
		hihashcnt,
		usedhash,
		MAX_STR_HASH);

	pop_call();
	return buf;
}

