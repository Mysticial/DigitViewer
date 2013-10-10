@call "Setup VSC x86.cmd"

mkdir "../Binaries"
mkdir "../Output Files"
cd "../Output Files"

mkdir "Launcher"
cd "Launcher"

cl  "../../source/launcher.cpp" "../../source/y-cruncher/y-cruncher.cpp" ^
    /W4 /O2 /Oi /Ot /Oy /GF /FD /fp:fast /nologo /GS- /Gy /EHsc /FAs /Gs999999 /D "WIN32" /D "DIGITVIEWER_STANDALONE" ^
    /arch:IA32 ^
    /link ^
    /OUT:"../../Digit Viewer.exe"