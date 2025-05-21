/**
 * @file main.c
 * @brief Point d'entrée principal du programme.
 *
 * Ce fichier contient la fonction principale et la machine à états du jeu.
 * Il gère les différents états du jeu : DEBUT, REGLES, JEU et FIN.
 *
 * @author hugog
 * @date May 21, 2025
 */

#include "question.h"
#include "affichage.h"
#include "argent.h"

/**
 * @brief États du jeu.
 *
 * Cette énumération représente les différents états possibles du jeu.
 */
typedef enum {
    DEBUT = 0, /**< État initial du jeu. */
    REGLES,    /**< État affichant les règles du jeu. */
    JEU,       /**< État principal du jeu. */
    FIN        /**< État de fin du jeu. */
} etat_t;

etat_t etat = DEBUT; /**< État actuel du jeu. */

/**
 * @brief Fonction principale.
 *
 * Initialise le système, configure les boutons et entre dans la boucle principale
 * du jeu, qui appelle la machine à états.
 */
void main(void) {
    HAL_Init(); /**< Initialisation de la bibliothèque HAL. */

    srand(time(NULL)); /**< Initialisation du générateur de nombres aléatoires. */

    // Initialisation des boutons
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); /**< Bouton haut. */
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); /**< Bouton droit. */
    BSP_GPIO_pin_config(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); /**< Bouton centre. */
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); /**< Bouton gauche. */
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); /**< Bouton bas. */

    while (1) {
        machine_etats(); /**< Appel de la machine à états. */
    }
}

/**
 * @brief Machine à états du jeu.
 *
 * Gère les différents états du jeu : DEBUT, REGLES, JEU et FIN.
 * Chaque état correspond à une étape spécifique du jeu.
 */
void machine_etats() {
    switch (etat) {
    case DEBUT:
        // Réinitialiser les questions posées
        for (int i = 0; i < 25; i++) {
            questions_posees[i] = false;
        }

        // Réinitialiser les variables du jeu
        argent_total = 150000;
        argent_place = 0;
        compteur_questions = 0; /**< Réinitialiser le compteur de questions. */
        numero_question = 1;    /**< Réinitialiser le numéro de la question. */
        for (int i = 0; i < 3; i++) {
            argent_trappes[i] = 0;
        }

        afficher_ecran_debut(); /**< Afficher l'écran de début. */

        // Attendre qu'un bouton soit pressé pour continuer
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) &&
               HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) &&
               HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) {}

        etat = REGLES; /**< Passer à l'état REGLES. */
        break;

    case REGLES:
        afficher_ecran_regles(); /**< Afficher l'écran des règles. */
        HAL_Delay(6000); /**< Attendre 6 secondes. */
        etat = JEU; /**< Passer à l'état JEU. */
        break;

    case JEU:
        while (1) {
            Question q = obtenir_question_suivante();
            if (q.bonne_reponse == -1) { /**< Si aucune question n'est disponible. */
                etat = FIN;
                return;
            }

            afficher_question(q); /**< Afficher la question et le numéro. */
            mettre_a_jour_couleurs_trappes();
            afficher_reponses(q);

            while (1) {
                gerer_boutons(q); /**< Gérer les boutons pour placer ou retirer de l'argent. */

                // Si tout l'argent est placé et le bouton de validation est pressé
                if (tout_argent_place() && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0) {
                    valider_reponses(q);
                    HAL_Delay(2000);

                    argent_total = calculer_argent_restant(q);

                    if (argent_total == 0) {
                        etat = FIN;
                        return;
                    }

                    afficher_argent_restant(argent_total);
                    HAL_Delay(3000);

                    for (int i = 0; i < 3; i++) {
                        argent_trappes[i] = 0;
                    }
                    argent_place = 0;

                    compteur_questions++; /**< Incrémenter le compteur de questions. */
                    numero_question++;    /**< Incrémenter le numéro de la question. */

                    if (compteur_questions >= 10) { /**< Vérifier si 10 questions ont été posées. */
                        etat = FIN;
                        return;
                    }

                    break;
                }
            }
        }
        etat = FIN;
        break;

    case FIN:
        if (argent_total == 0) {
            afficher_ecran_perdu(); /**< Afficher l'écran "PERDU". */
            HAL_Delay(5000); /**< Attendre 5 secondes avant de retourner à l'écran DEBUT. */
            etat = DEBUT; /**< Retourner à l'état DEBUT. */
        } else {
            afficher_ecran_fin(argent_total); /**< Afficher l'écran de fin classique. */
            HAL_Delay(5000); /**< Attendre 5 secondes avant de retourner à l'écran DEBUT. */
            etat = DEBUT; /**< Retourner à l'état DEBUT. */
        }
        break;
    }
}
