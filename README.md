y-cruncher's Digit Viewer
===========

This is the full self-contained source code for y-cruncher's integrated Digit Viewer.<br>
It includes everything including the heavily optimized back-end conversion code.

To Compile (Windows):
 - Open the Visual Studio project and compile from within the IDE.
 - Visual Studio 2015 is required.

To Compile (Linux):
 - Run `VSS - DigitViewer/Compile-Linux.sh` from the directory it is in. It will create a folder `Binaries` with the three executables.
 - x64 and a sufficiently new version of GCC is required.

-----

The supported build modes are:

 - x86 : Release
 - x86 : 04-SSE3
 - x64 : 04-SSE3
 - x64 : 07-Penryn (SSE4.1)
 - x64 : 13-Haswell (AVX2)

The x86 modes are not supported in Linux.

-----

To date, I've been too lazy to write a document on the compression format and how the .ycd files are laid out.
But it should be pretty easy to see from just examining it through a hex viewer.

Two things that might be useful:
 - Base 16 .ycd files are stored as 64-bit integers words with 16 digits per word.
 - Base 10 .ycd files are stored as 64-bit integers words with 19 digits per word.

In both cases, each 8-byte word is little-endian.

-----

Directories:

 - `Source/PublicLibs/` - The open-sourced subset of y-cruncher's support library.
 - `Source/DigitViewer/` - The Digit Viewer itself.
 - `Source/DigitViewer/DigitReaders/` - The Digit Reader classes.
 - `Source/DigitViewer/DigitWriter/` - The Digit Writer classes.
 - `Source/DigitViewer/DigitConverter/` - The highly optimized (and unreadable) digit conversion primitives.

y-cruncher has a build dependency on this Digit Viewer.
Not just because it embeds the Digit Viewer, but it actually uses the DigitWriter classes to output digits at the end of a computation.

So what you are seeing here is actual y-cruncher source code.
