#ifndef MAIN_H
#define MAIN_H

bool init();
void init_sdl();
void init_font();
void init_img();
void init_music();
void main_loop();
void title();
void game_start();
void play_game();
void game_clear();
void game_miss();
void game_over();
void game_pause();
void draw_score();
int poll_event();
void wait_game();
void draw_fps();
void end();
void end_music();
void draw_translucence();
int get_distance(int x1, int y1, int x2, int y2);

#endif
