#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <Arduino.h>

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include "TSL2561.h"

#define SEALEVELPRESSURE_HPA (1013.25)

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

void readData(){
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;
    
    ir = lum >> 16;
    full = lum & 0xFFFF;
    lux_int = tsl.calculateLux(full, ir);
  
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    tempC_int = temperature*10;
    hum_int = humidity*2;
    adc_enable(ADC1);
    vbat_int = 120 * 4096 / adc_read(ADC1, 17);
    adc_disable(ADC1);
      
    oled.clear();
    oled.println("Sensor Test");
    oled.setCursor(0,2);
    oled.print(temperature,1);oled.print("C ");oled.print(humidity,1);oled.print("%");
    oled.setCursor(0,3);
    oled.print("lux:");oled.print(lux_int);oled.print(" Batt:");oled.print(vbat_int*0.01);    
        
#ifdef DEBUG
    Serial.print(temperature);Serial.print(" ");Serial.print(humidity);Serial.print(" ");Serial.print(vbat_int*0.01);Serial.print(" ");Serial.println(tsl.calculateLux(full, ir));
    Serial.println();
#endif   
}

void setup_vdd_sensor() {
    adc_reg_map *regs = ADC1->regs;
    regs->CR2 |= ADC_CR2_TSVREFE; // enable VREFINT and temp sensor
    regs->SMPR1 = (ADC_SMPR1_SMP17 /* | ADC_SMPR1_SMP16 */); // sample rate for VREFINT ADC channel
}

void setup() {
  setup_vdd_sensor();
  Serial.begin(9600);

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
}

void loop() {
    readData();
    delay(5000);
}



