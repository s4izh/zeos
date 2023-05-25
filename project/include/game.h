#ifndef __GAME_H__
#define __GAME_H__

#include <libc.h>

extern float frames;

float get_fps();

void draw_hangman(int tries);

void clear_hangman();

void clear_screen();

void welcome_screen();

void marco();

void initial_screen();

void init_reader();

void game_loop();

void init_game();

#endif /* __GAME_H__ */
