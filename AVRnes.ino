/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITSNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.  

This code also contains most of the SNESpad library, which is licensed
under GPLv3 as well.

The original nespad/snespad library code is here:
http://code.google.com/p/nespad/

*/
#ifndef SNESpad_h
#define SNESpad_h

#include <inttypes.h>

#define SNES_B       0x01
#define SNES_Y       0x02
#define SNES_SELECT  0x04
#define SNES_START   0x08
#define SNES_UP      0x10
#define SNES_DOWN    0x20
#define SNES_LEFT    0x40
#define SNES_RIGHT   0x80
#define SNES_A       0x100
#define SNES_X       0x200
#define SNES_L       0x400
#define SNES_R       0x800


class SNESpad {

  public:
    SNESpad();
    SNESpad(int strobe, int clock, int data);
    int buttons();

  private:
    void strobe();
    int shiftin();
    int m_strobe, m_clock, m_data;

};

#endif
//snespad.cpp

SNESpad::SNESpad(int strobe, int clock, int data)
  : m_strobe (strobe),
  m_clock (clock),
  m_data (data)
  {
    pinMode(strobe, OUTPUT);
    pinMode(clock,  OUTPUT);
    pinMode(data, INPUT);
  }

int SNESpad::buttons(void)
{
  int ret = 0;
  byte i;
  strobe();
  for (i = 0; i < 16; i++) {
    ret |= shiftin() << i;
  }
  return ~ret;
}

void SNESpad::strobe(void)
{
  digitalWrite(m_strobe,HIGH);
  delayMicroseconds(12);
  digitalWrite(m_strobe,LOW);
}

int SNESpad::shiftin(void)
{
  int ret = digitalRead(m_data);
  delayMicroseconds(12);
  digitalWrite(m_clock,HIGH);
  delayMicroseconds(12);
  digitalWrite(m_clock,LOW);
  return ret;
}

//end of snespad.cpp


SNESpad nintendo = SNESpad(2,3,4);

int state = 0;
int isChangeMode=0; //Used to wait until user stops holding mode change buttons before continuing.
int currentMode=0; //Mode 0 is normal mode, mode 1 does mouse movement in minecraft mode.
int selectPressed=0; //Used to prevent select button repeating on every loop through
int countdown=120;
void setup() {
  pinMode(13, OUTPUT);
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  state = nintendo.buttons();
  if(isChangeMode==1){
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
  if ((state & SNES_R) && (state & SNES_L) && (state & SNES_START)) { //If the change-mode key combo is pressed
    if(isChangeMode==0){
      if(currentMode==0) {
      Keyboard.releaseAll();
      currentMode=1;
      isChangeMode=1;
      }
      else {
        Mouse.release(MOUSE_LEFT);
        Mouse.release(MOUSE_RIGHT);
        Keyboard.releaseAll();
        currentMode=0;
        isChangeMode=1;
      }
    }
    isChangeMode=1;
  }
  else {
    isChangeMode=0;
  }
  if (state & SNES_B) {
    if(currentMode==0)
    {
      Keyboard.press('z');
    }
    else {
      Mouse.move(0, 1, 0);
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release('z');
    }
  }
  if (state & SNES_Y) {
    if(currentMode==0)
    {
      Keyboard.press('s');
    }
    else {
      Mouse.move(-1, 0, 0);
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release('s');
    }
  }
  if (state & SNES_SELECT) {
    if(currentMode==0)
    {
      Keyboard.press(KEY_BACKSPACE);
    }
    else {
      //if(selectPressed==0){
        if(countdown==0){
          Mouse.move(0, 0, -1);
          countdown=120;
          selectPressed=1;
        }
        else{
          countdown=countdown - 1;
        }
      //}
    }
  }
  else { //if select is not pressed
    if(currentMode==0)
    {
      Keyboard.release(KEY_BACKSPACE);
    }
    selectPressed=0; //allow select to be pressed again in mouse mode
    countdown=5;
  }
  if (state & SNES_START) {
    if(currentMode==0)
    {
      Keyboard.press(KEY_RETURN);
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release(KEY_RETURN);
    }
  }
  if (state & SNES_UP) {
    if(currentMode==0)
    {
    Keyboard.press(KEY_UP_ARROW);
    }
    else if (currentMode==1) {
    Keyboard.press('w'); //for minecraft
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release(KEY_UP_ARROW);
    }
    else if (currentMode==1) {
      Keyboard.release('w');
    }
  }
  if (state & SNES_DOWN) {
    if(currentMode==0)
    {
      Keyboard.press(KEY_DOWN_ARROW);
    }
    else if(currentMode==1) {
      Keyboard.press('s');
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release(KEY_DOWN_ARROW);
    }
  }
  if (state & SNES_LEFT) {
    if(currentMode==0)
    {
      Keyboard.press(KEY_LEFT_ARROW);
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release(KEY_LEFT_ARROW);
    }
  }
  if (state & SNES_RIGHT) {
    if(currentMode==0)
    {
      Keyboard.press(KEY_RIGHT_ARROW);
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release(KEY_RIGHT_ARROW);
    }
  }
  if (state & SNES_A) {
    if(currentMode==0)
    {
      Keyboard.press('x');
    }
    else {
      Mouse.move(1, 0, 0);
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release('x');
    }
  }
  if (state & SNES_X) {
    if(currentMode==0)
    {
      Keyboard.press('a');
    }
    else {
      Mouse.move(0, -1, 0);
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release('a');
    }
  }
  if (state & SNES_L) {
    if(currentMode==0)
    {
      Keyboard.press('d');
    }
    else {
      Mouse.press(MOUSE_RIGHT); 
    }
  }
  else {
    if(currentMode==0)
    {
      Keyboard.release('d');
    }
    else {
      Mouse.release(MOUSE_RIGHT); 
    }
  }
  if (state & SNES_R) {
    if(currentMode==0)
    {
      Keyboard.press('c');
    }
    else {
      Mouse.press(MOUSE_LEFT);
    }
  }
  else {
    if(currentMode==0)
    {
    Keyboard.release('c');
    }
    else {
    Mouse.release(MOUSE_LEFT);
    }
  }
  delay(2);
/*  if(currentMode==1)
  {
    Keyboard.releaseAll();
    Mouse.release(MOUSE_LEFT);
    Mouse.release(MOUSE_RIGHT);
  }
*/
  
    
  /*
    test if button pressed
    if (state & SNES_A)

    test if button not pressed
    if !(state & SNES_A)

    Check multiple buttons pressed
    if ( (state & SNES_UP) && (state & SNES_LEFT) )

  */

}
