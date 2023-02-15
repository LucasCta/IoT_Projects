#include <TM1637Display.h>
TM1637Display tm(3, 4);

int jx = A0;
int jy = A1; 
int jb= 2;

int buz = 5;

enum buttons {o1=8,o2=9,o3=10,shr=11};
int octave = 1;
bool sharp = 0; 

enum notes {C=261,D=294,E=330,F=349,G=392,A=440,B=494,C2=523};

const uint8_t dC[] = {SEG_A | SEG_D | SEG_E | SEG_F};
const uint8_t dD[] = {SEG_G | SEG_B | SEG_C | SEG_D | SEG_E};
const uint8_t dE[] = {SEG_A | SEG_D | SEG_E | SEG_F | SEG_G};
const uint8_t dF[] = {SEG_A | SEG_E | SEG_F | SEG_G};
const uint8_t dG[] = {SEG_A | SEG_D | SEG_E | SEG_F | SEG_C};
const uint8_t dA[] = {SEG_A | SEG_B | SEG_C | SEG_G | SEG_E | SEG_F};
const uint8_t dB[] = {SEG_C | SEG_D | SEG_G | SEG_E | SEG_F};

void setup() {
  Serial.begin(115200);
  pinMode(jb, INPUT_PULLUP);
  pinMode(o1, INPUT_PULLUP);
  pinMode(o2, INPUT_PULLUP);
  pinMode(o3, INPUT_PULLUP);
  pinMode(shr, INPUT_PULLUP);
  tm.setBrightness(3); 
  tm.clear();
}

void loop() {
  
  int r = readJoystick();
  
  if (digitalRead(o1) == LOW) {
    octave = o1 - 7;
    Serial.println("Octave 1");
  } else if (digitalRead(o2) == LOW) {
    octave = o2 - 7;
    Serial.println("Octave 2");
  } else if (digitalRead(o3) == LOW) {
    octave = o3 - 7;
    Serial.println("Octave 3");
  }
    
    
  
  if (r == 0) {
    noTone(buz);
    tm.clear();
  } else {
    tone(buz,r*octave,10);
    displayNote(r, octave, sharp);
  }
  
}

int readJoystick() {
  int x = analogRead(jx);
  int y = analogRead(jy);
  if (x > 1000) x = 2;
  else if (x < 10) x = 0;
  else x = 1;
  if (y > 1000) y = 2;
  else if (y < 10) y = 0;
  else y = 1;
  switch (x*10 + y){
    case 12:
      return C;
    case 2:
      return D;
    case 1:
      return E;
    case 0:
      return F;
    case 10:
      return G; 
    case 20:
      return A;
    case 21:
      return B;
    case 22:
      return C2;
    case 11:
      return 0;
  }
}

void displayNote(int n, int oit, bool sus) {
  switch(n){
    case A:
      tm.setSegments(dA,1,1);
      break;
    case B:
      tm.setSegments(dB,1,1);
      break;
    case C:
      tm.setSegments(dC,1,1);
      break;
    case D:
      tm.setSegments(dD,1,1);
      break;
    case E:
      tm.setSegments(dE,1,1);
      break;
    case F:
      tm.setSegments(dF,1,1);
      break;
    case G:
      tm.setSegments(dG,1,1);
      break;
    case C2:
      tm.setSegments(dC,1,1);
      oit += 1;
      break;
  } 
  if (sus) tm.showNumberDecEx(oit,0B11100000,false,1,2);
  else tm.showNumberDec(oit,false,1,2);
  return;
}
