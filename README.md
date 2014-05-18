y-cruncher v0.6.5's Digit Viewer
===========

This is the full self-contained source code for y-cruncher v0.6.5's Digit Viewer.<br>
It includes everything including the heavily optimized back-end conversion code.

To Compile (Windows):
 - Double-click `Windows - Compile All.cmd`.
 - You need to have Visual Studio 2013 installed in the default directory.

To Compile (Linux):
 - Run `Linux - Compile All.sh` from the directory it is in.
 - x64 and a sufficiently new version of GCC is required.

A Visual Studio 2013 project has been provided.<br>
The compiled Windows binaries have also been included.<br>
Running `Digit Viewer.exe` will automatically do CPU-dispatching to select one of the 4 binaries to run.

If you intend to use and modify the code, the classes of interest are in:
[`/source/y-cruncher/DigitViewer`](https://github.com/Mysticial/DigitViewer/tree/master/source/y-cruncher/DigitViewer)<br>
You can see how they are used in
[`source/DigitViewerOptions.h`](https://github.com/Mysticial/DigitViewer/blob/master/source/DigitViewerUI.h)

-----

To date, I've been too lazy to write a document on the compression format and how the .ycd files are laid out.
But it should be pretty easy to see from just examining it through a hex viewer.

Two things that might be useful:
 - Base 16 .ycd files are stored as 64-bit integers words with 16 digits per word.
 - Base 10 .ycd files are stored as 64-bit integers words with 19 digits per word.

In both cases, each 8-byte word is little-endian.

-----

Directories:
 - [y-cruncher/CVN/](https://github.com/Mysticial/DigitViewer/tree/master/Digit%20Viewer%20%28VSS%29/source/y-cruncher) - This directory contains all the heavily optimized conversion code. You probably don't want to look at this. :)
 - [y-cruncher/ConsoleIO/](https://github.com/Mysticial/DigitViewer/tree/master/Digit%20Viewer%20%28VSS%29/source/y-cruncher/ConsoleIO) - This is where the console colors come from.
 - [y-cruncher/DigitViewer/](https://github.com/Mysticial/DigitViewer/tree/master/Digit%20Viewer%20%28VSS%29/source/y-cruncher/DigitViewer) - This has all the Digit Viewer classes.
 - [y-cruncher/FileIO/](https://github.com/Mysticial/DigitViewer/tree/master/Digit%20Viewer%20%28VSS%29/source/y-cruncher/FileIO) - What the names says. These are the file handles that the program uses.
 - [y-cruncher/ProcessorCapability/](https://github.com/Mysticial/DigitViewer/tree/master/Digit%20Viewer%20%28VSS%29/source/y-cruncher/ProcessorCapability) - The CPU feature detection.
 - [y-cruncher/Strings/](https://github.com/Mysticial/DigitViewer/tree/master/Digit%20Viewer%20%28VSS%29/source/y-cruncher/Strings) - Leftover from when y-cruncher was entirely a C program.
