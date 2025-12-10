#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "labyrinth.h"
#include "comptebon.h"
#include "mastermind.h"
#include "triultime.h"
#include "defi.h"

void clear() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void attendre_touche() {
    printf("\nAppuyez sur Entree pour continuer...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main() {
    setlocale(LC_ALL, ""); 

    char nom[MAX_NAME] = {0};
    int choix;

    printf("====================================\n");
    printf("    ARENA DEFIS - PROGRAMMATION C\n");
    printf("====================================\n\n");
    printf("Entre ton prenom : ");
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\r\n")] = '\0';
    if (strlen(nom) == 0) strcpy(nom, "Joueur");

    do {
        clear();
        printf("+======================================+\n");
        printf("|          MENU - Bonjour %-12s|\n", nom);
        printf("+======================================+\n\n");
        printf("  1. Labyrinthe\n");
        printf("  2. Le Compte est bon\n");
        printf("  3. Mastermind\n");
        printf("  4. Tri Ultime\n");
        printf("  5. Classement general\n");
        printf("  6. Quitter\n\n");
        printf("Choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = 0;
        }
        while(getchar() != '\n');

        switch(choix) {
            case 1: {
                clear();
                Labyrinth lab;
                if (!loadLabyrinth("labyrinth.txt", &lab)) {
                    printf("ERREUR : fichier 'labyrinth.txt' introuvable !\n");
                    attendre_touche();
                    break;
                }

                printf("DEFI LABYRINTHE\n\n");
                displayLabyrinth(&lab);

                char path[2000];
                printf("\nSequence de mouvements (H/B/G/D) : ");
                clock_t debut = clock();
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\r\n")] = '\0';
                double temps = (clock() - debut) / (double)CLOCKS_PER_SEC;

                int steps;
                if (checkPlayerPath(&lab, path, &steps)) {
                    int score = calculateScore(steps, temps);
                        sauvegarder_score(nom, "labyrinthe", steps, temps, score);
                    printf("\nBRAVO %s ! Tu as reussi en %d pas (%.2f s)\n", nom, steps, temps);
                    printf("Score : %d points !\n", score);
                } else {
                    printf("\nEchec ! Chemin invalide.\n");
                }
                    attendre_touche();
                break;
            }

            case 2:
                clear();
                jouer_defi_compte_bon(nom);
                attendre_touche();
                break;

            case 3:
                clear();
                jouer_defi_mastermind(nom);
                attendre_touche();
                break;

            case 4:
                clear();
                jouer_defi_tri_ultime(nom);
                attendre_touche();
                break;

            case 5:
                clear();
                afficher_classement_global();
                attendre_touche();
                break;

            case 6:
                clear();
                printf("Merci d'avoir joue, %s ! A bientot !\n", nom);
                break;

            default:
                printf("Choix invalide !\n");
                attendre_touche();
        }
    } while (choix != 6);

    return 0;
}
