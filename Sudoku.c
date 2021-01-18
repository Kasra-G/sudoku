#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>

#define PATH "puzzles/puzzle.txt"

const int size = 9;

int64_t currentTimeMillis() {
    struct timeval time;
    gettimeofday(&time, NULL);
    int64_t s1 = (int64_t)(time.tv_sec) * 1000;
    int64_t s2 = (time.tv_usec / 1000);
    return s1 + s2;
}

// return 0 for invalid, 1 for valid
int validateRow(int index, int** board) {
    int col = index % size;
    int row = index / size;
    int num = board[row][col];
    for (int i = 0; i < size; i++) {
        if (i != col && board[row][i] == num) {
            return 0;
        }
    }
    return 1;
}

// return 0 for invalid, 1 for valid
int validateCol(int index, int** board) {
    int col = index % size;
    int row = index / size;
    int num = board[row][col];
    for (int i = 0; i < size; i++) {
        if (i != row && board[i][col] == num) {
            return 0;
        }
    }
    return 1;
}

// return 0 for invalid, 1 for valid
int validateSquare(int index, int** board) {
    int col = index % size;
    int row = index / size;
    int num = board[row][col];
    const int root = 3;
    int srow = 1 + root * (row / root);
    int scol = 1 + root * (col / root);

    //iterate through the 3x3 square the number is in
    for (int i = srow - 1; i <= srow + 1; i++) {
        for (int j = scol - 1; j <= scol + 1; j++) {
            if (!(i == row && j == col) && board[i][j] == num) {
                return 0;
            }
        }
    }

    return 1;
}

// return 0 for invalid, 1 for valid
int isValidNum(int index, int** board) {
    return validateRow(index, board) &&
        validateCol(index, board) &&
        validateSquare(index, board);
}

int** test(int index, int** board) {

    if (index >= 81) {
        return board;
    } else if (board[index / size][index % size] > 0) {
        return test(index + 1, board);
    }
    //allocate ram for temp
    int** temp = (int**)(calloc(size, sizeof(int*)));
    for (int i = 0; i < size; i++) {
        temp[i] = (int*)(calloc(size, sizeof(int)));
    }

    //copy board to temp
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            temp[row][col] = board[row][col];
        }
    }

    //change temp

    //test the change to see if board is still valid
    for (int i = 1; i <= 9; i++) {
        temp[index / size][index % size] = i;
        if (isValidNum(index, temp)) {
            int** possible = test(index + 1, temp);
            if (possible != NULL) {
                return possible;
            }
        }
    }

    return NULL;
}

//print board
void printBoard(int** board) {
  if (board == NULL) {
        printf("Board is null\n");
        return;
    }
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            char str[3];
            sprintf(str, "%d", board[row][col]);
            if (str[0] == '0') {
                str[0] = ' ';
            }
            printf("[%s]", str);
        }
        printf("\n");
    }
    printf("\n");
}

//read contents of puzzle.txt file
void readFile(const char* path, int** board) {
    FILE* file = fopen(path, "r");
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            int num = fgetc(file) - '0';
            if (num < 0 || num > 9) {
                printf("File is corrupted\n");
                exit(1);
            }
            board[row][col] = num;
        }
    }
    fclose(file);
}

int file_exists (char* path) {
    struct stat buff;
    return (stat (path, &buff) == 0);
}

int main(int argc, char** args) {

    //allocate memory for sudoku board
    int** board = (int**)(calloc(size, sizeof(int*)));
    for (int i = 0; i < size; i++) {
        board[i] = (int*)(calloc(size, sizeof(int)));
    }

    const char* path;

    if (argc == 1) {
        printf("No file specified. Using default path puzzles/puzzle.txt\n");
        path = PATH;
    } else {
        if (file_exists(args[1])) {
            printf("Using file specified: %s\n", args[1]);
            path = args[1];
        } else {
            printf("File %s does not exist. Using default path puzzles/puzzle.txt instead\n", args[1]);
            path = PATH;
        }
    }

    int index;
    readFile(path, board);
    printBoard(board);
    
    int64_t beg = currentTimeMillis();
    printBoard(test(0, board));
    printf("Time elapsed: %d milliseconds\n", currentTimeMillis() - beg);
    return 0;
}
