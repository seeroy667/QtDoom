#include "LCD.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void SetupLCD()
{
    lcd.init();
    lcd.backlight();
}

void AfficheMouvAcc(int mouvement)
{
    lcd.setCursor(0, 0);
    switch (mouvement)
    {
    case 1:
        lcd.print("Gauche ");
        break;
    case -1:
        lcd.print("Droite ");
        break;
    case 2:
        lcd.print("Avant  ");
        break;
    case -2:
        lcd.print("Arriere");
        break;
    case 3:
        lcd.print("Haut   ");
        break;
    case -3:
        lcd.print("Bas    ");
        break;
    default:
        lcd.print("Centre ");
        break;
    }
}

void AfficheRotationAcc(float x, float y, float z)
{

    lcd.print(z, 2);
    lcd.print("  ");
    lcd.setCursor(0, 1);

    lcd.print(x, 2);
    lcd.print(", ");
    lcd.print(y, 2);
    lcd.print("  ");
}

void AfficheBouton(int bouton1, int bouton2)
{
    lcd.setCursor(0, 0);
    lcd.print("Btn: ");
    lcd.print(!bouton1 ? "1" : "0");
    lcd.print(", ");
    lcd.print(!bouton2 ? "1" : "0");
}

void AfficheEncodeur(int Encodeur)
{
    lcd.setCursor(0, 0);
    lcd.print("Enc: ");
    lcd.print(Encodeur);
    lcd.print("      ");
}

void AfficheJoystick(int x, int y)
{
    lcd.setCursor(0, 0);
    lcd.print("JoyX: ");
    lcd.print(x);
    lcd.print("      ");
    lcd.setCursor(0, 1);
    lcd.print("JoyY: ");
    lcd.print(y);
    lcd.print("      ");
}

void AfficheMessage(const char *message)
{
    lcd.setCursor(0, 0);
    lcd.print(message);
}

void AffichePot(int pot)
{
    lcd.setCursor(0, 1);
    lcd.print("Pot: ");
    lcd.print(pot);
    lcd.print("      ");
}