#ifndef MZK_HDR
#define MZK_HDR

//TODO(dlu): remove later if unneeded
#include "config.h"

// define this if you have a multicore cpu and can spare ~10 bytes for realtime playback
// undef for sound precalc
#define USE_SOUND_THREAD

// some song information
#include "4klang.h"
#define MZK_NUMCHANNELS 2

// MAX_SAMPLES gives you the number of samples for the whole song. 
//we always produce stereo samples, so times 2 for the buffer
static SAMPLE_TYPE	lpSoundBuffer[MAX_SAMPLES*2];  
HWAVEOUT	hWaveOut;

// Configurations for our audio
#pragma data_seg(".wavefmt")

static WAVEFORMATEX WaveFMT = {
#ifdef FLOAT_32BIT	
	WAVE_FORMAT_IEEE_FLOAT,
#else
	WAVE_FORMAT_PCM,
#endif	
  MZK_NUMCHANNELS, SAMPLE_RATE, 
  SAMPLE_RATE*sizeof(SAMPLE_TYPE)*2,
  sizeof(SAMPLE_TYPE)*2,
  sizeof(SAMPLE_TYPE)*8,
  0};

#pragma data_seg(".wavehdr")
static WAVEHDR WaveHDR = 
{
	(LPSTR)lpSoundBuffer, 
	MAX_SAMPLES*sizeof(SAMPLE_TYPE)*2,			// MAX_SAMPLES*sizeof(float)*2(stereo)
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

//initialization
#pragma code_seg(".initsnd")
void  InitSound()
{

#ifdef USE_SOUND_THREAD
	// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);
#else
        _4klang_render(lpSoundBuffer);
#endif
       	waveOutOpen( &hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL );
	waveOutPrepareHeader( hWaveOut, &WaveHDR, sizeof(WaveHDR) );

	//gives the render function some time to fill buffer before playback
#ifdef USE_SOUND_THREAD
	Sleep(3000);
#endif

	//sends a block of mem to the sound card
	//card begins playback as soon as mzk is sent
	waveOutWrite( hWaveOut, &WaveHDR, sizeof(WaveHDR) );	
}

#endif //MZK_HDR
