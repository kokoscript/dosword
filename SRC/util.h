#ifndef UTIL_H
#define UTIL_H

#include <allegro.h>
#include <stdio.h>
#include "gamestrs.h"

RLE_SPRITE *rle_from_blit(BITMAP *source, unsigned int x, unsigned int y,
						  unsigned int w, unsigned int h);
BITMAP *safe_load_bitmap(const char *filename);
MIDI *safe_load_midi(const char *filename);
void safe_load_ibk(const char *filename, int drums);
void exit_with_error(const char *message);
void exit_with_error_parameter(const char *message, void* parameter);

#endif
