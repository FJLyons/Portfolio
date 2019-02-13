@echo off
set sh_location="%ProgramW6432%\Git\bin\sh.exe"

if exist %sh_location% (
	%sh_location% --login -i -c 'sed -i -E \'s/"(\[)|(\])"/\1\2/g\' \'RockMap.json\''
) else (
	echo %sh_location% does not exist!
	pause
)