// Author: skogaby
// Lots of examples were followed using other emulators found on github

#include "audio.h"


/***
 * Basic audio initialization.
 */
void setup_audio()
{
	// setup the audio callback
	vols[0] = 0x8000;
	vols[1] = 0x8000;
	audio_out_handle = sceAudioOutOpenPort(PSP2_AUDIO_OUT_PORT_TYPE_VOICE, AUDIO_SAMPLE_COUNT, 
		AUDIO_OUTPUT_RATE, PSP2_AUDIO_OUT_MODE_STEREO);

	if(audio_out_handle < 0)
	{
		printf("Couldn't open audio port. Return value: %i. Bailing.", audio_out_handle);
		sceKernelExitProcess(0);
	}
}
