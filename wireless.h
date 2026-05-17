#pragma once
#include <Arduino.h>

// Initialize WiFi connection
void wireless_init();


// MQTT logic functions
void mqtt_loop();
void mqtt_stats();
//void mqtt_hits();
void mqtt_game_over();