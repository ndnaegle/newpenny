// Blink
//  Blinks the LED (on pin 13) in various patterns

int ledPin = 13;                // LED connected to digital pin 13
int loopCount = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  blinkNtimes(5);
  delay(1000);
  blinkNtimes(3);
  Serial.print("Finished a loop #");
  Serial.println(++loopCount, DEC);
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

