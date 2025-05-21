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
extern int argent_place;
extern int argent_trappes[3];
extern int argent_total;

int tout_argent_place(void);
void valider_reponses(Question q);
int calculer_argent_restant(Question q);

#endif /* ARGENT_H_ */
