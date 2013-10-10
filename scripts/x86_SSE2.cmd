@call "Setup VSC x86.cmd"

mkdir "../Binaries"
mkdir "../Output Files"
cd "../Output Files"

mkdir "x86 SSE2"
cd "x86 SSE2"

cl  "../../source/main.cpp" "../../source/y-cruncher/y-cruncher.cpp" ^
    /W4 /O2 /Oi /Ot /Oy /GF /FD /fp:fast /nologo /GS- /Gy /EHsc /FAs /Gs999999 /D "WIN32" /D "DIGITVIEWER_STANDALONE" /D "X86_SSE2" ^
    /arch:SSE2 ^
    /link ^
    /OUT:"../../Binaries/Digit Viewer - x86 SSE2.exe"