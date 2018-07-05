#!/bin/tcsh -f

# put PID in a file named script.pid
echo $$ > script.pid

# Set the port number.
set port = 4321
set __NOLOG = "NO"
set __AREADEBUG = 0
set __NOSOCKET = ""
set __COUNT = ""

if ( "$1" == "-p" ) set port="$2"
if ( "$2" == "-p" ) set port="$3"
if ( "$3" == "-p" ) set port="$4"
if ( "$4" == "-p" ) set port="$5"
if ( "$5" == "-p" ) set port="$6"
if ( "$6" == "-p" ) set port="$7"

if ( "$1" == "-areadebug") set __AREADEBUG = "$2"
if ( "$2" == "-areadebug") set __AREADEBUG = "$3"
if ( "$3" == "-areadebug") set __AREADEBUG = "$4"
if ( "$4" == "-areadebug") set __AREADEBUG = "$5"
if ( "$5" == "-areadebug") set __AREADEBUG = "$6"
if ( "$6" == "-areadebug") set __AREADEBUG = "$7"

if ( "$1" == "-count") set __COUNT = "-count"
if ( "$2" == "-count") set __COUNT = "-count"
if ( "$3" == "-count") set __COUNT = "-count"
if ( "$4" == "-count") set __COUNT = "-count"
if ( "$5" == "-count") set __COUNT = "-count"
if ( "$6" == "-count") set __COUNT = "-count"
if ( "$7" == "-count") set __COUNT = "-count"

if ( "$1" == "-h" ) then
	echo "syntax : emud [-h]"
	echo "    or : emud [-p <port#>] [-areadebug <level>] [-nosocket] [-n] [-count]"
	exit
endif

if ( "$1" == "-n" ) set __NOLOG = "YES"
if ( "$2" == "-n" ) set __NOLOG = "YES"
if ( "$3" == "-n" ) set __NOLOG = "YES"
if ( "$4" == "-n" ) set __NOLOG = "YES"
if ( "$5" == "-n" ) set __NOLOG = "YES"
if ( "$6" == "-n" ) set __NOLOG = "YES"
if ( "$7" == "-n" ) set __NOLOG = "YES"

if ( "$1" == "-nosocket" ) set __NOSOCKET = "-nosocket"
if ( "$2" == "-nosocket" ) set __NOSOCKET = "-nosocket"
if ( "$3" == "-nosocket" ) set __NOSOCKET = "-nosocket"
if ( "$4" == "-nosocket" ) set __NOSOCKET = "-nosocket"
if ( "$5" == "-nosocket" ) set __NOSOCKET = "-nosocket"
if ( "$6" == "-nosocket" ) set __NOSOCKET = "-nosocket"
if ( "$7" == "-nosocket" ) set __NOSOCKET = "-nosocket"

echo "Starting emud..."

set CUR_DIR = `pwd`
set RUN_FILE = $CUR_DIR/RUNNING

# Change to area directory.
cd $CUR_DIR/area_current

# set the default time to let the script wait before starting anew...
set _SLEEP_ = 3

# Remove the old core file.
if ( -e $CUR_DIR/area_current/core ) then
	rm $CUR_DIR/area_current/core >/dev/null
endif

#echo "About to set limits..."

# Set limits.
# nice -20
#
# Commented this out, to see what happens...
# -Manwe
#
# unlimit

while ( 1 )
	if ( -e shutdown.txt ) rm -f shutdown.txt

	if ( -e $RUN_FILE) then
		echo "The mud is already running. Remove the RUNNING file or shut it down first!"
		exit
	endif

	# Fill the RUNNING file with some information
	# It may happen you can do something usefull with it ;-)
	echo "homedir of the mud : ${CUR_DIR}" > $RUN_FILE
	echo "Started by         : ${uid} (${user})" >> $RUN_FILE
	echo -n "Started on         : " >> $RUN_FILE
	date >> $RUN_FILE	
	echo "Parameter list     : ${1} ${2} ${3} ${4} ${5} ${6} ${7} ${8} ${9}" >> $RUN_FILE

	echo Finding log file
	# If you want to have logs in a different directory,
	#   change the 'set logfile' line to reflect the directory name.
	set index = 1000
	while ( 1 )
		set logfile = $CUR_DIR/log/$index.log
		if ( ! -e $logfile ) break
		@ index++
	end


	echo Found new log file $logfile
	# Record starting time
	date > $logfile
	limit > $logfile

	if( "$__NOLOG" != "YES" ) $CUR_DIR/start_tail -f $logfile

	# Record initial charges
	# charges >> $logfile

	echo Startup with log file $logfile

	# Do as much as you can to get the binaries 'there'...
	if (! -d $CUR_DIR/bin) then
		mkdir $CUR_DIR/bin
	endif

	if (! -e $CUR_DIR/bin/next_emud ) then
		if (! -e $CUR_DIR/bin/emud ) then
			echo "There are no binaries!"
			if( -e $CUR_DIR/compile ) then
				echo "Run the script 'compile' to install"
			else
				echo "Go to the source dir and type 'make'"
			endif
			rm $RUN_FILE
			exit
		else
			cp -f $CUR_DIR/bin/emud $CUR_DIR/bin/next_emud
		endif
	endif

	cp -f $CUR_DIR/bin/next_emud $CUR_DIR/bin/current_emud 

	if ( ! -e $CUR_DIR/bin/current_emud ) then
		echo There is no binary file.
		echo Be sure to compile, and copy bin/emud to bin/next_emud.
		rm $RUN_FILE
		exit 0
	endif
        
	$CUR_DIR/bin/current_emud -p $port -areadebug $__AREADEBUG $__NOSOCKET $__COUNT >&! $logfile

	# Analyze the core file
	if ( -e $CUR_DIR/area_current/core ) then
		echo where | gdb ../bin/current_emud core >> $logfile
		echo "\n"
	endif

	# Record ending charges
	# charges >> $logfile

	# Restart, giving old connections a chance to die.

	#if ( -e shutdown.txt ) then
	#rm -f shutdown.txt
	#exit 0
	#endif
	echo

	if( "$__NOLOG" != "YES" ) then
		echo "Terminating tailing $logfile"
		sleep 2
		$CUR_DIR/down -tail
	endif

	if (-e $RUN_FILE ) then
		rm -f $RUN_FILE
	endif
	
	if ( -e shutdown.txt ) then
		rm -f shutdown.txt
		if ( -e script.pid ) then
			rm script.pid;
		endif
		echo "The game is shutdown and will not reboot now"
		echo "start $0 again when you are ready."
		exit 0
	else
		echo "Starting the mud in ${_SLEEP_} seconds again."
		sleep $_SLEEP_
	endif
end
