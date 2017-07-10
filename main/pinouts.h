// pinouts.h
//  Pinouts for how the hardware connects to the Arduino
//  July 2017

#ifndef _PINOUT_H
#define _PINOUT_H

// Arduino Inputs
// Pin 0 - NC
// Pin 1 - NC
// Pin 2 - 6818.9 - IN8 - NC - broken shield version
const int homeSensorPin = 2; // wired shield version
// Pin 3 - 6818.8 - IN7 - DB25.13 - Purple wire - Home sensor?
// const int homeSensorPin = 3; // broken shield version
const int degree90SensorPin = 3; // wired shield version
// Pin 4 - 6818.7 - IN6 - DB25.12 - Brown/White wire - 90 degree sensor? (these two could be swapped)
// int degree90SensorPin = 4;
const int coinAcceptPin = 4; // wired shield version
// Pin 5 - 6818.6 - IN5 - DB25.11 - Black/Red/White wire - Magnetic Alarm Contact (Coin accept?)
// int coinAcceptPin = 5; // broken shield version
const int button1Pin = 5; // wired shield version
// Pin 6 - 6818.5 - IN4 - DB25.10 - Brown wire - Button 1
// int button1Pin = 6; // broken shield version
const int button2Pin = 6; // wired shield version
// Pin 7 - 6818.4 - IN3 - DB25.09 - Orange wire - Button 2
// int button2Pin = 7; // broken shield version
const int button3Pin = 7; // wired shield version
// Pin 8 - 6818.3 - IN2 - DB25.08 - Blue wire - Button 3
// int button3Pin = 8; // broken shield version
const int button4Pin = 8; // wired shield version
// Pin 9 - 6818.2 - IN1 - DB25.07 - Red wire - Button 4
// int button4Pin = 9; // broken shield version
// Pin 9 not connected on wired shield version

// Arduino Outputs
// 62783.I1 - (dedicated plugin) - Counter
// int outputCounterPin = 10;
const int outputCounterPin = 12; // wired shield version
// 62783.I2 - DB25.5 - Black wire - Middle Relay - Lower Solenoid - Coin Drop
// int outputCoinDropPin = 11;
const int outputCoinDropPin = A1; // wired shield version (substitute 13 for A5)
// 62783.I3 - DB25.4 - Green wire - Left Relay - Motor Run
// int outputMotorRunPin = 12;
const int outputMotorRunPin = 10; // wired shield version
// 62783.I4 - DB25.2 - Yellow wire - Right Relay - Upper Solenoid - Coin Slot Protect
// int outputRedLeverLiftPin = 13; // Note that this is also the onboard LED pin
const int outputRedLeverLiftPin = 11; // wired shield version

// 6818 pinout
// 1 /EN Tri-state enable - NC
// 2 IN1 - DB25.7 - Red wire - Button 4
// 3 IN2 - DB25.8 - Blue wire - Button 3
// 4 IN3 - DB25.9 - Orange wire - Button 2

// 5 IN4 - DB25.10 - Brown wire - Button 1
// 6 IN5 - DB25.11 - Black/Red/White wire - Magnetic Alarm Contact
// 7 IN6 - DB25.12 - Brown/White wire - Home sensor or 90 degree
// 8 IN7 - DB25.13 - Purple wire - Home sensor or 90 degree
// 9 IN8 - NC
// 10 Ground
// 11 /CH - NC
// 12 OUT8 - Pin 2
// 13 OUT7 - Pin 3
// 14 OUT6 - Pin 4
// 15 OUT5 - Pin 5
// 16 OUT4 - Pin 6
// 17 OUT3 - Pin 7
// 18 OUT2 - Pin 8
// 19 OUT1 - Pin 9
// 20 Vcc


#endif // #ifndef _PINOUT_H
