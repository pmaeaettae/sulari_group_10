#include "game.h"
#include "config.h"

ty_player_stats player

void game_init() {
    player.hp = MAX_HP;
    player.deaths = 0;
    player.ammo MAX_AMMO;
}

void game_start() {
    display_show("Game starts");
}

void handle_shoot() {
    
}