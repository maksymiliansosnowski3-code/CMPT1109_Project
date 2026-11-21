#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;
const int COL = 30, ROW = 30;

void displayMenu();
void displayBoard(int arr[][COL]);
void loadBoard(int arr[][COL]);
void playOneYear(int arr[][COL], int newArr[][COL]);
void playGame(int arr[][COL], int newArr[][COL]);
void addCells(int arr[][COL], int newArr[][COL]);
void countCells(int arr[][COL]);
int countLivingCells(int arr[][COL]);
int countDeadCells(int arr[][COL]);
void randomBoard(int arr[][COL]);
void bestBoard(int arr[][COL], int newArr[][COL]);
void displayResults(int arr[][COL]);

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

void displayBoard(int arr[][COL]) {
    for (int i = 0; i < ROW; i++) {

        for (int j = 0; j < COL; j++) {

            cout << arr[i][j] << " ";

        }
        cout << endl;
    }

}
void loadBoard(int arr[][COL]) {

    ifstream inputFile;

    string fileName;

    cout << "Enter name of CSV file: ";
    cin >> fileName;

    inputFile.open(fileName);

    if (!inputFile) {
        cout << "Error opening file, Please Reenter the file\n";
        while (!inputFile) {
            cout << "Enter name of CSV file: ";
            cin >> fileName;

            inputFile.open(fileName);
        }
    }

    for (int i = 0; i < COL; i++) {

        for (int j = 0; j < ROW; j++) {

            inputFile >> arr[i][j];
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

void playOneYear(int arr[][COL], int newArr[][COL]) {

    for (int i = 0; i < COL; i++) {

        for (int j = 0; j < ROW; j++) {
            int aliveCounter = 0;
            int deadCounter = 0;
            if (arr[i][j] == 1) {
                for (int l = -1;l <= 1;l++) {
                    for (int m = -1;m <= 1;m++) {
                        if (i + l < 0 || i + l >= COL)
                            continue;
                        if (j + m < 0 || j + m >= COL)
                            continue;
                        if (l == 0 && m == 0)
                            continue;


                        if (arr[i + l][j + m] == 1) {

                            aliveCounter++;

                        }
                        else if (arr[i + l][j + m] == 0) {

                            deadCounter++;

                        }
                    }
                }
                //Rules
                cout << "our alive is " << aliveCounter << " our dead is " << deadCounter << endl;
            }
        }
    }
}

void playGame(int arr[][COL], int newArr[][COL]) {

    const int GAME_MAX = 30;
    int gameCount = 0;
    for (int gameCount = 1; gameCount < GAME_MAX; gameCount++) {
        playOneYear(arr, newArr);

    }




}

void addCells(int arr[][COL], int newArr[][COL]) {
    int numberOfCells;

    cout << "How many cells would you like to add? ";
    cin >> numberOfCells;

    while (cin.fail() || numberOfCells <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid number. Please enter a positive number: ";
        cin >> numberOfCells;
    }

    // Check how many empty positions are availible
    int deadCells = countDeadCells(arr);

    if (numberOfCells > deadCells) {
        cout << "There are not enough empty positions on the board.\n";
        return;
    }

    int added = 0;

    // Randomly find empty positions and make thrm alive
    while (added < numberOfCells) {
        int row = rand() % ROW;
        int col = rand() % COL;

        if (arr[row][col] == 0) {
            arr[row][col] = 1;
            added++;
        }
    }

    cout << numberOfCells << " cells added successfully!\n";
}

    int numberOfCells = stoi(input);
    cout << "Adding " << numberOfCells << " cells.\n";

    for (int i = 0; i < COL; i++) {

        for (int j = 0; j < ROW; j++) {

        }
     }
}   

void countCells(int arr[][COL]) {
    int living = countLivingCells(arr);
    int dead = countDeadCells(arr);

    cout << "Living cells: " << living << endl;
    cout << "Dead cells: " << dead << endl;
}

void randomBoard(int arr[][COL]) {

}

void bestBoard(int arr[][COL], int newArr[][COL]) {

}

void displayResults(int arr[][COL]) {

}

// Counts the number of living cells
int countLivingCells(int arr[][COL])
{
    int total = 0;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (arr[i][j] == 1)
            {
                total++;
            }
        }
    }

    return total;
}

// Counts the number of dead cells
int countDeadCells(int arr[][COL])
{
    int total = 0;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (arr[i][j] == 0)
            {
                total++;
            }
        }
    }

    return total;
}

// Validate user menu choice input 
int getMenuChoice()
{
    int choice;

    cout << "\nEnter your choice: ";
    cin >> choice;

    // Repeat if the user enters invalid input
    while (cin.fail() || choice < 1 || choice > 8)
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid choice. Enter a number between 1 and 8: ";
        cin >> choice;
    }

    return choice;
}

int main() {

    int board[ROW][COL] = { 0 };
    int newBoard[ROW][COL] = { 0 };

    srand(time(0));

    int status;
    do {
        displayMenu();

        status = getMenuChoice();

        switch (status) {
        case 1: displayBoard(board);
            break;

        case 2: loadBoard(board);
            break;

        case 3: playGame(board, newBoard);
            break;

        case 4: addCells(board, newBoard);
            break;

        case 5: displayResults(board);
            break;

        case 6: randomBoard(board);
            break;

        case 7:  bestBoard(board, newBoard);
            break;

        case 8:cout << "Thanks for playing\n";
            break;

        default: cout << "Invalid selection. Please enter a number between 1 and 8.\n";
            break;
        }

    } while (status != 8);

    return 0;
}
