#pragma once

#include <Arduino.h>

// Game states
typedef enum {
    ST_INIT,
    ST_WAIT_MODE,
    ST_WAIT_START,
    ST_PLAY,
    ST_GAME_OVER,
    ST_DEBUG
} ty_game_state;

// Game stats
typedef struct {
    int id1;
    int hp1;
    int deaths1;
    int hits1;
    int ammo1;
} ty_player_1_stats;

typedef struct {
    int id2;
    int hp2;
    int deaths2;
    int hits2;
    int ammo2;
} ty_player_2_stats;

// Global player stats for both players.
extern ty_player_1_stats player_1;
extern ty_player_2_stats player_2;

// Initialize game state and player stats
void game_init();

// Starting a new game
void game_start();

// Main game loop
void game_update();

// Handle hits
void game_handle_hit(int player_id);

// Check if game is over
bool game_over();

// Game over stats
void game_over_stats();

// Debug mode
void debug_mode();