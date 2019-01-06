// Log: manages a log in persistent memory
#include <Arduino.h>

#include "main.h"
#include "log.h"
#include <Wire.h>
#include <stdarg.h>
#include "FRAM_I2C.h"

Log theLog; // use as a singleton

FRAM_I2C fram = FRAM_I2C(); // used only in this module

#define READ_ADDRESS 0
#define WRITE_ADDRESS 2

void fram_setup(void) {
    if (fram.begin()) {
        Serial.println(F("Found I2C FRAM"));
        haveFRAM = FOUND_FRAM;
    } else {
        Serial.println(F("I2C FRAM not found"));
        haveFRAM = NO_FRAM;
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
    uint16_t read = 4;
    uint16_t write = 5;
    fram.write16(READ_ADDRESS, read);
    fram.write16(WRITE_ADDRESS, write);    
    fram.write8(read, '^'); // for debugging: start of log
}

uint16_t Log::addWithWraparound(uint16_t addr, uint16_t delta) {
    uint32_t bigaddr = (uint32_t)addr + (uint32_t)delta;
    if (bigaddr > (uint32_t)fram.m_maxaddress) {
        bigaddr -= (uint32_t)fram.m_maxaddress + 1; // 32767 + 2 = 32769. 32769 - (32767 + 1) = 1 (wraps around)
        bigaddr += 4; // first four bytes for start and next pointers
    }

    return (uint16_t)bigaddr;    
}

void Log::output(const char* str) {
    uint16_t read = fram.read16(READ_ADDRESS);
    uint16_t write = fram.read16(WRITE_ADDRESS);
    // data starts at address 4

    // values will be zero if the log is uninitialized or cleared
    if (read >= fram.m_maxaddress || read == 0) {
        read = 4;
        write = 5;
        fram.write8(read, '^'); // for debugging: start of log
    }

    // Write the string into the fram and ensure the end is zero-terminated
    const char* p = str;
    while (*p != 0) {
        fram.write8(write, *p++);
        if (read == write) { // we have wrapped around and from now on, read will equal write
            ++read;
        }
        ++write;

        // see if we need to wrap around the end of the buffer
        if (write > fram.m_maxaddress) {
            write = 4;
        }
        if (read > fram.m_maxaddress) {
            read = 4;
        }
    }
    fram.write16(READ_ADDRESS, read);
    fram.write16(WRITE_ADDRESS, write);    
}

void Log::dump(void) {
    uint16_t read = fram.read16(READ_ADDRESS);
    uint16_t write = fram.read16(WRITE_ADDRESS);
    Serial.print("Read = ");
    Serial.println(read, DEC);
    Serial.print("Write = ");
    Serial.println(write, DEC);

    char ch;
    do {
        ch = fram.read8(read++);
        Serial.print(ch);
        if (read > fram.m_maxaddress) {
            read = 4;
        }
    } while (read != write);
    Serial.println();
}
