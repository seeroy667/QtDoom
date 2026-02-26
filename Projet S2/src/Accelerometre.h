#ifndef ACCELEROMETRE_H
#define ACCELEROMETRE_H

#include <Arduino.h>

#define PIN_X A2
#define PIN_Y A3
#define PIN_Z A4

#define HORAIRE 1
#define ANTI_HORAIRE -1
#define PAS_DE_ROTATION 0

#define GAUCHE 1
#define DROITE -1
#define AVANT 2
#define ARRIERE -2
#define HAUT 3
#define BAS -3

#define threshold 0.1

void SetupAccelerometre(void);
int detecterMouvement(float dx, float dy, float dz);
int detecterRotation(float dx, float dy);
void lireAxes(float &x, float &y, float &z);
void CalibrerAccelerometre(int nbMesures);

#endif
