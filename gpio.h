#pragma once

#include <Arduino.h>

// Initializing GPIO pins
void gpio_init();

// Trigger button pressed
bool trigger_pressed();

// Reload button pressed
bool reload_pressed();

// Start button pressed
bool start_button_pressed();

// Debug mode active
bool debug_mode_active();

// Get player ID
int player_id();

// Rumble motor on/off
void rumble_motor_on();
void rumble_motor_off();

// Buzzer beep
void buzzer(int buzz_duration);

// Status leds shows player ID
void status_leds();