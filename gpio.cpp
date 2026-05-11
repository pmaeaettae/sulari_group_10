#include "gpio.h"
#include "pins.h"

// Initialize GPIO pins
void gpio_init() {
    pinMode(PIN_TRIGGER, INPUT_PULLUP);
    pinMode(PIN_RELOAD, INPUT_PULLUP);
    pinMode(PIN_START, INPUT_PULLUP);
    pinMode(DEBUG_SW, INPUT_PULLUP);
    pinMode(PIN_RUMBLE_MOTOR, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    // Default off
    digitalWrite(PIN_RUMBLE_MOTOR, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    digitalWrite(STATUS_LED1, LOW);
    digitalWrite(STATUS_LED2, LOW);
    digitalWrite(STATUS_LED3, LOW);
}

// Trigger button pressed
bool trigger_pressed() {
    return digitalRead(PIN_TRIGGER) == LOW;
}

// Reload button pressed
bool reload_pressed() {
    return digitalRead(PIN_RELOAD) == LOW;
}

// Start button pressed
bool start_button_pressed() {
    return digitalRead(PIN_START) == LOW;
}

// Debug mode on if TRIGGER and RELOAD held together
bool debug_mode_active() {
    return trigger_pressed() && reload_pressed();
}

// Get player ID
int player_id() {
    if (digitalRead(DEBUG_SW) == LOW) return 1;
    else return 2;
}

// Update LEDs
void status_leds() {
    int id = player_id();
    digitalWrite(STATUS_LED1, id == 1 ? HIGH : LOW);
    digitalWrite(STATUS_LED2, id == 2 ? HIGH : LOW);
    digitalWrite(STATUS_LED3, LOW);
    // Status led 3 for debug mode
    digitalWrite(STATUS_LED3, debug_mode_active() ? HIGH : LOW);
}

// Rumble motor on/off
void rumble_motor_on() {
    digitalWrite(PIN_RUMBLE_MOTOR, HIGH);
}

void rumble_motor_off() {
    digitalWrite(PIN_RUMBLE_MOTOR, LOW);
}

// Buzzer beep
void buzzer(int buzz_duration) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(buzz_duration);
    digitalWrite(PIN_BUZZER, LOW);
}
