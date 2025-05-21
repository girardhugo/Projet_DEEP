/*
 * affichage.h
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "question.h"
#include "tft_ili9341/stm32g4_ili9341.h"
#include "tft_ili9341/stm32g4_fonts.h"
#include "config.h"

#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

// États du jeu
typedef enum {
    DEBUT = 0,
    REGLES,
    JEU,
    FIN
} etat_t; // A METTRE AVEC LA MACHINE A ETATS !!!!!

// États des trappes
typedef enum {
    TRAPPE1 = 0,
    TRAPPE2,
    TRAPPE3
} etat_trappe_t;	// A METTRE AVEC LA GESTION DES TRAPPES !!!!!

// Variables pour la gestion de l'argent
static int argent_place = 0;      // Somme d'argent actuellement placée sur les trappes
static int argent_trappes[3] = {0, 0, 0}; // Argent sur chaque trappe
static int argent_total = 150000; // Somme d'argent totale restante
// A METTRE DANS LA GESTION DE L'ARGENT

// Variables globales
static etat_t etat = DEBUT;
static etat_trappe_t etat_trappe = TRAPPE1;  // A METTRE DANS LA GESTION DES TRAPPES !!!!!


void afficher_argent_total(void);
void afficher_argent_trappes(void);
void afficher_argent_restant(int argent_restant);
void afficher_question(Question q);
void afficher_reponses(Question q);
void afficher_ecran_perdu(void);
void afficher_ecran_fin(int argent_total);
void afficher_ecran_debut(void);
void afficher_ecran_regles(void);


#endif /* AFFICHAGE_H_ */
