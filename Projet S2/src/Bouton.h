#ifndef BOUTON_H
#define BOUTON_H

#include <Arduino.h>

#define BROCHE_BOUTON1 51
#define BROCHE_BOUTON2 53

#define DELAI_ANTI_REBOND 25 

void bouton_init(void);
void bouton_update(void);
bool bouton1_est_appuye(void);
bool bouton1_vient_detre_appuye(void);
bool bouton1_vient_detre_relache(void);
bool bouton2_est_appuye(void);
bool bouton2_vient_detre_appuye(void);
bool bouton2_vient_detre_relache(void);

#endif
