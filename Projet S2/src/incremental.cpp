/*
 * incremental.cpp
 *
 * Serge Caron, feb 2026
 *
 * Decode cheap rotary encoder. Contrary to a quadrature encoder, some rotary encoders always send 2 pulses per detend (1 falling edge and 1 rising edge per channel)
 * before going back to an idle state.
 * In such case, all you have to do is interrupt on the falling edge of one channel. When it occurs, the state of the second channel tells you the direction of rotation.
 * Original code is from there:
 * https://arduino.stackexchange.com/questions/61785/cant-you-easily-use-quadrature-incremental-rotary-encoders-at-2x-or-4x-their
 *
 * ArduinoMega digital pin usable for interrupt: 2, 3, 18, 19, 20, 21
 */

#include <Arduino.h>
#include "incremental.h"


int rotaryPinA;      
int rotaryPinB;      
int rotaryValue = 0; 


void encode()
{
  
  bool pinBState = digitalRead(rotaryPinB);
  rotaryValue += (pinBState) ? -1 : 1;
}


int setup_encoder(int intPinA, int PinB)
{
  if (digitalPinToInterrupt(intPinA) == -1)
    return -1; 
  else
  {
    rotaryPinA = intPinA;
    rotaryPinB = PinB;
    pinMode(intPinA, INPUT);
    pinMode(PinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(intPinA), encode, FALLING);
    return 0; 
  }
}


int read_encoder()
{
  return rotaryValue;
}


void write_encoder(int value)
{
  noInterrupts();
  rotaryValue = value;
  interrupts();
}


int getEncoderdiff()
{
  int val = read_encoder();
  static int ancienval = 0;
  static uint16_t i = 1;

  if (abs(val - ancienval) >= 15)
  {
    ancienval = val;   // on met à jour seulement si seuil atteint
    i++;
  }

  if (i > 4)
    i = 0;

  return i;
}
