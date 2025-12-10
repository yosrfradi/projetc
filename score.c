#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "defi.h"

void activer_utf8(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif
}

void sauvegarder_score(const char* nom, const char* nom_defi, int steps, double temps, int score) {
    FILE* f = fopen("scores.txt", "a");
    if (f) {
        fprintf(f, "%s;%s;%d;%.2f;%d\n", nom, nom_defi, steps, temps, score);
        fflush(f);  // Force write to disk immediately
        fclose(f);
        printf("[Score sauvegarde: %d points]\n", score);
    } else {
        printf("[ERREUR: Impossible de sauvegarder le score!]\n");
    }
}

void afficher_classement_global() {
    FILE* f = fopen("scores.txt", "r");
    if (!f) {
        printf("Aucun score enregistre pour le moment.\n");
        return;
    }

    ScoreEntry scores[1000];
    int count = 0;

    // Lecture ligne par ligne
    char buf[256];
    while (count < 1000 && fgets(buf, sizeof(buf), f)) {
        // Supprimer les caracteres de fin de ligne
        int len = strlen(buf);
        while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) {
            buf[--len] = '\0';
        }
        
        // Ignorer les lignes vides
        if (len == 0) {
            continue;
        }

        // Remplacer les virgules decimales par des points
        for (int i = 0; i < len; i++) {
            if (buf[i] == ',') buf[i] = '.';
        }

        // Parser avec strtok pour plus de robustesse
        char* token;
        char* rest = buf;
        char name[50] = "", defi[50] = "";
        int steps = 0, score = 0;
        double time_sec = 0.0;
        int field = 0;
        
        while ((token = strtok_s(rest, ";", &rest)) != NULL && field < 5) {
            switch (field) {
                case 0: strncpy(name, token, 49); name[49] = '\0'; break;
                case 1: strncpy(defi, token, 49); defi[49] = '\0'; break;
                case 2: steps = atoi(token); break;
                case 3: time_sec = atof(token); break;
                case 4: score = atoi(token); break;
            }
            field++;
        }
        
        if (field == 5 && strlen(name) > 0) {
            strncpy(scores[count].name, name, MAX_NAME-1);
            scores[count].name[MAX_NAME-1] = '\0';
            strncpy(scores[count].defi, defi, 49);
            scores[count].defi[49] = '\0';
            scores[count].steps = steps;
            scores[count].time_sec = time_sec;
            scores[count].score = score;
            count++;
        }
    }

    fclose(f);

    // Tri par score decroissant
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[i].score < scores[j].score) {
                ScoreEntry temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    
    printf("\n+==================================================+\n");
    printf("|              CLASSEMENT GENERAL                  |\n");
    printf("+==================================================+\n");
    printf("Pos Joueur        Defi         Etapes  Temps   Score\n");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%3d %-12s %-11s %4d   %6.2fs  %d\n",
               i+1,
               scores[i].name,
               scores[i].defi,
               scores[i].steps,
               scores[i].time_sec,
               scores[i].score);
    }

    if (count == 0) {
        printf("     Aucun score enregistre pour le moment.\n");
    }
    printf("\n");
}
