// display.h
//  Use tiny OLED display for diagnostics
//  July 2017

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern void initDisplay(const char* version);

#endif
