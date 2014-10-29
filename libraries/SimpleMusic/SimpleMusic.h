/*
  SimpleMusic.h - Library for playing musics with arduino.
  Created by Brett Hagman (bhagman@roguerobotics.com) www.roguerobotics.com
  edited by Alison Bento (alisonbnt@gmail.com)
*/

#ifndef SimpleMusic_h
#define SimpleMusic_h
#endif

#include "Arduino.h"

class SimpleMusic{
  public:
    SimpleMusic(int pin);
    void play_rtttl(char *p);
    void setDebug(int shouldDebug);
  private:
    //int *notes;
    int _pin;
    int _shouldSerial;
    int _octaveoffset;
    int isvaliddigit(char n);
};
  
