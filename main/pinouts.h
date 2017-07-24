// pinouts.h
//  Pinouts for how the hardware connects to the Arduino
//  July 2017

#ifndef _PINOUT_H
#define _PINOUT_H

// Arduino Inputs
// Pin 0 - NC
// Pin 1 - NC
const int homeSensorPin = 2;
const int degree90SensorPin = 3;
const int coinAcceptPin = 4;
const int button1Pin = 5;
const int button2Pin = 6;
const int button3Pin = 7;
const int button4Pin = 8;
//const int buttonResetPin = 9; // not working, so "disconnect"
const int buttonResetPin = 0;

// Arduino Outputs
const int outputCounterPin = 10;
const int outputCoinDropPin = A1;
const int outputMotorRunPin = 12;
const int outputRedLeverLiftPin = 11;

#endif // #ifndef _PINOUT_H
