@call "Setup VSC x64.cmd"

cd "../../Output Files"

mkdir "x64 SSE2"
cd "x64 SSE2"

cl  "../../Digit Viewer (VSS)/source/main.cpp" ^
    "../../Digit Viewer (VSS)/source/y-cruncher/y-cruncher.cpp" ^
    /W4 /O2 /Oi /Ot /Oy /GF /FD /fp:fast /nologo /GS- /Gy /EHsc /FAs /Gs999999 /D "WIN32" /D "DIGITVIEWER_STANDALONE" ^
    /D "X86_SSE2" ^
    /arch:SSE2 ^
    /link ^
    /OUT:"../../y-cruncher/Binaries/Digit Viewer - x64 SSE2.exe"