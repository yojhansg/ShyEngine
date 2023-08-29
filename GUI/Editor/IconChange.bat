cd %1

ResourceHacker.exe -open EngineRelease.exe -save %2 -action addoverwrite -res %3 -mask ICONGROUP,MAINICON,1

ie4uinit.exe -ClearIconCache

