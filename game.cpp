#include "game.h"
#include "defs.h"
#include "display.h"
#include "ir.h"
#include "gyro.h"
#include "wireless.h"
#include "gpio.h"

// Global player stats
ty_player_1_stats player_1;
ty_player_2_stats player_2;

// Static variables for timing
static uint32_t last_shot_time = 0;

// Wait until the player has stayed stationary for the full cooldown window.
static void wait_for_respawn() {
    uint32_t stationary_start = 0;

    display_show("KO!");
    delay(8000);
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
    player_1.id1 = 1;
    player_1.hp1 = MAX_HP;
    player_1.deaths1 = 0;
    player_1.hits1 = 0;
    player_1.ammo1 = MAX_AMMO;

    player_2.id2 = 2;
    player_2.hp2 = MAX_HP;
    player_2.deaths2 = 0;
    player_2.hits2 = 0;
    player_2.ammo2 = MAX_AMMO;

    last_shot_time = 0;
}

// Starting a new game
void game_start() {
    player_1.hp1 = MAX_HP;
    player_1.ammo1 = MAX_AMMO;
    player_1.hits1 = 0;
    player_1.deaths1 = 0;
    player_1.id1 = 1;

    player_2.id2 = 2;
    player_2.hp2 = MAX_HP;
    player_2.deaths2 = 0;
    player_2.hits2 = 0;
    player_2.ammo2 = MAX_AMMO;

    status_leds();
}

// Main game loop
void game_update() {
    // Update player ID in case of changes
    int current_player = player_id();
    
    // Check for incoming shots (decoder consumes one pulse event)
    int hit_player_id = ir_get_player_id();
    if (hit_player_id != 0) {
        game_handle_hit(hit_player_id);
        mqtt_stats();
    }

    // Update display with current stats
    display_update(player_1.ammo1, player_1.hp1, player_2.ammo2, player_2.hp2);

    // Trigger: shoot IR pulse and add cooldown between shots
        if (trigger_pressed()) {
            if (millis() - last_shot_time > SHOOT_COOLDOWN) {
                if (current_player == 1) {
                    if (player_1.ammo1 > 0) {
                        ir_shoot(player_1.id1);
                        player_1.ammo1--;
                        last_shot_time = millis();
                        buzzer(50);
                        mqtt_stats();
                    }
                } else if (current_player == 2) {
                    if (player_2.ammo2 > 0) {
                        ir_shoot(player_2.id2);
                        player_2.ammo2--;
                        last_shot_time = millis();
                        buzzer(50);
                        mqtt_stats();
                    }
                }
            }
        }

    // Reload: reset ammo
    if (reload_pressed()) {
        if (current_player == 1) {
            player_1.ammo1 = MAX_AMMO;
        } else if (current_player == 2) {
            player_2.ammo2 = MAX_AMMO;
        }
    }

}

// Hit handling
void game_handle_hit(int player_id) {
    if (player_id == 1) {
        // Player 1 shot, so player 2 takes damage
        player_1.hits1++;
        player_2.hp2--;
    } else if (player_id == 2) {
        // Player 2 shot, so player 1 takes damage
        player_2.hits2++;
        player_1.hp1--;
    } else {
        return;
    }

    buzzer(100);
    rumble_motor_on();
    delay(200);
    rumble_motor_off();

    if (player_1.hp1 <= 0) {
        player_1.deaths1++;
        // Player must stay still for the full cooldown period to respawn.
        wait_for_respawn();
        player_1.hp1 = MAX_HP;
    }

    if (player_2.hp2 <= 0) {
        player_2.deaths2++;
        // Player must stay still for the full cooldown period to respawn.
        wait_for_respawn();
        player_2.hp2 = MAX_HP;
    }
}

// Game over after MAX_DEATHS
bool game_over() {
    if (player_1.deaths1 >= MAX_DEATHS || player_2.deaths2 >= MAX_DEATHS) {
        display_show("GAME OVER");
        delay(3000);
        return true;
    }
    return false;
}

// Debug mode
void debug_mode() {
    display_show("DEBUG MODE");
    // Debug functions can be added here, such as simulating hits or displaying sensor data
}