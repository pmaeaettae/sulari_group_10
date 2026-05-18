#include "wireless.h"
#include "game.h"
#include "defs.h"
#include "display.h"

#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_BROKER "your IP"
#define MQTT_PORT 1883
#define MQTT_ID "group10_lasertag_esp32_client"

#define MQTT_STATS "lasertag/stats"
#define MQTT_EVENTS "lasertag/events"

#define WIFI_ID "Your WiFi ID"
#define WIFI_PASSWORD "Your WiFi Password"

// Connect to MQTT broker
WiFiClient espClient;
PubSubClient mqttClient(espClient);

uint32_t last_mqtt_reconnect = 0;


// Initialize wifi and mqtt
void wireless_init() {
    Serial.println("Connecting WiFi");

    WiFi.begin(WIFI_ID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting...");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
        Serial.print("IP:");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Connection failed");
    }

    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
}

// Main MQTT loop
void mqtt_loop() {
    if (WiFi.status() != WL_CONNECTED) {
    return;
    }
    mqtt_reconnect();

    if (mqttClient.connected()) {
        mqttClient.loop();
    }
}

// Send game stats via mqtt
void mqtt_stats() {
    if (!mqttClient.connected()) {
        return;
    }

    char message[256];
snprintf(
    message,
    sizeof(message),
    "{"
    "\"type\":\"stats\","
    "\"p1\":{\"id\":%d,\"hp\":%d,\"deaths\":%d,\"hits\":%d,\"ammo\":%d},"
    "\"p2\":{\"id\":%d,\"hp\":%d,\"deaths\":%d,\"hits\":%d,\"ammo\":%d}"
    "}",
    player_1.id1,
    player_1.hp1,
    player_1.deaths1,
    player_1.hits1,
    player_1.ammo1,
    player_2.id2,
    player_2.hp2,
    player_2.deaths2,
    player_2.hits2,
    player_2.ammo2
);

    mqttClient.publish(MQTT_STATS, message);

    Serial.print("MQTT stats sent: ");
    Serial.println(message);
}

void mqtt_game_over () {
    if (!mqttClient.connected()) {
        return;
    }

    int winner = 0;
    if (player_1.deaths1 >= MAX_DEATHS) {
        winner = 2;
    } else if (player_2.deaths2 >= MAX_DEATHS) {
        winner = 1;
    }

    char message[256];
    snprintf(
        message,
        sizeof(message),
        "{"
        "\"game_over\":true,"
        "\"winner\":%d,"
        "\"p1_deaths\":%d,"
        "\"p2_deaths\":%d"
        "}",
        winner,
    player_1.deaths1,
    player_2.deaths2
);

    mqttClient.publish(MQTT_EVENTS, message);
    Serial.print("Game over ");
    Serial.println(message);

    mqtt_stats();
}


// Non blocking MQTT reconnect logic => try connection every 5 sec
void mqtt_reconnect() {
    if (mqttClient.connected()) {
        return;
    }

    if (millis() - last_mqtt_reconnect > 5000) {
        last_mqtt_reconnect = millis();
        Serial.print("Connecting to MQTT...");
        if (mqttClient.connect(MQTT_ID)) {
            Serial.println("Connected");
        } else {
            Serial.print("Failed, state=");
            Serial.println(mqttClient.state());
        }
    }
}