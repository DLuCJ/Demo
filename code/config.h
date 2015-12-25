#include <stdint.h>
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

extern int win_xres;
extern int win_yres;
extern int win_xpos;
extern int win_ypos;

#ifndef Assert
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#endif

#define DEFAULT_FULLSCREEN 1
extern int fullscreen;
