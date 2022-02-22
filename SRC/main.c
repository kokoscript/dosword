#include "eax_game.h"

BEGIN_GFX_DRIVER_LIST
	GFX_DRIVER_VGA
END_GFX_DRIVER_LIST

BEGIN_COLOR_DEPTH_LIST
	COLOR_DEPTH_8
END_COLOR_DEPTH_LIST

BEGIN_MIDI_DRIVER_LIST
END_MIDI_DRIVER_LIST

BEGIN_DIGI_DRIVER_LIST
END_DIGI_DRIVER_LIST

BEGIN_JOYSTICK_DRIVER_LIST
END_JOYSTICK_DRIVER_LIST

int main(int argc, char* argv[]) {
	printf("Loaded: ");

	if (allegro_init() != 0) {
		printf(str_init_couldNotInitialize, str_init_allegro, str_init_contact);
		return 1;
	} else {
		printf("Allegro");
	}

	if (install_keyboard() != 0) {
		printf(str_init_couldNotInitialize, str_init_keyboard, str_init_contact);
		return 1;
	} else {
		printf("...Keyboard");
	}

	if (install_timer() != 0) {
		printf(str_init_couldNotInitialize, str_init_timerSystem, str_init_contact);
		return 1;
	} else {
		printf("...Timers");
	}
	// Enter VGA last so prior setup text stays in cmd
	if (set_gfx_mode(GFX_VGA, 320, 200, 0, 0) != 0) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		printf(str_init_couldNotInitialize, str_init_screen, str_init_vgaReq);
		return 1;
	}

	// todo: move these bad boys
	BITMAP *font_main_bmp;
	BITMAP *font_main_small_bmp;
	font_main_bmp = safe_load_bitmap("data/font_000.bmp");
	font_main_small_bmp = safe_load_bitmap("data/font_001.bmp");
	font_main = grab_font_from_bitmap(font_main_bmp);
	font_main_small = grab_font_from_bitmap(font_main_small_bmp);
	destroy_bitmap(font_main_bmp);
	destroy_bitmap(font_main_small_bmp);

	if (!font_main || !font_main_small) {
		exit_with_error(str_err_couldNotLoadFont);
	}

	dblbuffer = create_bitmap(320, 200);
	
	srand(time(NULL));
	game_ticks = 0;
	exit_state = 0;
	// Default gamestate
	current_state = game;
	enter_game();
	
	do {
		switch (current_state) {
			case game:
				do_game();
				break;
			default:
				// exit and error if the state doesn't exist
				exit_with_error_parameter(str_err_invalidState, &current_state);
		}
		game_ticks += game_tick_step;

		vsync();
		blit(dblbuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		
		if (key[KEY_ESC]) {
			exit_state = 1;
		}
	} while (exit_state == 0);

	destroy_font(font_main);
	destroy_font(font_main_small);
	destroy_bitmap(dblbuffer);
	// do we need to do this??
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	return 0;
}
END_OF_MAIN()
