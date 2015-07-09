mkdir "Binaries" #
#
gccflags="-std=c++0x -static -Wall -Wno-unused-function -O2" #
source0="../Source Files/DigitViewer/Main.cpp" #
source1="../Source Files/DigitViewer/SMC_DigitViewer.cpp" #
source2="../Source Files/Public/SMC_Public.cpp" #
#
echo "Compiling x64 SSE2..." #
g++ "${source0}" "${source1}" "${source2}" ${gccflags} -D X64_04_SSE3   -msse2   -o "Binaries/Digit Viewer - x64 SSE2.out" #
#
echo "Compiling x64 SSE4.1..." #
g++ "${source0}" "${source1}" "${source2}" ${gccflags} -D X64_07_Penryn -msse4.1 -o "Binaries/Digit Viewer - x64 SSE4.1.out" #
#