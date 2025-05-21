/**
 * @file argent.h
 * @brief Gestion de l'argent dans le jeu.
 *
 * Ce fichier contient les déclarations des variables et des fonctions
 * utilisées pour gérer l'argent dans le jeu, y compris le placement,
 * la validation des réponses et le calcul de l'argent restant.
 *
 * @author hugog
 * @date May 21, 2025
 */

#ifndef ARGENT_H_
#define ARGENT_H_

#include "question.h"

/**
 * @brief Somme d'argent actuellement placée sur les trappes.
 */
extern int argent_place;

/**
 * @brief Argent placé sur chaque trappe.
 *
 * Ce tableau contient la somme d'argent placée sur chacune des trois trappes.
 */
extern int argent_trappes[3];

/**
 * @brief Somme totale d'argent restante.
 */
extern int argent_total;

/**
 * @brief Vérifie si tout l'argent a été placé.
 *
 * Cette fonction vérifie si la totalité de l'argent disponible a été placée
 * sur les trappes.
 *
 * @return 1 si tout l'argent est placé, 0 sinon.
 */
int tout_argent_place(void);

/**
 * @brief Valide les réponses pour une question donnée.
 *
 * Cette fonction compare les réponses placées sur les trappes avec la bonne réponse
 * pour une question donnée et met à jour les montants d'argent en conséquence.
 *
 * @param q La question actuelle.
 */
void valider_reponses(Question q);

/**
 * @brief Calcule l'argent restant après validation des réponses.
 *
 * Cette fonction calcule la somme d'argent restante après avoir validé les réponses
 * pour une question donnée.
 *
 * @param q La question actuelle.
 * @return La somme d'argent restante.
 */
int calculer_argent_restant(Question q);

#endif /* ARGENT_H_ */
