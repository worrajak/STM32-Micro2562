#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClock.h>
#include <TimeLib.h>
//this is where we will store the unix time
uint32_t tt = 0;

// initialise the RTC library. 
// NOTE: 'RTCSEL_LSE' assumes you have a 32.768Mhz crystal setup physically attached to the appropriate pins 
// on the microcontroller. Pins PC15 and PC14 are the RTC crystal pins.
RTClock rtc (RTCSEL_LSE); 


void setup() {
  Serial.begin(9600); //i work with STM32F103RET (64K RAM 512K Flash, it have 4 Serial ports)
  delay(500);
  Serial.println("Enter onEvent");
  //rtc.setTime(1581935340);//1556469115 02/17/2020 @ 10:29am
  //you can also SET the rtc time with rtc.setTime(unixTimestamp) where unixTimestamp is a unix timestamp
}

void loop() {
  //get the RTC time (returns unix time, a 32bit integer representing seconds elapsed from Jan 01, 1970)
  tt = rtc.getTime();
  breakTime(tt); 
  delay(1000);//or whatever
}

void breakTime(uint32_t timeInput){
  Serial.print(year(tt));Serial.print("/");
  Serial.print(month(tt));Serial.print("/");
  Serial.print(day(tt));Serial.print("/ ");
  //Serial.print(weekday(tt));Serial.print("// ");
  Serial.print(hour(tt));Serial.print(":");
  Serial.print(minute(tt));Serial.print(":"); 
  Serial.print(second(tt));Serial.print(" ");
  Serial.println();
}
