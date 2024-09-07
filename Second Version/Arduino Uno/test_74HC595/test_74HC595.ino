
#define pinSer 2
#define pinLatch 3
#define pinClk 4

uint16_t digit = 1;


void setup() {
  pinMode(pinClk, OUTPUT);
  pinMode(pinSer, OUTPUT);
  pinMode(pinLatch, OUTPUT);

}

void loop() {
 
  delay(100);
  digit = (digit << 1) | (digit >> 15); // 16 - 1 = 15 (from Q8 to Q0) 
  //digit = (digit >> 1) | (digit << 15); // 16 - 1 = 15 (from Q0 to Q8)
  updateLeds(digit);
  

}

void updateLeds(uint16_t digit)
{
  digitalWrite(pinLatch, LOW);   

  //for two chained 74HC595 we use twice the shiftOut to send two bytes
  shiftOut(pinSer, pinClk, LSBFIRST, digit);
  shiftOut(pinSer, pinClk, LSBFIRST, digit>>8);
  
  digitalWrite(pinLatch, HIGH);
}
