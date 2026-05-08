#include "pins.h"
#include "defs.h"
#include "display.h"
#include "gyro.h"

void setup () {
    Serial.begin(115200);

    display_init();
    gyro_init();
    game_init();


}

void loop() {

    // mqtt loop needed here!!



}