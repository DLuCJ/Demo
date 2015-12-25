#ifndef OTHERMZK_H
#define OTHERMZK_H

#include <math.h>
#include "config.h"

#define SamplesPerSec 8000
#define BytesPerSmpl sizeof(int16)*2
#define MZK_NUMCHANNELS 2
#define SecondsRunTime 180
#define MAX_SAMPLEZ SamplesPerSec*SecondsRunTime

int16 lpSoundBuffer[SamplesPerSec*SecondsRunTime*BytesPerSmpl];
HWAVEOUT	hWaveOut;

// Configurations for our audio
static WAVEFORMATEX WaveFMT = {WAVE_FORMAT_PCM, MZK_NUMCHANNELS, SamplesPerSec, 
				  SamplesPerSec*BytesPerSmpl,
				  BytesPerSmpl,
				  16,
				  0};

#pragma data_seg(".wavehdr")
WAVEHDR WaveHDR = 
{
	(LPSTR)lpSoundBuffer, 
        MAX_SAMPLEZ*BytesPerSmpl,
	0, 
	0, 
	0, 
	0, 
	0, 
	0
};

//handle to time
MMTIME MMTime = 
{ 
	TIME_SAMPLES,
	0
};

void RenderSound(int16 *buffer)
{
 static int t = 0;
  
  //This controls how loud the mzk is
  int16 ToneVolume = 200;

  //Get a pointer to the beginner of the sound buffer
  int16 *SampleOut = buffer;

  for(int SampleIndex = 0;
      SampleIndex < SamplesPerSec*SecondsRunTime;
      ++SampleIndex)
    {

      //DA FORMULAZ
      int16 yval = t & 4095;
      int16 xval = t*(15 & 0x9866 >> (t >> 12 & 12)) / 6 & 127;
      
      real32 val = (sinf(2000.0f / yval)) * 25 + xval * yval / 10000.0f + 
	((t >> 6 ^ t >> 8 | t >> 12 | xval )& 63);

      //WRITE 2 SOUND BUFFER
      int16 SampleValue = (int16) (val * ToneVolume);
      *SampleOut++ = SampleValue;
      *SampleOut++ = SampleValue;
      
      t++;
    }
}

//initialization
#pragma code_seg(".initsnd")
void  InitSound()
{
        RenderSound(lpSoundBuffer);

       	waveOutOpen( &hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL );
	waveOutPrepareHeader( hWaveOut, &WaveHDR, sizeof(WaveHDR) );

	//sends a block of mem to the sound card
	//card begins playback as soon as mzk is sent
	waveOutWrite( hWaveOut, &WaveHDR, sizeof(WaveHDR) );	
}

#endif
