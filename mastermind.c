#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "mastermind.h"
#include "defi.h"

void generate_code(int code[]) {
    for (int i = 0; i < CODE_LENGTH; i++) {
        code[i] = rand() % 10;
    }
}

void get_guess(int guess[]) {
    printf("Votre essai (%d chiffres) : ", CODE_LENGTH);
    for (int i = 0; i < CODE_LENGTH; i++) {
        scanf("%1d", &guess[i]);
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void check_guess(int secret[], int guess[], int *well, int *misplaced) {
    int used_secret[10] = {0};
    int used_guess[10] = {0};

    *well = 0;
    *misplaced = 0;

    // Bien places
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            (*well)++;
        }
    }

    // Comptage des chiffres presents
    for (int i = 0; i < CODE_LENGTH; i++) {
        used_secret[secret[i]]++;
        used_guess[guess[i]]++;
    }

    // Mal places 
    for (int d = 0; d < 10; d++) {
        int common = (used_secret[d] < used_guess[d]) ? used_secret[d] : used_guess[d];
        *misplaced += common;
    }
    *misplaced -= *well; // on enleve les bien places deja comptes
}

void jouer_defi_mastermind(char nom_joueur[]) {
    activer_utf8();
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    int secret[CODE_LENGTH];
    int guess[CODE_LENGTH];
    int well, misplaced;
    int score = 0;
    int essais = 0;

    generate_code(secret);

    time_t debut = time(NULL);

    printf("\n=== DEFI MASTERMIND ===\n");
    printf("Joueur : %s\n", nom_joueur);
    printf("Trouvez la combinaison secrete a %d chiffres !\n\n", CODE_LENGTH);

    while (1) {
        essais++;
        get_guess(guess);
        check_guess(secret, guess, &well, &misplaced);

        int points_essai = well * 10 + misplaced * 2;
        score += points_essai;

        printf("   Bien places : %d\n", well);
        printf("   Mal places  : %d\n", misplaced);
        printf("   Points gagnes : +%d\n", points_essai);
        printf("   Score total : %d\n\n", score);

        if (well == CODE_LENGTH) {
            printf("FELICITATIONS %s ! Combinaison trouvee en %d essai(s) !\n", nom_joueur, essais);
            break;
        }
    }

    time_t fin = time(NULL);
    double temps = difftime(fin, debut);

    double multiplicateur = 1.0;
    if (temps < 20) multiplicateur = 2.0;
    else if (temps < 40) multiplicateur = 1.5;
    else if (temps < 60) multiplicateur = 1.2;

    int score_final = (int)(score * multiplicateur);

    // Sauvegarder le score
    sauvegarder_score(nom_joueur, NOM_MASTERMIND, essais, temps, score_final);

    printf("\n=== RESULTATS FINAUX ===\n");
    printf("Temps : %.1f secondes\n", temps);
    printf("Essais : %d\n", essais);
    printf("Score brut : %d\n", score);
    printf("Multiplicateur : x%.1f\n", multiplicateur);
    printf(">>> SCORE FINAL : %d <<<\n", score_final);

    printf("\nAppuyez sur Entree pour revenir au menu...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}