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

    // Setup serial port and wait until it's ready
    Serial.begin(115200);
    while (!Serial)
        ;

    // Give the user a chance to start the debug console
    debugConsole();

    // make sure hardware is in a sane state
    outputRedLeverLift.write(LOW);
    outputMotorRun.write(LOW);
    outputCoinDrop.write(LOW);
    outputCounter.write(LOW);

    // Start at reset state
    Serial.println("Starting state machine...");
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
        gotoState(WAIT_FOR_HOME_STATE);
        break;

    case WAIT_FOR_HOME_STATE:
        if (inputHome.read() == LOW) {
            Serial.println("Motor reached home position");
            outputMotorRun.write(LOW);
            outputRedLeverLift.write(HIGH);             
            gotoState(WAIT_FOR_COIN_STATE);
        }
        break;
    case WAIT_FOR_COIN_STATE:
        if (inputCoinAccept.read() == LOW) {
            Serial.println("Coin has been accepted");
            outputRedLeverLift.write(LOW);
            inputButton1.start();
            inputButton2.start();
            inputButton3.start();
            inputButton4.start();
            gotoState(WAIT_FOR_BUTTON_STATE);            
        }
        break;
        
    case WAIT_FOR_BUTTON_STATE:
        if (inputButton1.read() == LOW) {
            Serial.println("Button 1 has been pressed");
            stamp = 1;
            outputMotorRun.write(HIGH);
            gotoState(STAMP_SELECTOR_STATE);
        }
        else if (inputButton2.read() == LOW) {
            Serial.println("Button 2 has been pressed");
            stamp = 2;
            outputMotorRun.write(HIGH);
            gotoState(STAMP_SELECTOR_STATE);
        }
        else if (inputButton3.read() == LOW) {
            Serial.println("Button 3 has been pressed");
            stamp = 3;
            outputMotorRun.write(HIGH);
            gotoState(STAMP_SELECTOR_STATE);
        }
        else if (inputButton4.read() == LOW) {
            Serial.println("Button 4 has been pressed");
            stamp = 4;
            outputMotorRun.write(HIGH);
            input90Degree.start();
            gotoState(STAMP_SELECTOR_STATE);
        }
        break;
    case STAMP_SELECTOR_STATE:
        if (input90Degree.read() == LOW) {
            edgeDetect = true;  // We need to make sure we don't count down until the 90 degree sensor has risen *and* fallen
        } else if (input90Degree.read() == HIGH && edgeDetect) {
            edgeDetect = false;                    
            if (--stamp == 0) { // countdown the number of 90 degree sensor hits we've had
                outputCoinDrop.write(HIGH);
                Serial.println("The coin has been dropped");
                outputCounter.write(HIGH);
                countdown = millis();
                gotoState(COIN_DROP_STATE);
            }
        }
        break;
    case COIN_DROP_STATE:
        if (millis() > countdown + 1000) {
            outputCoinDrop.write(LOW);
            outputCounter.write(LOW);
            gotoState(WAIT_FOR_STAMP_FINISH_STATE);
            countdown = millis();
        }
        break;
    case WAIT_FOR_STAMP_FINISH_STATE:
        if (millis() > countdown + 5 * 1000) {
            gotoState(START_STATE);
        }
        break;                                                          

    case SHUTDOWN_STATE:
        outputRedLeverLift.write(LOW);
        outputMotorRun.write(LOW);
        outputCoinDrop.write(LOW);
        outputCounter.write(LOW);        
        gotoState(FOREVER_STATE);
        break;

    case RESET_STATE:
        Serial.println("Resetting board using watchdog...\n\n");
        Serial.flush();
        
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Reset!");
        display.display();

        // enable watchdog timer (which will reboot the board) and wait until it fires
        wdt_enable(WDTO_15MS);
        while (true)
            ;
        break;

    case FOREVER_STATE:
        break;
    }
    updateSerialKeys();
    displayStatus();
}


// displayStatus
//  Shows the status of the system on the OLED screen
void displayStatus(void) {
    static unsigned long timeLastStateChange = 0;
    static int lastState = INVALID_STATE;
    
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
    inputHome.displayLevel();
    input90Degree.displayLevel();
    inputCoinAccept.displayLevel();
    inputButton1.displayLevel();
    inputButton2.displayLevel();
    inputButton3.displayLevel();
    inputButton4.displayLevel();    
    display.println();

    // time since last state change (TODO: use for screen saver)
    unsigned long timeNow = millis();
    if (theState != lastState) {
        lastState = theState;
        timeLastStateChange = timeNow;
    }
    unsigned long timeSinceLastStateChange = timeNow - timeLastStateChange;
    display.setTextSize(1);
    display.print("LS: ");
    displayElapsedTime(timeSinceLastStateChange / 1000);
    display.display();
}

// gotoState
//  Transitions to the specified state
//  (and gives us a chance to log the state change)
void gotoState(int newState) {
    // Display on serial
    Serial.print("Transitioning state: ");
    Serial.print(dumpState(theState));
    Serial.print("-->");
    Serial.print(dumpState(newState));
    Serial.println();

    // Also store it in the EEPROM log
    theLog.output(theState, newState);

    // Finally set the global state for the next loop
    theState = newState;
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
    display.print(":");
    if (digits < 10)
        display.print('0');
    display.print(digits, DEC);  
}

