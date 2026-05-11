// Developed with the assistance of AI

#include "pins.h"
#include "defs.h"

#include "display.h"
#include "gyro.h"
#include "game.h"
#include "ir.h"
#include "wireless.h"
#include "gpio.h"

// Starts from INIT state
ty_game_state state = ST_INIT;

void setup () {
    Serial.begin(115200);

    gpio_init();
    display_init();
    ir_init();
    gyro_init();
    // wireless_init();
    game_init();


}

void loop() {

    // mqtt loop needed here!!

    status_leds();

    switch (state) {

        case ST_INIT:
            state = ST_WAIT_MODE;
            break;

        case ST_WAIT_MODE:
            if (debug_mode_active()) state = ST_DEBUG;
            else state = ST_WAIT_START;
            break;

        case ST_WAIT_START:
            if (start_button_pressed()) {
                game_start();
                state = ST_PLAY;
            }
            break;

        case ST_PLAY:
            game_update();
            if (game_over()) {
                state = ST_GAME_OVER;
            }
            break;

        case ST_GAME_OVER:
            game_over();
            state = ST_WAIT_START;
            break;

        case ST_DEBUG:
            debug_mode();
            if (!debug_mode_active()) {
                state = ST_WAIT_MODE;
            }
            break;
    }
    delay(10);
}