// Author: skogaby
// Lots of examples were followed using other emulators found on github

#ifndef _AUDIO_H_
#define _AUDIO_H_

#define AUDIO_SAMPLE_COUNT 512
#define AUDIO_OUTPUT_RATE 32000

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/audioout.h>

// helpers for sound
int vols[2];
int audio_out_handle;

void setup_audio();
size_t retro_audio_sample_batch_callback(const int16_t *data, size_t frames);

#endif