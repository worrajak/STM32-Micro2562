
ADC -> PWM arduino STM32

ต่อเชื่อมขา PA8 เข้ากับขา PC14 เพื่อแสดงค่า PWM 

![ScreenShot](https://github.com/worrajak/STM32-Micro2562/blob/master/Lab4-PWM/S__2154498.jpg?raw=true)

Youtube 
https://www.youtube.com/watch?v=l4d_har-nIg

การคำนวนความถี่และหาค่า duty cycle
![ScreenShot](https://github.com/worrajak/STM32-Micro2562/blob/master/Lab4-PWM/uCCC037.jpg?raw=true)

Code โดย อ.จักร์ 
```
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiWire oled;

Adafruit_BME280 bme;

void setup() {
  
 Serial.begin(9600);
 
 bme.begin(); 
 #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
 #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
 #endif // RST_PIN >= 0

 oled.setFont(Adafruit5x7);
 
 pinMode(PA0,INPUT_ANALOG);
 ADC1->regs->CR2 |= ADC_CR2_TSVREFE;// enable VREFINT and temp sensor
 float volt = 3.3 * analogRead(PA0)/4096;//คำนวนเป็นโวลท์โดยใช้ VREF = 3.3v
  
 pinMode(PA8,PWM);
 Timer1.setPrescaleFactor(22);
 Timer1.setOverflow(65454);
 
 oled.clear();
 oled.println("STM32F103");
 oled.println("ADC PWM test");
}

void loop() {
  // put your main code here, to run repeatedly:
  float vref = 1200 * 4096/adc_read(ADC1,17);
  float volt = vref * analogRead(PA0)/4096;//คำนวนโวลท์โดยใช้ค่า VRef ขณะนั้น
  volt = volt * 0.001;
  int ADC = analogRead(PA0);
  int pwm_out = map(ADC,0,4095,0,65454);
  pwmWrite(PA8,pwm_out);
  oled.setCursor(0,3);oled.print("volt ADC = ");oled.print(volt,2);
    Serial.print("Volt = ");Serial.println(volt,2);  
  oled.setCursor(0,4);oled.print("ADC read = ");oled.print(ADC);
    Serial.print("ADC = ");Serial.println(ADC);
  oled.setCursor(0,5);oled.print("PWM out = ");oled.print(pwm_out);
    Serial.print("PWM out = ");Serial.println(pwm_out);

  delay(500);
}
```

Credit:  
https://www.makearduino.com/article/6/stm32-arduino-%E0%B9%80%E0%B8%88%E0%B8%B2%E0%B8%B0%E0%B8%A5%E0%B8%B6%E0%B8%81-pwm-timer

