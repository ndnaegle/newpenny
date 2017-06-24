/*
 * Blink
 *
 * The basic Arduino example.  Turns on an LED on for one second,
 * then off for one second, and so on...  We use pin 13 because,
 * depending on your Arduino board, it has either a built-in LED
 * or a built-in resistor so that you need only an LED.
 *
 * http://www.arduino.cc/en/Tutorial/Blink
 */

int ledPin = 13;                // LED connected to digital pin 13
int loopCount = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(ledPin, HIGH);
//  delay(100);
//  digitalWrite(ledPin, LOW);
//  delay(100);
//  Serial.print("Finished a loop #");
//  Serial.println(++loopCount, DEC);
   blinkNtimes(5);
   delay(1000);
   blinkNtimes(3);
   delay(2000);
}

void blinkNtimes(int n) {
  for (int i = 0 ; i < n ; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);    
  }
}

