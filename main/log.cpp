// Log: manages a log in EEPROM of state transitions
#include <Arduino.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include <EEPROM.h>
#pragma GCC diagnostic pop

#include "main.h"
#include "log.h"
#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"

Log theLog; // use as a singleton
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();

void fram_setup(void) {
    Serial.begin(115200);
    while (!Serial)
        ; //wait until Serial ready

    Serial.println(F("About to init FRAM..."));
    Serial.flush();

    if (fram.begin()) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
        Serial.println("Found I2C FRAM");
        Serial.flush();
    } else {
        Serial.println("I2C FRAM not identified ... check your connections?\r\n");
        Serial.println("Will continue in case this processor doesn't support repeated start\r\n");
        Serial.flush();
    }
    
    // Read the first byte
    uint8_t test = fram.read8(0x0);
    Serial.print("Restarted "); Serial.print(test); Serial.println(" times");
    // Test write ++
    fram.write8(0x0, test+1);
    
    // dump the entire 32K of memory!
    uint8_t value;
    for (uint16_t a = 0; a < 32768; a++) {
    value = fram.read8(a);
    if ((a % 32) == 0) {
        Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
        Serial.print('0');
        Serial.print(value, HEX); Serial.print(" ");
    }
}


Log::Log() {
    // we don't clear the log at startup in case we want to read it
//    fram_setup();
}
    
void Log::clear() {
    int i = eepromStart;
    while (i < bufferEnd) {
        EEPROM.write(i++, INVALID_STATE);
        EEPROM.write(i++, INVALID_STATE);
    }
    EEPROM.write(i++, 0); // next new log entry stored at bufferEnd
    EEPROM.write(i, 0); // length of current log stored at bufferEnd + 1
}

void Log::output(int fromState, int toState) {
    int nextItem = EEPROM.read(bufferEnd);
    EEPROM.write(nextItem++, fromState);
    EEPROM.write(nextItem++, toState);
    if (nextItem > bufferEnd) {
        nextItem = eepromStart;
    }
    EEPROM.write(bufferEnd, nextItem);
    int length = EEPROM.read(bufferEnd + 1) + 1;
    if (length > bufferEnd) {
        length = maxLength;
    }
    EEPROM.write(bufferEnd + 1, length);
}

void Log::dump() {
    int start = EEPROM.read(bufferEnd);
    int length = EEPROM.read(bufferEnd + 1);

    // dump the log entries in reverse chronological order
    for (int i = 0 ; i < length ; ++i) {
        // wrap around if we're at the start of the buffer
        if (start == 0) {
            start = bufferEnd;
        }
        Serial.print(F("Recorded state #"));
        Serial.print(length - i, DEC);
        Serial.print(": ");
        int toState = EEPROM.read(--start);
        int fromState = EEPROM.read(--start);
        dumpState(fromState);
        Serial.print(F("-->"));
        dumpState(toState);
        Serial.println();
    }
}

