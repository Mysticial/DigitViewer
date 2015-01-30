y-cruncher v0.6.8's Digit Viewer
===========

This is the full self-contained source code for y-cruncher v0.6.8's integrated Digit Viewer.<br>
It includes everything including the heavily optimized back-end conversion code.

The v0.6.8 integrated Digit Viewer is functionally the same as the v0.6.5 stand-alone Digit Viewer. But it has been rebased a whole bunch of times.
Note that this rebased Digit Viewer has yet to be re-tested. So it may contain bugs. But it will be thoroughly tested before y-cruncher v0.6.8 goes live.

To Compile (Windows):
 - Open the Visual Studio project and compile from within the IDE. The options are x86/Release, x86/SSE3, x64/SSE3, x64/SSE4.1.
 - Visual Studio 2013 is required.

To Compile (Linux):
 - Run `Digit Viewer (VSS)/Linux - Compile All.sh` from the directory it is in. It will create a folder `Binaries` with the two executables.
 - x64 and a sufficiently new version of GCC is required.

-----

To date, I've been too lazy to write a document on the compression format and how the .ycd files are laid out.
But it should be pretty easy to see from just examining it through a hex viewer.

Two things that might be useful:
 - Base 16 .ycd files are stored as 64-bit integers words with 16 digits per word.
 - Base 10 .ycd files are stored as 64-bit integers words with 19 digits per word.

In both cases, each 8-byte word is little-endian.

-----

Directories:

 - `Source Files/Public/` - The open-sourced subset of y-cruncher's support library.
 - `Source Files/DigitViewer/` - The Digit Viewer itself.
 - `Source Files/DigitViewer/DigitReaders/` - The Digit Reader classes.
 - `Source Files/DigitViewer/DigitWriter/` - The Digit Writer classes.
 - `Source Files/DigitViewer/DigitConverter/` - The highly optimized (and unreadable) digit conversion primitives.

y-cruncher v0.6.8 has a build dependency on this Digit Viewer.
Not just because it embeds the Digit Viewer, but it actually uses the DigitWriter classes to output digits at the end of a computation.

So what you are seeing here is actual y-cruncher source code.
