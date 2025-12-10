#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "labyrinth.h"

int loadLabyrinth(const char* filename, Labyrinth* lab) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;

    memset(lab, 0, sizeof(Labyrinth));
    lab->startRow = lab->startCol = lab->endRow = lab->endCol = -1;

    char line[MAX_COLS + 2];
    int r = 0;

    while (fgets(line, sizeof(line), f) && r < MAX_ROWS) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;
        strcpy(lab->grid[r], line);

        for (int c = 0; c < (int)strlen(line); c++) {
            if (line[c] == 'S') { lab->startRow = r; lab->startCol = c; }
            if (line[c] == 'E') { lab->endRow = r; lab->endCol = c; }
        }
        if ((int)strlen(line) > lab->cols) lab->cols = (int)strlen(line);
        r++;
    }
    lab->rows = r;
    fclose(f);
    return (lab->startRow != -1 && lab->endRow != -1);
}

void displayLabyrinth(const Labyrinth* lab) {
    for (int i = 0; i < lab->rows; i++) {
        printf("%s\n", lab->grid[i]);
    }
}

int isValidMove(const Labyrinth* lab, int row, int col) {
    if (row < 0 || row >= lab->rows || col < 0 || col >= lab->cols)
        return 0;
    char cell = lab->grid[row][col];
    return (cell != '#' && cell != '\0' && cell != ' '); // 'E' et '.' sont autorises !
}

// Verifie le chemin saisi par l'etudiant (ex: "DDDDBBBG")
int checkPlayerPath(const Labyrinth* lab, const char* path, int* steps) {
    int r = lab->startRow, c = lab->startCol;
    *steps = 0;

    for (int i = 0; path[i]; i++) {
        char move = path[i];
        int nr = r, nc = c;

        if (move == 'H' || move == 'h') nr--;      // Haut
        else if (move == 'B' || move == 'b') nr++; // Bas
        else if (move == 'G' || move == 'g') nc--; // Gauche
        else if (move == 'D' || move == 'd') nc++; // Droite
        else {
            printf("Mouvement invalide : %c (utilise H/B/G/D)\n", move);
            return 0;
        }

        if (!isValidMove(lab, nr, nc)) {
            printf("ERREUR : tu as fonce dans un mur ou hors limite !\n");
            return 0;
        }

        r = nr; c = nc;
        (*steps)++;

        if (r == lab->endRow && c == lab->endCol) {
            return 1; // Arrive !
        }
    }
    return 0; // Pas arrive
}

int calculateScore(int steps, double time_sec) {
    const int max_steps = 50;   // nombre de pas maximum raisonnable
    const double max_time = 60; // temps max considere en secondes

    double step_score = 1.0 - ((double)steps / max_steps);   // 0..1
    double time_score = 1.0 - (time_sec / max_time);         // 0..1

    if(step_score < 0) step_score = 0;
    if(time_score < 0) time_score = 0;

    double score = (step_score * 0.5 + time_score * 0.5) * 100; // ponderation 50/50

    return (int)score;
}



void saveScore(const Player* p, const char* maze_file) {
    FILE* f = fopen("scores.txt", "a");
    if (f) {
        fprintf(f, "%s;%s;%d;%.2f;%d\n", p->name, maze_file, p->steps, p->time_sec, p->score);
        fclose(f);
    }
}

void showRanking() {
    FILE* f = fopen("scores.txt", "r");
    if (!f) {
        printf("Aucun score enregistre.\n");
        return;
    }

    char name[MAX_NAME], maze[100];
    int steps, score;
    double time;

    printf("\n===== CLASSEMENT =====\n");
    printf("%-15s %-10s %-10s %-10s\n", "Nom", "Pas", "Temps(s)", "Score");

    while (fscanf(f, "%[^;];%[^;];%d;%lf;%d\n", name, maze, &steps, &time, &score) == 5) {
        printf("%-15s %-10d %-10.2f %-10d\n", name, steps, time, score);
    }

    fclose(f);
}

