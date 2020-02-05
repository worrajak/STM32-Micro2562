โปรแกรมการทดลอง

```
// the setup function runs once when you press reset or power the board
void setup() {
// initialize digital pin LED_BUILTIN as an output.
   pinMode(PC14, OUTPUT);
   pinMode(PC15, OUTPUT);
}
// the loop function runs over and over again forever
void loop() {
   digitalWrite(PC14, HIGH); // turn the LED on (HIGH is the voltage level)
   digitalWrite(PC15, HIGH);
     delay(1000);                  // wait for a second
   digitalWrite(PC14, LOW); // turn the LED off by making the voltage LOW
   digitalWrite(PC15, LOW);
     delay(1000);                  // wait PC13
}
```

วงจรการทดลอง

![ScreenShot](https://github.com/worrajak/STM32-Micro2562/blob/master/Lab1/Screen%20Shot%202562-12-18%20at%2006.44.00.jpg?raw=true)

