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

#include <unistd.h>
#include "emud.h"
#include "telnet.h"

#define MAX_TELOPT 14

char iac_ayt[]				= { IAC, 		AYT,				0 };
char iac_sb_naws[]			= { IAC, SB,	TELOPT_NAWS,		0 };
char	iac_will_linemode[]		= { IAC, WILL,	TELOPT_LINEMODE,	0 };
char iac_sb_linemode[]		= { IAC, SB,	TELOPT_LINEMODE,	0 };
char iac_do_compress2[]		= { IAC, DO,	TELOPT_COMPRESS2,	0 };
char iac_dont_compress2[]	= { IAC, DONT,	TELOPT_COMPRESS2,	0 };
char iac_will_ttype[]		= { IAC, WILL,	TELOPT_TTYPE,		0 };
char iac_sb_ttype[]			= { IAC, SB,	TELOPT_TTYPE,		0 };
char iac_will_environ[]		= { IAC, WILL,	TELOPT_ENVIRON,	0 };
char iac_sb_environ[]		= { IAC, SB,	TELOPT_ENVIRON,	0 };
char iac_ip[]				= { IAC, IP,					0 };
char	iac_do_tm[]			= { IAC, DO,	TELOPT_TM,		0 };
char iac_do_eor[]			= { IAC, DO,	TELOPT_EOR,		0 };
char iac_dont_sga[]           = { IAC, DONT, TELOPT_SGA,         0 };

struct telopt_type
{
	bool	     size;
	char    * code;
	int    (* func) (DESCRIPTOR_DATA *d, bool *argument);
};

const struct telopt_type telopt_table [MAX_TELOPT] =
{
	{	2,	iac_ayt,			&process_ayt},
	{	3,	iac_sb_naws,		&process_sb_naws},
	{	3,	iac_will_linemode,	&process_will_linemode},
	{	3,	iac_sb_linemode,	&process_sb_linemode},
	{	3,	iac_do_compress2,	&process_do_compress2},
	{	3,	iac_dont_compress2,	&process_dont_compress2},
	{	3,	iac_will_ttype,	&process_will_ttype},
	{	3,	iac_sb_ttype,		&process_sb_ttype},
	{	3,	iac_will_environ,	&process_will_environ},
	{	3,	iac_sb_environ,	&process_sb_environ},
	{	3,	iac_do_tm,		&process_do_tm},
	{	2,	iac_ip,			&process_ip},
	{	3,	iac_do_eor,		&process_do_eor},
	{	3,	iac_dont_sga,		&process_dont_sga}
};

void	*zlib_alloc( void *opaque, unsigned int items, unsigned int size )
{
	return calloc(items, size);
}


void zlib_free( void *opaque, void *address ) 
{
	free(address);
}


bool start_compress( DESCRIPTOR_DATA *d )
{
	char start_compress2[]		= { IAC, SB,	TELOPT_COMPRESS2,	IAC, SE, 0 };
	z_stream *stream;

	push_call("start_compress(%p)",d);

	if (d->mccp)
	{
		pop_call();
		return TRUE;
	}

	ALLOCMEM(stream, z_stream, 1);

	stream->next_in	= NULL;
	stream->avail_in	= 0;

	stream->next_out	= mud->mccp_buf;
	stream->avail_out	= COMPRESS_BUF_SIZE;

	stream->data_type	= Z_ASCII;
	stream->zalloc		= zlib_alloc;
	stream->zfree		= zlib_free;
	stream->opaque		= Z_NULL;

	/*
		12, 5 = 32K of memory, should be more than enough - Scandum
	*/

	if (deflateInit2(stream, Z_BEST_COMPRESSION, Z_DEFLATED, 12, 5, Z_DEFAULT_STRATEGY) != Z_OK)
	{
		log_god_printf("start_compress: failed deflateInit2 D%d@%s", d->descriptor, d->host);
		FREEMEM(stream);

		pop_call();
		return FALSE;
	}

	write_to_descriptor(d, start_compress2, 0);

	d->mccp = stream;

	SET_BIT(d->comm_flags, COMM_FLAG_MCCP);

	pop_call();
	return TRUE;
}


void end_compress( DESCRIPTOR_DATA *d )
{
	push_call("end_compress(%p)",d);

	if (d->mccp == NULL)
	{
		pop_call();
		return;
	}


	d->mccp->next_in	= NULL;
	d->mccp->avail_in	= 0;

	d->mccp->next_out	= mud->mccp_buf;
	d->mccp->avail_out	= COMPRESS_BUF_SIZE;

	if (deflate(d->mccp, Z_FINISH) != Z_STREAM_END)
	{
		log_printf("end_compress: failed to deflate");
	}

	if (!IS_SET(d->comm_flags, COMM_FLAG_DISCONNECT))
	{
		process_compressed(d);
	}

	if (deflateEnd(d->mccp) != Z_OK)
	{
		log_printf("end_compress: failed to deflateEnd");
	}

	FREEMEM(d->mccp);

	REMOVE_BIT(d->comm_flags, COMM_FLAG_MCCP);

	pop_call();
	return;
}


void write_compressed( DESCRIPTOR_DATA *d )
{
	push_call("write_compressed(%p)",d);

	d->mccp->next_in	= d->outbuf;
	d->mccp->avail_in	= d->outtop;

	d->mccp->next_out	= mud->mccp_buf;
	d->mccp->avail_out	= COMPRESS_BUF_SIZE;

	d->outtop			= 0;

	if (deflate(d->mccp, Z_SYNC_FLUSH) != Z_OK)
	{
		pop_call();
		return;
	}

	process_compressed(d);

	pop_call();
	return;
}


void process_compressed( DESCRIPTOR_DATA *d )
{
	int length, nWrite, tWrite, nBlock;

	push_call("process_compressed(%p)",d);

	length = COMPRESS_BUF_SIZE - d->mccp->avail_out;

	for (tWrite = 0 ; tWrite < length ; tWrite += nWrite)
	{
		nBlock = UMIN(length - tWrite, d->port_size);

		if ((nWrite = write(d->descriptor, mud->mccp_buf + tWrite, nBlock)) < 1)
		{
			log_god_printf("process_compressed D%d@%s", d->descriptor, d->host);
			SET_BIT(d->comm_flags, COMM_FLAG_DISCONNECT);

			pop_call();
			return;
		}
	}

	mud->total_io_bytes += tWrite;

	pop_call();
	return;
}


void send_compress_on( DESCRIPTOR_DATA *d )
{
	char iac_will_compress2[]	= { IAC, WILL, TELOPT_COMPRESS2,	0 };

	write_to_descriptor(d, iac_will_compress2, 0);
}

void send_naws( DESCRIPTOR_DATA *d )
{
	char iac_do_naws[] = { IAC, DO, TELOPT_NAWS, 0 };

	write_to_descriptor(d, iac_do_naws, 0);
}

void send_ttype_on( DESCRIPTOR_DATA *d )
{
	char iac_do_ttype[]			= { IAC, DO,	TELOPT_TTYPE,		0 };

	write_to_descriptor(d, iac_do_ttype, 0);
}

void send_eor_on( DESCRIPTOR_DATA *d )
{
	char iac_will_eor[]			= { IAC, WILL,	TELOPT_EOR,		0 };

	write_to_descriptor(d, iac_will_eor, 0);
}

void send_echo_off( DESCRIPTOR_DATA *d )
{
	char iac_will_echo[]		= { IAC, WILL,	TELOPT_ECHO,		0 };

	SET_BIT(d->comm_flags, COMM_FLAG_PASSWORD);
	write_to_descriptor(d, iac_will_echo, 0);
}

void send_echo_on( DESCRIPTOR_DATA *d )
{
	char iac_wont_echo[]		= { IAC, WONT,	TELOPT_ECHO,		0 };

	REMOVE_BIT(d->comm_flags, COMM_FLAG_PASSWORD);
	write_to_descriptor(d, iac_wont_echo, 0);
}

void send_environ_on( DESCRIPTOR_DATA *d )
{
	char iac_do_environ[]		= { IAC, DO,	TELOPT_ENVIRON,	0 };

	write_to_descriptor(d, iac_do_environ, 0);
}

void send_ga( DESCRIPTOR_DATA *d )
{
	char iac_ga[]				= { IAC, GA,	0 };

	write_to_descriptor(d, iac_ga, 0);
}

void send_eor( DESCRIPTOR_DATA *d )
{
	char iac_eor[]				= { IAC, EOR,	0 };

	write_to_descriptor(d, iac_eor, 0);
}

int process_ttype( DESCRIPTOR_DATA *d, bool *argument )
{
	int i;

	bool buf[MAX_INPUT_LENGTH];

	push_call("process_ttype(%p,%p)",d,argument);

	strncpy(buf, &argument[4], 12);

	buf[15] = '\0';

	for (i = 4 ; argument[i] != '\0' && argument[i] != SE ; i++);

	buf[i - 5] = '\0';

	STRFREE(d->terminal_type);

	d->terminal_type = STRALLOC(capitalize_all(buf));
	pop_call();
	return ++i;
}

/*
	Adjust vt102 settings to given NAWS - Scandum 20-04-3003
*/

void process_naws( DESCRIPTOR_DATA *d, int width, int height )
{
	push_call("process_naws(%p,%d,%d)",d, width, height);

	if (CH(d) == NULL)
	{
		pop_call();
		return;
	}

	if (height)
	{
		height = URANGE(15, height, 99);

		if (height != CH(d)->pcdata->vt100_type % 100)
		{
			CH(d)->pcdata->vt100_type = (CH(d)->pcdata->vt100_type / 100) * 100 + height;

			height = URANGE(1, (CH(d)->pcdata->vt100_type % 100) / 2, MAX_TACTICAL_ROW - 1);

			if (CH(d)->pcdata->tactical_mode % 100 > height)
			{
				CH(d)->pcdata->tactical_mode = CH(d)->pcdata->tactical_mode - CH(d)->pcdata->tactical_mode % 100 + height;
			}

			if (d->connected >= CON_PLAYING && CH(d)->pcdata->vt100 == 1)
			{
				CH(d)->pcdata->vt100 = 3;
				SET_BIT(CH(d)->pcdata->interp, INTERP_TACT_UPDATE);
			}
		}
	}

	if (width)
	{
		width = URANGE(80, width, MAX_TACTICAL_COL);

		if (width != CH(d)->pcdata->vt100_type / 100)
		{
			CH(d)->pcdata->vt100_type = (CH(d)->pcdata->vt100_type % 100) + 100 * width;

			if (d->connected >= CON_PLAYING && CH(d)->pcdata->vt100 == 1)
			{
				CH(d)->pcdata->vt100 = 3;
				SET_BIT(CH(d)->pcdata->interp, INTERP_TACT_UPDATE);
			}
		}
	}
	pop_call();
	return;
}


int process_environ( DESCRIPTOR_DATA *d, bool *argument )
{
	char buf[20];
	int i, j;

	push_call("process_environ(%p,%p)",d,argument);

	memset(buf, 0, 20);

	for (i = 5 ; isprint(argument[i]) ; i++);

	i++;

	for (j = i ; isprint(argument[i]) ; i++);

	strncpy(buf, &argument[j], UMIN(12, i - j));

	if (!strcasecmp(buf, "WIN32"))
	{
		RESTRING(d->terminal_type, "WIN32");
		SET_BIT(d->comm_flags, COMM_FLAG_ECHO);
	}

	for (i++ ; argument[i] != 0 && argument[i] != SE ; i++);

	pop_call();
	return ++i;
}

/*
	IAC Sequences emud listens to
*/

int lookup_telopt( bool *s )
{

	pop_call();
	return -1;
}

/*
	interp the telnet stuff
*/

int do_telopts( DESCRIPTOR_DATA *d, bool *argument )
{
	int i;

	push_call("do_telopts(%p, %p)",d,argument);

	if (FALSE)
	{
		fprintf(stderr, "%12s: IAC: %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n",
			CH(d) ? CH(d)->name : "Nanny",
			argument[ 0], argument[ 1], argument[ 2],
			argument[ 3], argument[ 4], argument[ 5],
			argument[ 6], argument[ 7], argument[ 8],
			argument[ 9], argument[10], argument[11],
			argument[12], argument[13], argument[14]);
	}

	for (i = 0 ; i < MAX_TELOPT ; i++)
	{
		if (!memcmp(argument, telopt_table[i].code, telopt_table[i].size))
		{
			pop_call();
			return telopt_table[i].func(d, argument);
		}
	}
	pop_call();
	return 3;
}

int process_ayt( DESCRIPTOR_DATA *d, bool *argument )
{
	write_to_buffer(d, "*Emud Nods*\n\r", 0);

	return 2;
}

int process_sb_naws( DESCRIPTOR_DATA *d, bool *argument )
{
	if (argument[3] == 0 && argument[5] == 0)
	{
		process_naws(d, argument[4], argument[6]);
	}
	else
	{
		fprintf(stderr, "%12s: IAC: %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n",
			CH(d) ? CH(d)->name : "Nanny",
			argument[ 0], argument[ 1], argument[ 2],
			argument[ 3], argument[ 4], argument[ 5],
			argument[ 6], argument[ 7], argument[ 8],
			argument[ 9], argument[10], argument[11],
			argument[12], argument[13], argument[14]);
	}
	return 9;
}

int process_will_linemode( DESCRIPTOR_DATA *d, bool *argument )
{
	char	iac_do_linemode[]		= { IAC, DO,	TELOPT_LINEMODE,	0 };
	char	iac_edit_linemode[]		= { IAC, SB,	TELOPT_LINEMODE,	1, 1, IAC, SE, 0 };

	write_to_descriptor(d, iac_do_linemode, 0);
	write_to_descriptor(d, iac_edit_linemode, 0);

	return 3;
}

int process_sb_linemode( DESCRIPTOR_DATA *d, bool *argument )
{
	/*
		Don't feel like interpreting these, do you? - Scandum
	*/
	return strlen(argument);
}

int process_do_compress2( DESCRIPTOR_DATA *d, bool *argument )
{
	start_compress(d);

	return 3;
}

int process_dont_compress2( DESCRIPTOR_DATA *d, bool *argument )
{
	end_compress(d);

	return 3;
}

int process_will_ttype( DESCRIPTOR_DATA *d, bool *argument )
{
	char iac_send_ttype[]		= { IAC, SB,	TELOPT_TTYPE,		1, IAC, SE, 0 };

	if (d->terminal_type == str_empty)
	{
		write_to_descriptor(d, iac_send_ttype, 0);
	}
	return 3;
}

int process_sb_ttype( DESCRIPTOR_DATA *d, bool *argument )
{
	return process_ttype(d, argument);
}

int process_will_environ( DESCRIPTOR_DATA *d, bool *argument )
{
	char iac_environ_os[]		= { IAC, SB,	TELOPT_ENVIRON,	1, 0, 'S', 'Y', 'S', 'T', 'E', 'M', 'T', 'Y', 'P', 'E', IAC, SE, 0 };

	write_to_descriptor(d, iac_environ_os, 17);

	return 3;
}

int process_sb_environ( DESCRIPTOR_DATA *d, bool *argument )
{
	return process_environ(d, argument);
}

int process_do_tm( DESCRIPTOR_DATA *d, bool *argument )
{
	char iac_wont_tm[]			= { IAC, WONT,	TELOPT_TM,		0 };

	write_to_descriptor(d, iac_wont_tm, 0);

	return 3;
}

int process_ip( DESCRIPTOR_DATA *d, bool *argument )
{
	return 2;
}

int process_do_eor( DESCRIPTOR_DATA *d, bool *argument )
{
	char iac_will_eor[]			= { IAC, WILL,	TELOPT_EOR,		0 };

	if (!IS_SET(d->comm_flags, COMM_FLAG_EOR))
	{
		SET_BIT(d->comm_flags, COMM_FLAG_EOR);
		write_to_descriptor(d, iac_will_eor, 0);
	}
	return 3;
}

int process_dont_sga( DESCRIPTOR_DATA *d, bool *argument )
{
	char iac_wont_sga[]			= { IAC, WONT, TELOPT_SGA,		0 };

	write_to_descriptor(d, iac_wont_sga, 0);

	return 3;
}
