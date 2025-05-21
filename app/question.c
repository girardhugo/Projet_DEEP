/**
 * @file question.c
 * @brief Implémentation des fonctions liées aux questions.
 *
 * Ce fichier contient les définitions des fonctions permettant de gérer
 * les questions dans le jeu, ainsi que la liste des questions disponibles.
 *
 * @author hugog
 * @date May 21, 2025
 */

#include "question.h"

/**
 * @brief Liste des questions disponibles dans le jeu.
 *
 * Chaque question est composée d'un texte, de trois réponses possibles,
 * et de l'index de la bonne réponse (0, 1 ou 2).
 */
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

/**
 * @brief Tableau pour suivre les questions déjà posées.
 *
 * Chaque élément du tableau est un booléen indiquant si la question
 * correspondante a déjà été posée (`true`) ou non (`false`).
 */
bool questions_posees[20] = {false};

/**
 * @brief Compteur pour suivre le nombre de questions posées.
 */
int compteur_questions = 0;

/**
 * @brief Numéro de la question actuelle.
 */
int numero_question = 1;

/**
 * @brief Obtenir la prochaine question non posée.
 *
 * Cette fonction sélectionne une question aléatoire parmi celles qui n'ont pas encore été posées.
 * Si toutes les questions ont été posées, elle retourne une question vide.
 *
 * @return La prochaine question ou une question vide si toutes les questions ont été posées.
 */
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
