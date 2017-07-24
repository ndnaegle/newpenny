// Log: manages a log in persistent memory
#include <Arduino.h>

#include "main.h"
#include "log.h"
#include <Wire.h>
#include <stdarg.h>
#include "FRAM_I2C.h"

Log theLog; // use as a singleton

FRAM_I2C fram = FRAM_I2C(); // used only in this module

void fram_setup(void) {
    if (fram.begin()) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
        Serial.println(F("Found I2C FRAM"));
    } else {
        Serial.println(F("I2C FRAM not found"));
    }
}

Log::Log() {
}

void Log::init(void) {
    // we don't clear the log at startup in case we want to read it
    fram_setup();    
}

// clear
//  An empty log is all zeroes. We start filling the log after the first zero and always zero-terminate. When we wrap around, we wipe out the starting zero.
void Log::clear(void) {
    fram.erase();
}

void Log::output(const char* str) {
    uint16_t addr = 1; // skip any character at address zero. It's zero until the log wraps around.

    // first find the end of buffer (zero terminated)
    for ( ; addr < fram.m_maxaddress ; ++addr) {
        if (fram.read8(addr) == 0) {
            break;
        }
    }

    // Make sure we found a zero
    if (addr == fram.m_maxaddress) { // Log must be corrupt. Clear it.
        clear();
        addr = 1;
    }

    // Write the string into the fram and ensure the end is zero-terminated
    const char* p = str;
    while (*p != 0) {
        fram.write8(addr, *p++);
        addr = fram.offset(addr, 1); // adds 1 and wraps the address around the end of the buffer
    }
    // if we've wrapped around and land exactly on address 0, we insert a space.
    // zero at address zero means the log is not full yet.
    if (addr == 0) {
        fram.write8(addr++, ' ');
    }
    
    // zero-terminate the log always
    fram.write8(addr, 0);
}

void Log::dump(void) {
    uint16_t addr = 0;

    // first find the end of buffer (zero terminated)
    // log always starts after the end of the buffer
    // this works because we always leave a zero at address zero until the log wraps around
    for ( ; addr < fram.m_maxaddress ; ++addr) {
        if (fram.read8(addr) == 0) {
            break;
        }
    }
    addr = fram.offset(addr, 1); // adds 1 and wraps the address around the end of the buffer if necessary
    char ch;
    while (true) {
        ch = fram.read8(addr);
        if (ch == 0)
            break;
        Serial.print(ch);
        addr = fram.offset(addr, 1);
    }
    Serial.println();
}

