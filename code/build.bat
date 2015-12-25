@echo off

REM FOR STORAGE
REM d3d11.lib 

set CommonCompilerFlags= -QIfist -O1 -Gr -GS- -Gs999999999 -nologo -Gm- -GR- -EHs-c-a- -Oi -WX -W4 -wd4244 -wd4201 -wd4100 -wd4189 -FC -Z7
set CommonLinkerFlags= -opt:ref kernel32.lib user32.lib gdi32.lib winmm.lib opengl32.lib msvcrt.lib 

REM TO BUILD MZK (requires nasm, 4klang .asm and .inc)
REM nasmw.exe -fwin32 -o"4klang.obj" 4klang.asm

REM *** HERE ARE THE LATER SIZE OPTS FLAGS, DONT USE RN
REM TODO - can we just build both with one exe?

IF NOT EXIST ..\..\build mkdir ..\build
pushd ..\build

REM 32-bit build
REM cl %CommonCompilerFlags% ..\code\win32_plat.cpp ..\code\4klang.obj /link -subsystem:windows,5.1 /NODEFAULTLIB /ENTRY:"entrypoint" %CommonLinkerFlags% 

REM 32-bit GUI build
cl %CommonCompilerFlags% -DBLOAT_IS_GROSS=1 ..\code\win32_plat.cpp ..\code\4klang.obj ..\code\very_last_engine_ever.res /link -subsystem:windows,5.1 /NODEFAULTLIB /ENTRY:"entrypoint" %CommonLinkerFlags% 










REM 64-bit build
REM cl %CommonCompilerFlags% ..\code\win32_plat.cpp ..\code\4klang.obj /link -subsystem:windows /NODEFAULTLIB:libcmt.lib %CommonLinkerFlags%
REM 
popd

