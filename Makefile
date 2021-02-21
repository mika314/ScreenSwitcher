all: local

local:
	TEMP=C:/Users/teant/AppData/Local/Temp TMP=C:/Users/teant/AppData/Local/Temp \
"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/MSBuild/Current/Bin/amd64/MSBuild.exe" \
/e/prj/SceeneSwitcher/SceeneSwitcher/SceeneSwitcher.sln
