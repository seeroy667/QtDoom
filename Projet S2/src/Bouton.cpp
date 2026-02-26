#include "Bouton.h"


static bool etatStable1 = false;
static bool etatPrecedent1 = false;
static bool derniereLecture1 = false;
static unsigned long dernierChangement1 = 0;


static bool etatStable2 = false;
static bool etatPrecedent2 = false;
static bool derniereLecture2 = false;
static unsigned long dernierChangement2 = 0;


void bouton_init(void)
{
    pinMode(BROCHE_BOUTON1, INPUT_PULLUP);
    pinMode(BROCHE_BOUTON2, INPUT_PULLUP);

    bool appuye = (digitalRead(BROCHE_BOUTON1) == LOW);
    etatStable1 = etatPrecedent1 = derniereLecture1 = appuye;
    dernierChangement1 = millis();

   
    appuye = (digitalRead(BROCHE_BOUTON2) == LOW);
    etatStable2 = etatPrecedent2 = derniereLecture2 = appuye;
    dernierChangement2 = millis();
}


void bouton_update(void)
{
    
    bool lecture = (digitalRead(BROCHE_BOUTON1) == LOW);
    if (lecture != derniereLecture1)
    {
        derniereLecture1 = lecture;
        dernierChangement1 = millis();
    }
    if ((millis() - dernierChangement1) >= DELAI_ANTI_REBOND)
    {
        etatPrecedent1 = etatStable1;
        etatStable1 = derniereLecture1;
    }

    
    lecture = (digitalRead(BROCHE_BOUTON2) == LOW);
    if (lecture != derniereLecture2)
    {
        derniereLecture2 = lecture;
        dernierChangement2 = millis();
    }
    if ((millis() - dernierChangement2) >= DELAI_ANTI_REBOND)
    {
        etatPrecedent2 = etatStable2;
        etatStable2 = derniereLecture2;
    }
}


bool bouton1_est_appuye(void) { return etatStable1; }
bool bouton1_vient_detre_appuye(void) { return (!etatPrecedent1 && etatStable1); }
bool bouton1_vient_detre_relache(void) { return (etatPrecedent1 && !etatStable1); }


bool bouton2_est_appuye(void) { return etatStable2; }
bool bouton2_vient_detre_appuye(void) { return (!etatPrecedent2 && etatStable2); }
bool bouton2_vient_detre_relache(void) { return (etatPrecedent2 && !etatStable2); }
