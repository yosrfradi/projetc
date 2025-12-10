#ifndef TRI_ULTIME_H
#define TRI_ULTIME_H

#define SIZE 6


typedef struct {
    int comparisons;
    int swaps;
    int score_brut;
    double temps;
    int score_final;
} StatsTri;

void print_array(int arr[], int n);
int is_sorted(int arr[], int n);
void jouer_defi_tri_ultime(char nom_joueur[]);

#endif