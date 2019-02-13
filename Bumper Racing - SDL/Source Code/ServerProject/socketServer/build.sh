#!/bin/bash
if [ -f "./cid" ]; then
	"./stop.sh"
	make all
	"./start.sh"
else
	make all
fi

