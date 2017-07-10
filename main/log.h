// Log: manages a log in EEPROM of state transitions
#ifndef _LOG_H
#define _LOG_H

class Log {
private:
    const int eepromStart = 0;
    const int maxLength = 128; // must be an even number
    const int bufferEnd = eepromStart + maxLength;
    // stored at bufferEnd is index of next new log entry
    // stored at bufferEnd + 1 is length of log in number of entries (max is maxLength--the log wraps around)
public:
    Log();
    void clear();
    void output(int fromState, int toState);
    void dump();
};

extern Log theLog;

#endif // #ifndef _LOG_H

