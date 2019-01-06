// main.h
//
//  Definitions for the entire program
#ifndef _MAIN_H
#define _MAIN_H

#include "log.h"

#define VERSION "0.6.1"

#define SCREENSAVER_INTERVAL_MS ((uint32_t)1000 * (uint32_t)60 * (uint32_t)10) // 10 minutes

// State indices. Must be less than 256 (8 bits)
// Make sure to add new states to the dumpState() function for debugging
#define INVALID_STATE 0
#define START_STATE 5
#define WAIT_FOR_HOME_STATE 10
#define WAIT_FOR_LIFT_STATE 12
#define WAIT_FOR_COIN_STATE 15
#define WAIT_FOR_BUTTON_STATE 20
#define STAMP_SELECTOR_STATE 45
#define COIN_DROP_STATE 50
#define WAIT_FOR_STAMP_FINISH_STATE 55
#define SHUTDOWN_STATE 253
#define RESET_STATE 254
#define FOREVER_STATE 255

// Globals
extern int theState;
extern int haveFRAM;

#define NO_FRAM 0
#define FOUND_FRAM 1

extern unsigned long int theTime;

// fdump
//  This tricky macro forces all format strings used with the macro into code (flash) storage rather than SRAM
#define fdump(fmt, ...) (dump(F(fmt), ##__VA_ARGS__))

// Extern functions
void dump(const __FlashStringHelper *fmt, ...);
const char* dumpState(int state);
void pinWrite(int pin, int value);
void gotoState(int newState);

#endif // #ifndef _MAIN_H
