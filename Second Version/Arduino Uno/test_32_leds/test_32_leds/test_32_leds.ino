//
// - start up 
// - game (succeed, failed, keeping score, end of game)
// - session 

#define pinSer 2
#define pinLatch 3
#define pinClk 4

#define pinSwitchA 5
#define pinSwitchB 6

#define playerALed 0x80000000 //green leds
#define playerBLed 0x00040000 //blue leds

uint32_t digit = 1; //turn on one led
uint32_t playerAscore = 0x00000000;
uint32_t playerBscore = 0x00000000;


void setup() {
  pinMode(pinClk, OUTPUT);
  pinMode(pinSer, OUTPUT);
  pinMode(pinLatch, OUTPUT);

  pinMode(pinSwitchA, INPUT_PULLUP);
  pinMode(pinSwitchB, INPUT_PULLUP);
  Serial.begin(9600);
   
}

void loop() {
 
  delay(100);

  //~~~led running~~~
  //digit = ((digit << 1) | (digit >> 25)) & 0xFFFFFFC0; // 32 - 6 - 1 = 31 (from Q8 to Q0) 
  digit = ((digit >> 1) | (digit << 25)) & 0xFFFFFFC0; // 32 - 6- 1 = 31 (from Q0 to Q8)
  updateLeds(digit | playerAscore | playerBscore);
  // Serial.print(digit);
  // Serial.print(" ");
  // Serial.println(playerAscore);
  

  if(digitalRead(pinSwitchA)==LOW)
    {
      if (digit == playerALed)
          AddScore(true);
    }
  if(digitalRead(pinSwitchB)==LOW)
    {
      if (digit == playerBLed)
          AddScore(false);
    }

}

void updateLeds(uint32_t digit)
{
  digitalWrite(pinLatch, LOW);   

  //for two chained 74HC595 we use twice the shiftOut to send two bytes
  shiftOut(pinSer, pinClk, LSBFIRST, digit);
  shiftOut(pinSer, pinClk, LSBFIRST, digit>>8);
  shiftOut(pinSer, pinClk, LSBFIRST, digit>>16);
  shiftOut(pinSer, pinClk, LSBFIRST, digit>>24);
  
  digitalWrite(pinLatch, HIGH);
}

void AddScore(bool isPlayerA)
{
  if(isPlayerA)
  {
    uint32_t playerAscoreNew = (playerAscore >> 1) | 0x00000004; 
    for (int i = 0; i < 3; i++) {
      updateLeds(playerALed | playerAscoreNew | playerBscore);
      delay(500);
      updateLeds(playerALed | playerAscore | playerBscore);
      delay(500);
    }
    playerAscore = playerAscoreNew;
  }
  else {
    uint32_t playerBscoreNew = ((playerBscore >> 1) | 0x00000020) & 0x00000038; 
    for (int i = 0; i < 3; i++) {
      updateLeds(playerBLed | playerBscoreNew | playerAscore);
      delay(500);
      updateLeds(playerBLed | playerBscore | playerAscore);
      delay(500);
    }
    playerBscore = playerBscoreNew;
  }



  


}
