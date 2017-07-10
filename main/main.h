// main.h
//
//  Definitions for the entire program
#ifndef _MAIN_H
#define _MAIN_H

#include "log.h"

#define VERSION "0.2.0"

// State indices. Must be less than 256 (8 bits)
// Make sure to add new states to the dumpState() function below for debugging
#define INVALID_STATE 0
#define START_STATE 5
#define WAIT_FOR_HOME_STATE 10
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
extern unsigned long int theTime;

// Extern functions
const char* dumpState(int state);
void pinWrite(int pin, int value);
void gotoState(int newState);

#endif // #ifndef _MAIN_H
