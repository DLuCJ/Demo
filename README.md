Demo
----

Version 1.0 (December 13, 2015)

Note: there is one effect that doesn't work, which relies on conversion of audio fft data to a 2D texture like in Shadertoy.  It's probably because I don't know what I'm doing when it comes to GLSL textures. However, based on my email to IQ (co-author of Shadertoy), it seems like I have the right general idea.

*****

Hello Inigo, 

I saw your explanation for this, but I am new to this so I want to confirm if I have the right idea.

If I have a buffer of int16s for two-channels audio, then I should:

1)  average the sample values in the two channels together and put into array of size 512.

2)  transform the values in this array with fft, to get real and imaginary components for the fft coefficients.

3)  sqrt(re * re + im * im) for each coefficients to get the power for each frequency, and then divide those by the largest magnitude to scale to 0..1, and then multiply each by 255 for the grayscale value. 

4) Then scale the waveform samples (0.5 * int16 for -16k to 16k range?) to 0..1 as well?

5) Store those in a 512x2 texture.

Is this correct?  Or should I do something else too?
Thanks!

And the answer: That is all correct!

*****

Well, I'll try to get it working eventually.


Credits
------- 
Crinkler (exe packer) : http://crinkler.net/

4klang (soft synth) : http://4klang.untergrund.net/

The shaders are all modifications and/or combinations of previously-written shaders.
They are credited in a comment at the top of each, in the \code\shaders\ folder.
The originals can all be found on Shadertoy.com


PLATFORM
--------
This currently runs only on Windows,
although it may be ported to Linux later.  The code
has been tested only on Windows 7, and an earlier version
worked on Windows 10 as well.

OpenGL is used as the 3D graphics API, which should be
already supported on any Windows installation.  So, the demo 
requires GLSL support.  OpenGL is used for
easy prototyping in GLSL sandboxes (e.g Shadertoy), but 
I may convert to D3D in the future.


BUILD
-----
If you just want to run the demo:

To run:
demo\build> win32_plat.exe

To exit:
Press the esc key at any time.  Later, the demo will
also exit whenever the music is finished playing.

Otherwise, you can build a new executable as follows.
The executable will still be in the build directory.

The demo is compiled in x86 mode using the
Microsoft vc++ compiler.

To start up Microsoft cl (assuming standard installation path for Visual Studio):
call c:\"Program Files (x86)"\"Microsoft visual studio < ** INSERT VERSION NUMBER HERE ** >"\vc\vcvarsall.bat x86

For reference, this is what my call looks like:
call c:\"Program Files (x86)"\"Microsoft visual studio 12.0"\vc\vcvarsall.bat x86

To build:
demo\code> build.bat

Any warnings that may appear can probably be safely
ignored, but it's up to user discretion.

Alternatively, the framework can be built using a 
different compiler, but it is up to the user to modify
the build script accordingly.



AREAS OF INTEREST
-----------------
Platform:
Attempts window creation, Windows message handling, OGL
initialization, and some preliminary CRT handling in as
small a size (final executable size) as possible.  

Also controls audio and video startup/shutdown.

Video:
The relatively interesting video code is all in the code\shaders
directory.  Visuals.cpp does some initialization work
to get the shaders running.

Audio:
If you want to use the included 3rd party synth to generate
music, see usage description in the 3RD PARTY SOFTwARE section.
Otherwise, comment out the USE_SYNTH macro in win32_plat.cpp
and the audio will be generated from the formula in othermzk.h.
You can also write your own formula and test it out, but it takes
a little patience to find a good result. 

Note that music generated using the 3rd party synth can be either
precalculated before the video starts up, or it can be calculated
in parallel (real-time).  Precalculation requires a few hundred
bytes in extra executable size, however.  This option is toggled
by a macro define in mzk.h, assuming USE_SYNTH is defined. 

Music generated formulaically in othermzk.h is always precalculated, 
for now.

3RD PARTY SOFTWARE
----------------------------------------
Crinkler : exe packer 
4klang   : software synth

Crinkler is currently not being used, so the executable
size is in the ballpark of 64 kb including visuals and music.


The audio is currently being generated by 4klang synth.
Music is written in Renoise DAW and exported through the 
4klang VSTi plugin, resulting in a 4klang.h file and a 
4klang.inc file, which hold useful defines and song data.

The 4klang.inc file should be compiled along with 4klang.asm,
which holds the synth impl, together into 4klang.obj, using
NASM assembler. The obj file can then be linked to the 
demo executable.  If you want to write your own 4klang song,
then you should generate new 4klang.inc and 4klang.h files
and assemble the 4klang.inc/4klang.asm into a new 4klang.obj.

Then place 4klang.h and 4klang.obj into the code directory
and run build.bat.
