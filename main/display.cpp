// display.cpp
//  Use tiny OLED display for diagnostics
//  July 2017

#include "display.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void initDisplay(const char* version) {
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    // init done
    
    // Clear the buffer.
    display.clearDisplay();
    display.display();

    // Display test
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("PENNYMAN!");
    display.setTextSize(1);
    display.println(version);
    display.setTextColor(WHITE);
//    display.setCursor(0, 24);
//    display.println("Please wait...");
    display.display();
}

