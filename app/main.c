#include "question.h"
#include "affichage.h"
#include "argent.h"

etat_t etat = DEBUT;

// Fonction principale
void main(void) {
    HAL_Init();

    srand(time(NULL));

    // Initialisation des boutons
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); // bouton haut
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); // bouton droit
    BSP_GPIO_pin_config(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); // bouton centre
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); // bouton gauche
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF); // bouton bas

    while (1) {
        machine_etats();
    }
}

// Machine à états
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
        compteur_questions = 0; // Réinitialiser le compteur de questions
        numero_question = 1;    // Réinitialiser le numéro de la question
        for (int i = 0; i < 3; i++) {
            argent_trappes[i] = 0;
        }

        // Afficher l'écran de début
        afficher_ecran_debut();

        // Attendre qu'un bouton soit pressé pour continuer
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) &&
               HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) &&
               HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) {}

        etat = REGLES; // Passer à l'état REGLES
        break;

        case REGLES:
        	// Afficher l'écran des règles
            afficher_ecran_regles();
            // Attendre 6 secondes
            HAL_Delay(6000);
            etat = JEU;
            break;

        case JEU:
        	while (1) {
        		Question q = obtenir_question_suivante();
        		if (q.bonne_reponse == -1) { // Si aucune question n'est disponible
        			etat = FIN;
        			return;
        		}

        		afficher_question(q); // Afficher la question et le numéro
        		mettre_a_jour_couleurs_trappes();
        		afficher_reponses(q);

        		while (1) {
        			// Supprimer l'affichage du temps restant
        			// Gérer les boutons pour placer ou retirer de l'argent
        			gerer_boutons(q);

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

        				compteur_questions++; // Incrémenter le compteur de questions
        				numero_question++;    // Incrémenter le numéro de la question

        				if (compteur_questions >= 10) { // Vérifier si 10 questions ont été posées
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
                // Si le joueur a perdu, afficher l'écran "PERDU"
                afficher_ecran_perdu();
                // Attendre 5 secondes avant de retourner à l'écran DEBUT
                HAL_Delay(5000);
                etat = DEBUT; // Retourner à l'état DEBUT
            } else {
                // Si le joueur a gagné, afficher l'écran de fin classique
                afficher_ecran_fin(argent_total);
                // Attendre 5 secondes avant de retourner à l'écran DEBUT
                HAL_Delay(5000);
                etat = DEBUT; // Retourner à l'état DEBUT
            }
            HAL_Delay(4);
            break;
    }
}
