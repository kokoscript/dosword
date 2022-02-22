#ifndef COMMON_H
#define COMMON_H

#include <allegro.h>
#include "states.h"

// GFX related
BITMAP *dblbuffer;
RGB *white_palette;
RGB *working_pal;

// Fonts
FONT *font_main;
FONT *font_main_small;

// "Engine"
char exit_state;
gamestate_t current_state;
// This should only *ever* be either 1 (60fps) or 2 (30fps). The timers have only been designed around these two values.
static const int game_tick_step = 1;
int game_ticks;
#endif
