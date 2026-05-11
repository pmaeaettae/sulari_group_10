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
    display.setCursor(28, 30);
    display.println("OLED OK");
    display.display();
}

void display_update (int ammo, int hp) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    
    display.setCursor(10, 10);
    display.printf("Ammo: %d", ammo);

    display.setCursor(28, 35);
    display.printf("HP: %d", hp);

    display.display();
}

void display_show(const char* msg) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(28, 30);
    display.println(msg);
    display.display();
}