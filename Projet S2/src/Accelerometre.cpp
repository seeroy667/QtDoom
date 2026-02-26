#include "Accelerometre.h"

float xBase = 0;
float yBase = 0;
float zBase = 0;

void SetupAccelerometre()
{
    pinMode(PIN_X, INPUT);
    pinMode(PIN_Y, INPUT);
    pinMode(PIN_Z, INPUT);
}


void CalibrerAccelerometre(int nbMesures)
{
    float sumX = 0, sumY = 0, sumZ = 0;
    for (int i = 0; i < nbMesures; i++)
    {
        sumX += analogRead(PIN_X) * (3.3 / 1023.0);
        sumY += analogRead(PIN_Y) * (3.3 / 1023.0);
        sumZ += analogRead(PIN_Z) * (3.3 / 1023.0);
        delay(5); 
    }
    xBase = sumX / nbMesures;
    yBase = sumY / nbMesures;
    zBase = sumZ / nbMesures;
}

void lireAxes(float &x, float &y, float &z)
{
    x = analogRead(PIN_X) * (3.3 / 1023.0) - xBase;
    y = analogRead(PIN_Y) * (3.3 / 1023.0) - yBase;
    z = analogRead(PIN_Z) * (3.3 / 1023.0) - zBase;
}

int detecterMouvement(float dx, float dy, float dz)
{
    
    float deltaX = fabs(dx) - threshold;
    float deltaY = fabs(dy) - threshold;
    float deltaZ = fabs(dz) - threshold;

  
    if (deltaX <= 0 && deltaY <= 0 && deltaZ <= 0)
        return HAUT;

    
    if (deltaX >= deltaY && deltaX >= deltaZ)
    {
        return (dx > 0) ? GAUCHE : DROITE;
    }
    else if (deltaY >= deltaX && deltaY >= deltaZ)
    {
        return (dy < 0) ? AVANT : ARRIERE;
    }
    else
    { 
        return BAS;
    }
}