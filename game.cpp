#include "game.h"
#include "defs.h"
#include "display.h"
#include "ir.h"
#include "gyro.h"
//#include "wireless.h"
#include "gpio.h"

// Global player stats
ty_player_stats player;

// Static variables for timing
static uint32_t last_shot_time = 0;

// Wait until the player has stayed stationary for the full cooldown window.
static void wait_for_respawn() {
    uint32_t stationary_start = 0;

    display_show("KO!");

    while (true) {
        // Check if player has stayed still for the full cooldown period to respawn
        if (mpu_stationary()) {
            if (stationary_start == 0) {
                stationary_start = millis();
            }

            if (millis() - stationary_start >= DEATH_COOLDOWN) {
                return;
            }
        } else {
            stationary_start = 0;
            display_show("Don't Move!");
        }

        delay(50);
    }
}

// Initialize game and player stats
void game_init() {
    player.id = player_id();
    player.hp = MAX_HP;
    player.deaths = 0;
    player.hits = 0;
    player.ammo = MAX_AMMO;
    last_shot_time = 0;
}

// Starting a new game
void game_start() {
    player.hp = MAX_HP;
    player.ammo = MAX_AMMO;
    player.hits = 0;
    player.deaths = 0;
    player.id = player_id();

    status_leds();
    display_show("Game starts");
}

// Main game loop
void game_update() {
    // Update player ID in case of changes
    player.id = player_id();
    
    // Check for incoming shots (decoder consumes one pulse event)
    int hit_player_id = ir_get_player_id();
    if (hit_player_id != 0) {
        game_handle_hit(hit_player_id);
    }

    // Update display with current stats
    display_update(player.ammo, player.hp);

    // Trigger: shoot IR pulse and add cooldown between shots
        if (trigger_pressed()) {
            if (millis() - last_shot_time > SHOOT_COOLDOWN) {
                if (player.ammo > 0) {
                    ir_shoot(player.id);
                    player.ammo--;
                    last_shot_time = millis();
                    buzzer(50);
                }
            }
        }

    // Reload: reset ammo
    if (reload_pressed()) {
        player.ammo = MAX_AMMO;
    }

}

// Hit handling
void game_handle_hit(int player_id) {
    player.hp--;
    buzzer(100);
    rumble_motor_on();
    delay(200);
    rumble_motor_off();

    if (player.hp <= 0) {
        player.deaths++;
        // Player must stay still for the full cooldown period to respawn.
        wait_for_respawn();
        player.hp = MAX_HP;
    }
}

// Game over after MAX_DEATHS
bool game_over() {
    if (player.deaths >= MAX_DEATHS) {
        display_show("GAME OVER");
        delay(3000);
        return true;
    }
    return false;
}

// Debug mode
void debug_mode() {
    display_show("DEBUG MODE");
}