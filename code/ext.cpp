//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

//--- d a t a ---------------------------------------------------------------

#include "ext.h"

static char *strs[] = {
	"glCreateProgram",
	"glCreateShader",
	"glShaderSource",
	"glCompileShader",
	"glAttachShader",
	"glLinkProgram",
	"glUseProgram",
    "glUniform4fv",
    "glUniform1i",
    "glGetUniformLocation",
    //--
    #ifdef DEBUG
	"glGetObjectParameterivARB",
	"glGetInfoLogARB",
    #endif
    };

void *myglfunc[NUMFUNCIONES];

int EXT_Init( void )
{
    for( int i=0; i<NUMFUNCIONES; i++ )
    {
        #ifdef WIN32
        myglfunc[i] = wglGetProcAddress( strs[i] );
        #endif
        #ifdef LINUX
        myglfunc[i] = glXGetProcAddress( (const unsigned char *)strs[i] );
        #endif
        if( !myglfunc[i] )
			return( 0 );
    }
    return( 1 );
}


