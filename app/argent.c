/*
 * argent.c
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "argent.h"


int argent_place = 0;
int argent_trappes[3] = {0, 0, 0};
int argent_total = 150000;

// Vérifier si tout l'argent est placé
int tout_argent_place(void) {
    return argent_total == 0;
}

// Valider les réponses
void valider_reponses(Question q) {
    int bonne_trappe = q.bonne_reponse;

    for (int i = 0; i < 3; i++) {
        int x_debut = 10 + (i * 103);
        int x_fin = x_debut + 93;

        if (i == bonne_trappe) {
            ILI9341_DrawFilledRectangle(x_debut, 90, x_fin, 130, ILI9341_COLOR_GREEN);
        } else {
            ILI9341_DrawFilledRectangle(x_debut, 90, x_fin, 130, ILI9341_COLOR_BLACK);
        }
    }
}

// Calculer l'argent restant
int calculer_argent_restant(Question q) {
    return argent_trappes[q.bonne_reponse];
}
