#include <Arduino.h>
#include "defs.h"
#include "pins.h"

// These are set in an interrupt handler => volatile
volatile uint32_t pulse_width = 0;
volatile uint32_t pulse_start_time = 0;
volatile bool pulse_received = false;

static const uint32_t RX_MIN_PULSE_US = 800;
static const uint32_t RX_MAX_PULSE_US = 2200;
static const uint32_t RX_TOLERANCE_US = 150;
static const uint32_t RX_LOCKOUT_MS = 100;
static const uint32_t HEADER_WIDTH_US = 4000;
static const uint32_t HEADER_TOLERANCE = 500;

static const uint32_t ir_pwm_freq = 38000;
static const uint32_t ir_pwm_duty_cycle = 128; // 50% duty cycle
static const uint32_t ir_pwm_resolution = 8; // 8-bit resolution (0-255)

static uint32_t last_valid_hit_ms = 0;

// Interrupt when IR reciever detects a pulse
volatile uint32_t last_header_time = 0;
volatile bool header_detected = false;

void IRAM_ATTR ir_rx_interrupt() {
    static uint32_t fall_time = 0;
    bool pin_state = digitalRead(PIN_IR_RX);
    uint32_t now = micros();

    if (pin_state == LOW) { // Start of a pulse
        fall_time = now;
    } else { // End of a pulse
        uint32_t width = now - fall_time;

        // Check if this is a Header pulse
        if (width >= (HEADER_WIDTH_US - HEADER_TOLERANCE) && 
            width <= (HEADER_WIDTH_US + HEADER_TOLERANCE)) {
            header_detected = true;
            last_header_time = now;
        } 
        // Check if this is a Data pulse AND we just had a header
        else if (header_detected && (now - last_header_time < 5000)) { // 5ms timeout
            if (width >= RX_MIN_PULSE_US && width <= RX_MAX_PULSE_US) {
                pulse_width = width;
                pulse_received = true;
            }
            header_detected = false; // Reset for next shot
        } else {
            header_detected = false; // Noise or invalid sequence
        }
    }
}

// Initializing IR transmitter and reciever
void ir_init() {
    pinMode(PIN_IR_RX, INPUT);

    // PWM setup
    ledcAttach(PIN_IR_TX, ir_pwm_freq, ir_pwm_resolution);
    ledcWrite(PIN_IR_TX, 0);

    attachInterrupt(digitalPinToInterrupt(PIN_IR_RX), ir_rx_interrupt, CHANGE);
}

// Send 38Khz signal
static void ir_pwm_burst(int duration) {
    ledcWrite(PIN_IR_TX, ir_pwm_duty_cycle);
    delayMicroseconds(duration);
    ledcWrite(PIN_IR_TX, 0);
}

// Shooting logic
void ir_shoot(int player_id) {
    // Select pulse width based on player ID
    int width = PLAYER1_PULSE_WIDTH;
    if (player_id == 2) width = PLAYER2_PULSE_WIDTH;

    // 38kHz signal for the duration of the pulse width
    // Header to help with noise rejection on the receiver
    ir_pwm_burst(HEADER_WIDTH_US);
    delayMicroseconds(500); // Short gap between header and data
    // Send data pulse
    ir_pwm_burst(width);
}

bool ir_pulse_received() {
    // True if a pulse has been recieved
    return pulse_received;
}

int ir_read_pulse_width() {
    // Clear recieved flag and return pulse width
    noInterrupts();
    uint32_t width = pulse_width;
    pulse_received = false;
    interrupts();

    return (int)width;
}


int ir_get_player_id() {
    noInterrupts();
    bool recieved = pulse_received;
    uint32_t width = pulse_width;
    pulse_received = false;
    interrupts();

    if (!recieved) return 0;

    Serial.printf("Received pulse width: %d us\n", width);

    uint32_t now_ms = millis();
    if (now_ms - last_valid_hit_ms < RX_LOCKOUT_MS) {
        return 0;
    }

    if (width >= PLAYER1_PULSE_WIDTH - RX_TOLERANCE_US &&
        width <= PLAYER1_PULSE_WIDTH + RX_TOLERANCE_US) {
        last_valid_hit_ms = now_ms;
        return 1;
    }

    if (width >= PLAYER2_PULSE_WIDTH - RX_TOLERANCE_US &&
        width <= PLAYER2_PULSE_WIDTH + RX_TOLERANCE_US) {
        last_valid_hit_ms = now_ms;
        return 2;
    }

    return 0; // Unknown player/noise
}