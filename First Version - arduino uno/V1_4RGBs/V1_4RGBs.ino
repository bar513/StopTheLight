int redPin1 = 2;
int greenPin1 = 3;
int bluePin1 = 4;

int redPin2 = 5;
int greenPin2 = 6;
int bluePin2 = 7;

int redPin3 = 8;
int greenPin3 = 9;
int bluePin3 = 10;

int redPin4 = 11;
int greenPin4 = 12;
int bluePin4 = 13;

int switchPin = A4; //connected to ground, when pressed its connected to 5V 

int ledNum = 1;
int chosenLedNum = 3;
int speed = 100; //milliseconds to be on for each led 

void setup() {
pinMode(redPin1,  OUTPUT);              
pinMode(greenPin1, OUTPUT);
pinMode(bluePin1, OUTPUT);

pinMode(redPin2,  OUTPUT);              
pinMode(greenPin2, OUTPUT);
pinMode(bluePin2, OUTPUT);

pinMode(redPin3,  OUTPUT);              
pinMode(greenPin3, OUTPUT);
pinMode(bluePin3, OUTPUT);

pinMode(redPin4,  OUTPUT);              
pinMode(greenPin4, OUTPUT);
pinMode(bluePin4, OUTPUT);

pinMode(switchPin, INPUT);

setColorBlue(chosenLedNum);

}

void loop() {

delay(speed);

if (digitalRead(switchPin)==LOW) 
{
  if (ledNum!=chosenLedNum)
    setColorBlack(ledNum);
  else
    setColorBlue(ledNum);

  ledNum++;
  if (ledNum > 4)
    ledNum = 1;

  setColorRed(ledNum);
}
}



void setColor(bool redOn, bool greenOn,  bool blueOn, int ledNum) {
  switch (ledNum)
  { 
    case 1:
    digitalWrite(redPin1, redOn);
    digitalWrite(greenPin1,  greenOn);
    digitalWrite(bluePin1, blueOn);
    break;
    case 2:
    digitalWrite(redPin2, redOn);
    digitalWrite(greenPin2,  greenOn);
    digitalWrite(bluePin2, blueOn);
    break;
    case 3:
    digitalWrite(redPin3, redOn);
    digitalWrite(greenPin3,  greenOn);
    digitalWrite(bluePin3, blueOn);
    break;
    case 4:
    digitalWrite(redPin4, redOn);
    digitalWrite(greenPin4,  greenOn);
    digitalWrite(bluePin4, blueOn);
    break;
  }
}

void setColorWhite(int ledNum){
setColor(HIGH, HIGH, HIGH,ledNum);
}
void setColorRed(int ledNum){
setColor(HIGH, LOW, LOW,ledNum);
}
void setColorGreen(int ledNum){
setColor(LOW, HIGH, LOW,ledNum);
}
void setColorBlue(int ledNum){
setColor(LOW, LOW, HIGH,ledNum);
}
void setColorBlack(int ledNum){
setColor(LOW, LOW, LOW,ledNum);
}
