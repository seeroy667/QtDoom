#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void SetupLCD(void);

void AfficheMouvAcc(int mouvement);

void AfficheRotationAcc(float x, float y, float z);

void AfficheBouton(int bouton1, int bouton2);

void AfficheEncodeur(int Encodeur);

void AfficheJoystick(int x, int y);

void AfficheMessage(const char *message);

void AffichePot(int pot);

#endif
