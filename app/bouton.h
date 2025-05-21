/*
 * bouton.h
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "question.h"
#include "argent.h"
#include "affichage.h"

#ifndef BOUTON_H_
#define BOUTON_H_

// États des trappes
typedef enum {
    TRAPPE1 = 0,
    TRAPPE2,
    TRAPPE3
} etat_trappe_t;

// Variables globales
extern etat_trappe_t etat_trappe;

// Variables pour stocker l'état précédent des boutons
static uint8_t bouton_droit_prec = 1;
static uint8_t bouton_gauche_prec = 1;
static uint8_t bouton_haut_prec = 1;
static uint8_t bouton_bas_prec = 1;

void gerer_boutons(Question q);

#endif /* BOUTON_H_ */
