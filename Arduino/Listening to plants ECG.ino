#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

int L0Plus = 10;
int LOMinus = 11;
int speaker = 9;
int serialCom = 9600;

int ecgValue = 0;
int lastTone = 0;
int filterValue = 20; // Filter level for the "noise" (flatten)

int notes[] = {
  31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123, 131,
  139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
  523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 998, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661,
  1760, 1865, 1976  
};

void setup() {
  // initialize the serial communication:
  Serial.begin(serialCom);
  pinMode(L0Plus, INPUT); // Setup for leads off detection LO +
  pinMode(LOMinus, INPUT); // Setup for leads off detection LO -
  pinMode(speaker, OUTPUT); // Setup for speaker output
}

void loop() {
  
  if((digitalRead(10) != 1)||(digitalRead(11) != 1)) {
    int toneToPlay = chooseNote(cleanUpECGValue(analogRead(A0)));
    
    Serial.println(toneToPlay);
    tone(speaker, toneToPlay);
  } else {

    Serial.println('!');
  }
  
  // Wait for a bit to keep serial data from saturating
  delay(50);
}

int cleanUpECGValue(int ecgValue) {
  int toneToPlay = chooseNote(lastTone);
  
  if((lastTone-ecgValue) > filterValue || (lastTone-ecgValue) < -filterValue) {
    lastTone = ecgValue;
    toneToPlay = chooseNote(lastTone);
  }

  return toneToPlay;
}

int chooseNote(int lastTone) {
  
  int i;
  int bestNote;
  int bestScore = 10000;
  int currentScore;
  
  for (i = 0; i < arr_len(notes); i = i + 1) {
    currentScore = abs(notes[i] - lastTone);
    if(currentScore < bestScore) {
      bestScore = currentScore;
      bestNote = notes[i];
    }
  }

  return bestNote;
}
