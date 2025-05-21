/*
 * bouton.c
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "bouton.h"

etat_trappe_t etat_trappe = TRAPPE1;

void gerer_boutons(Question q) {
    static uint32_t dernier_temps_haut = 0; // Dernier temps où le bouton haut a été traité
    static uint32_t dernier_temps_bas = 0;  // Dernier temps où le bouton bas a été traité

    uint8_t bouton_droit = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
    uint8_t bouton_gauche = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
    uint8_t bouton_haut = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    uint8_t bouton_bas = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);

    // Gestion du bouton droit
    if (bouton_droit == 0 && bouton_droit_prec == 1) {
        if (etat_trappe < TRAPPE3) etat_trappe++;
        mettre_a_jour_couleurs_trappes();
        afficher_reponses(q);
    }

    // Gestion du bouton gauche
    if (bouton_gauche == 0 && bouton_gauche_prec == 1) {
        if (etat_trappe > TRAPPE1) etat_trappe--;
        mettre_a_jour_couleurs_trappes();
        afficher_reponses(q);
    }

    // Gestion du bouton haut (ajouter de l'argent)
    if (bouton_haut == 0) {
        uint32_t temps_actuel = HAL_GetTick();
        if (temps_actuel - dernier_temps_haut >= 200) { // Ajouter toutes les 200 ms
            if (argent_total >= 10000) {
                argent_trappes[etat_trappe] += 10000;
                argent_total -= 10000;
                afficher_argent_total();
                afficher_argent_trappes();
            }
            dernier_temps_haut = temps_actuel; // Mettre à jour le dernier temps
        }
    } else {
        dernier_temps_haut = 0; // Réinitialiser si le bouton est relâché
    }

    // Gestion du bouton bas (retirer de l'argent)
    if (bouton_bas == 0) {
        uint32_t temps_actuel = HAL_GetTick();
        if (temps_actuel - dernier_temps_bas >= 200) { // Retirer toutes les 200 ms
            if (argent_trappes[etat_trappe] >= 10000) {
                argent_trappes[etat_trappe] -= 10000;
                argent_total += 10000;
                afficher_argent_total();
                afficher_argent_trappes();
            }
            dernier_temps_bas = temps_actuel; // Mettre à jour le dernier temps
        }
    } else {
        dernier_temps_bas = 0; // Réinitialiser si le bouton est relâché
    }

    // Mettre à jour les états précédents des boutons
    bouton_droit_prec = bouton_droit;
    bouton_gauche_prec = bouton_gauche;
    bouton_haut_prec = bouton_haut;
    bouton_bas_prec = bouton_bas;
}
