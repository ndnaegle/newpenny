// Log: manages a log in EEPROM of state transitions
#ifndef _LOG_H
#define _LOG_H

class Log {
public:
    Log();
    void init(void);
    void clear(void);
    void output(const char* str);
    void dump(void);

private:
    uint16_t addWithWraparound(uint16_t addr, uint16_t delta);
};

extern Log theLog;

#endif // #ifndef _LOG_H
