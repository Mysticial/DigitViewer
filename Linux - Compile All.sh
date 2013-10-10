echo "Compiling x64 SSE2..." #
g++ "../Digit Viewer (VSS)/source/main.cpp" "../Digit Viewer (VSS)/source/y-cruncher/y-cruncher.cpp" -std=c++0x -static -Wall -Wno-unused-function -O2 -D DIGITVIEWER_STANDALONE -D X86_SSE2 -msse2 -o "Digit Viewer - x64 SSE2.out" #
#
echo "Compiling x64 SSE4.1..." #
g++ "../Digit Viewer (VSS)/source/main.cpp" "../Digit Viewer (VSS)/source/y-cruncher/y-cruncher.cpp" -std=c++0x -static -Wall -Wno-unused-function -O2 -D DIGITVIEWER_STANDALONE -D X86_SSE2 -msse4.1 -o "Digit Viewer - x64 SSE4.1.out" #
#
strip "Digit Viewer - x64 SSE2.out" #
strip "Digit Viewer - x64 SSE4.1.out" #