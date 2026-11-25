/*
Authors: Andres Batiz, Maksymilian Sosnowski, Atef Abdulhafedh
Purpose: Final Project Conways Game of Life
Date: 11/24/2026
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;

const int COL = 30, ROW = 30;
const int MAX_GAMES = 100;
const int GAME_MAX = 30;

// Function prototypes
void displayMenu();
void displayBoard(int arr[][COL]);
void loadBoard(int arr[][COL]);
void playOneYear(int arr[][COL], int newArr[][COL]);

void playGame(int arr[][COL], int newArr[][COL],
    int& gamesPlayed, int yearCount[], int aliveCount[],
    int bestStartBoard[][COL], int& bestAlive);

void addCells(int arr[][COL]);

void randomBoard(int arr[][COL], int newArr[][COL],
    int& gamesPlayed, int yearCount[], int aliveCount[],
    int bestStartBoard[][COL], int& bestAlive);

void bestBoard(int bestStartBoard[][COL], int bestAlive);

void displayResults(int gamesPlayed, int yearCount[], int aliveCount[]);

int countLivingCells(int arr[][COL]);
int countDeadCells(int arr[][COL]);
int getMenuChoice();


// Displays the menu
void displayMenu() {
    cout << "Hello! Welcome to the game of life in CMPT 1109. What would you like to do?\n"
        << "To display the initial state for the game of life press 1\n"
        << "To load a new initial state from a CSV file into memory press 2\n"
        << "To play the game of life with the current initial state press 3\n"
        << "To seed more cells into the initial state, press 4\n"
        << "To report summary statistics on all games played press 5\n"
        << "To randomly play the game multiple times press 6\n"
        << "To output the best initial board press 7\n"
        << "To quit the program press 8\n";
}


// Displays the board
void displayBoard(int arr[][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}


// Loads a board from a CSV file
void loadBoard(int arr[][COL]) {
    ifstream inputFile;
    string fileName;

    cout << "Enter name of CSV file: ";
    cin >> fileName;

    inputFile.open(fileName);

    while (!inputFile) {
        cout << "Error opening file. Please reenter the file.\n";
        inputFile.clear();

        cout << "Enter name of CSV file: ";
        cin >> fileName;

        inputFile.open(fileName);
    }

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            inputFile >> arr[i][j];

            if (!inputFile || (arr[i][j] != 0 && arr[i][j] != 1)) {
                cout << "Invalid board. Only 0 and 1 are allowed.\n";
                return;
            }

            if (j < COL - 1) {
                inputFile.ignore();
            }

            cout << arr[i][j] << ' ';
        }
        cout << endl;
    }

    inputFile.close();
    cout << "Board successfully loaded!!\n";
}


// Calculates the next generation
void playOneYear(int arr[][COL], int newArr[][COL]) {
    const int ALIVE_MIN = 2;
    const int ALIVE_AND_REPRODUCE = 3;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int aliveCounter = 0;

            for (int l = -1; l <= 1; l++) {
                for (int m = -1; m <= 1; m++) {
                    if (i + l < 0 || i + l >= ROW)
                        continue;

                    if (j + m < 0 || j + m >= COL)
                        continue;

                    if (l == 0 && m == 0)
                        continue;

                    if (arr[i + l][j + m] == 1) {
                        aliveCounter++;
                    }
                }
            }

            if (arr[i][j] == 1) {
                if (aliveCounter == ALIVE_MIN ||
                    aliveCounter == ALIVE_AND_REPRODUCE) {
                    newArr[i][j] = 1;
                }
                else {
                    newArr[i][j] = 0;
                }
            }
            else {
                if (aliveCounter == ALIVE_AND_REPRODUCE) {
                    newArr[i][j] = 1;
                }
                else {
                    newArr[i][j] = 0;
                }
            }
        }
    }
}


// Plays one game and saves its results
void playGame(int arr[][COL], int newArr[][COL],
    int& gamesPlayed, int yearCount[], int aliveCount[],
    int bestStartBoard[][COL], int& bestAlive) {

    if (gamesPlayed >= MAX_GAMES) {
        cout << "Maximum number of games reached.\n";
        return;
    }

    // Save the original starting board
    int initialBoard[ROW][COL];
    int currentBoard[ROW][COL];

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            initialBoard[i][j] = arr[i][j];
            currentBoard[i][j] = arr[i][j];
        }
    }

    int yearsPlayed = 0;

    for (int year = 1; year <= GAME_MAX; year++) {
        playOneYear(currentBoard, newArr);

        // Check whether the board has stopped changing
        bool unchanged = true;

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                if (currentBoard[i][j] != newArr[i][j]) {
                    unchanged = false;
                }
            }
        }

        // Copy the new generation into the current board
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                currentBoard[i][j] = newArr[i][j];
            }
        }

        yearsPlayed++;

        displayBoard(currentBoard);
        cout << endl << endl;

        // Stop if all cells have died
        if (countLivingCells(currentBoard) == 0) {
            cout << "All cells have died. Simulation stopped.\n";
            break;
        }

        // Stop if the board has stopped changing
        if (unchanged) {
            cout << "The board has stopped changing. Simulation stopped.\n";
            break;
        }
    }

    // Save this game's results
    yearCount[gamesPlayed] = yearsPlayed;
    aliveCount[gamesPlayed] = countLivingCells(currentBoard);

    // Check whether this is the best game
    int aliveAtEnd = aliveCount[gamesPlayed];

    if (aliveAtEnd > bestAlive) {
        bestAlive = aliveAtEnd;

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                bestStartBoard[i][j] = initialBoard[i][j];
            }
        }
    }

    gamesPlayed++;
}


// Adds a specified number of random living cells
void addCells(int arr[][COL]) {
    string input;
    bool isNumber = false;

    while (!isNumber) {
        cout << "How many cells would you like to add?\n";
        cin >> input;

        isNumber = !input.empty();

        for (int i = 0; i < input.length(); i++) {
            if (!isdigit(static_cast<unsigned char>(input[i]))) {
                isNumber = false;
                break;
            }
        }

        if (isNumber && stoi(input) <= 0) {
            isNumber = false;
            cout << "Please enter a positive number!\n";
        }

        if (isNumber && stoi(input) > countDeadCells(arr)) {
            isNumber = false;
            cout << "There are not enough empty cells. Enter a smaller number.\n";
        }

        if (!isNumber) {
            cout << "Invalid answer!\n";
        }
    }

    int numberOfCells = stoi(input);
    int cellsAdded = 0;

    cout << "Adding " << numberOfCells << " cells.\n";

    while (cellsAdded < numberOfCells) {
        int randomRow = rand() % ROW;
        int randomCol = rand() % COL;

        if (arr[randomRow][randomCol] == 0) {
            arr[randomRow][randomCol] = 1;
            cellsAdded++;
        }
    }

    displayBoard(arr);
}


// Runs multiple games using randomly generated boards
void randomBoard(int arr[][COL], int newArr[][COL],
    int& gamesPlayed, int yearCount[], int aliveCount[],
    int bestStartBoard[][COL], int& bestAlive) {

    int gameCount;
    double probability;

    if (gamesPlayed >= MAX_GAMES) {
        cout << "Maximum number of games reached.\n";
        return;
    }

    // Ask how many games to play
    cout << "\nHow many times do you want to play the game? ";
    cin >> gameCount;

    while (cin.fail() || gameCount <= 0 ||
        gamesPlayed + gameCount > MAX_GAMES) {

        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid input. Enter a positive number (max "
            << MAX_GAMES - gamesPlayed << " games remaining): ";

        cin >> gameCount;
    }

    // Ask for the probability of a cell being alive
    cout << "What probability for any given cell is alive (0.0 to 1.0)? ";
    cin >> probability;

    while (cin.fail() || probability < 0.0 || probability > 1.0) {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid probability. Enter a decimal between 0.0 and 1.0: ";
        cin >> probability;
    }

    // Generate and play each random game
    for (int game = 0; game < gameCount; game++) {
        cout << "\n--- Random Game " << game + 1 << " ---\n";

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                double randomNum =
                    static_cast<double>(rand()) / (RAND_MAX + 1.0);

                if (randomNum < probability) {
                    arr[i][j] = 1;
                }
                else {
                    arr[i][j] = 0;
                }
            }
        }

        cout << "Year 0 (Initial Board):\n";
        displayBoard(arr);
        cout << endl << endl;

        playGame(arr, newArr, gamesPlayed, yearCount, aliveCount,
            bestStartBoard, bestAlive);
    }
}


// Displays the best initial board
void bestBoard(int bestStartBoard[][COL], int bestAlive) {
    if (bestAlive == -1) {
        cout << "No games have been played yet.\n";
        return;
    }

    cout << "The best initial board ended with "
        << bestAlive << " living cells:\n";

    displayBoard(bestStartBoard);

}


// Displays the statistics for all games
void displayResults(int gamesPlayed, int yearCount[], int aliveCount[]) {
    if (gamesPlayed == 0) {
        cout << "No games have been played yet.\n";
        return;
    }

    for (int i = 0; i < gamesPlayed; i++) {
        cout << "\nGame " << i + 1 << endl;
        cout << "Living cells: " << aliveCount[i] << endl;
        cout << "Dead cells: " << (ROW * COL) - aliveCount[i] << endl;
        cout << "Years played: " << yearCount[i] << endl;
    }
}


// Counts the number of living cells
int countLivingCells(int arr[][COL]) {
    int total = 0;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (arr[i][j] == 1) {
                total++;
            }
        }
    }

    return total;
}


// Counts the number of dead cells
int countDeadCells(int arr[][COL]) {
    int total = 0;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (arr[i][j] == 0) {
                total++;
            }
        }
    }

    return total;
}


// Gets and validates the menu choice
int getMenuChoice() {
    int choice;

    cout << "\nEnter your choice: ";
    cin >> choice;

    while (cin.fail() || choice < 1 || choice > 8) {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid choice. Enter a number between 1 and 8: ";
        cin >> choice;
    }

    return choice;
}


// Main function
int main() {
    int board[ROW][COL] = { 0 };
    int newBoard[ROW][COL] = { 0 };

    int gamesPlayed = 0;
    int yearCount[MAX_GAMES] = { 0 };
    int aliveCount[MAX_GAMES] = { 0 };

    int bestStartBoard[ROW][COL] = { 0 };
    int bestAlive = -1;

    srand(static_cast<unsigned int>(time(0)));

    int status;

    do {
        displayMenu();
        status = getMenuChoice();

        switch (status) {
        case 1:
            displayBoard(board);
            break;

        case 2:
            loadBoard(board);
            break;

        case 3:
            playGame(board, newBoard, gamesPlayed, yearCount,
                aliveCount, bestStartBoard, bestAlive);
            break;

        case 4:
            addCells(board);
            break;

        case 5:
            displayResults(gamesPlayed, yearCount, aliveCount);
            break;

        case 6:
            randomBoard(board, newBoard, gamesPlayed, yearCount,
                aliveCount, bestStartBoard, bestAlive);
            break;

        case 7:
            bestBoard(bestStartBoard, bestAlive);
            break;

        case 8:
            cout << "Thanks for playing\n";
            break;
        }
    } while (status != 8);

    return 0;
}
