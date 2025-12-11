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

    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            (*well)++;
        }
    }

    for (int i = 0; i < CODE_LENGTH; i++) {
        used_secret[secret[i]]++;
        used_guess[guess[i]]++;
    }

    for (int d = 0; d < 10; d++) {
        int common = (used_secret[d] < used_guess[d]) ? used_secret[d] : used_guess[d];
        *misplaced += common;
    }
    *misplaced -= *well;
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
    int score_brut = 0;
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
        score_brut += points_essai;

        printf("   Bien places : %d\n", well);
        printf("   Mal places  : %d\n", misplaced);
        printf("   Points gagnes : +%d\n", points_essai);
        printf("   Score brut : %d\n\n", score_brut);

        if (well == CODE_LENGTH) {
            printf("FELICITATIONS %s ! Combinaison trouvee en %d essai(s) !\n", nom_joueur, essais);
            break;
        }
    }

    time_t fin = time(NULL);
    double temps = difftime(fin, debut);

    // ========== CALCUL DU SCORE SUR 100 ==========
    
    double base_score;
    
    if (essais == 1) {
        base_score = 100.0;
    } else if (essais <= 3) {
        base_score = 80.0;
    } else if (essais <= 5) {
        base_score = 60.0;
    } else if (essais <= 8) {
        base_score = 40.0;
    } else if (essais <= 12) {
        base_score = 20.0;
    } else {
        base_score = 10.0;
    }
    
    double ajustement_qualite = (double)score_brut / (essais * 40.0);
    
    if (ajustement_qualite < 0.5) ajustement_qualite = 0.5;
    if (ajustement_qualite > 1.5) ajustement_qualite = 1.5;
    
    double score_intermediaire = base_score * ajustement_qualite;
    
    double multiplicateur = 1.0;
    if (temps < 15) multiplicateur = 1.3;
    else if (temps < 30) multiplicateur = 1.2;
    else if (temps < 45) multiplicateur = 1.1;
    else if (temps < 60) multiplicateur = 1.05;
    
    int score_final = (int)(score_intermediaire * multiplicateur);
    
    if (score_final > 100) score_final = 100;
    if (score_final < 0) score_final = 0;
    
    // =============================================

    // Sauvegarder le score
    sauvegarder_score(nom_joueur, NOM_MASTERMIND, essais, temps, score_final);

    printf("\n=== RESULTATS FINAUX ===\n");
    printf("Temps : %.1f secondes\n", temps);
    printf("Essais : %d\n", essais);
    printf("Score brut : %d\n", score_brut);
    printf("Base (essais) : %.0f\n", base_score);
    printf("Ajustement qualite : x%.2f\n", ajustement_qualite);
    printf("Multiplicateur temps : x%.2f\n", multiplicateur);
    printf("Score intermediaire : %.0f\n", score_intermediaire);
    printf(">>> SCORE FINAL : %d/100 <<<\n", score_final);  // CORRIGÉ ICI

    printf("\nAppuyez sur Entree pour revenir au menu...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}