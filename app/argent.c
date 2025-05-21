/**
 * @file argent.c
 * @brief Implémentation des fonctions de gestion de l'argent dans le jeu.
 *
 * Ce fichier contient les définitions des fonctions permettant de gérer
 * l'argent dans le jeu, y compris le placement, la validation des réponses
 * et le calcul de l'argent restant.
 *
 * @author hugog
 * @date May 21, 2025
 */

#include "argent.h"

/**
 * @brief Somme d'argent actuellement placée sur les trappes.
 */
int argent_place = 0;

/**
 * @brief Argent placé sur chaque trappe.
 *
 * Ce tableau contient la somme d'argent placée sur chacune des trois trappes.
 */
int argent_trappes[3] = {0, 0, 0};

/**
 * @brief Somme totale d'argent restante.
 */
int argent_total = 150000;

/**
 * @brief Vérifie si tout l'argent a été placé.
 *
 * Cette fonction vérifie si la totalité de l'argent disponible a été placée
 * sur les trappes.
 *
 * @return 1 si tout l'argent est placé, 0 sinon.
 */
int tout_argent_place(void) {
    return argent_total == 0;
}

/**
 * @brief Valide les réponses pour une question donnée.
 *
 * Cette fonction compare les réponses placées sur les trappes avec la bonne réponse
 * pour une question donnée. Elle met à jour l'affichage des trappes en colorant
 * la bonne trappe en vert et les autres en noir.
 *
 * @param q La question actuelle.
 */
void valider_reponses(Question q) {
    int bonne_trappe = q.bonne_reponse;

    for (int i = 0; i < 3; i++) {
        int x_debut = 10 + (i * 103); /**< Coordonnée X de début pour la trappe. */
        int x_fin = x_debut + 93;     /**< Coordonnée X de fin pour la trappe. */

        if (i == bonne_trappe) {
            ILI9341_DrawFilledRectangle(x_debut, 90, x_fin, 130, ILI9341_COLOR_GREEN); /**< Colorer la bonne trappe en vert. */
        } else {
            ILI9341_DrawFilledRectangle(x_debut, 90, x_fin, 130, ILI9341_COLOR_BLACK); /**< Colorer les autres trappes en noir. */
        }
    }
}

/**
 * @brief Calcule l'argent restant après validation des réponses.
 *
 * Cette fonction retourne la somme d'argent placée sur la trappe correspondant
 * à la bonne réponse pour une question donnée.
 *
 * @param q La question actuelle.
 * @return La somme d'argent restante sur la bonne trappe.
 */
int calculer_argent_restant(Question q) {
    return argent_trappes[q.bonne_reponse];
}
