/**
 * @file question.h
 * @brief Gestion des questions du jeu.
 *
 * Ce fichier contient les déclarations des structures, variables et fonctions
 * utilisées pour gérer les questions dans le jeu.
 *
 * @author hugog
 * @date May 21, 2025
 */

#ifndef QUESTION_H_
#define QUESTION_H_

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

/**
 * @brief Tableau pour suivre les questions posées.
 *
 * Chaque élément du tableau est un booléen indiquant si la question
 * correspondante a déjà été posée (`true`) ou non (`false`).
 */
extern bool questions_posees[20];

/**
 * @brief Compteur pour suivre le nombre de questions posées.
 */
extern int compteur_questions;

/**
 * @brief Numéro de la question actuelle.
 */
extern int numero_question;

/**
 * @brief Structure représentant une question.
 *
 * Cette structure contient le texte de la question, un tableau de réponses possibles,
 * et l'index de la bonne réponse.
 */
typedef struct {
    char question[256];      /**< Texte de la question. */
    char reponses[3][256];   /**< Tableau des réponses possibles. */
    int bonne_reponse;       /**< Index de la bonne réponse (0, 1 ou 2). */
} Question;

/**
 * @brief Liste des questions disponibles.
 *
 * Ce tableau contient toutes les questions disponibles dans le jeu.
 */
extern Question questions[25];

/**
 * @brief Obtenir la prochaine question non posée.
 *
 * Cette fonction sélectionne une question aléatoire parmi celles qui n'ont pas encore été posées.
 * Si toutes les questions ont été posées, elle retourne une question vide.
 *
 * @return La prochaine question ou une question vide si toutes les questions ont été posées.
 */
Question obtenir_question_suivante(void);

#endif /* QUESTION_H_ */
