
#define pinSer 2
#define pinLatch 3
#define pinClk 4



void setup() {
  

  pinMode(pinClk, OUTPUT);
  pinMode(pinSer, OUTPUT);
  pinMode(pinLatch, OUTPUT);

 //byte digit = 0b00100000;
byte digit = 0b10000000;
  
  digitalWrite(pinLatch, LOW);   
  
   for (int i = 0; i < 8; i++)
   {
    delay(20);
     digitalWrite(pinSer,  bitRead(digit,i));   
     delay(20);
     digitalWrite(pinClk, HIGH);   
     delay(20);
     digitalWrite(pinClk, LOW);
     
   } 
   digitalWrite(pinLatch, HIGH);
   

}

void loop() {
 

}
