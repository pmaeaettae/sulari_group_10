#include <Arduino.h>
#include "defs.h"
#include "pins.h"

// IR channel for PWM
#define IR_PWM_CHANNEL 0
#define IR_FREQUENCY 38000  // 38kHz carrier
#define IR_DUTY_CYCLE 128   // 50% duty (8-bit resolution)

// Initialize IR module
void initIR() {
    pinMode(PIN_IR_TX, OUTPUT);
    pinMode(PIN_IR_RX, INPUT);
    
    // Setup PWM for IR transmission
    ledcSetup(IR_PWM_CHANNEL, IR_FREQUENCY, 8);
    ledcAttachPin(PIN_IR_TX, IR_PWM_CHANNEL);
    ledcWrite(IR_PWM_CHANNEL, 0);  // Start with IR off
}

// Send IR pulse for a specific player ID
void sendIR(int playerID) {
    int pulseWidth = 0;
    
    // Determine pulse width based on player ID
    switch (playerID) {
        case 1:
            pulseWidth = PLAYER1_PULSE_WIDTH;
            break;
        case 2:
            pulseWidth = PLAYER2_PULSE_WIDTH;
            break;
        default:
            return;  // Invalid player ID
    }
    
    // Send modulated IR pulse
    ledcWrite(IR_PWM_CHANNEL, IR_DUTY_CYCLE);  // Turn on IR with carrier
    delayMicroseconds(pulseWidth);
    ledcWrite(IR_PWM_CHANNEL, 0);  // Turn off IR
}

// Receive and decode IR pulse, return player ID (0 if no valid pulse detected)
int receiveIR() {
    // Wait for IR signal to start (receiver goes HIGH when IR detected)
    unsigned long timeout = micros() + 1000000;  // 1 second timeout
    while (digitalRead(PIN_IR_RX) == LOW) {
        if (micros() > timeout) return 0;  // Timeout, no signal
    }
    
    // Measure pulse duration
    unsigned long startTime = micros();
    while (digitalRead(PIN_IR_RX) == HIGH) {
        if (micros() - startTime > 5000) break;  // Max pulse 5ms
    }
    unsigned long pulseDuration = micros() - startTime;
    
    // Determine player ID based on pulse length
    if (abs((long)pulseDuration - PLAYER1_PULSE_WIDTH) < 200) {
        return 1;
    } else if (abs((long)pulseDuration - PLAYER2_PULSE_WIDTH) < 200) {
        return 2;
    }
    
    return 0;  // No valid player ID
}