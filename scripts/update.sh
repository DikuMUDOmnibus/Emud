#!/bin/sh

SOURCEDIR=/home/emud/testsite
SRCDEST=/home/emud/src
AREDEST=/home/emud/area_current
CFILES=${SOURCEDIR}/src/*.c
HFILES=${SOURCEDIR}/src/*.h
OFILES=${SOURCEDIR}/src/*.o
AREFILES=${SOURCEDIR}/area_current/*

if [[ "${@}" = "" ]]
then
	echo "usage: update.sh [-all] | [[-src] | [-are]]"
	exit
fi
	
for i in ${@}
do
	if [[ ${i} = -src || ${i} = -all ]]
	then
		echo "updating source files."
		rsync -avub ${CFILES} ${SRCDEST}
		rsync -avub ${HFILES} ${SRCDEST}
		rsync -avub ${OFILES} ${SRCDEST}
		rsync -avub /home/emud/testsite/src/VERSION /home/emud/src
		./compile
		echo "Compile complete, if succesfull you can now reboot."
	fi
	if [[ ${i} = -are || ${i} = -all ]]
	then
		echo "updating areafiles..."
		rsync -avuzb ${AREFILES} ${AREDEST}
		echo "update succesfull, you can now do a copyover!"
	fi
	if [[ ${i} = -help ]]
	then
		echo "usage: update.sh [-all] | [[-src] | [-are]]"
	fi
done
