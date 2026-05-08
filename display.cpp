#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void display_start() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void display_update (int ammo, int hp) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.printf("Ammo: %d\n HP: %d", ammo, hp);
    display.display();
}

void display_show(const char* msg) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(msg);
    display.display();
}