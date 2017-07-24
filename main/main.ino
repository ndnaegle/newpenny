// Main.ino
//  Main source file for newpenny project
//  June 2017

// Libraries used:
//  AdaFruit GFX
//  AdaFruit SSD1306

#include <avr/wdt.h>

#include "main.h"
#include "log.h"
#include "input.h"
#include "console.h"
#include "display.h"
#include "output.h"
#include "pinouts.h"

// Globals
int theState = INVALID_STATE;

const char* dumpState(int state) {
    const char* pTag = NULL;
    switch (state) {
      case INVALID_STATE: pTag = "Invalid"; break;
      case START_STATE: pTag = "Start"; break;
      case WAIT_FOR_HOME_STATE: pTag = "Wait for Home"; break;
      case WAIT_FOR_COIN_STATE: pTag = "Wait for Coin"; break;
      case WAIT_FOR_BUTTON_STATE: pTag = "Wait for Button"; break;
      case STAMP_SELECTOR_STATE: pTag = "Position Stamp"; break;
      case COIN_DROP_STATE: pTag = "Coin Drop"; break;
      case WAIT_FOR_STAMP_FINISH_STATE: pTag = "Wait for Finish"; break;
      case SHUTDOWN_STATE: pTag = "Shutdown"; break;
      case RESET_STATE: pTag = "Reset"; break;
      case FOREVER_STATE: pTag = "Wait Forever"; break;
      default: pTag = "Unknown!"; break;
    }
    return pTag;
}

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;

    // Initialize FRAM logging
    theLog.init();
        
    // initialize our screen
    initDisplay(VERSION);
    
    // Modes for pins assumed to be INPUT so we don't set modes for inputs

    // Initialize pin input emulation via serial (input.cpp)
    initSerialKeys();

    // Set the mode for the output pins
    pinMode(outputCounterPin, OUTPUT);
    pinMode(outputCoinDropPin, OUTPUT);
    pinMode(outputMotorRunPin, OUTPUT);
    pinMode(outputRedLeverLiftPin, OUTPUT);

    // Give the user a chance to start the debug console
    debugConsole();

    // make sure hardware is in a sane state
    outputRedLeverLift.write(LOW);
    outputMotorRun.write(LOW);
    outputCoinDrop.write(LOW);
    outputCounter.write(LOW);

    // Start at reset state
    fdump("Starting state machine at %lu ticks\n", millis());
    gotoState(START_STATE);
}

int stamp = 0;
boolean edgeDetect = false; 

void loop() {
    static unsigned long int countdown = 0;
    
    switch (theState) {
        
    case START_STATE:
        outputRedLeverLift.write(LOW);
        outputMotorRun.write(HIGH);
        inputHome.start();
        inputReset.start();
        gotoState(WAIT_FOR_HOME_STATE);
        break;

    case WAIT_FOR_HOME_STATE:
        if (inputHome.read() == ACTIVE) {
            fdump("Motor reached home position\n");
            outputMotorRun.write(LOW);
            outputRedLeverLift.write(HIGH);
            gotoState(WAIT_FOR_COIN_STATE);
        }
        break;
        
    case WAIT_FOR_COIN_STATE:
        if (inputCoinAccept.read() == ACTIVE) {
            fdump("Coin has been accepted\n");
            outputRedLeverLift.write(LOW);
            inputButton1.start();
            inputButton2.start();
            inputButton3.start();
            inputButton4.start();
            gotoState(WAIT_FOR_BUTTON_STATE);            
        }
        break;
        
    case WAIT_FOR_BUTTON_STATE:
        if (inputButton1.read() == ACTIVE) {
            fdump("Button 1 has been pressed\n");
            stamp = 1;
            outputMotorRun.write(HIGH);
            input90Degree.start();
            gotoState(STAMP_SELECTOR_STATE);
        }
        else if (inputButton2.read() == ACTIVE) {
            fdump("Button 2 has been pressed\n");
            stamp = 2;
            outputMotorRun.write(HIGH);
            input90Degree.start();
            gotoState(STAMP_SELECTOR_STATE);
        }
        else if (inputButton3.read() == ACTIVE) {
            fdump("Button 3 has been pressed\n");
            stamp = 3;
            outputMotorRun.write(HIGH);
            input90Degree.start();
            gotoState(STAMP_SELECTOR_STATE);
        }
        else if (inputButton4.read() == ACTIVE) {
            fdump("Button 4 has been pressed\n");
            stamp = 4;
            outputMotorRun.write(HIGH);
            input90Degree.start();
            gotoState(STAMP_SELECTOR_STATE);
        }
        break;
        
    case STAMP_SELECTOR_STATE:
        if (input90Degree.read() == ACTIVE) {
            edgeDetect = true;  // We need to make sure we don't count down until the 90 degree sensor has risen *and* fallen
        } else if (input90Degree.read() == NOT_ACTIVE && edgeDetect) {
            edgeDetect = false;                    
            if (--stamp == 0) { // countdown the number of 90 degree sensor hits we've had
                outputCoinDrop.write(HIGH);
                fdump("The coin has been dropped\n");
                outputCounter.write(HIGH);
                countdown = millis();
                gotoState(COIN_DROP_STATE);
            }
        }
        break;
        
    case COIN_DROP_STATE:
        if (millis() > countdown + 1000) { // wait one second after the coin drops to reset the coin drop solenoid
            outputCoinDrop.write(LOW);
            outputCounter.write(LOW);
            gotoState(WAIT_FOR_STAMP_FINISH_STATE);
            countdown = millis();
        }
        break;
        
    case WAIT_FOR_STAMP_FINISH_STATE: // wait five seconds which is enough for the coin to make it through
        if (millis() > countdown + 5 * 1000) {
            gotoState(START_STATE); // Going back to start will wait for the motor to hit Home
        }
        break;                                                          

    case SHUTDOWN_STATE: // not currently used
        outputRedLeverLift.write(LOW);
        outputMotorRun.write(LOW);
        outputCoinDrop.write(LOW);
        outputCounter.write(LOW);        
        gotoState(FOREVER_STATE);
        break;

    case RESET_STATE:
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(F("Reset!"));
        display.display();

        fdump("Resetting board using watchdog...\n");
        Serial.flush();
        
        // enable watchdog timer (which will reboot the board) and wait until it fires
        wdt_enable(WDTO_15MS);
        while (true)
            ;
        break;

    case FOREVER_STATE:
        break;
    }

    // housekeeping for each loop
    updateSerialKeys();
    displayStatus();

    // see if we're supposed to reset
    if (inputReset.read() == ACTIVE) {
//        fdump("---Reset initiated---\n");
//        gotoState(RESET_STATE);
    }
}


// displayStatus
//  Shows the status of the system on the OLED screen
void displayStatus(void) {
    static unsigned long timeLastStateChange = 0;
    static int lastState = INVALID_STATE;
    
    // time since last state change
    unsigned long timeNow = millis();
    if (theState != lastState) {
        lastState = theState;
        timeLastStateChange = timeNow;
    }
    unsigned long timeSinceLastStateChange = timeNow - timeLastStateChange;

    // after some period, clear the display to save the poor OLED
    if (timeSinceLastStateChange > SCREENSAVER_INTERVAL_MS) {
        display.clearDisplay();
        display.display();
        return;        
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    // state
    display.println(dumpState(theState));

    // Outputs
    display.setTextSize(2);
    outputRedLeverLift.displayLevel();
    outputMotorRun.displayLevel();
    outputCoinDrop.displayLevel();

    // Inputs
//    inputHome.displayLevel();
    input90Degree.displayLevel();
    inputCoinAccept.displayLevel();
    inputButton1.displayLevel();
    inputButton2.displayLevel();
    inputButton3.displayLevel();
    inputButton4.displayLevel();
    inputReset.displayLevel();
    display.println();

    display.setTextSize(1);
    display.print("LS: ");
    displayElapsedTime(timeSinceLastStateChange / 1000);
    display.display();
}

// gotoState
//  Transitions to the specified state
//  (and gives us a chance to log the state change)
void gotoState(int newState) {
    fdump("State %s -> %s\n", dumpState(theState), dumpState(newState));
    theState = newState;
}

// dump
//  !!Always use fdump macro (main.h)!!
//  This function is called by the fdump macro and logs a string to both log and to Serial
//  Truncated to 128 characters

void dump(const __FlashStringHelper *fmt, ... ) {
    char buf[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt);
    vsnprintf_P(buf, sizeof (buf), (const char *)fmt, args); // progmem for AVR
    va_end(args);
    
    Serial.print(buf);
//    theLog.output(buf);
}

// macros from DateTime.h 
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

// prepare to compute elapsed time
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

// displayElapsedTime
//  displays elapsed time from number of input seconds
void displayElapsedTime(unsigned long elapsed) {
    int days = elapsedDays(elapsed);
    int hours = numberOfHours(elapsed);
    int minutes = numberOfMinutes(elapsed);
    int seconds = numberOfSeconds(elapsed);

    display.print(days,DEC);  
    printDigits(hours);  
    printDigits(minutes);
    printDigits(seconds);
}

// printDigits
//  Prints decimal digits for elapsed time
void printDigits(byte digits){
    display.print(F(":"));
    if (digits < 10)
        display.print('0');
    display.print(digits, DEC);  
}

