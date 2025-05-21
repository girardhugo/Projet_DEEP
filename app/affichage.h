/**
 * @file affichage.h
 * @brief Gestion de l'affichage dans le jeu.
 *
 * Ce fichier contient les déclarations des fonctions utilisées pour gérer
 * l'affichage des différentes informations et écrans du jeu.
 *
 * Ces fonctions permettent d'afficher l'argent, les questions, les réponses,
 * ainsi que les écrans de début, de fin et de règles.
 *
 * @author hugog
 * @date May 21, 2025
 */

#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

#include "question.h"
#include "argent.h"

/**
 * @brief Affiche la somme totale d'argent restante.
 */
void afficher_argent_total(void);

/**
 * @brief Affiche la somme d'argent placée sur chaque trappe.
 */
void afficher_argent_trappes(void);

/**
 * @brief Affiche la somme d'argent restante après validation des réponses.
 *
 * @param argent_restant La somme d'argent restante.
 */
void afficher_argent_restant(int argent_restant);

/**
 * @brief Affiche la question actuelle.
 *
 * @param q La question à afficher.
 */
void afficher_question(Question q);

/**
 * @brief Affiche les réponses possibles pour la question actuelle.
 *
 * @param q La question contenant les réponses à afficher.
 */
void afficher_reponses(Question q);

/**
 * @brief Affiche l'écran "PERDU" lorsque le joueur perd la partie.
 */
void afficher_ecran_perdu(void);

/**
 * @brief Affiche l'écran de fin lorsque le joueur termine la partie.
 *
 * @param argent_total La somme totale d'argent restante.
 */
void afficher_ecran_fin(int argent_total);

/**
 * @brief Affiche l'écran de début du jeu.
 */
void afficher_ecran_debut(void);

/**
 * @brief Affiche l'écran des règles du jeu.
 */
void afficher_ecran_regles(void);

/**
 * @brief Met à jour les couleurs des trappes en fonction de leur état.
 */
void mettre_a_jour_couleurs_trappes(void);

#endif /* AFFICHAGE_H_ */
