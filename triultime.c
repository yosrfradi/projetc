#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "triultime.h"
#include "defi.h"

void print_array(int arr[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("]\n");
}

int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1]) return 0;
    return 1;
}

void jouer_defi_tri_ultime(char nom_joueur[]) {
    int arr[SIZE];
    StatsTri stats = {0};

    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 50;

    printf("\n=== Defi TRI ULTIME ===\n");
    printf("Joueur : %s\n", nom_joueur);
    printf("Tableau initial : ");
    print_array(arr, SIZE);

    time_t debut = time(NULL);

    while (1) {
        int choix;
        printf("\n1. Comparer | 2. Echanger | 3. Afficher | 4. Terminer\nChoix : ");
        scanf("%d", &choix);

        if (choix == 1) {
            int i, j;
            printf("Comparer indices : ");
            scanf("%d %d", &i, &j);
            if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                stats.comparisons++;
                stats.score_brut += 1;
                if (arr[i]<arr[j]) printf("arr[%d]=%d < arr[%d]=%d\n", i, arr[i], j, arr[j]);
                else if (arr[i]>arr[j])  printf("arr[%d]=%d > arr[%d]=%d\n", i, arr[i], j, arr[j]);
                else printf("arr[%d]=%d = arr[%d]=%d\n", i, arr[i], j, arr[j]);
            } else printf("Invalide !\n");

        } else if (choix == 2) {
            int i, j;
            printf("Echanger indices : ");
            scanf("%d %d", &i, &j);
            if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
                stats.swaps++;
                stats.score_brut += 5;
                printf("Echange effectue !\n");
                print_array(arr, SIZE);
            } else printf("Invalide !\n");

        } else if (choix == 3) {
            print_array(arr, SIZE);

        } else if (choix == 4) {
            break;
        }
    }

    time_t fin = time(NULL);
    stats.temps = difftime(fin, debut);

    if (!is_sorted(arr, SIZE)) {
        printf("ECHEC : tableau non trie ! Score = 0\n");
        stats.score_final = 0;
        return;
    }

    double multi = (stats.temps < 20) ? 2.0 : (stats.temps < 40) ? 1.5 : (stats.temps < 60) ? 1.2 : 1.0;
    stats.score_final = (int)(stats.score_brut * multi);

    // Sauvegarder le score
    int steps = stats.comparisons + stats.swaps;
    sauvegarder_score(nom_joueur, NOM_TRIULTIME, steps, stats.temps, stats.score_final);

    printf("\nREUSSI ! Tableau trie !\n");
    printf("Comparaisons : %d | Echanges : %d | Temps : %.0fs\n", stats.comparisons, stats.swaps, stats.temps);
    printf("Score brut : %d x %.1f = SCORE FINAL : %d\n", stats.score_brut, multi, stats.score_final);
    
    printf("\nAppuyez sur Entree pour revenir au menu...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    getchar();
}