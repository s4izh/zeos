#ifndef __GAME_H__
#define __GAME_H__

#define INIT_SHARED 284

#include <libc.h>

extern float frames;

float get_fps();
float get_frames();

void clear_screen();
void welcome_screen();


#endif /* __GAME_H__ */
