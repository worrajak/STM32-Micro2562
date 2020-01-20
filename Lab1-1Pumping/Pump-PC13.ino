/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

int S1State = 0;
int S2State = 0;
int S3State = 0;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC13, OUTPUT);
  pinMode(PB4, INPUT); // S1
  pinMode(PB5, INPUT); // S2
  pinMode(PB6, INPUT); // S3
  Serial.begin(9600);
  Serial.println(F("Serial monitoring"));
  delay(1000);
}

// the loop function runs over and over again forever
void loop() {
    S1State = digitalRead(PB4);
    S2State = digitalRead(PB5);
    S3State = digitalRead(PB6);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if ((S3State == HIGH)&&(S2State == LOW)&&(S1State == LOW)) {
    // turn LED on:
    digitalWrite(PC13, LOW);
  } else {
    // turn LED off:
    digitalWrite(PC13, HIGH);
  }                       // wait for a second
}
