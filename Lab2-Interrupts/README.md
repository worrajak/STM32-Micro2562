
https://circuitdigest.com/microcontroller-projects/how-to-use-interrupts-in-stm32f103c8

```
volatile boolean ledOn = false;   

void setup() {
  
  Serial.begin(9600);
  pinMode(PC14,OUTPUT);
  pinMode(PA1,INPUT_PULLUP);       //  set pin PA0 as input 
  int i = 0;                                                        //  declare variable i and initiliaze with 0 
  attachInterrupt(PA1,buttonPressed,CHANGE);
  
}

void loop(){
  Serial.print("LED = ");Serial.println(ledOn);
  delay(1000);
}


void buttonPressed(){                    
  if(ledOn){
     ledOn=false;                                                 //  Makes ledOn false if it is True
     digitalWrite(PC14,LOW);                                       //  digital writs the low vale to PC13 pin makes led OFF
  }
  else {
     ledOn = true;                                                //  Makes ledOn True if it is False
     digitalWrite(PC14,HIGH);                                      //  digital writs the HIGH vale to PC13 pin makes led ON
  }
}
```
