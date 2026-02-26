#ifndef LED_H
#define LED_H

#include <Arduino.h>

#define PIN_A 52 
#define PIN_B 50
#define PIN_C 48
#define PIN_D4 40
#define PIN_D5 42
#define PIN_Vert 36
#define PIN_Rouge 38
#define PIN_ENABLE 46

void UpdateLed(uint16_t choix);
void SetupLed(void);
uint16_t ShiftRight(uint16_t value);
uint16_t ShiftLeft(uint16_t value);
uint16_t CalculeBargraph(int valeurAnalogique);
void EteindreLeds();

#endif
