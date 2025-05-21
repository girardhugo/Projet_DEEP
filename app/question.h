/*
 * question.h
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

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

#ifndef QUESTION_H_
#define QUESTION_H_

static bool questions_posees[20] = {false}; // Tableau pour suivre les questions posées
static int compteur_questions = 0; // Compteur pour suivre le nombre de questions posées
static int numero_question = 1; // Numéro de la question actuelle

// Structure pour une question
typedef struct {
    char question[256];
    char reponses[3][256];
    int bonne_reponse; // Index de la bonne réponse (0, 1 ou 2)
} Question;

// Liste des questions
extern Question questions[25];

Question obtenir_question_suivante(void);

#endif /* QUESTION_H_ */
