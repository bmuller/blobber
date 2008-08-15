#!/bin/sh

for i in $*
do 
	if [ "$i" = "--help" ] ; then
		echo syntax: $0 [OPTIONS]
		echo
		echo OPTIONS List - Use only absolute paths:
		python scons-local/scons.py -Q -f SInit -h && exit
	fi
	if [ "$i" = "clean" ] ; then
		echo Cleaning project:
		python scons-local/scons.py -c && exit
	fi
done

SYSTEM_ROOT=`cd / && python -c "import os;print os.getcwd()"`
python scons-local/scons.py -Q -f SInit SYSTEM_ROOT=$SYSTEM_ROOT $*
python scons-local/scons.py  $*
