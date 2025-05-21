/*
 * question.c
 *
 *  Created on: May 21, 2025
 *      Author: hugog
 */

#include "question.h"

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
