#ifndef LABYRINTH_H
#define LABYRINTH_H

#define MAX_ROWS 50
#define MAX_COLS 80
#define MAX_NAME 30

typedef struct {
    char grid[MAX_ROWS][MAX_COLS + 1];
    int rows, cols;
    int startRow, startCol;
    int endRow, endCol;
} Labyrinth;

typedef struct {
    char name[MAX_NAME];
    int steps;
    double time_sec;
    int score;
} Player;

int  loadLabyrinth(const char* filename, Labyrinth* lab);
void displayLabyrinth(const Labyrinth* lab);
int  isValidMove(const Labyrinth* lab, int row, int col);
int  checkPlayerPath(const Labyrinth* lab, const char* path, int* steps);
int  calculateScore(int steps, double time_sec);
void saveScore(const Player* p, const char* maze_file);
void showRanking(void);

#endif
