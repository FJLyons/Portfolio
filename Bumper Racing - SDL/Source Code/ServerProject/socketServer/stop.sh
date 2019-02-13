#!/bin/bash
file="./cid"
if [ -f "$file" ]; then
	cid=$(cat "$file")
	kill -9 $cid
	rm "$file"
	echo "Stopped server ("$cid")."
	exit 0
else
	echo "Server not currently running."
	exit 1
fi
