

#define pinBuzzer 7
int noteHz = 300;

// Existing:
#define noteA    440  // A5
#define noteAh   466  // A#5 / Bb5
#define noteB    494  // B5
#define noteC    523  // C5
#define noteCh   554  // C#5 / Db5
#define noteD    587  // D5
#define noteDh   622  // D#5 / Eb5
#define noteE    659  // E5
#define noteF    698  // F5
#define noteFh   740  // F#5 / Gb5
#define noteG    784  // G5
#define noteGh   831  // G#5 / Ab5
#define noteSilence   0  // silence
// #define noteA    880  // A5 (redefines noteA from 440)






int* currentNotes;
int* currentDurations;
int currentSoundLength;



int openingSoundNotes[] = {
    noteB,
    noteE,
    noteSilence,
    noteB,
    noteE,
    noteSilence,
    noteB,
    noteE,
    noteSilence,
    noteB,
    noteE,
    noteSilence,
    noteB,
    noteG
    
};
int openingSoundDurations[] = {
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    7000,  // 3
    2000,
    6600
};

int winningPointSoundNotes[] = {
    noteB,
    noteC,
    noteD,
    noteC,
    noteD,
    noteE,
    noteD,
    noteE,
    noteF,
    noteE,
    noteF,
    noteG,
    noteSilence,
    noteG,
    noteSilence,
    noteG

    
};
int winningPointSoundDurations[] = {
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    7000,  // 3
    2000,
    2000,
    2000,
    2000
};


int losingPointSoundNotes[] = {
    noteG, 
    noteF, 
    noteE,
    noteF,
    noteE,
    noteD,
    noteE,
    noteD,
    noteC,
    noteD,
    noteC,
    noteB,
    noteC,
    noteB,
    noteA,
    noteSilence,
    noteA,
    noteSilence,
    noteA

    
};
int losingPointSoundDurations[] = {
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    4000,  // 3
    2000,  // 1 -> 300 ms
    2000,  // 2
    7000,  // 3
    2000,
    2000,
    2000,
    2000
};




// int winningSoundDurations[] = {
//     4687,  // 1 -> 300 ms
//     4687,  // 2
//     12500,  // 3
//     4687,  // 4 -> a bit longer
//     4687,  // 5
//     12500,  // 6
//     4687,  // 7
//     4687,  // 8 -> finish with a longer note
//     12500,
//     2000,
//     2000,
//     2000
// };

int currentSoundIdx = 0;
int winningPointSoundLength = 16;
int losingPointSoundLength = 16 ;
int openingSoundLength = 14 ;
void setup() {

// tone(pinBuzzer, 440); // A4
//   delay(1000);

  //  playWinningPoint();
  //  delay(10000);
  //  playlosingPoint();
  
  for (int i = 0; i < openingSoundLength; i++) {
    openingSoundDurations[i] = openingSoundDurations[i] * 2;
  }

  playOpening();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void playOpening()
{
    playSound(openingSoundNotes, openingSoundDurations,openingSoundLength);
}

void playWinningPoint()
{
    playSound(winningPointSoundNotes, winningPointSoundDurations,winningPointSoundLength);
}

void playlosingPoint()
{
    playSound(losingPointSoundNotes, losingPointSoundDurations,losingPointSoundLength);
}


void playSound(int notesArr[], int durationsArr[], int arrLength)
{
  currentNotes = notesArr;
  currentDurations= durationsArr;
  currentSoundLength = arrLength;

  currentSoundIdx=0;
  TCCR1A = 0;           // Init Timer1
  TCCR1B = 0;           // Init Timer1
  TCCR1B |= B00000101;  // Prescalar = 64
  OCR1A = 25000;        // Timer CompareA Register
  TIMSK1 |= B00000010;  // Enable Timer COMPA Interrupt
  //restart clock
  //attach internal intterupt - depend on length
  //run clock
  
}

ISR(TIMER1_COMPA_vect)
{
  TCNT1 = 0; // Timer Preloading with 0
  if (currentSoundIdx == currentSoundLength)
  {
    
    noTone(pinBuzzer);
    TIMSK1 &= B11111101;
  }
  else
  {
    
    OCR1A = currentDurations[currentSoundIdx];
    if (currentNotes[currentSoundIdx]==0)
      noTone(pinBuzzer);
    else
      tone(pinBuzzer, currentNotes[currentSoundIdx]); 
    currentSoundIdx++;
  }
  
  
}
