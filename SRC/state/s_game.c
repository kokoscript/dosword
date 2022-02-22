#include "s_game.h"

RLE_SPRITE *game_sprites[5];

enum tile_state {
	TILE_BLANK = 0,
	TILE_INCORRECT = 1,
	TILE_INWORD = 2,
	TILE_INSPOT = 3
};
typedef enum tile_state tile_state_t;

tile_state_t tile_states[6][5];	// current states of the tiles
char tile_chars[6][6];			// character guesses
int win_offsets[5];				// offsets for win animation
int invalid_offset = 0;			// offset for invalid word animation

int num_guesses = 0;			// the current number of guesses
const char *answer;				// today's word
int day_num;					// number of days since the Epoch of Wordle
int input_loc = 0;				// char input location
int check_state = 0;			// 1 = do checking animation, -1 = do invalid word animation
int check_loc = 0;				// character index to check, or countdown to end invalid word anim
int num_missed_letter = 0;		// number of missed letters
char missed_letter[26];			// the missed letters
int enable_input = 1;			// your guess is as good as mine
int end_state = 0;				// 1 = win, -1 = lose

// this is bad im sorry
int days_since_start() {
	time_t now;
	struct tm *epoch;

	time(&now);
	epoch = localtime(&now);
	epoch->tm_year = 2021 - 1900;
	epoch->tm_mon = 5;
	epoch->tm_mday = 17;
	epoch->tm_hour = 0;
	epoch->tm_min = 0;
	epoch->tm_sec = 0;
	time_t epoch2 = mktime(epoch);

	return (int)((now - epoch2)/86400);
}

// returns 1 if word is in the game dict, 0 if not
int check_in_dict() {
	int start = 0;
	int end = GAME_DICT_LEN - 1;
	int mid, cmp;

	while (start <= end) {
		mid = (start + end) / 2;
		cmp = strcmp(tile_chars[num_guesses], game_dict[mid]);
		if (cmp == 0)
			return 1;
		else if (cmp < 0)
			end = mid - 1;
		else
			start = mid + 1;
	}
	return 0;
}

// returns 1 if word is in the answer list, 0 if not
int check_in_answers() {
	// unfortunately can't do binary search here, though a seperate sorted answer list could work...
	int i;
	for (i = 0; i < ANSWER_LEN; i++) {
		if (strcmp(tile_chars[num_guesses], answers[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

// returns 1 if the letter to check is in the answer, 0 if not
int letter_is_in_answer(char check) {
	int i;
	for (i = 0; i < 5; i++) {
		if (answer[i] == check) return 1;
	}
	return 0;
}

// returns 1 if the letter was already missed; 0 if not
int letter_is_missed(char check) {
	int i;
	for (i = 0; i < num_missed_letter; i++) {
		if (missed_letter[i] == check) return 1;
	}
	return 0;
}

// check the character at index check_loc against the answer
void check_against_answer() {
	if (tile_chars[num_guesses][check_loc] == answers[day_num][check_loc]) {
		tile_states[num_guesses][check_loc] = TILE_INSPOT;
	} else if (letter_is_in_answer(tile_chars[num_guesses][check_loc])) {
		tile_states[num_guesses][check_loc] = TILE_INWORD;
	} else {
		tile_states[num_guesses][check_loc] = TILE_INCORRECT;
		if (!letter_is_missed(tile_chars[num_guesses][check_loc])) {
			missed_letter[num_missed_letter] = tile_chars[num_guesses][check_loc];
			num_missed_letter++;
		}
	}
	check_loc++;
	if (check_loc == 5) {
		check_loc = 0;
		check_state = 0;
		if (strcmp(tile_chars[num_guesses], answer) != 0 && num_guesses < 5) {
			num_guesses++;
			input_loc = 0;
			enable_input = 1;
		} else if (strcmp(tile_chars[num_guesses], answer) != 0 && num_guesses == 5) {
			end_state = -1;
			enable_input = 0;
		} else if (strcmp(tile_chars[num_guesses], answer) == 0) {
			end_state = 1;
			enable_input = 0;
		}
	}
}

void enter_game() {
	// Convert main bitmap to rle sprites
	BITMAP *sprites_atlas = safe_load_bitmap("data/sprites.bmp");
	int i;

	// Game sprites
	for (i = 0; i < 4; i++) {
		game_sprites[i] = rle_from_blit(sprites_atlas, i * 24, 0, 24, 24);
	}
	game_sprites[4] = rle_from_blit(sprites_atlas, 0, 24, 10, 16);
	destroy_bitmap(sprites_atlas);

	day_num = days_since_start();
	answer = answers[day_num];
}

void do_game() {
	// Drawing events
	int i, j;
	clear_to_color(dblbuffer, 19);
	textout_centre_ex(dblbuffer, font_main, str_game_title, 160, 10, 15, -1);
	textout_ex(dblbuffer, font_main_small, str_game_cred1, 1, 160, 22, -1);
	textout_ex(dblbuffer, font_main_small, str_game_cred2, 1, 170, 22, -1);
	textout_ex(dblbuffer, font_main_small, str_game_cred3, 1, 180, 22, -1);
	textout_ex(dblbuffer, font_main_small, str_game_cred4, 1, 190, 22, -1);

	// Missed letters
	textout_ex(dblbuffer, font_main_small, str_game_missedLetters, 224, 33, 15, -1);
	fastline(dblbuffer, 224, 41, 295, 41, 15);
	for (i = 0; i < num_missed_letter; i++) {
		textprintf_ex(dblbuffer, font_main_small, 224 + ((i % 4) * 10), 43 + ((i / 4) * 12), 15, -1, "%c", missed_letter[i]);
	}

	// Arrow
	if (enable_input && num_guesses < 6) {
		draw_rle_sprite(dblbuffer, game_sprites[4], 84, 36 + (25 * num_guesses));
	}

	// Game tiles
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 5; j++) {
			if (i == num_guesses) {
				draw_rle_sprite(dblbuffer, game_sprites[tile_states[i][j]], 98 + (j * 25) + invalid_offset, 32 + (i * 25) + win_offsets[j]);
				textprintf_centre_ex(dblbuffer, font_main, 110 + (j * 25) + invalid_offset, 38 + (i * 25) + win_offsets[j], 15, -1, "%c", tile_chars[i][j]);
			} else {
				draw_rle_sprite(dblbuffer, game_sprites[tile_states[i][j]], 98 + (j * 25), 32 + (i * 25));
				textprintf_centre_ex(dblbuffer, font_main, 110 + (j * 25), 38 + (i * 25), 15, -1, "%c", tile_chars[i][j]);
			}
		}
	}

	textprintf_ex(dblbuffer, font_main_small, 10, 10, 15, -1, "#%d", day_num - 2);

	// Delay checking each character deliberately
	if (check_state == 1) {
		if (game_ticks % 24 == 0) {
			check_against_answer();
		}
	// Invalid char animation
	} else if (check_state == -1) {
		invalid_offset = sin((game_ticks) / 2.0) * 6;
		check_loc -= game_tick_step;
		if (check_loc < 0) {
			check_loc = 0;
		}
		if (check_loc == 0) {
			check_state = 0;
			enable_input = 1;
			invalid_offset = 0;
		}
	}

	// End state (win/lose) text
	if (end_state == 1) {
		int i;
		for (i = 0; i < 5; i ++) {
			win_offsets[i] = (sin((game_ticks - (i * 8)) / 10.0) * 6);
		}
		switch (num_guesses) {
			case 0:
			textout_centre_ex(dblbuffer, font_main_small, str_game_end1, 160, 187, 15, -1);
			break;
			case 1:
			textout_centre_ex(dblbuffer, font_main_small, str_game_end2, 160, 187, 15, -1);
			break;
			case 2:
			textout_centre_ex(dblbuffer, font_main_small, str_game_end3, 160, 187, 15, -1);
			break;
			case 3:
			textout_centre_ex(dblbuffer, font_main_small, str_game_end4, 160, 187, 15, -1);
			break;
			case 4:
			textout_centre_ex(dblbuffer, font_main_small, str_game_end5, 160, 187, 15, -1);
			break;
			case 5:
			textout_centre_ex(dblbuffer, font_main_small, str_game_end6, 160, 187, 15, -1);
			break;
			default:
			textout_centre_ex(dblbuffer, font_main_small, str_game_end5, 160, 187, 15, -1);
			break;
		}
	} else if (end_state == -1) {
		textout_centre_ex(dblbuffer, font_main_small, answer, 160, 187, 15, -1);
	}

	// Input events
	int temp_char;
	while (keypressed() && num_guesses < 6 && enable_input) {
		temp_char = readkey() % 256; // don't even ask
		
		if ((temp_char >= 97 && temp_char <= 122) && input_loc < 5) { // alphabet
			tile_chars[num_guesses][input_loc] = temp_char;
			input_loc++;
		} else if (temp_char == 8) { // backspace
			tile_chars[num_guesses][input_loc-1] = 0;
			if (input_loc > 0) input_loc--;
		} else if (temp_char == 13 && input_loc == 5) { // return
			tile_chars[num_guesses][5] = 0; // null terminate
			if (check_in_dict() == 1 || check_in_answers() == 1) {
				check_state = 1;
				enable_input = 0;
				check_against_answer();
			} else {
				check_state = -1;
				check_loc = 40;
				enable_input = 0;
			}
		}
	}
}