#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <Adafruit_MAX31865.h>

#define SPI_NSS_PIN PA4
SPIClass SPI_2(2);

Adafruit_MAX31865 max = Adafruit_MAX31865(PA4);

#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C #define BME280_ADDRESS 

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiWire oled;

float temperature = 0;
float humidity = 0;
uint16_t tempC_int = 0;
uint8_t hum_int = 0;
uint16_t lux_int = 0;
int vbat_int;

void setup_vdd_sensor() {
    adc_reg_map *regs = ADC1->regs;
    regs->CR2 |= ADC_CR2_TSVREFE; // enable VREFINT and temp sensor
    regs->SMPR1 = (ADC_SMPR1_SMP17 /* | ADC_SMPR1_SMP16 */); // sample rate for VREFINT ADC channel
}


float extT;

void readData(){

  uint16_t rtd = max.readRTD();
  extT = max.temperature(RNOMINAL, RREF);
  //Serial.print("RTD value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  //Serial.print("Ratio = "); Serial.println(ratio,8);
  //Serial.print("Resistance = "); Serial.println(RREF*ratio,8);

    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    adc_enable(ADC1);
    vbat_int = 120 * 4096 / adc_read(ADC1, 17);
    adc_disable(ADC1);

    oled.clear();
    oled.println("PT100 Demo"); 
    oled.setCursor(0,2);
    oled.print("T:");oled.print(temperature,1);oled.print(" RH:");oled.print(humidity,1);
    oled.setCursor(0,4);oled.print("TR Temp:");oled.print(extT,2);

    Serial.print(temperature);Serial.print(" ");
    Serial.print(humidity);Serial.print(" ");
    Serial.print(vbat_int);Serial.print(" ");
    Serial.print(extT,2);Serial.print(" ");
}

void setup(){ 

  Serial.begin(9600);
  delay(1000);

  SPI.begin();
  pinMode(SPI_NSS_PIN, OUTPUT);

  max.begin(MAX31865_3WIRE); 

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

unsigned long delTime = 10000;

void loop()
{ 
      readData();     
      delay(5000); 
}

