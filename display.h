#pragma once
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void display_start();
void display_update(int ammo, int hp);
void display_show(const char* msg);