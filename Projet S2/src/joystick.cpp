/*Projet S2 Hiver 2026
Code pour le joystick
Écrit par Félix Nolin-Aubertin*/

#include "joystick.h"

void joystick_setup()
{
    pinMode(JOYSTICK_X, INPUT);
    pinMode(JOYSTICK_Y, INPUT);
    pinMode(JOYSTICK_SW, INPUT);
}

bool isButtonPressed()
{
    return (digitalRead(JOYSTICK_SW));
}

int readJoystick_x()
{
    int read = analogRead(JOYSTICK_X);

    
    if (read < MIDDLE + DEAD_SPACE && read > MIDDLE - DEAD_SPACE)
    {
        read = MIDDLE;
    }

    
    if (read < MIN_VALUE + DEAD_SPACE_MAX)
    {
        read = MIN_VALUE;
    }

    
    if (read > MAX_VALUE - DEAD_SPACE_MAX)
    {
        read = MAX_VALUE;
    }

    return read;
}

int readJoystick_y()
{
    int read = analogRead(JOYSTICK_Y);

   
    if (read < MIDDLE + DEAD_SPACE && read > MIDDLE - DEAD_SPACE)
    {
        read = MIDDLE;
    }

    
    if (read < MIN_VALUE + DEAD_SPACE_MAX)
    {
        read = MIN_VALUE;
    }

    
    if (read > MAX_VALUE - DEAD_SPACE_MAX)
    {
        read = MAX_VALUE;
    }

    return read;
}
