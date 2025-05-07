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

// États du jeu
typedef enum {
    DEBUT = 0,
    REGLES,
    JEU,
    FIN
} etat_t;

// États des trappes
typedef enum {
    TRAPPE1 = 0,
    TRAPPE2,
    TRAPPE3
} etat_trappe_t;

// Structure pour une question
typedef struct {
    char question[256];
    char reponses[3][256];
    int bonne_reponse; // Index de la bonne réponse (0, 1 ou 2)
} Question;

// Variables globales
static etat_t etat = DEBUT;
static etat_trappe_t etat_trappe = TRAPPE1;
static bool questions_posees[20] = {false}; // Tableau pour suivre les questions posées
static int compteur_questions = 0; // Compteur pour suivre le nombre de questions posées
static int numero_question = 1; // Numéro de la question actuelle

// Variables pour stocker l'état précédent des boutons
static uint8_t bouton_droit_prec = 1;
static uint8_t bouton_gauche_prec = 1;
static uint8_t bouton_haut_prec = 1;
static uint8_t bouton_bas_prec = 1;

// Variables pour la gestion de l'argent
static int argent_total = 150000; // Somme d'argent totale restante
static int argent_place = 0;      // Somme d'argent actuellement placée sur les trappes
static int argent_trappes[3] = {0, 0, 0}; // Argent sur chaque trappe

// Liste des questions
Question questions[25] = {
        {"Quel est le plus grand desert ?", {"Sahara", "Gobi", "Atacama"}, 0},
        {"Quelle est la capitale de l'Allemagne ?", {"Munich", "Berlin", "Hambourg"}, 1},
        {"Combien fait 7x8 ?", {"49", "64", "56"}, 2},
        {"Qui a peint la Joconde ?", {"Leonard", "Michel", "Raphael"}, 0},
        {"Quelle est la couleur du sang dans les veines ?", {"Bleu", "Rouge", "Violet"}, 1},
        {"Quel sport se joue avec une batte ?", {"Golf", "Cricket", "Baseball"}, 2},
        {"Qui a peint 'La Nuit etoilee' ?", {"Van Gogh", "Monet", "Picasso"}, 0},
        {"Combien d’ailes a un papillon ?", {"6", "2", "4"}, 2},
        {"Quelle planete est surnommee 'l'etoile du berger' ?", {"Mars", "Mercure", "Venus"}, 2},
        {"Quelle matiere est liquide a temperature ambiante ?", {"Fer", "Cuivre", "Eau"}, 2},
        {"Quelle est la valeur de pi ?", {"3,15", "3,14", "3,16"}, 1},
        {"Quel est le nom de l’alphabet grec ?", {"Gamma", "Alpha", "Omega"}, 1},
        {"Qui dirige un orchestre ?", {"Chef", "Soliste", "Pianiste"}, 0},
        {"Combien d'annees dans un siecle ?", {"1000", "10", "100"}, 2},
        {"Quel est le symbole de la paix ?", {"Etoile", "Colombe", "Rose"}, 1},
        {"Quelle est la monnaie utilisee aux Etats-Unis ?", {"Euro", "Livre", "Dollar"}, 2},
        {"Quel pays est connu pour les pyramides ?", {"Inde", "Mexique", "Egypte"}, 2},
        {"Qui a decouvert l'Amerique ?", {"Magellan", "Colomb", "Cook"}, 1},
        {"Quelle est la capitale de la Russie ?", {"Kiev", "Moscou", "Pekin"}, 1},
        {"Quelle est la vitesse de la lumiere (km/s)?", {"290 000", "299 792", "300 000"}, 1},
        {"Quel metal est le plus leger ?", {"Argent", "Cuivre", "Aluminium"}, 2},
        {"Quelle galaxie contient notre systeme solaire ?", {"Centaur", "Voie Lactee", "Andromede"}, 1},
        {"Qui a ecrit 'L'Odyssee' ?", {"Homere", "Sophocle", "Virgile"}, 0},
        {"Combien de cordes a un violon ?", {"6", "4", "5"}, 1},
        {"Quel est le point de congelation de l'eau en degres C ?", {"0", "-1", "-5"}, 0},
    };

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

// Afficher l'argent total
void afficher_argent_total(void) {
    // Effacer la zone où le total est affiché
    ILI9341_DrawFilledRectangle(10, 200, 200, 220, ILI9341_COLOR_BLUE);

    // Afficher le nouveau total
    char argent_str[20];
    sprintf(argent_str, "Total: %d$", argent_total);
    ILI9341_Puts(10, 200, argent_str, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
}

// Afficher l'argent sur les trappes
void afficher_argent_trappes(void) {
    for (int i = 0; i < 3; i++) {
        // Position X des trappes (écartées de 90 pixels)
        int x = 10 + (i * 103);

        // Effacer la zone d'affichage de la trappe
        ILI9341_DrawFilledRectangle(x, 140, x + 103, 180, ILI9341_COLOR_BLUE);

        // Afficher l'argent sur la trappe
        char argent_str[10];
        sprintf(argent_str, "%d$", argent_trappes[i]);
        ILI9341_Puts(x, 140, argent_str, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    }
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

// Afficher l'argent restant
void afficher_argent_restant(int argent_restant) {
    // Effacer l'écran avec une couleur de fond agréable
    ILI9341_Fill(ILI9341_COLOR_CYAN);

    // Dessiner un rectangle pour encadrer le texte
    ILI9341_DrawRectangle(20, 50, 300, 150, ILI9341_COLOR_BLACK);

    // Remplir l'intérieur du rectangle avec une couleur douce
    ILI9341_DrawFilledRectangle(21, 51, 299, 149, ILI9341_COLOR_WHITE);

    // Afficher un titre centré
    ILI9341_Puts(50, 60, "Argent restant", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    // Afficher le montant restant centré dans le rectangle
    char argent_str[20];
    sprintf(argent_str, "%d$", argent_restant);
    ILI9341_Puts(100, 100, argent_str, &Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);

    ILI9341_Puts(20, 160, "Bonne chance pour la suite", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
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

// Afficher une question
void afficher_question(Question q) {
    // Effacer l'écran et afficher la question
    ILI9341_Fill(ILI9341_COLOR_BLUE);

    // Afficher le numéro de la question
    char numero_str[20];
    sprintf(numero_str, "Question %d/10", numero_question);
    ILI9341_Puts(10, 10, numero_str, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);

    // Afficher la question
    ILI9341_Puts(10, 40, q.question, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);

    // Afficher l'argent total
    afficher_argent_total();
}

// Afficher les réponses d'une question
void afficher_reponses(Question q) {
    ILI9341_Puts(10, 100, q.reponses[0], &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_Puts(113, 100, q.reponses[1], &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_Puts(216, 100, q.reponses[2], &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    switch(etat_trappe) {
    case TRAPPE1:
    	ILI9341_Puts(10, 100, q.reponses[0], &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    	break;
    case TRAPPE2:
        ILI9341_Puts(113, 100, q.reponses[1], &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
        break;
    case TRAPPE3:
        ILI9341_Puts(216, 100, q.reponses[2], &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
        break;
    }
}

// Afficher l'écran perdu
void afficher_ecran_perdu(void) {
    // Effacer l'écran avec une couleur sombre
    ILI9341_Fill(ILI9341_COLOR_RED);

    // Dessiner un rectangle blanc pour encadrer le message
    ILI9341_DrawRectangle(20, 50, 300, 200, ILI9341_COLOR_WHITE);

    // Remplir l'intérieur du rectangle avec une couleur noire
    ILI9341_DrawFilledRectangle(21, 51, 299, 199, ILI9341_COLOR_BLACK);

    // Afficher un message de défaite centré
    ILI9341_Puts(45, 70, "Vous avez perdu", &Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
    ILI9341_Puts(40, 120, "Plus d'argent restant.", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    // Ajouter un message motivant
    ILI9341_Puts(37, 170, "Reessayez pour gagner !", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
}

// Afficher l'écran de fin (en cas de victoire)
void afficher_ecran_fin(int argent_total) {
    // Effacer l'écran avec une couleur agréable
    ILI9341_Fill(ILI9341_COLOR_GREEN);

    // Dessiner un rectangle blanc pour encadrer le message
    ILI9341_DrawRectangle(20, 50, 300, 200, ILI9341_COLOR_BLACK);

    // Remplir l'intérieur du rectangle avec une couleur noire
    ILI9341_DrawFilledRectangle(21, 51, 299, 199, ILI9341_COLOR_CYAN);

    // Afficher un message de fin centré
    ILI9341_Puts(55, 70, "Fin du jeu !", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
    ILI9341_Puts(50, 120, "Merci d'avoir joue.", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);

    // Afficher le total final
    char argent_final_str[20];
    sprintf(argent_final_str, "Total: %d$", argent_total);
    ILI9341_Puts(50, 160, argent_final_str, &Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_CYAN);
}

// Afficher_ecran_debut
void afficher_ecran_debut(void) {
	// Initialiser l'écran
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);

    // Effacer l'écran avec une couleur agréable
    ILI9341_Fill(ILI9341_COLOR_BLUE);

    // Dessiner un rectangle blanc pour encadrer le titre
    ILI9341_DrawRectangle(20, 30, 300, 100, ILI9341_COLOR_WHITE);

    // Remplir l'intérieur du rectangle avec une couleur noire
    ILI9341_DrawFilledRectangle(21, 31, 299, 99, ILI9341_COLOR_BLACK);

    // Afficher le titre centré
    ILI9341_Puts(80, 50, "Money Drop", &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    // Ajouter des instructions pour l'utilisateur
    ILI9341_Puts(40, 140, "Appuyez sur un bouton", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    ILI9341_Puts(80, 160, "pour commencer", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);

    // Ajouter un message motivant
    ILI9341_Puts(80, 200, "Bonne chance !", &Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLUE);
}

// Afficher ecran regles
void afficher_ecran_regles(void) {
	ILI9341_Fill(ILI9341_COLOR_CYAN);
	ILI9341_Puts(50, 10, "Regles du jeu", &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_CYAN);
	ILI9341_Puts(37, 50, "Vous avez 20 liasses de", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
	ILI9341_Puts(70, 70, "billets au debut.", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
	ILI9341_Puts(37, 100, "Repartissez les billets", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
	ILI9341_Puts(70, 120, "sur les trappes.", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
	ILI9341_Puts(50, 150, "Chaque manche, les", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
	ILI9341_Puts(13, 170, "mauvaises trappes tombent !", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
	ILI9341_Puts(30, 200, "Conservez un maximum de", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
	ILI9341_Puts(110, 220, "billets !", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
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
            break;
    }
}
