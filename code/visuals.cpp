//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

//NOTE: adapted from example by iq, as credited above

#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include "config.h"
#include "ext.h"
#include "ext.cpp"
#include "shaders/toyfsh_boy.inl"
#include "shaders/toyfsh_loz.inl"
#include "shaders/toyfsh_flap.inl"
#include "shaders/toyfsh_mrio.inl"
#include "shaders/toyfsh_kali.inl"
//#include "shaders/toyvsh.inl"
#include "fp.h"
#include "visuals.h"
#include "rvfft.cpp"
#include "mzk.h"

#ifdef USE_SYNTH
#include "4klang.h"
#endif

#define NUM_SCENES 5
static int scenecheck = 0;

//=================================================================================================================
//static int vsId;
/*
static void InitVsh(const char *vs)
{
  vsId = oglCreateShader(GL_VERTEX_SHADER);
  oglShaderSource( vsId, 1, &vs, 0 );
  oglCompileShader( vsId );
}
*/

static void InitShader( int *pid, const char *fs )
{
  scenecheck++;
  pid[0] = oglCreateProgram();                           
  const int fsId = oglCreateShader( GL_FRAGMENT_SHADER );
  oglShaderSource( fsId, 1, &fs, 0 );
  oglCompileShader( fsId );
  oglAttachShader( pid[0], fsId );
  //oglAttachShader( pid[0], vsId );
  oglLinkProgram( pid[0] );
  
#ifdef DEBUG
  int		result;
  char    info[1536];
  oglGetObjectParameteriv( vsId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); oglGetInfoLog( vsId,   1024, NULL, (char *)info ); if( !result ) DebugBreak();
  oglGetObjectParameteriv( fsId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); oglGetInfoLog( fsId,   1024, NULL, (char *)info ); if( !result ) DebugBreak();
  oglGetObjectParameteriv( pid[0], GL_OBJECT_LINK_STATUS_ARB,    &result ); oglGetInfoLog( pid[0], 1024, NULL, (char*)info ); if( !result ) DebugBreak();
#endif
}

//=================================================================================================================
static int pid_arr[NUM_SCENES];

//=================================================================================================================
#define SAMPLES_TO_PROCESS 4194304   //largest power of two less than MAX_SAMPLES (2^22)
static real64 lpFftbuf[SAMPLES_TO_PROCESS];
static real64 lpFftPows[SAMPLES_TO_PROCESS/2];
static real64 lpNormWaveSamps[SAMPLES_TO_PROCESS/2];

int InitIntro( void )
{
  if( !EXT_Init() )
    return( 0 );    
  
  //InitVsh(vsh_2d);  
  for(int i = 0; i < SAMPLES_TO_PROCESS;++i) {
    int samp_idx = 2*i;
    SAMPLE_TYPE samp_sum = lpSoundBuffer[samp_idx]+lpSoundBuffer[samp_idx+1];
    lpFftbuf[i] = 0.5 * (real64)samp_sum;
  }

  //Sorensen in-place split-radix FFT
  realfft_split_unshuffled(lpFftbuf,SAMPLES_TO_PROCESS);

  lpFftPows[0] = lpFftbuf[0];
  for(int i = 1; i < SAMPLES_TO_PROCESS / 2; ++i) {
    int samp_idx = 2*i;
    lpFftPows[i] = sqrt(lpFftbuf[samp_idx]*lpFftbuf[samp_idx]+
			lpFftbuf[samp_idx+1]*lpFftbuf[samp_idx+1]);
  }

  real64 max = 0.0;
  for(int i = 0; i < SAMPLES_TO_PROCESS/2; ++i) {
    if(lpFftPows[i] > max)
      max = lpFftPows[i];
  }

  for(int i = 0; i < SAMPLES_TO_PROCESS/2; ++i) {
    lpFftPows[i] = lpFftPows[i] / max;
  }

  real64 max2 = 0.0;
  for(int i = 0; i < SAMPLES_TO_PROCESS/2; ++i) {
    int samp_idx = 2*i;
    SAMPLE_TYPE samp_sum = lpSoundBuffer[samp_idx]+lpSoundBuffer[samp_idx+1];
    lpNormWaveSamps[i] = (real64)samp_sum * 0.5;
    if(fabs(lpNormWaveSamps[i]) > max2)
      max2 = fabs(lpNormWaveSamps[i]);
  }

  for(int i = 0; i < SAMPLES_TO_PROCESS/2; ++i) {
    lpNormWaveSamps[i] /= max2;
  }

  InitShader(&pid_arr[0],fsh_boy);
  InitShader(&pid_arr[1],fsh_loz);
  InitShader(&pid_arr[2],fsh_flap);
  InitShader(&pid_arr[3],fsh_mrio);
  InitShader(&pid_arr[4],fsh_kali);
  Assert(scenecheck == NUM_SCENES);

  return 1;
}

//=================================================================================================================

static void InitShadUniforms(int pid, float t)
{
  GLint loc = oglGetUniformLocation(pid,"iResolution");
  ((PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f"))(loc, win_xres,win_yres, p1d00); //1.0 : p1d00
  
  GLint loc2 = oglGetUniformLocation(pid,"iGlobalTime");
  ((PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f"))(loc2,t);

  GLint loc3 = oglGetUniformLocation(pid,"BPM");
  ((PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f"))(loc3,BPM);

}

void intro_do( long time, DWORD sample  )
{
  glViewport(0,0,win_xres,win_yres);
  glClear(GL_COLOR_BUFFER_BIT);
  
  const float t = 0.001f*(float)time;

  //time in milliseconds
  if(time <= 63000){
    GLuint tex;
    glGenTextures(1,&tex);
    ((PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture"))(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, tex);
      
    oglUseProgram( pid_arr[0] );  
    if(time > 23000 && time <= 44000){
      /*
      uint8 signal[1024];
      for(int i = 0; i < 512; ++i) {
	signal[i] = (uint8)(lpFftPows[i+sample]*255);
	signal[i+512]= (uint8)((lpNormWaveSamps[i+sample]*0.5+0.5)*255.0);
      }
      */
      
      uint8 signal[1024];
      for(int i = 0; i < 1024; ++i) {
	signal[i] = 255;
      }
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 2, 0, GL_RED, GL_UNSIGNED_BYTE, signal);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      
    }
    InitShadUniforms(pid_arr[0],t);
    GLint loc4 = oglGetUniformLocation(pid_arr[0],"iChannel0");
    ((PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i"))(loc4,0);
    
  }
  else if(time > 63000 && time <= 75000) {
    oglUseProgram( pid_arr[1] );  
    InitShadUniforms(pid_arr[1],t);
  } 
  else if(time > 75000 && time <= 80000) {
    oglUseProgram( pid_arr[2] );  
    InitShadUniforms(pid_arr[2],t);
  }
  else if(time > 80000 && time <= 105000) {
    oglUseProgram( pid_arr[3] );  
    InitShadUniforms(pid_arr[3],t);
  }
  else {
    oglUseProgram( pid_arr[4] );  
    InitShadUniforms(pid_arr[4],t);
  }

  glBegin(GL_TRIANGLE_STRIP);
  glVertex2f(-p1d00, -p1d00); 
  glVertex2f(-p1d00, p1d00);
  glVertex2f(p1d00, -p1d00); 
  glVertex2f(p1d00, p1d00);
  glEnd();
}
