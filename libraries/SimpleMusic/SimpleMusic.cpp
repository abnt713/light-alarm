#include "Arduino.h"
#include "SimpleMusic.h"

SimpleMusic::SimpleMusic(int pin){
  _pin = pin;
  _shouldSerial = 0;
  _octaveoffset = 0;
}

int SimpleMusic::isvaliddigit(char n){
  return n >= '0' && n <= '9';
}

void SimpleMusic::setDebug(int shouldDebug){
  _shouldSerial = shouldDebug;
}

void SimpleMusic::play_rtttl(char *p)
{
  int notes[] = { 0,
    262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
    523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
    1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
    2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
  };
  
  // Absolutely no error checking in here
  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isvaliddigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  if(_shouldSerial){
    Serial.print("ddur: "); Serial.println(default_dur, 10);
  }

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  if(_shouldSerial){
    Serial.print("doct: "); Serial.println(default_oct, 10);
  }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isvaliddigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }
  
  if (_shouldSerial == 1){
    Serial.print("bpm: "); 
    Serial.println(bpm, 10);
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  if (_shouldSerial){
    Serial.print("wn: "); Serial.println(wholenote, 10);
  }


  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isvaliddigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isvaliddigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += _octaveoffset;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      if(_shouldSerial){
        Serial.print("Playing: ");
        Serial.print(scale, 10); Serial.print(' ');
        Serial.print(note, 10); Serial.print(" (");
        Serial.print(notes[(scale - 4) * 12 + note], 10);
        Serial.print(") ");
        Serial.println(duration, 10);
      }
      tone(_pin, notes[(scale - 4) * 12 + note]);
      delay(duration);
      noTone(_pin);
    }
    else
    {
      if(_shouldSerial){
        Serial.print("Pausing: ");
        Serial.println(duration, 10);
      }
      delay(duration);
    }
  }
}
