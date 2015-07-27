/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#ifndef FONT_H
#define FONT_H

#include <psp2/types.h>

#define RGBA8(r, g, b, a)      ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

#define SCREEN_W 960
#define SCREEN_H 544

#define RED   RGBA8(255,0,0,255)
#define GREEN RGBA8(0,255,0,255)
#define BLUE  RGBA8(0,0,255,255)
#define BLACK RGBA8(0,0,0,255)
#define WHITE RGBA8(255,255,255,255)

void font_draw_char(int x, int y, unsigned int color, char c);
void font_draw_string(int x, int y, unsigned int color, const char *string);
void font_draw_stringf(int x, int y, unsigned int color, const char *s, ...);

#endif
