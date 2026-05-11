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
    int id;
    int hp;
    int deaths;
    int hits;
    int ammo;
} ty_player_stats;

// Global player stats
extern ty_player_stats player;

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