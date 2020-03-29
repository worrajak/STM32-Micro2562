code 

![ScreenShot](https://github.com/worrajak/STM32-Micro2562/blob/master/Lab10_Sdcard_RTC/S__5079055.jpg?raw=true)

```
#include <RTClock.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include "TSL2561.h"
#include <SD.h>
#include <SPI.h>

Adafruit_BME280 bme; // I2C #define BME280_ADDRESS                (0x76)
TSL2561 tsl(TSL2561_ADDR_FLOAT); 

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiWire oled;

float temperature = 0;
float humidity = 0;
uint16_t tempC_int = 0;
uint8_t hum_int = 0;
uint16_t lux_int = 0;
int vbat_int;

RTClock rtclock(RTCSEL_LSE);

#define SPI_NSS_PIN PA4

File myFile;
#define tLog    1

#define LOG_INTERVAL  1000
int timezone = 7;
time_t tt, tt1;
tm_t curTime;
char bld[40];

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

void setup_vdd_sensor() {
    adc_reg_map *regs = ADC1->regs;
    regs->CR2 |= ADC_CR2_TSVREFE; // enable VREFINT and temp sensor
    regs->SMPR1 = (ADC_SMPR1_SMP17 /* | ADC_SMPR1_SMP16 */); // sample rate for VREFINT ADC channel
}

void readData(){
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;
    
    ir = lum >> 16;
    full = lum & 0xFFFF;
    lux_int = tsl.calculateLux(full, ir);
  
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    adc_enable(ADC1);
    vbat_int = 120 * 4096 / adc_read(ADC1, 17);
    adc_disable(ADC1);
  
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
  setup_vdd_sensor();
  Serial.begin(9600); 
  delay(3000);

  pinMode(SPI_NSS_PIN, OUTPUT);

  if (tsl.begin()) {
    Serial.println("Found TLS2561 sensor");
  } else {
    Serial.println("No TLS2561 sensor?");
    while (1);
  }  

  tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)  
   
  bool status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.println("STM32 Data logger ");
  oled.setCursor(0,1);oled.print("initial SDcard");

  //rtclock.setTime(1582285560);
  
  if (!SD.begin(SPI_NSS_PIN)) {
    oled.setCursor(0,1);oled.print("SDcard failed!");
    while (1);
  }
  myFile = SD.open("datalog.csv", FILE_WRITE);
  myFile.println("Date,Time,Flowrate(L/min),Volume(L),Vbat(V)");
  myFile.close();
  oled.setCursor(0,1);oled.print("SD Write done!");
}

void loop() {
    readFlow();
    readData();
    int mm = curTime.minute;
    int ss = curTime.second;
    
    rtclock.breakTime(rtclock.now(), curTime);
    sprintf(s,"%u/%s/2020,%02u:%02u",curTime.day, months[curTime.month],curTime.hour, curTime.minute);
    oled.setCursor(0,6);oled.print(s);
  
    if((mm%tLog==0)&&(ss==0))
    {
      myFile = SD.open("datalog.csv", FILE_WRITE);
      if(myFile){
      myFile.print(s);myFile.print(",");
      myFile.print(flowRate);myFile.print(",");
      myFile.print(total1);myFile.print(",");
      myFile.print(temperature,2);myFile.print(",");
      myFile.print(humidity,0);myFile.print(",");
      myFile.print(lux_int);myFile.print(",");
      myFile.println(vbat_int*0.01);
      myFile.close();
      oled.setCursor(0,1);oled.print("SD record done!  ");
      delay(2000);
      }
    }
    oled.setCursor(0,1);oled.print("---------------------");  
    oled.setCursor(0,2);oled.print("Q :");oled.print(total1);       oled.setCursor(50,2);oled.print(" Fr :");oled.print(flowRate);
    oled.setCursor(0,3);oled.print("T :");oled.print(temperature,1);oled.setCursor(50,3);oled.print(" RH :");oled.print(humidity,0);
    oled.setCursor(0,4);oled.print("Vb:");oled.print(vbat_int*0.01);oled.setCursor(50,4);oled.print(" lux:");oled.print(lux_int);
    oled.setCursor(0,5);oled.print("---------------------"); 
    delay(1000);
}

```
