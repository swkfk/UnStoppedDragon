@echo off
set raw_name=UnStoppedDragon-V
set exe_name=.exe
set file_name=%raw_name%%1%exe_name%
echo %file_name%

gcc src\main.c src\MapLoader.c src\BuiltinMap.c src\utils\CharArray.c -o %file_name% && %file_name%