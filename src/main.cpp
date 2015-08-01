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

#include "../nes_emu/Nes_Emu.h"
#include "abstract_file.h"
#include "file_chooser.h"
#include "font.h"
#include <vita2d.h>

PSP2_MODULE_INFO(0, 0, "NES4Vita");

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

unsigned update_input(SceCtrlData *pad)
{
	unsigned res = 0;
	unsigned int keys_down = pad->buttons;

	for (unsigned bind = 0; bind < sizeof(bindmap) / sizeof(bindmap[0]); bind++)
		res |= (keys_down & bindmap[bind].psp2) ? bindmap[bind].nes : 0;

	return res;
}

void show_fps()
{
	clock_t now = clock();
	float curr_fps = 1000000.0F / ((float)now - (float)last_render_time);

	sprintf(fps_string, "FPS: %d", (int)curr_fps);
	font_draw_stringf(10, 10, 0xFFFFFFFF, fps_string);

	last_render_time = now;
}

int run_emu(const char *path)
{
	SceCtrlData pad;
	unsigned int joypad1, joypad2;

	emu->set_sample_rate(44100);
	emu->set_equalizer(Nes_Emu::nes_eq);
	emu->set_palette_range(0);

	vita2d_texture *tex = vita2d_create_empty_texture(Nes_Emu::image_width, Nes_Emu::image_height);
	void *tex_data = vita2d_texture_get_datap(tex);

	static uint32_t video_buffer[Nes_Emu::image_width * Nes_Emu::image_height];
	emu->set_pixels(video_buffer, Nes_Emu::image_width);

	Auto_File_Reader freader(path);
	emu->load_ines(freader);

	int scale = 2;
	int pos_x = SCREEN_W/2 - (Nes_Emu::image_width/2)*scale;
	int pos_y = SCREEN_H/2 - (Nes_Emu::image_height/2)*scale;

	while (1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if ((pad.buttons & CHANGE_GAME_MASK) == CHANGE_GAME_MASK) break;

		joypad1 = joypad2 = update_input(&pad);

		emu->emulate_frame(joypad1, joypad2);
		const Nes_Emu::frame_t &frame = emu->frame();

		const uint8_t *in_pixels = frame.pixels;
		uint32_t *out_pixels = (uint32_t *)tex_data;

		for (unsigned h = 0; h < Nes_Emu::image_height;
			h++, in_pixels += frame.pitch, out_pixels += Nes_Emu::image_width) {
			for (unsigned w = 0; w < Nes_Emu::image_width; w++) {
				unsigned col = frame.palette[in_pixels[w]];
				const Nes_Emu::rgb_t& rgb = emu->nes_colors[col];
				unsigned r = rgb.red;
				unsigned g = rgb.green;
				unsigned b = rgb.blue;
				out_pixels[w] = 0xFF000000 | (r << 0) | (g << 8) | (b << 16);
			}
		}

		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture_scale(tex, pos_x, pos_y, scale, scale);
		show_fps();

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}

	return 0;
}

static const char *supported_ext[] = {
	"nes", NULL
};

int main()
{
	char rom_path[PATH_MAX];

	printf("Starting NES4Vita by SMOKE");

	vita2d_init();

	while (1) {
		emu = new Nes_Emu();

		file_choose(
			"cache0:/VitaDefilerClient/Documents",
			rom_path,
			"Choose a NES ROM:",
			supported_ext
			);

		printf("Loading emulator.... %s", rom_path);
		run_emu(rom_path);

		delete emu;
	}

	vita2d_fini();
	sceKernelExitProcess(0);
	return 0;
}
