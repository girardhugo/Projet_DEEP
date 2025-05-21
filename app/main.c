#include "config.h"
#include "stm32g4_sys.h"
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "stm32g4xx_hal.h"
#include "tft_ili9341/stm32g4_ili9341.h"
#include "tft_ili9341/stm32g4_fonts.h"
#include "stm32g4xx_hal_gpio.h"
#include <stdlib.h>
#include <time.h>
#include "affichage.h"

// Variables pour stocker l'état précédent des boutons
static uint8_t bouton_droit_prec = 1;
static uint8_t bouton_gauche_prec = 1;
static uint8_t bouton_haut_prec = 1;
static uint8_t bouton_bas_prec = 1;

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

// Obtenir la prochaine question
Question obtenir_question_suivante(void) {
    int questions_restantes = 0;

    // Vérifier combien de questions restent à poser
    for (int i = 0; i < 25; i++) {
        if (!questions_posees[i]) {
            questions_restantes++;
        }
    }

    // Si toutes les questions ont été posées, retourner une question vide
    if (questions_restantes == 0) {
        return (Question){"", {"", "", ""}, -1}; // Retourner une question vide
    }

    // Choisir une question au hasard parmi celles qui n'ont pas été posées
    int index;
    do {
        index = rand() % 25; // Générer un index aléatoire entre 0 et 24
    } while (questions_posees[index]);

    // Marquer la question comme posée
    questions_posees[index] = true;

    // Retourner la question choisie
    return questions[index];
}

// Mettre à jour les couleurs des trappes
void mettre_a_jour_couleurs_trappes() {
	int largeur_trappe = 93; // Largeur de chaque trappe
	int hauteur_trappe = 40; // Hauteur de chaque trappe
	int espacement = 10;     // Espacement entre les trappes

	for (int i = 0; i < 3; i++) {
		// Calculer les positions des trappes
		int x_debut = 10 + i * (largeur_trappe + espacement);
		int x_fin = x_debut + largeur_trappe;
		int y_debut = 90; // Position verticale des trappes
		int y_fin = y_debut + hauteur_trappe;

		// Déterminer la couleur de la trappe
		uint16_t couleur_trappe = (etat_trappe == i) ? ILI9341_COLOR_BLACK : ILI9341_COLOR_WHITE;

		// Dessiner la trappe
		ILI9341_DrawFilledRectangle(x_debut, y_debut, x_fin, y_fin, couleur_trappe);
	}
    afficher_argent_trappes();
}

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
