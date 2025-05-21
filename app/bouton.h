/**
 * @file bouton.h
 * @brief Gestion des boutons et des trappes.
 *
 * Ce fichier contient les déclarations des structures, variables et fonctions
 * utilisées pour gérer les boutons et les trappes dans le jeu.
 *
 * @author hugog
 * @date May 21, 2025
 */

#ifndef BOUTON_H_
#define BOUTON_H_

#include "question.h"
#include "argent.h"
#include "affichage.h"

/**
 * @brief États des trappes.
 *
 * Cette énumération représente les différentes trappes disponibles dans le jeu.
 */
typedef enum {
    TRAPPE1 = 0, /**< Trappe 1. */
    TRAPPE2,     /**< Trappe 2. */
    TRAPPE3      /**< Trappe 3. */
} etat_trappe_t;

/**
 * @brief État actuel de la trappe sélectionnée.
 */
extern etat_trappe_t etat_trappe;

/**
 * @brief Gérer les boutons pour interagir avec les trappes.
 *
 * Cette fonction permet de gérer les actions des boutons pour placer ou retirer
 * de l'argent sur les trappes en fonction de la question en cours.
 *
 * @param q La question actuelle.
 */
void gerer_boutons(Question q);

#endif /* BOUTON_H_ */
