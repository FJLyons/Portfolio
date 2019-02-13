#!/bin/bash
if [ ! -f "./server" ]; then
	echo "Building..."
	"./build.sh"
fi
if [ -f "./cid" ]; then
	echo "Already a server running!"
	exit 1
else
	nohup ./server >/dev/null 2>&1 &
	echo $! >> cid
	echo "Server started ($!)."
	exit 0
fi
