

void setup() {
  
 Serial.begin(9600);
 
 pinMode(PA0,INPUT_ANALOG);
 ADC1->regs->CR2 |= ADC_CR2_TSVREFE;// enable VREFINT and temp sensor
 float volt = 3.3 * analogRead(PA0)/4096;//คำนวนเป็นโวลท์โดยใช้ VREF = 3.3v
  
 pinMode(PA8,PWM);
 Timer1.setPrescaleFactor(22);
 Timer1.setOverflow(65454);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  float vref = 1200 * 4096/adc_read(ADC1,17);
  float volt = vref * analogRead(PA0)/4096;//คำนวนโวลท์โดยใช้ค่า VRef ขณะนั้น
  volt = volt * 0.001;
  int ADC = analogRead(PA0);
  int pwm_out = map(ADC,0,4095,0,65454);
  pwmWrite(PA8,pwm_out);
    Serial.print("Volt = ");Serial.println(volt,2);  
    Serial.print("ADC = ");Serial.println(ADC);
    Serial.print("PWM out = ");Serial.println(pwm_out);
  delay(500);
}
