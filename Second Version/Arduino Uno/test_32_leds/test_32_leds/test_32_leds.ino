//
// - start up (X)
// -- leds shows and start
//
// - game (succeed (V), , keeping score (V), )
// - failed(X):
// - - random place, random direction, 1 second penalty 
//
// - end of game (X):
// - - lights towards winner
//
//
// - session(?) 
//buzzer implemenation
//change to interupt (V)

#define pinSer 5
#define pinLatch 6
#define pinClk 4

#define pinSwitchA 2
#define pinSwitchB 3

#define playerALed 0x80000000 //green leds
#define playerBLed 0x00040000 //blue leds

uint32_t digit = 256; //turn on one led.    //32 bits from left to right the bits of the game, the right most 6 bits are the score
uint32_t playerAscore = 0x00000000;
uint32_t playerBscore = 0x00000000;
uint32_t gameSpeed = 100; //milis between each led 

long AlastTimePressed = millis();
uint32_t debounceTime = 10; // in miliseconds  
// bool Bpressable = true;

bool Aplayed = false;
bool sessionWonA = false;



void setup() {
  pinMode(pinClk, OUTPUT);
  pinMode(pinSer, OUTPUT);
  pinMode(pinLatch, OUTPUT);

  pinMode(pinSwitchA, INPUT_PULLUP);
  pinMode(pinSwitchB, INPUT_PULLUP);

  
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pinSwitchA), switchAPressed, FALLING);
  // attachInterrupt(digitalPinToInterrupt(pinSwitchB), switchBPressed, FALLING);
   
}

void loop() {
 
  

  //~~~led running~~~
  
   digit = ((digit << 1) | (digit >> 25)) & 0xFFFFFFC0; // 32 - 6 - 1 = 31 (from Q8 to Q0) 
  // Serial.println(digit);
  //  digit = ((digit >> 1) | (digit << 25)) & 0xFFFFFFC0; // 32 - 6- 1 = 31 (from Q0 to Q8)
  updateLeds(digit | playerAscore | playerBscore);
  // Serial.print(digit);
  // Serial.print(" ");
  // Serial.println(playerAscore);

  delay(gameSpeed);

  if(Aplayed)
  {
    if (sessionWonA)
      AddScore(true);
    else
      lowerScore(true);
    
    Aplayed=false;
  }
  // if(digitalRead(pinSwitchA)==LOW)
  //   {
      
  //     if (digit == playerALed)
  //         AddScore(true);
  //     // else
  //       //  statement;
  //   }
  // if(digitalRead(pinSwitchB)==LOW)
  //   {
      
  //     if (digit == playerBLed)
  //         AddScore(false);
  //   }

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
      updateLeds(digit | playerAscoreNew | playerBscore);
      delay(500);
      updateLeds(digit | playerAscore | playerBscore);
      delay(500);
    }
    playerAscore = playerAscoreNew;
  }
  else {
    uint32_t playerBscoreNew = ((playerBscore >> 1) | 0x00000020) & 0x00000038; 
    for (int i = 0; i < 3; i++) {
      updateLeds(digit | playerBscoreNew | playerAscore);
      delay(500);
      updateLeds(digit | playerBscore | playerAscore);
      delay(500);
    }
    playerBscore = playerBscoreNew;
  }
}
void lowerScore(bool isPlayerA)
{
if(isPlayerA)
  {
    uint32_t playerAscoreNew = (playerAscore << 1) & 0x00000007; 
    for (int i = 0; i < 3; i++) {

      if (i==2)
        updateLeds(digit | playerAscoreNew | playerBscore);
      else
        updateLeds(digit | playerAscore | playerBscore);
      delay(250);

      updateLeds(playerAscoreNew | playerBscore);
      delay(250);

      if (i==2)
        updateLeds(digit | playerAscoreNew | playerBscore);
      else
        updateLeds(digit | playerAscore | playerBscore);
      delay(250);

      updateLeds(playerAscoreNew | playerBscore);
      delay(250);

    }
    playerAscore = playerAscoreNew;
  }
  else {
    uint32_t playerBscoreNew = ((playerBscore >> 1) | 0x00000020) & 0x00000038; 
    for (int i = 0; i < 3; i++) {
      updateLeds(digit | playerBscoreNew | playerAscore);
      delay(500);
      updateLeds(digit | playerBscore | playerAscore);
      delay(500);
    }
    playerBscore = playerBscoreNew;
  }
}

void switchAPressed()
{
  if(AlastTimePressed + debounceTime < millis())
  {
    AlastTimePressed = millis();
    Aplayed=true;
    Serial.print("A_");
    if (digit == playerALed)
    { 
      Serial.println("Won");
      sessionWonA = true;
    }
    else 
    {
     Serial.println("Lost");
     sessionWonA = false;
    }
  }
}
// void switchBPressed()
// {
//   Serial.print("B");
//   if (digit == playerBLed)
//   {
        
//         AddScore(false);
//   }
// }

  



