@echo off

del ZipMaker.dll

cmake -A Win32 -B build && cmake --build build --config Release

move build\src\Release\ZipMaker.dll ZipMaker.dll

py gm82gex.py ZipMaker.gej

pause