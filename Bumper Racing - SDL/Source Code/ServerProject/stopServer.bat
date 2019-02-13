@echo off
plink -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" ubuntu@34.250.8.240 "cd socketServer/; ./stop.sh"

