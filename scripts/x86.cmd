@call "Setup VSC x86.cmd"

mkdir "../Binaries"
mkdir "../Output Files"
cd "../Output Files"

mkdir "x86"
cd "x86"

cl  "../../source/main.cpp" "../../source/y-cruncher/y-cruncher.cpp" ^
    /W4 /O2 /Oi /Ot /Oy /GF /FD /fp:fast /nologo /GS- /Gy /EHsc /FAs /Gs999999 /D "WIN32" /D "DIGITVIEWER_STANDALONE" ^
    /arch:IA32 ^
    /link ^
    /OUT:"../../Binaries/Digit Viewer - x86.exe"