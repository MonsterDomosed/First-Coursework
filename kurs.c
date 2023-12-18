#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define FILENAME "map.txt"
#define SCOREBOARD "scoreboard.txt"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"
#define PURPLE "\x1B[35m"

#ifdef _WIN32
#include <windows.h>
#endif

/* Function declaration */
void randomizeMap(char board[BOARD_SIZE][BOARD_SIZE]);
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void loadMapFromFile(char board[BOARD_SIZE][BOARD_SIZE]);
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void writeScoreboard(int hits, int misses);
int isGameOver(char targetBoard[BOARD_SIZE][BOARD_SIZE], char playerBoard[BOARD_SIZE][BOARD_SIZE]);
void clearScreen();
void game();

int main() {
    char targetBoard[BOARD_SIZE][BOARD_SIZE];
    int choice;

    while (TRUE){
        printf("\n1 - Start game\n");
        printf("\n2 - Randomize map\n");
        printf("\n3 - Exit application\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                clearScreen();
                game();
                break;
            case 2:
                randomizeMap(targetBoard);
                clearScreen();
                game();
                break;
            case 3:
                return 0;
                break;
        }
    }
}

void game(){
    char targetBoard[BOARD_SIZE][BOARD_SIZE];
    char playerBoard[BOARD_SIZE][BOARD_SIZE];
    int hits, misses, x, y;

    hits = 0;
    misses = 0;

    #ifdef _WIN32
    /* Enable ANSI escape codes on Windows */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif


    
printf(PURPLE " _______               __      __      __                      __        __           \n");
printf("/       \\             /  |    /  |    /  |                    /  |      /  |          \n");
printf("$$$$$$$  |  ______   _$$ |_  _$$ |_   $$ |  ______    _______ $$ |____  $$/   ______  \n");
printf("$$ |__$$ | /      \\ / $$   |/ $$   |  $$ | /      \\  /       |$$      \\ /  | /      \\ \n");
printf("$$    $$<  $$$$$$  |$$$$$$/ $$$$$$/   $$ |/$$$$$$  |/$$$$$$$/ $$$$$$$  |$$ |/$$$$$$  |\n");
printf("$$$$$$$  | /    $$ |  $$ | __ $$ | __ $$ |$$    $$ |$$      \\ $$ |  $$ |$$ |$$ |  $$ |\n");
printf("$$ |__$$ |/$$$$$$$ |  $$ |/  |$$ |/  |$$ |$$$$$$$$/  $$$$$$  |$$ |  $$ |$$ |$$ |__$$ |\n");
printf("$$    $$/ $$    $$ |  $$  $$/ $$  $$/ $$ |$$       |/     $$/ $$ |  $$ |$$ |$$    $$ |\n");
printf("$$$$$$$/   $$$$$$$/    $$$$/   $$$$/  $$/  $$$$$$$/ $$$$$$$/  $$/   $$/ $$/ $$$$$$$/  \n");
printf("                                                                            $$ |      \n");
printf("                                                                            $$ |      \n");
printf("                                                                            $$/       \n" RESET);



    /* Initialize player and target tables */
    initializeBoard(playerBoard);
    loadMapFromFile(targetBoard);

    while (!isGameOver(targetBoard, playerBoard)) {
        /* Display the current game state if the game is not over */
        printf(GREEN " __     __                _                         _ \n");
        printf(" \\ \\   / /               | |                       | |\n");
        printf("  \\ \\_/ /__  _   _ _ __  | |__   ___   __ _ _ __ __| |\n");
        printf("   \\   / _ \\| | | | '__| | '_ \\ / _ \\ / _` | '__/ _` |\n");
        printf("    | | (_) | |_| | |    | |_) | (_) | (_| | | | (_| |\n");
        printf("    |_|\\___/ \\__,_|_|    |_.__/ \\___/ \\__,_|_|  \\__,_|\n");
        printf("                                                      \n" RESET);

        printBoard(playerBoard);

        /* Player's guess*/
        printf("\nEnter coordinates (row(x) and column(y)): ");
        int inputResult = scanf("%d %d", &x, &y);
        clearScreen();

        /* Convert 1-based user input to 0-based array indices */
        x--;
        y--;

        /* Check for correct input */
        if (inputResult != 2 || x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
            printf(RED "\nInvalid input. Please enter coordinates within the board limits.\n" RESET);

            continue;  /* Skip the rest of the loop and request coordinates again*/
        }
        /* Consume the newline character remaining in the buffer */
        getchar();

        /* Checking if player got it right or not */
        if (playerBoard[x][y] == 'X' || playerBoard[x][y] == 'O') {
            printf(RED "You've already guessed that coordinate. Try again.\n" RESET);
            continue;  /* Skip the rest of the loop and request coordinates again */
        }

        if (targetBoard[x][y] == 'S') {
            printf(GREEN "\nHit!\n" RESET);
            playerBoard[x][y] = 'X';  /* Mark the hit on the player's board*/
            hits++;
        } else {
            printf(RED "\nMiss!\n" RESET);
            playerBoard[x][y] = 'O';  /* Mark a miss on the player's board*/
            misses++;
        }
    }

    /* Output the final results */
    printf(PURPLE "\nGame Over! All ships sunk.\n" RESET);
    printf("\nPlayer Board:\n");
    printBoard(playerBoard);
    writeScoreboard(hits, misses);
    printf("\nScoreboard:\nHits: %s%d%s\nMisses: %s%d%s\nTotal tries: %s%d%s\nScoreboard written in scoreboard.txt", GREEN, hits, RESET, RED, misses, RESET, PURPLE, hits + misses, RESET);
    


    printf("\nPress enter to open menu...");
    getchar();  /* Wait for any key to be pressed before exiting*/
    clearScreen();
}

/* Function for initializing a board with empty spaces*/
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }
}


/* Function for loading a map from a file*/
void loadMapFromFile(char board[BOARD_SIZE][BOARD_SIZE]) {
    FILE *file = fopen(FILENAME, "r");

    /* If there's no map.txt then create and randomize it */
    if (file == NULL) {
        randomizeMap(board);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fscanf(file, " %c", &board[i][j]);
        }
    }

    fclose(file);
}


/* Function for printing a board*/
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("   ");
    for (int i = 1; i <= BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d|", i+1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

/* Function to check the end of the game (all ships sunk)*/
int isGameOver(char targetBoard[BOARD_SIZE][BOARD_SIZE], char playerBoard[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (targetBoard[i][j] == 'S' && playerBoard[i][j] != 'X') {
                return 0; 
            }
        }
    }
    return 1;
}

/* Function to randomize map in the map.txt file */
void randomizeMap(char board[BOARD_SIZE][BOARD_SIZE]) {
    srand(time(NULL)); // Initialize random number generator

    FILE *file = fopen(FILENAME, "w");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Assign 'S' with a 20% probability, '-' otherwise
            board[i][j] = (rand() % 100 < 20) ? 'S' : '-';
            fprintf(file, "%c ", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

/* Function to write scoreboard into scoreboard.txt file at the end of the game*/
void writeScoreboard(int hits, int misses) {

    FILE *file = fopen(SCOREBOARD, "w");
        fprintf(file, "Misses: %d\n", misses);
        fprintf(file, "Hits: %d\n", hits);
        fprintf(file, "Total tries: %d", hits + misses);
        fprintf(file, "\n");
    fclose(file);
}

/* Function to clear screen after each action depending on OS  */
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}