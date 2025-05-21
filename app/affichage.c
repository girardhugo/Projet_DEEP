/*
 * affichage.c
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "affichage.h"

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
