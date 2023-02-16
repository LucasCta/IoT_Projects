#include <EEPROM.h>

enum buttons {o1=8,o2=9,o3=10,shr=11};
#include <TM1637Display.h>
TM1637Display tm(3, 4);
int jx = A0;
int jy = A1; 
int jb= 2;
int buz = 5;

enum notes {C=0,D,E,F,G,A,B,C2};
const int tones[] = {261,294,330,349,392,440,494,523};
const int stones[] = {277,311,349,370,415,466,523,554};

int note = -1;
int noteBef = -1;
int octave = 1;
bool sharp = 0; 

long lDs = 0;
long lDj = 0; 
long lDo = 0;  
long debounceDelay = 5;
bool jbef = 1;
unsigned long antes = 0;
int epromPos = -4;
int nrec = 1;

const uint8_t dC[] = {SEG_A | SEG_D | SEG_E | SEG_F};
const uint8_t dD[] = {SEG_G | SEG_B | SEG_C | SEG_D | SEG_E};
const uint8_t dE[] = {SEG_A | SEG_D | SEG_E | SEG_F | SEG_G};
const uint8_t dF[] = {SEG_A | SEG_E | SEG_F | SEG_G};
const uint8_t dG[] = {SEG_A | SEG_D | SEG_E | SEG_F | SEG_C};
const uint8_t dA[] = {SEG_A | SEG_B | SEG_C | SEG_G | SEG_E | SEG_F};
const uint8_t dB[] = {SEG_C | SEG_D | SEG_G | SEG_E | SEG_F};

const uint8_t FREE[] = {SEG_A | SEG_E | SEG_F | SEG_G, 
                        SEG_G | SEG_C | SEG_E,
                        SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
                        SEG_A | SEG_D | SEG_E | SEG_F | SEG_G};
                        
const uint8_t SONG[] = {SEG_A | SEG_C | SEG_D | SEG_G | SEG_F,
                        SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
                        SEG_G | SEG_C | SEG_E,
                        SEG_A | SEG_D | SEG_E | SEG_F | SEG_C};

                        
const uint8_t TAPE[] = {SEG_G | SEG_D | SEG_E | SEG_F,
                        SEG_A | SEG_B | SEG_C | SEG_G | SEG_E | SEG_F,
                        SEG_A | SEG_B | SEG_G | SEG_F | SEG_E,
                        SEG_A | SEG_D | SEG_E | SEG_F | SEG_G};
                        
const uint8_t HEAR[] = {SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
                        SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
                        SEG_A | SEG_B | SEG_C | SEG_G | SEG_E | SEG_F,
                        SEG_G | SEG_C | SEG_E};

const int BEET[] = {
  E,2,10,0,
  E,2,10,0,
  F,2,10,0,
  G,2,10,0,
  G,2,10,0,
  F,2,10,0,
  E,2,10,0,
  D,2,10,0,
  C,2,10,0,
  C,2,10,0,
  D,2,10,0,
  E,2,10,0,
  E,2,15,0,
  D,2,5,0,
  D,2,20,0,
  E,2,10,0,
  E,2,10,0,
  F,2,10,0,
  G,2,10,0,
  G,2,10,0,
  F,2,10,0,
  E,2,10,0,
  D,2,10,0,
  C,2,10,0,
  C,2,10,0,
  D,2,10,0,
  E,2,10,0,
  D,2,15,0,
  C,2,5,0,
  C,2,20,0
};

const int NIVER[] = {
  C,2,5,0,
  C,2,5,0,
  D,2,10,0,
  C,2,10,0,
  F,2,10,0,
  E,2,20,0,
  C,2,5,0,
  C,2,5,0,
  D,2,10,0,
  C,2,10,0,
  G,2,10,0,
  F,2,10,0,
  F,2,10,0,
  A,2,5,0,
  A,2,5,0,
  C,3,10,0,
  A,2,10,0,
  F,2,10,0,
  E,2,10,0,
  D,2,10,0,
  A,2,5,1,
  A,2,5,1,
  A,2,10,0,
  F,2,10,0,
  G,2,10,0,
  F,2,10,0,
  F,2,20,0
};

const int BACH[] = {
  G,2,40,0,
  G,2,40,0,
  G,2,10,0,
  E,3,5,0,
  C,3,5,0,
  A,2,5,0,
  G,2,5,0,
  F,2,5,1,
  G,2,5,0,
  F,2,20,1,
  E,2,5,0,
  D,2,15,0,
  D,3,40,0,
  D,3,5,0,
  B,2,5,0,
  F,2,5,0,
  E,2,5,0,
  A,2,5,0,
  G,2,5,1,
  D,3,5,0,
  C,3,5,0,
  C,3,40,0,
  C,3,5,0,
  A,2,5,0,
  F,2,5,1,
  E,2,5,0,
  G,2,5,0,
  F,2,5,1,
  C,3,5,0,
  B,2,5,0,
  B,2,30,0,
  C,3,5,1,
  D,3,5,0,
  G,2,10,0,
  G,2,5,0,
  A,2,2,0,
  B,2,2,0,
  B,2,5,0,
  A,2,5,0,
  A,2,5,0,
  G,2,5,0,
  F,2,5,1,
  E,2,5,0,
  F,2,2,1,
  E,2,2,0,
  G,2,5,0,
  G,2,10,0,
  F,2,5,1,
  E,2,5,0,
  D,2,40,0
};

enum modes {menu,freeplay,song,recrd,hear};
int tmode = freeplay;
int mode = menu;

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
  switch (mode) {
    case menu: 
      menuRen();
      break;
    case freeplay:
      freePlay();
      break;
    case song:
      switch (readOctave()){
        case 0:
          int rec[200];
          for (int i = 0; i < 200; i++)
            rec[i] = EEPROM.read(i);       
          songPlay(rec,200);
          break;
        case 1:
          songPlay(BEET,120);
          break;
        case 2:
          songPlay(NIVER,27*4);
          break;
        case 3:
          songPlay(BACH,200);
          break;
      } break;  
    case recrd:
      record();
      break;
    case hear:
      switch (readOctave()){
        case 0:
          int rec[200];
          for (int i = 0; i < 200; i++)
            rec[i] = EEPROM.read(i);       
          playSong(rec,200);
          break;
        case 1:
          playSong(BEET,120);
          break;
        case 2:
          playSong(NIVER,27*4);
          break;
        case 3:
          playSong(BACH,200);
          break;
      } break;  
      break;
  }
}

void menuRen() {
  int j;
  int input = readNote();
  if (input == C) tmode = freeplay;
  if (input == E) tmode = song;
  if (input == G) tmode = recrd; 
  if (input == B) tmode = hear;
  switch (tmode) {
    case freeplay:
      tm.setSegments(FREE,4,0);
      j = digitalRead(jb);
      if (millis()-lDj > debounceDelay) {
        if (j != jbef && j == LOW) {
          jbef = j;
          mode = freeplay;
          lDj = millis();
        }
        if (j != jbef && j == HIGH) {
          jbef = j;
          lDj = millis();
        }
      } break;
    case song:
      tm.setSegments(SONG,4,0);
      j = digitalRead(jb);
      if (millis()-lDj > debounceDelay) {
        if (j != jbef && j == LOW) {
          jbef = j;
          mode = song;
          lDj = millis();
        }
        if (j != jbef && j == HIGH) {
          jbef = j;
          lDj = millis();
        }
      } break;
    case recrd:
      tm.setSegments(TAPE,4,0);
      j = digitalRead(jb);
      if (millis()-lDj > debounceDelay) {
        if (j != jbef && j == LOW) {
          jbef = j;
          mode = recrd;
          lDj = millis();
        }
        if (j != jbef && j == HIGH) {
          jbef = j;
          lDj = millis();
        }
      } break; 
    case hear:
      tm.setSegments(HEAR,4,0);
      j = digitalRead(jb);
      if (millis()-lDj > debounceDelay) {
        if (j != jbef && j == LOW) {
          jbef = j;
          mode = hear;
          lDj = millis();
        }
        if (j != jbef && j == HIGH) {
          jbef = j;
          lDj = millis();
        }
      } break;  
  }
}

void songPlay (const int sng[], int sz) {
  tm.clear();
  for (int i = 0; i < sz; i += 4) {
    if (sng[i] == 0 && sng[i+1] == 0 && sng[i+3] == 0 && sng[i+2] == 0) break;
    displayNote(sng[i], sng[i+1], sng[i+3]);
    int t = sng[i+2];
    antes = millis();
    while (t > 0) {
      if (millis()-antes > 50) {
        if (sng[i+3]) tm.showNumberDecEx(t,0b11100000,false,2,0);
        else tm.showNumberDec(t,false,2,0);
        int o = readOctave();
        if (o > 0) octave = o;
        sharp = readSharp();
        note = readNote(); 
        if ((sng[i] == note && sng[i+1] == octave && sng[i+3] == sharp) || 
        (sng[i] == C && note == C2 && sng[i+1] == octave+1 && sng[i+3] == sharp) ||
        (sng[i] == C2 && note == C && sng[i+1] == octave-1 && sng[i+3] == sharp)) {
          if (sharp) tone(buz,stones[note]*pow(2,octave-1), 100);
          else tone(buz,tones[note]*pow(2,octave-1), 100);
          antes = millis();
          t -= 1;
        } 
      }
      int j = digitalRead(jb);
      if (millis()-lDj > debounceDelay){
        if (j != jbef && j == LOW) {
          jbef = j;
          i = sz; t = 0;
          mode = menu;
          lDj = millis();
        }
        if (j != jbef && j == HIGH) {
          jbef = j;
          lDj = millis();
        }
      }
    } tm.clear();
  } mode = menu;
}

void playSong (const int sng[], int sz) {
  tm.clear();
  for (int i = 0; i < sz; i += 4) {
    if (sng[i] == 0 && sng[i+1] == 0 && sng[i+3] == 0 && sng[i+2] == 0) break;
    displayNote(sng[i], sng[i+1], sng[i+3]);
    int t = sng[i+2];
    antes = millis();
    while (t > 0) {
      if (millis()-antes > 50) {
        if (sng[i+3]) tm.showNumberDecEx(t,0b11100000,false,2,0);
        else tm.showNumberDec(t,false,2,0);
        if (sng[i+3]) tone(buz,stones[sng[i]]*pow(2,sng[i+1]-1), 100);
        else tone(buz,tones[sng[i]]*pow(2,sng[i+1]-1), 100);
        antes = millis();
        t -= 1;
      }
      int j = digitalRead(jb);
      if (millis()-lDj > debounceDelay){
        if (j != jbef && j == LOW) {
          jbef = j;
          i = sz; t = 0;
          mode = menu;
          lDj = millis();
        }
        if (j != jbef && j == HIGH) {
          jbef = j;
          lDj = millis();
        }
      }
    } tm.clear();
  } mode = menu;
}

void freePlay() {
  note = readNote();  
  if (note == -1) {
    noTone(buz);
    noteBef = -1;
    tm.clear();
  } else {
    int o = readOctave();
    if ((millis()-lDo) > debounceDelay) {
      if (octave != o && o > LOW && note != -1) {
        octave = o;
        displayNote(note, octave, sharp);
        lDo = millis();
      }
    }
    int s = readSharp();
    if ((millis()-lDs) > debounceDelay) {
      if (sharp != s && note != -1) {
        sharp = s;
        displayNote(note, octave, sharp);
        lDs = millis();
      }
    }
    if (s) tone(buz,stones[note]*pow(2,octave-1),100);
    else tone(buz,tones[note]*pow(2,octave-1),100);
    if (noteBef != note) displayNote(note, octave, sharp);
    noteBef = note;
  } 
  int j = digitalRead(jb);
  if (millis()-lDj > debounceDelay){
    if (j != jbef && j == LOW) {
      jbef = j;
      mode = menu;
      lDj = millis();
    }
    if (j != jbef && j == HIGH) {
      jbef = j;
      lDj = millis();
    }
  }
}

void record() {
  if (nrec) {
    for (int i = 0 ; i < 200 ; i++)
      EEPROM.write(i, 0);
    epromPos = -4;
    nrec = 0;
  }
  note = readNote();  
  if (note == -1) {
    noTone(buz);
    noteBef = -1;
    tm.clear();
  } else {
    int o = readOctave();
    if ((millis()-lDo) > debounceDelay) {
      if (octave != o && o > 0 && note != -1) {
        octave = o;
        displayNote(note, octave, sharp);
        epromPos += 4;
        EEPROM.write(epromPos, note);
        EEPROM.write(epromPos+1, octave);
        EEPROM.write(epromPos+3, sharp);
        EEPROM.write(epromPos+2, 1);
        antes = millis();
        lDo = millis();
      }
    }
    int s = readSharp();
    if ((millis()-lDs) > debounceDelay) {
      if (sharp != s && note != -1) {
        sharp = s;
        displayNote(note, octave, sharp);
        epromPos += 4;
        EEPROM.write(epromPos, note);
        EEPROM.write(epromPos+1, octave);
        EEPROM.write(epromPos+3, sharp);
        EEPROM.write(epromPos+2, 1); 
        antes = millis();
        lDs = millis();
      }
    }
    if (s) tone(buz,stones[note]*octave,100);
    else tone(buz,tones[note]*octave,100);
    if (noteBef != note) {
      displayNote(note, octave, sharp);
      epromPos += 4;
      EEPROM.write(epromPos, note);
      EEPROM.write(epromPos+1, octave);
      EEPROM.write(epromPos+3, sharp);
      EEPROM.write(epromPos+2, 1);
      antes = millis();
    } else {
      if (EEPROM.read(epromPos+2) < 40) {
        if (millis()-antes > 50) {
          EEPROM.write(epromPos+2, EEPROM.read(epromPos+2)+1);
          antes = millis();
        }
      } else {
        epromPos += 4;
        EEPROM.write(epromPos, note);
        EEPROM.write(epromPos+1, octave);
        EEPROM.write(epromPos+3, sharp);
        EEPROM.write(epromPos+2, 1);
      }
    }
    noteBef = note;
  } 
  int j = digitalRead(jb);
  if (millis()-lDj > debounceDelay){
    if (j != jbef && j == LOW) {
      jbef = j;
      mode = menu;
      nrec = 1;
      lDj = millis();
    }
    if (j != jbef && j == HIGH) {
      jbef = j;
      lDj = millis();
    }
  }
  if (epromPos == 200){
    nrec = 1;
    mode = menu;
  }
}

int readOctave(){
  if (digitalRead(o1) == LOW) 
    return 1;
  else if (digitalRead(o2) == LOW)
    return 2;
  else if (digitalRead(o3) == LOW)
    return 3;
  else return 0;
}

int readSharp(){
  if (digitalRead(shr) == LOW)
    return 1;
  else
    return 0;
}

int readNote() {
  int x = analogRead(jx);
  int y = analogRead(jy);
  if (x > 900) x = 2;
  else if (x < 30) x = 0;
  else x = 1;
  if (y > 900) y = 2;
  else if (y < 30) y = 0;
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
      return -1;
  }
}   

void displayNote(int n, int oit, bool sus) {
  if (n == C2) oit += 1;
  if (sharp) tm.showNumberDecEx(oit,0b11100000,false,4,0);
  else tm.showNumberDec(oit,false,4,0);
  switch(n){
    case A:
      tm.setSegments(dA,1,2);
      break;
    case B:
      tm.setSegments(dB,1,2);
      break;
    case C:
      tm.setSegments(dC,1,2);
      break;
    case D:
      tm.setSegments(dD,1,2);
      break;
    case E:
      tm.setSegments(dE,1,2);
      break;
    case F:
      tm.setSegments(dF,1,2);
      break;
    case G:
      tm.setSegments(dG,1,2);
      break;
    case C2:
      tm.setSegments(dC,1,2);
      break;
  } return;
}
