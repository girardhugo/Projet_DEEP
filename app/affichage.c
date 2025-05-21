/**
 * @file affichage.c
 * @brief Implémentation des fonctions d'affichage pour le jeu.
 *
 * Ce fichier contient les définitions des fonctions permettant de gérer
 * l'affichage des différentes informations et écrans du jeu, y compris
 * l'argent, les questions, les réponses, et les écrans de début, de fin,
 * et de règles.
 *
 * @author hugog
 * @date May 21, 2025
 */

#include "affichage.h"
#include <stdio.h>
#include "bouton.h"

/**
 * @brief Affiche la somme totale d'argent restante.
 */
void afficher_argent_total(void) {
    ILI9341_DrawFilledRectangle(10, 200, 200, 220, ILI9341_COLOR_BLUE);
    char argent_str[20];
    sprintf(argent_str, "Total: %d$", argent_total);
    ILI9341_Puts(10, 200, argent_str, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
}

/**
 * @brief Affiche la somme d'argent placée sur chaque trappe.
 */
void afficher_argent_trappes(void) {
    for (int i = 0; i < 3; i++) {
        int x = 10 + (i * 103);
        ILI9341_DrawFilledRectangle(x, 140, x + 103, 180, ILI9341_COLOR_BLUE);
        char argent_str[10];
        sprintf(argent_str, "%d$", argent_trappes[i]);
        ILI9341_Puts(x, 140, argent_str, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    }
}

/**
 * @brief Affiche la somme d'argent restante après validation des réponses.
 *
 * @param argent_restant La somme d'argent restante.
 */
void afficher_argent_restant(int argent_restant) {
    ILI9341_Fill(ILI9341_COLOR_CYAN);
    ILI9341_DrawRectangle(20, 50, 300, 150, ILI9341_COLOR_BLACK);
    ILI9341_DrawFilledRectangle(21, 51, 299, 149, ILI9341_COLOR_WHITE);
    ILI9341_Puts(50, 60, "Argent restant", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    char argent_str[20];
    sprintf(argent_str, "%d$", argent_restant);
    ILI9341_Puts(100, 100, argent_str, &Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
    ILI9341_Puts(20, 160, "Bonne chance pour la suite", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
}

/**
 * @brief Affiche la question actuelle.
 *
 * @param q La question à afficher.
 */
void afficher_question(Question q) {
    ILI9341_Fill(ILI9341_COLOR_BLUE);
    char numero_str[20];
    sprintf(numero_str, "Question %d/10", numero_question);
    ILI9341_Puts(10, 10, numero_str, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    ILI9341_Puts(10, 40, q.question, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    afficher_argent_total();
}

/**
 * @brief Affiche les réponses possibles pour la question actuelle.
 *
 * @param q La question contenant les réponses à afficher.
 */
void afficher_reponses(Question q) {
    ILI9341_Puts(10, 100, q.reponses[0], &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_Puts(113, 100, q.reponses[1], &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_Puts(216, 100, q.reponses[2], &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    switch (etat_trappe) {
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

/**
 * @brief Affiche l'écran "PERDU" lorsque le joueur perd la partie.
 */
void afficher_ecran_perdu(void) {
    ILI9341_Fill(ILI9341_COLOR_RED);
    ILI9341_DrawRectangle(20, 50, 300, 200, ILI9341_COLOR_WHITE);
    ILI9341_DrawFilledRectangle(21, 51, 299, 199, ILI9341_COLOR_BLACK);
    ILI9341_Puts(45, 70, "Vous avez perdu", &Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
    ILI9341_Puts(40, 120, "Plus d'argent restant.", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    ILI9341_Puts(37, 170, "Reessayez pour gagner !", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
}

/**
 * @brief Affiche l'écran de fin lorsque le joueur termine la partie.
 *
 * @param argent_total La somme totale d'argent restante.
 */
void afficher_ecran_fin(int argent_total) {
    ILI9341_Fill(ILI9341_COLOR_GREEN);
    ILI9341_DrawRectangle(20, 50, 300, 200, ILI9341_COLOR_BLACK);
    ILI9341_DrawFilledRectangle(21, 51, 299, 199, ILI9341_COLOR_CYAN);
    ILI9341_Puts(55, 70, "Fin du jeu !", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
    ILI9341_Puts(50, 120, "Merci d'avoir joue.", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
    char argent_final_str[20];
    sprintf(argent_final_str, "Total: %d$", argent_total);
    ILI9341_Puts(50, 160, argent_final_str, &Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_CYAN);
}

/**
 * @brief Affiche l'écran de début du jeu.
 */
void afficher_ecran_debut(void) {
    ILI9341_Init();
    ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
    ILI9341_Fill(ILI9341_COLOR_BLUE);
    ILI9341_DrawRectangle(20, 30, 300, 100, ILI9341_COLOR_WHITE);
    ILI9341_DrawFilledRectangle(21, 31, 299, 99, ILI9341_COLOR_BLACK);
    ILI9341_Puts(80, 50, "Money Drop", &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    ILI9341_Puts(40, 140, "Appuyez sur un bouton", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    ILI9341_Puts(80, 160, "pour commencer", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    ILI9341_Puts(80, 200, "Bonne chance !", &Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLUE);
}

/**
 * @brief Affiche l'écran des règles du jeu.
 */
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

/**
 * @brief Met à jour les couleurs des trappes en fonction de leur état.
 */
void mettre_a_jour_couleurs_trappes() {
    int largeur_trappe = 93;
    int hauteur_trappe = 40;
    int espacement = 10;

    for (int i = 0; i < 3; i++) {
        int x_debut = 10 + i * (largeur_trappe + espacement);
        int x_fin = x_debut + largeur_trappe;
        int y_debut = 90;
        int y_fin = y_debut + hauteur_trappe;

        uint16_t couleur_trappe = (etat_trappe == i) ? ILI9341_COLOR_BLACK : ILI9341_COLOR_WHITE;
        ILI9341_DrawFilledRectangle(x_debut, y_debut, x_fin, y_fin, couleur_trappe);
    }
    afficher_argent_trappes();
}
