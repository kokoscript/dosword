#include "util.h"

BITMAP *safe_load_bitmap(const char *filename) {
	BITMAP *temp;
	temp = load_bitmap(filename, NULL);
	if (!temp) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		printf(str_err_couldNotLoad, filename);
		exit(1);
	} else {
		return temp;
	}
}

RLE_SPRITE *rle_from_blit(BITMAP *source, unsigned int x, unsigned int y,
						  unsigned int w, unsigned int h) {
	BITMAP *blit = create_bitmap(w, h);
	if (!blit) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		printf(str_err_couldNotMakeRle);
		exit(1);
	}
	clear_to_color(blit, 0);
	masked_blit(source, blit, x, y, 0, 0, w, h);
	RLE_SPRITE *rle;
	rle = get_rle_sprite(blit);
	destroy_bitmap(blit);
	if (!rle) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		printf(str_err_couldNotMakeRle);
		exit(1);
	} else {
		return rle;
	}
}

MIDI *safe_load_midi(const char *filename) {
	MIDI *temp;
	temp = load_midi(filename);
	if (!temp) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		printf(str_err_couldNotLoad, filename);
		exit(1);
	} else {
		return temp;
	}
}

void safe_load_ibk(const char *filename, int drums) {
	if (load_ibk(filename, drums) != 0) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		printf(str_err_couldNotLoad, filename);
		exit(1);
	}
}

void exit_with_error(const char *message) {
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	printf(message);
	exit(1);
}

void exit_with_error_parameter(const char *message, void *parameter) {
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	printf(message, parameter);
	exit(1);
}
