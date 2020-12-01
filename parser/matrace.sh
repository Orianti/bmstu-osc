#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 <pid>"; 
	exit 1;
fi

if [ "$UID" -ne 0 ]; then
	echo "$0: could not read kernel buffer: Operation not permitted"
	exit 1
fi

MODULE=lkmah
PID=$1
FILENAME=matracing$1.txt

dmesg | grep "$MODULE: $PID:" > $FILENAME

if [ ! -s "$FILENAME" ]; then
	rm -f $FILENAME;
	echo "No such process or process did not make any memory allocation system calls.";
fi
