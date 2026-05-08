#include "pins.h"
#include "defs.h"
#include "display.h"
#include "gyro.h"
#include "game.h"

ty_game_state state = ST_INIT;

void setup () {
    Serial.begin(115200);

    display_init();
    gyro_init();
    game_init();


}

void loop() {

    // mqtt loop needed here!!

    swtich (state) {

        case ST_INIT:
            ST = ST_WAIT_MODE;
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

        case ST_GAME_OVER;
            game_over();
            state = ST_WAIT_START;
            break;

        case STATE_DEBUG_
            debug_mode();
            break;
    }
}