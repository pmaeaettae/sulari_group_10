#include <Arduino.h>
#include "defs.h"
#include "pins.h"

// These are se in an interrupt handler => volatile
volatile int pulse_width = 0;
volatile int pulse_start_time = 0;
volatile bool pulse_received = false;

// Interrupt when IR reciever detects a pulse
void IRAM_ATTR ir_rx_interrupt() {
    if (digitalRead(PIN_IR_RX) == HIGH) {
        // Start of pulse
        pulse_start_time = micros();
    } else {
        // End of pulse
        pulse_width = micros() - pulse_start_time;
        pulse_received = true;
    }
}

// Initializing IR transmitter and reciever
void ir_init() {
    pinMode(PIN_IR_TX, OUTPUT);
    pinMode(PIN_IR_RX, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_IR_RX), ir_rx_interrupt, CHANGE);
}

// Shooting logic
void ir_shoot(int player_id) {
    // Select pulse width based on player ID
    int width = PLAYER1_PULSE_WIDTH;
    if (player_id == 2) width = PLAYER2_PULSE_WIDTH;

    // One shot per trigger press
    // 38kHz signal for the duration of the pulse width
    uint32_t t0 = micros();
    while (micros() - t0 < width) {
        digitalWrite(PIN_IR_TX, HIGH);
        delayMicroseconds(10);
        digitalWrite(PIN_IR_TX, LOW);
        delayMicroseconds(10);
    }
}

bool ir_pulse_received() {
    // True if a pulse has bveen recieved
    return pulse_received;
}

int ir_read_pulse_width() {
    // Clear recieved flag and return pulse width
    pulse_received = false;
    return pulse_width;
}


int ir_get_player_id() {
    if (pulse_width == PLAYER1_PULSE_WIDTH) return 1;
    if (pulse_width == PLAYER2_PULSE_WIDTH) return 2;
    return 0; // Unknown player
}