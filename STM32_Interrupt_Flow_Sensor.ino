byte sensorInterrupt = 0;
byte sensorPin      = PB1;
float calibrationFactor = 0.24;
volatile byte pulseCount;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
int total1;
unsigned long oldTime;

void pulseCounter() {
  pulseCount++;
}

void readData(){
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) * calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    total1 = totalMilliLitres;
    pulseCount = 0;
    attachInterrupt(sensorPin, pulseCounter, FALLING);
    
    Serial.print("flowrate ");Serial.println(flowRate);Serial.print(" mL/min. ");
}

void setup() {
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);  
  Serial.begin(9600);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);  
    
}

void loop() {
  // put your main code here, to run repeatedly:
  readData();  
  delay(5000);

}
