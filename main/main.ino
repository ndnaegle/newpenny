
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

// Not found: Coin Accept, Go/No-Go

// Arduino Inputs
// Pin 0 - Not connected
// Pin 1 - Not connected
// Pin 2 - 6818.8 - NC
// Pin 3 - 6818.7 - DB25.13 - 


// P0.0    GO/NO-GO    GO=0, HALT=1
// P0.1    COIN ACCEPT
// P0.2    DIE # 4
// P0.3    DIE # 3
// P0.4    DIE # 2
// P0.5    DIE # 1
// P0.6    90 DEGREE
// PO.7    HOME

// Outputs
// P1.0    CYCLE START
// P1.1    SOLENOID
// P1.2    MOTOR RUN
// P1.3    COUNTER
// P1.4    "A"     UNUSED
// P1.5    "B"     UNUSED
// P1.6    "C"     UNUSED
// P1.7    "D"     UNUSED


void setup() {
  
}

void loop() {

}

void reset() {
  // put the code here to reset the device back to the starting state
}

void wait_for_coin() {
  // this routine waits for a coin. While it's waiting, it flashes the LEDs in sequence A, B, C, D
}



