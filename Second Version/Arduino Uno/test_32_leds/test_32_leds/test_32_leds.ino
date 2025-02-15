//
// - start up (V)
// -- leds shows and start (V)
//
// - game (succeed (V), , keeping score (V), )
// - failed(X):
// - - random place, random direction, 1 second penalty 
//
// - end of game (X):
// - - lights towards winner (V)
//
//
// - session(?) 
//buzzer implemenation
//check for bugs (click any time - does it work)
//change to interupt (V)
//
//check pcb or make new one + order parts
//3d print the model
//
//bgus:
//when leave button also count as press (maybe not fixable)

#define pinSer 5
#define pinLatch 6
#define pinClk 4

#define pinSwitchA 2
#define pinSwitchB 3

#define playerALed 0x80000000 //green leds
#define playerBLed 0x00040000 //blue leds

uint32_t digit = 256; //turn on one led.    //32 bits from left to right the bits of the game, the right most 6 bits are the score
bool digitDirection = true; //True - clockwise, Flase - counter clockwise
uint32_t digitStartShow = 0xAAAAAAAA;
uint32_t digitwinningShowUp = 0x00001000;
uint32_t digitwinningShowDown = 0x01000000;

uint32_t playerAscore = 0x00000000;
uint32_t playerBscore = 0x00000000;
uint32_t gameSpeed = 100; //milis between each led 
uint32_t startupShowSpeed = 300;
uint32_t winningShowSpeed = 100;
long AlastTimePressed = millis();
long BlastTimePressed = millis();
uint32_t debounceTime = 10; // in miliseconds  
// bool Bpressable = true;

bool Aplayed = false;
bool Bplayed = false;

bool sessionWonA = false;
bool sessionWonB = false;



void setup() {
  pinMode(pinClk, OUTPUT);
  pinMode(pinSer, OUTPUT);
  pinMode(pinLatch, OUTPUT);

  pinMode(pinSwitchA, INPUT_PULLUP);
  pinMode(pinSwitchB, INPUT_PULLUP);

  
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pinSwitchA), switchAPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinSwitchB), switchBPressed, FALLING);
   
  restartGame();
  // winningShow(playerALed);
  
}

void loop() {
 
  
  
  //~~~led running~~~
  if (digitDirection)
    digit = ((digit >> 1) | (digit << 25)) & 0xFFFFFFC0; // 32 - 6- 1 = 31 (from Q0 to Q8)
  else
   digit = ((digit << 1) | (digit >> 25)) & 0xFFFFFFC0; // 32 - 6 - 1 = 31 (from Q8 to Q0)
  // Serial.println(digit);

  updateLeds(digit | playerAscore | playerBscore);
  // Serial.print(digit);
  // Serial.print(" ");
  // Serial.println(playerAscore);

  delay(gameSpeed);

  if(Aplayed)
  {
    if (sessionWonA){
      AddScore(true);
      changeLedDirection();
      randomizeLedPosition();
    }
    else
      lowerScore(true);
    
    Aplayed=false;
  }

  if(Bplayed)
  {
    if (sessionWonB){
      AddScore(false);
      changeLedDirection();
      randomizeLedPosition();
    }
    else
      lowerScore(false);
    
    Bplayed=false;
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
      updateLeds(digit | playerAscoreNew | playerBscore);
      delay(500);
      updateLeds(digit | playerAscore | playerBscore);
      delay(500);
    }
    playerAscore = playerAscoreNew;
    if(playerAscore==0x00000007)
    {
      winningShow(playerALed);
      restartGame(); 
    }
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
    if(playerBscore==0x00000038)
    {
      winningShow(playerBLed);
      restartGame(); 
    }
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
    uint32_t playerBscoreNew = (playerBscore << 1) & 0x00000038; 
    for (int i = 0; i < 3; i++) {
      if (i==2)
        updateLeds(digit | playerAscore | playerBscoreNew);
      else
        updateLeds(digit | playerAscore | playerBscore);
      delay(250);

      updateLeds(playerAscore | playerBscoreNew);
      delay(250);

      if (i==2)
        updateLeds(digit | playerAscore | playerBscoreNew);
      else
        updateLeds(digit | playerAscore | playerBscore);
      delay(250);

      updateLeds(playerAscore | playerBscoreNew);
      delay(250);
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
      Serial.println("A Won");
      sessionWonA = true;
    }
    else 
    {
     Serial.println("A Lost");
     sessionWonA = false;
    }
  }
}

void switchBPressed()
{
  if(BlastTimePressed + debounceTime < millis())
  {
    BlastTimePressed = millis();
    Bplayed=true;
    Serial.print("B_");
    if (digit == playerBLed)
    { 
      Serial.println("B Won");
      sessionWonB = true;
    }
    else 
    {
     Serial.println("B Lost");
     sessionWonB = false;
    }
  }
}

void startupShow()
{
  updateLeds(0x00000000);
  delay(500);

   for (int i=0; i<6; i++) {

      updateLeds(0x00000000); //clean because otherwise some leds magicly turn on
      updateLeds(digitStartShow);
      delay(startupShowSpeed);

      updateLeds(0x00000000); //clean because otherwise some leds magicly turn on
      updateLeds(~digitStartShow);
      delay(startupShowSpeed);
   }
}  

void winningShow(uint32_t WinningPlayerLed)
{
  updateLeds(0x00000000);
  delay(500);


  updateLeds(0x00000000);
  for (int j=0; j<10; j++) {

    if (WinningPlayerLed==playerBLed)
    {
     digitwinningShowUp = 0x00001000;
     digitwinningShowDown = 0x01000000;
    }
    else {
      digitwinningShowUp = 0x00000800;
      digitwinningShowDown = 0x02000000;
    }

      for (int i=0; i<6; i++) {
        updateLeds(0x00000000);//clean because otherwise some leds magicly turn on
        updateLeds((digitwinningShowUp | digitwinningShowDown) & ~WinningPlayerLed);
        delay(winningShowSpeed);
        if(WinningPlayerLed==playerBLed)
        {
        digitwinningShowUp = digitwinningShowUp | (digitwinningShowUp<<1);
        digitwinningShowDown = digitwinningShowDown | (digitwinningShowDown>>1);
        }
        else {
        digitwinningShowUp = digitwinningShowUp | (digitwinningShowUp>>1);
        digitwinningShowDown = digitwinningShowDown | (digitwinningShowDown<<1);
        }
      }
      // updateLeds(0x00000000);//clean because otherwise some leds magicly turn on
      updateLeds(WinningPlayerLed);
      delay(winningShowSpeed);
      winningShowSpeed-=8;
  }
  for (int i=0; i<8; i++) {
    delay(100);
    updateLeds(WinningPlayerLed);
    delay(100);
    updateLeds(0x00000000);
  }
}

void restartGame()
{
  randomizeLedPosition(); 
  playerAscore = 0x00000000;
  playerBscore = 0x00000000;
  winningShowSpeed = 100;
  startupShow();
}
void changeLedDirection() {
  digitDirection = !digitDirection;
}

void randomizeLedPosition() {
  digit = 0x80000000 >> random(0,26);
}


