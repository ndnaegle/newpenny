// Log: manages a log in EEPROM of state transitions
#include <Arduino.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include <EEPROM.h>
#pragma GCC diagnostic pop

#include "main.h"
#include "log.h"

Log theLog; // use as a singleton

Log::Log() {
    // we don't clear the log at startup in case we want to read it
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
        Serial.print("Recorded state #");
        Serial.print(length - i, DEC);
        Serial.print(": ");
        int toState = EEPROM.read(--start);
        int fromState = EEPROM.read(--start);
        dumpState(fromState);
        Serial.print("-->");
        dumpState(toState);
        Serial.println();
    }
}

