#ifndef DEFI_H
#define DEFI_H

#define MAX_NAME 30

#define NOM_LABYRINTHE "labyrinthe"
#define NOM_COMPTEBON   "comptebon"
#define NOM_MASTERMIND  "mastermind"
#define NOM_TRIULTIME   "triultime"

typedef struct {
    char name[MAX_NAME];
    char defi[50];
    int steps;
    double time_sec;
    int score;
} ScoreEntry;

void sauvegarder_score(const char* nom, const char* nom_defi, int steps, double temps, int score);
void afficher_classement_global();
void activer_utf8(void);

#endif
