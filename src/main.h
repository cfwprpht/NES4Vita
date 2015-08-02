#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <time.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>

#include "Nes_Emu.h"
#include "abstract_file.h"
#include "file_chooser.h"
#include "font.h"

#define SCREEN_W 960
#define SCREEN_H 544

const SceSize sceUserMainThreadStackSize = 8*1024*1024;

static Nes_Emu *emu;

static int scale;

unsigned int *framebuffer;
vita2d_texture *framebufferTex;

clock_t last_render_time;
char fps_string[11];

static uint8_t nes_width = 160;
static uint8_t nes_height = 102;

struct keymap { unsigned psp2; unsigned nes; };

#define CHANGE_GAME_MASK (PSP2_CTRL_TRIANGLE | PSP2_CTRL_LTRIGGER)

#define JOY_A           1
#define JOY_B           2
#define JOY_SELECT      4
#define JOY_START       8
#define JOY_UP       0x10
#define JOY_DOWN     0x20
#define JOY_LEFT     0x40
#define JOY_RIGHT    0x80

static const keymap bindmap[] = {
   { PSP2_CTRL_CIRCLE, JOY_A },
   { PSP2_CTRL_CROSS,  JOY_B },
   { PSP2_CTRL_SELECT, JOY_SELECT },
   { PSP2_CTRL_START,  JOY_START },
   { PSP2_CTRL_UP,     JOY_UP },
   { PSP2_CTRL_DOWN,   JOY_DOWN },
   { PSP2_CTRL_LEFT,   JOY_LEFT },
   { PSP2_CTRL_RIGHT,  JOY_RIGHT },
};

static const char *supported_ext[] = {
	"nes", NULL
};

#endif
