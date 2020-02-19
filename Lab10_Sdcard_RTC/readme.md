code 


```
#include <RTClock.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <SD.h>
#include <SPI.h>


Adafruit_BME280 bme; // I2C #define BME280_ADDRESS                (0x76)

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiWire oled;

RTClock rtclock(RTCSEL_LSE);RTClock rtclock(RTCSEL_LSE);

#define SPI_NSS_PIN PA4

File myFile;
#define tLog    1

#define LOG_INTERVAL  1000
int timezone = 7;
time_t tt, tt1;
tm_t curTime;
char bld[40];

uint8_t dateread[11];
int globAlmCount = 0;
int lastGlobAlmCount;
int SPECAlmCount = 0;
int lastSPECAlmCount;
int alarmcount = 3;
uint8_t AlarmExchange = 0;
bool dispflag = true;

const char * months[] = {"Dummy", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12" };


uint8_t str2month(const char * d)
{
  uint8_t i = 13;
  while((--i) && strcmp(months[i], d));
  return i;
}
const char *delim = " :";
char s[128];



char buf[25];
//char *dowTxt[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", "??"};

void SecondCount ()
{
  tt++;
}

byte sensorInterrupt = 0;
byte sensorPin      = PB1;
float calibrationFactor = 4.52; // G1" = 0.24, G2" = 4.52
volatile byte pulseCount;
float flowRate;
unsigned long flowMilliLiters;
unsigned long totalMilliLiters;
int total1,total2;
unsigned long oldTime;

void pulseCounter()
{
  pulseCount++;
}

void readFlow(){
    if ((millis() - oldTime) > 1000)   // Only process counters once per second
  {
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) * calibrationFactor;
    oldTime = millis();
    flowMilliLiters = (flowRate / 60) * 1000;
    totalMilliLiters += flowMilliLiters;
    total1 = totalMilliLiters / 1000;
    total2 = (totalMilliLiters / 1000 ) / 1000;
    pulseCount = 0;
    attachInterrupt(sensorPin, pulseCounter, FALLING);
  }
}

void setup() {
  bme.begin();
  Serial.begin(9600); 
  delay(3000);

  pinMode(SPI_NSS_PIN, OUTPUT);

  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.println("Flow meter logger");
  oled.setCursor(0,1);oled.print("initial SDcard");

  //rtclock.setTime(1582140360);
  
  if (!SD.begin(SPI_NSS_PIN)) {
    oled.setCursor(0,1);oled.print("SDcard failed!");
    while (1);
  }
  myFile = SD.open("datalog.csv", FILE_WRITE);
  myFile.println("Date,Time,Flowrate(L/min),Volume(L)");
  myFile.close();
  oled.setCursor(0,1);oled.print("SD Write done!");
}

void loop() {
    readFlow();
 
    int mm = curTime.minute;
    int ss = curTime.second;
    
    rtclock.breakTime(rtclock.now(), curTime);
    sprintf(s,"%u/%s/2020,%02u:%02u",curTime.day, months[curTime.month],curTime.hour, curTime.minute);
    oled.setCursor(0,6);oled.print(s);
    

    if((mm%tLog==0)&&(ss==0))
    {
      myFile = SD.open("datalog.csv", FILE_WRITE);
      if(myFile){
      myFile.print(s);myFile.print(",");myFile.print(flowRate);myFile.print(",");myFile.println(total1);
      myFile.close();
      oled.setCursor(0,1);oled.print("SD record done!  ");
      delay(2000);
      }
    }
    oled.setCursor(0,1);oled.print("Waiting for data");  
    oled.setCursor(0,3);oled.print("Volume:  ");oled.print(total1);oled.print(" L");
    oled.setCursor(0,4);oled.print("Flowrate:");oled.print(flowRate);oled.print(" L/min");
    delay(1000);
}
```
