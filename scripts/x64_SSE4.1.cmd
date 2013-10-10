@call "Setup VSC x64.cmd"

mkdir "../Binaries"
mkdir "../Output Files"
cd "../Output Files"

mkdir "x64 SSE4.1"
cd "x64 SSE4.1"

cl  "../../source/main.cpp" "../../source/y-cruncher/y-cruncher.cpp" ^
    /W4 /O2 /Oi /Ot /Oy /GF /FD /fp:fast /nologo /GS- /Gy /EHsc /FAs /Gs999999 /D "WIN32" /D "DIGITVIEWER_STANDALONE" /D "X86_SSE41" ^
    /arch:SSE2 ^
    /link ^
    /OUT:"../../Binaries/Digit Viewer - x64 SSE4.1.exe"