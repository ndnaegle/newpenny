
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

// Arduino Inputs
// Pin 0 - NC
// Pin 1 - NC
// Pin 2 - 6818.9 - IN8 - NC
// Pin 3 - 6818.8 - IN7 - DB25.13 - Purple wire - Home sensor?
int homeSensorPin = 3
// Pin 4 - 6818.7 - IN6 - DB25.12 - Brown/White wire - 90 degree sensor? (these two could be swapped)
int degree90SensorPin = 4
// Pin 5 - 6818.6 - IN5 - DB25.11 - Black/Red/White wire - Magnetic Alarm Contact (Coin accept?)
int coinAcceptPin = 5
// Pin 6 - 6818.5 - IN4 - DB25.10 - Brown wire - Button 1
int button1Pin = 6
// Pin 7 - 6818.4 - IN3 - DB25.09 - Orange wire - Button 2
int button2Pin = 7
// Pin 8 - 6818.3 - IN2 - DB25.08 - Blue wire - Button 3
int button3Pin = 8
// Pin 9 - 6818.2 - IN1 - DB25.07 - Red wire - Button 4
int button4Pin = 9

// Arduino Outputs (going to have to use trial and error to figure this out)
// Pin 10 - 62783.I1 - ? Counter?
int outputCounterPin = 10
// Pin 11 - 62783.I2 - DB25.5 - Black wire - Middle Relay - Lower Solenoid
int outputSolenoidPin = 11
// Pin 12 - 62783.I3 - DB25.4 - Green wire - Left Relay - Motor Run
int outputMotorRunPin = 12
// Pin 13 - 62783.I4 - DB25.2 - Yellow wire - Right Relay - Upper Solenoid - Cycle Start?
int outputCycleStart = 13

// Outputs from old system...these probably map into the Arduino outputs above
// P1.0    CYCLE START
// P1.1    SOLENOID
// P1.2    MOTOR RUN
// P1.3    COUNTER


void setup() {
  
}

void loop() {

}

void reset() {
  // put the code here to reset the device back to the starting state
}

void wait_for_coin() {
  // this routine waits for the correct coins to be inserted
}



