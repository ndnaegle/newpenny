// output.cpp
//  Manage the output pins
//  July 2017

#include <Arduino.h>
#include "main.h"
#include "output.h"
#include "pinouts.h"
#include "display.h"

Output outputCounter(outputCounterPin, "Counter", 'X');
Output outputCoinDrop(outputCoinDropPin, "Coin Drop", 'D');
Output outputMotorRun(outputMotorRunPin, "Motor Run", 'M');
Output outputRedLeverLift(outputRedLeverLiftPin, "Lift Red Lever", 'L');

Output::Output(int pin, const char* name, char displaySymbol):
    m_pin(pin),
    m_name(name),
    m_displaySymbol(displaySymbol)
{
    m_value = LOW;
}

void Output::write(int value) {
    m_value = value;
    digitalWrite(m_pin, value);
    Serial.print("Writing value to ");
    Serial.print(m_name);
    Serial.print("-->");
    Serial.println(value, DEC);
}

int Output::readback(void) {
    return m_value;
}

// displayLevel
//  Displays the output's level on the OLED screen
void Output::displayLevel(void) {
    if (this->readback() == HIGH) {
        display.setTextColor(BLACK, WHITE);
    }
    display.print(m_displaySymbol);
    display.setTextColor(WHITE);
}

