@echo off
plink -P 2222 ubuntu@34.250.8.240 -pw argo "cd socketServer/; ./stop.sh"

