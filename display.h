#pragma once
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void display_start();
void display_update(int ammo1, int hp1, int ammo2, int hp2);
void display_show(const char* msg);