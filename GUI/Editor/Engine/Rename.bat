ResourceHacker.exe -open EngineRelease.exe -save %1 -action addoverwrite -res %2 -mask ICONGROUP,MAINICON,1

del EngineRelease.exe
del ResourceHacker.exe
del ResourceHacker.ini

ie4uinit.exe -ClearIconCache
