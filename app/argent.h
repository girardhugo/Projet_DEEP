/*
 * argent.h
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "question.h"

#ifndef ARGENT_H_
#define ARGENT_H_

// Variables pour la gestion de l'argent
static int argent_place = 0;      // Somme d'argent actuellement placée sur les trappes
static int argent_trappes[3] = {0, 0, 0}; // Argent sur chaque trappe
static int argent_total = 150000; // Somme d'argent totale restante

int tout_argent_place(void);
void valider_reponses(Question q);
int calculer_argent_restant(Question q);

#endif /* ARGENT_H_ */
