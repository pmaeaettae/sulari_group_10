#pragma once

#include <Arduino.h>

typedef enum {
    ST_INIT,
    ST_WAIT_MODE,
    ST_WAIT_START,
    ST_PLAY,
    ST_GAME_OVER,
    ST_DEBUG
} ty_game_state;

typedef struct {
    int hp;
    int deaths;
    int ammo;
} ty_player_stats;

extern ty_player_stats player;


void game_init();
void game_start();
void game_update();
void game_over();
void debug_mode();