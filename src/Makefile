# Makefile for compiling DEFOR.
#
# Read the COMPILE file to see more information on the various options
CFLAGS  = -DLINUX 
LIBS    = -lm -lc -lz
CC      = gcc -Wall -g

OUTFILE = ../bin/emud

OBJECTS01 =	reset.o		edit.o		act_comm.o	act_info.o	\
		act_move.o	act_obj.o	act_wiz.o	comm.o		\
		const.o		db.o		fight.o		handler.o	\
		interp.o	magic.o		save.o		special.o	\
		update.o	mob_prog.o	mob_commands.o	castle.o	\
		clans.o		str_hash.o	race.o		drunkify.o	\
		debug.o		mob_handler.o	games.o		skills.o	\
		data.o		mount.o		map.o		telopt.o	\
		purger.o	bounty.o	dynamic.o	pathfind.o	\
		math.o

$(OUTFILE): $(OBJECTS01) 
	rm -f $(OUTFILE)
	$(CC) $(CFLAGS) $(OFLAGS) $(OBJECTS01) -o $(OUTFILE) $(LIBS)
	chmod 700 $(OUTFILE)
#	strip $(OUTFILE)

clean:
	rm -f $(OBJECTS01) 
	rm -f *~

port:
	rm $(OBJECTS01);
	make 
