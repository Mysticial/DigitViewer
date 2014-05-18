@call "Setup VSC x86.cmd"

cd "../../Output Files"

mkdir "Launcher"
cd "Launcher"

cl  "../../Digit Viewer (VSS)/source/launcher.cpp" ^
    /W4 /O2 /Oi /Ot /Oy /GF /FD /fp:fast /nologo /GS- /Gy /EHsc /FAs /Gs999999 /D "WIN32" /D "DIGITVIEWER_STANDALONE" ^
    /arch:IA32 ^
    /link ^
    /OUT:"../../y-cruncher/Digit Viewer.exe"