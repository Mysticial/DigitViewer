y-cruncher's Digit Viewer
===========

This is the full self-contained source code for y-cruncher's integrated Digit Viewer.<br>
It includes everything including the heavily optimized back-end conversion code.

This Digit Viewer can also be found inside y-cruncher: https://github.com/Mysticial/y-cruncher/tree/master/trunk/VSS%20-%20DigitViewer

-----

## Version 1:

Version 1 is the original DigitViewer that's currently in use by y-cruncher v0.7.5.

**Sources:**

 - Project: `VSS - DigitViewer/`
 - Source: `Source/DigitViewer/`
 - Namespace: `DigitViewer`


**Compile Instructions:**

Windows:
 - Open the Visual Studio project and compile from within the IDE.
 - Visual Studio 2017 is required.

Linux:
 - Run `VSS - DigitViewer/Compile-Linux.sh` from the directory it is in. It will create a folder `Binaries` with the three executables.
 - x64 and a sufficiently new version of GCC is required.

-----

**Build Modes:**

 - x86 : Release
 - x86 : 04-SSE3
 - x64 : 04-SSE3
 - x64 : 07-Penryn (SSE4.1)
 - x64 : 13-Haswell (AVX2)

The x86 modes are not supported in Linux.

-----

## Version 2:

Version 2 is a work-in-progress rewrite of the Digit Viewer slated for y-cruncher v0.7.6. It isn't here yet, since it's not ready. As of this writing, the implementation is mostly done. But it hasn't been properly tested yet.

**Changes:**

 - New namespace: `DigitViewer2`
 - Classes renamed to, `BasicDigitReader/Writer`, `BasicTextReader/Writer`, and `BasicYcdSetReader/Writer`. The "basic" prepending is because they now operate at a lower level.
 - The "iterator" paradigm has been removed for all digit readers and writers. Instead, the new paradigm is random access which is better suited for high-level parallelism in the future.
 - The automatic buffering that is used to support the iterator paradigm has also been removed. Buffering is now explicit.
 - All classes are now thread-safe in anticipation of high-level parallelism in the future.


Because buffering has been removed, the new classes will be harder to use casually in a performant manner. While it's trivial to build a wrapper that re-adds the buffering to support efficient iteration, this is discouraged as processing large amounts of digits one-at-a-time is going to be very slow.


**Optimizations:**

 - All computational operations are now fully parallelized.
 - All computational operations are now fully vectorized for everything from SSE2 through AVX512.
 - Cache blocking optimizations to reduce bandwidth consumption.

y-cruncher v0.7.5 has already picked up a subset of these optimizations when writing out digits to disk at the end of a computation. The rest will be in v0.7.6.


**New Features**

 - Digit hashing is now supported for hexadecimal digits as well.
 - Stats tracking is now done automatically for all digit conversion operations even when it is not requested. The performance improvements from parallelism and vectorization have made the cost negligible.
 - Unfinished `.ycd` files which have not been completely written out are marked with "(incomplete)" in their names.


**To-Do's for the Future:**
 - Enable drive parallelism. When digits are distributed over multiple storage devices, they can be processed in parallel. This will require reading the storage topology. It will also require algorithm to do the actual parallelism when given a specific topology.


-----

**Compressed File Format:**

To date, I've been too lazy to write a document on the compression format and how the .ycd files are laid out.
But it should be pretty easy to see from just examining it through a hex viewer.

Two things that might be useful:
 - Base 16 .ycd files are stored as 64-bit integers words with 16 digits per word.
 - Base 10 .ycd files are stored as 64-bit integers words with 19 digits per word.

In both cases, each 8-byte word is little-endian.

-----

y-cruncher has a build dependency on this Digit Viewer.
Not just because it embeds the Digit Viewer, but it actually uses the DigitWriter classes to output digits at the end of a computation.

So what you are seeing here is actual y-cruncher source code.
