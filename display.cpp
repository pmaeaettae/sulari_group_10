#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"

TwoWire OLEDWire = TwoWire(1);

// Help found from this tutorial by Random Nerd Tutorials: https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &OLEDWire, -1);

void display_start() {
    OLEDWire.begin(4, 5);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED failed");
        while(1);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 30);
    display.println("Waiting for players...");
    display.display();
}

void display_update (int ammo1, int hp1, int ammo2, int hp2) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    
    display.setCursor(15, 5);
    display.printf("P1: Ammo: %d", ammo1);

    display.setCursor(30, 15);
    display.printf("HP1: %d", hp1);

    display.setCursor(15, 35);
    display.printf("P2: Ammo: %d", ammo2);

    display.setCursor(30, 45);
    display.printf("HP2: %d", hp2);

    display.display();
}

void display_show(const char* msg) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(28, 30);
    display.println(msg);
    display.display();
}