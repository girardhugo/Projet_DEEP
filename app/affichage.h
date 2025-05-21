/*
 * affichage.h
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "question.h"
#include "argent.h"

#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

void afficher_argent_total(void);
void afficher_argent_trappes(void);
void afficher_argent_restant(int argent_restant);
void afficher_question(Question q);
void afficher_reponses(Question q);
void afficher_ecran_perdu(void);
void afficher_ecran_fin(int argent_total);
void afficher_ecran_debut(void);
void afficher_ecran_regles(void);
void mettre_a_jour_couleurs_trappes(void);


#endif /* AFFICHAGE_H_ */
