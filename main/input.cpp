// Input.cpp
//  Reads from debounced inputs using a variable settle time
//  June 2017

#include <Arduino.h>
#include "main.h"
#include "input.h"
#include "pinouts.h"
#include "display.h"

typedef struct {
    int key;
    int pin;
    int timeout;
} SERIALKEY;
SERIALKEY serialkeys[MAX_PINS];
#define PIN_ON_TIME 200 // simulated keypress time

void initSerialKeys(void) {
    for (int i = 0 ; i < MAX_PINS ; ++i) {
        serialkeys[i].key = 0;
        serialkeys[i].pin = HIGH;
        serialkeys[i].timeout = 0;
    }
    serialkeys[homeSensorPin].key = 'h';
    serialkeys[degree90SensorPin].key = '9';
    serialkeys[coinAcceptPin].key = 'c';
    serialkeys[button1Pin].key = '1';
    serialkeys[button2Pin].key = '2';
    serialkeys[button3Pin].key = '3';
    serialkeys[button4Pin].key = '4';
    serialkeys[buttonReset].key = 'r';
}

// updateSerialKeys
//  Uses serial key entry to simulate pin state. Called every loop()
void updateSerialKeys(void) {
    static unsigned long int lastTime = 0;
    unsigned long int nowTime;

    while (Serial.available() > 0) {
        int incoming = Serial.read() | 32; // or with 32 forces value to lowercase
        for (int i = 0 ; i < MAX_PINS ; ++i) {
            if (serialkeys[i].key == incoming) {
                serialkeys[i].pin = LOW;
                serialkeys[i].timeout = PIN_ON_TIME;
                break;
            }
        }
    }

    // Walk through and make sure all timeouts go down appropriately
    nowTime = millis();
    for (int i = 0 ; i < MAX_PINS ; ++i) {
        if (serialkeys[i].timeout > 0) {
            serialkeys[i].timeout -= nowTime - lastTime;
            if (serialkeys[i].timeout <= 0) {
                serialkeys[i].timeout = 0;
                serialkeys[i].pin = HIGH;
            }
        }
    }
    lastTime = millis();
}

// with hardware debounce, we don't need much here. Zero might be OK.
// with no hardware debounce, increase this to 40-50 ms
#define DEBOUNCE_TIME 5 

Input inputHome(homeSensorPin, DEBOUNCE_TIME, LOW, 'H');
Input input90Degree(degree90SensorPin, DEBOUNCE_TIME, LOW, '9');
Input inputCoinAccept(coinAcceptPin, DEBOUNCE_TIME, LOW, 'A');
Input inputButton1(button1Pin, DEBOUNCE_TIME, LOW, '1');
Input inputButton2(button2Pin, DEBOUNCE_TIME, LOW, '2');
Input inputButton3(button3Pin, DEBOUNCE_TIME, LOW, '3');
Input inputButton4(button4Pin, DEBOUNCE_TIME, LOW, '4');
Input inputReset(buttonReset, DEBOUNCE_TIME, LOW, 'R');

Input::Input(int pin, int delay, int activeOn, char displaySymbol):
    m_lastDebounceTime(0),
    m_lastButtonState(HIGH),
    m_pin(pin),
    m_debounceDelay(delay),
    m_activeOn(activeOn),
    m_displaySymbol(displaySymbol)
{
}

// start
//  starts the counter for debounced input
void Input::start(void) {
    m_lastDebounceTime = millis();
    m_lastButtonState = digitalRead(m_pin) & serialkeys[m_pin].pin;
}

// read
//  debounces the input. Returns whether the pin is active (1 or 0) or -1 if we haven't waited long enough yet.
int Input::read(void) {
    int reading = this->readBouncy();
    if (reading != m_lastButtonState) {
        m_lastDebounceTime = millis();
        m_lastButtonState = reading;
    }
    if (millis() > m_lastDebounceTime + m_debounceDelay) {
        return m_lastButtonState == m_activeOn;
    } else {
        return -1;
    }
}

// readBouncy
//  reads the true state of either pin or serial keys with no debouncing
int Input::readBouncy(void) {
    return digitalRead(m_pin) & serialkeys[m_pin].pin;
}

// displayLevel
//  Displays the input's level on the OLED screen
void Input::displayLevel(void) {
    if (this->readBouncy() == LOW) {
        display.setTextColor(BLACK, WHITE);
    }
    display.print(m_displaySymbol);
    display.setTextColor(WHITE);
}

