mkdir "../y-cruncher" #
mkdir "../y-cruncher/Binaries" #
mkdir "../Output Files" #
#
#
echo "Compiling Launcher..." #
g++ "source/launcher.cpp" -std=c++0x -static -Wall -Wno-unused-function -O2 -D DIGITVIEWER_STANDALONE -o "../y-cruncher/Digit Viewer.out" #
#
echo "Compiling x64 SSE2..." #
g++ "source/main.cpp" "source/y-cruncher/y-cruncher.cpp" -std=c++0x -static -Wall -Wno-unused-function -O2 -D DIGITVIEWER_STANDALONE -D X86_SSE2 -msse2 -o "../y-cruncher/Binaries/Digit Viewer - x64 SSE2.out" #
#
echo "Compiling x64 SSE4.1..." #
g++ "source/main.cpp" "source/y-cruncher/y-cruncher.cpp" -std=c++0x -static -Wall -Wno-unused-function -O2 -D DIGITVIEWER_STANDALONE -D X86_SSE41 -msse4.1 -o "../y-cruncher/Binaries/Digit Viewer - x64 SSE4.1.out" #
#
strip "../y-cruncher/Digit Viewer.out" #
strip "../y-cruncher/Binaries/Digit Viewer - x64 SSE2.out" #
strip "../y-cruncher/Binaries/Digit Viewer - x64 SSE4.1.out" #