// comptebon.c - VERSION PARFAITE ET CORRIGEE (ENSI Ready)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "comptebon.h"
#include "defi.h"

// Variables globales (static = privees au fichier)
static int nums[N];
static int used[N];
static int target;
static int current_result;
static int score = 0;
static int step_count = 0;
static char history[10][100];
static int history_idx = 0;

void generate_numbers() {
    int petits[] = {1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
    int grands[] = {25,50,75,100};

    for (int i = 0; i < 4; i++)
        nums[i] = petits[rand() % 18];
    nums[4] = grands[rand() % 4];
    nums[5] = petits[rand() % 18];
}

int generate_target() {
    // Simple target generation to avoid infinite loops
    // Generate a target between 100 and 500 that's achievable
    int base = nums[rand() % N];
    int multiplier = nums[rand() % N];
    int target = base * multiplier;
    
    // Ensure it's in a reasonable range
    if (target < 50) target += 100;
    if (target > 999) target = target % 500 + 100;
    
    return target;
}

int find_available(int val) {
    for (int i = 0; i < N; i++)
        if (!used[i] && nums[i] == val) return i;
    return -1;
}

void print_available() {
    printf("Disponibles : ");
    for (int i = 0; i < N; i++)
        if (!used[i]) printf("%d ", nums[i]);
    printf("\n");
}

int apply_op(int a, int b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/' && b != 0 && a % b == 0) return a / b;
    return -99999;
}

void jouer_defi_compte_bon(char nom_joueur[]) {
    activer_utf8();
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    generate_numbers();
    target = generate_target();

    // Reinitialisation
    for (int i = 0; i < N; i++) used[i] = 0;
    score = 0; step_count = 0; history_idx = 0;
    int first = 1;

    printf("\n\n");
    printf("+============================================+\n");
    printf("|       DEFI : LE COMPTE EST BON             |\n");
    printf("+============================================+\n");
    printf("Joueur : %s\n", nom_joueur);
    printf("Nombres : ");
    for (int i = 0; i < N; i++) printf("%d ", nums[i]);
    printf("\nCIBLE -> %d\n\n", target);
    
    // Vider le tampon et attendre l'utilisateur
    char dummy[100];
    printf("Appuie sur Entree pour commencer...");
    fflush(stdout);
    fgets(dummy, sizeof(dummy), stdin);
    printf("\n");

    time_t debut = time(NULL);

    while (step_count < 5) {
        print_available();
        if (first) printf("Resultat actuel : (premier calcul)\n");
        else printf("Resultat actuel : %d\n", current_result);

        int v1, v2 = 0;
        char op;
        int old_result = current_result;  
        char input[100];

        if (first) {
            
            while (1) {
                printf("\nEtape %d : Choisis deux nombres (ex: 5 10) -> ", step_count + 1);
                fflush(stdout);
                
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("[X] Erreur de lecture !\n");
                    continue;
                }
                
                if (sscanf(input, "%d %d", &v1, &v2) == 2) {
                    break;  
                }
                
                printf("[X] Erreur ! Il faut entrer DEUX nombres separes par un espace.\n");
                printf("   Exemple: 5 10\n");
            }
        } else {

            while (1) {
                printf("\nEtape %d : Choisis un nombre disponible -> ", step_count + 1);
                fflush(stdout);
                
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("[X] Erreur de lecture !\n");
                    continue;
                }
                
                if (sscanf(input, "%d", &v1) == 1) {
                    break;  
                }
                
                printf("[X] Erreur ! Il faut entrer UN nombre.\n");
            }
        }

        // lire les opérations avec fgets
        while (1) {
            printf("Operation (+ - * /) : ");
            fflush(stdout);
            
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("[X] Erreur de lecture !\n");
                continue;
            }
            
            if (sscanf(input, " %c", &op) == 1 && 
                (op == '+' || op == '-' || op == '*' || op == '/')) {
                break;  // Valid operator
            }
            
            printf("[X] Operateur invalide ! Utilise: + - * /\n");
        }

        // Logique pour les operations
        int operand1, operand2;
        if (first) {
            // First operation: use two numbers from the list
            int idx1 = find_available(v1);
            int idx2 = find_available(v2);
            if (idx1 == -1 || idx2 == -1 || idx1 == idx2) {
                printf("[X] Nombres invalides ou deja utilises !\n");
                printf("   Nombres disponibles : ");
                print_available();
                continue;
            }
            used[idx1] = used[idx2] = 1;
            operand1 = v1;
            operand2 = v2;
        } else {
            // After first: use current_result and one new number
            int idx1 = find_available(v1);
            if (idx1 == -1) {
                printf("[X] Le nombre %d n'est plus disponible !\n", v1);
                printf("   Nombres disponibles : ");
                print_available();
                continue;
            }
            used[idx1] = 1;
            operand1 = current_result;
            operand2 = v1;
        }

        int r = apply_op(operand1, operand2, op);
        if (r == -99999) {
            printf("[X] Operation impossible : %d %c %d\n", operand1, op, operand2);
            if (op == '/') {
                printf("   (La division doit etre exacte, sans reste)\n");
            }
            continue;
        }

        current_result = r;

        // Historique correct
        if (first) {
            sprintf(history[history_idx++], "%d %c %d = %d", operand1, op, operand2, r);
        } else {
            sprintf(history[history_idx++], "%d %c %d = %d", old_result, op, operand2, r);
        }

        score += 15;
        step_count++;
        first = 0;

        printf("-> Nouveau resultat = %d\n", current_result);

        if (current_result == target) {
            printf("\nCIBLE ATTEINTE !!!\n");
            score += 150;
            break;
        }
    }

    double temps = difftime(time(NULL), debut);
    double multi = (temps < 30) ? 2.0 : (temps < 60) ? 1.5 : (temps < 90) ? 1.2 : 1.0;
    int score_final = (int)(score * multi);

    printf("\n");
    printf("============================================\n");
    printf("           HISTORIQUE DES OPERATIONS\n");
    printf("============================================\n");
    for (int i = 0; i < history_idx; i++) {
        printf("   %s\n", history[i]);
    }

    printf("\nTemps : %.1f s | Multiplicateur : x%.1f\n", temps, multi);
    printf("Score brut : %d -> SCORE FINAL : %d\n", score, score_final);
    printf("============================================\n");

    // Sauvegarder le score
    sauvegarder_score(nom_joueur, NOM_COMPTEBON, step_count, temps, score_final);

    printf("\nAppuyez sur Entree pour revenir au menu...");
    char buffer_wait[10];
    fgets(buffer_wait, sizeof(buffer_wait), stdin);
}