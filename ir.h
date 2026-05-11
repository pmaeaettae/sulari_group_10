#pragma once

#include <Arduino.h>

// Set up IR transmitter and reciever pins and interrupts
void ir_init();

// Send IR shot with player ID encoded in pulse width
void ir_shoot(int player_id);

// Check for incoming IR pulses
bool ir_pulse_received();

// Read pulse width
int ir_read_pulse_width();

// Get player ID from received pulse
int ir_get_player_id();