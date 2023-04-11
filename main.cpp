#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "conio2.h"
#include "Board.h"
#include "StringVector.h"

#define DISPLAY 1 // 1 - legend left, board on the right, 0 - legend right, board on the left
#define POSITION 48 // the number of chars in longest legend line plus the space between board and legend
#define SPECIAL_CHAR 0 //for handling key event from getch()
#define ENTER '\r'
#define NEWGAME 'n'
#define PLACESTONE 'i'
#define SAVE 's'
#define LOAD 'l'
#define FINISH 'f'
#define EXIT 'q'
#define ESC 27
#define UP_KEY 0x48
#define DOWN_KEY 0x50
#define LEFT_KEY 0x4b
#define RIGHT_KEY 0x4d


int countDigit(int n) {
    if (n / 10 == 0)
        return 1;
    return 1 + countDigit(n / 10);
} //returns the number of digits in an integer number


void convertToText(int number, char **txt) {
    int size = countDigit(number);
    *txt = new char[size];
    sprintf(*txt, "%d", number);
} //changes the number to a char table representation


/*Function asks for user input to determine the sizes of the board, from 3 predefined options and a custom one*/
void chooseBoardSize(int *sizeX, int *sizeY, int defaultWindowSizeX, int defaultWindowSizeY) {
    int windowSizeX, windowSizeY;
    //changing terminal buffer size to window size
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(Handle, &csbiInfo);
    windowSizeX = defaultWindowSizeX - POSITION - 2;
    windowSizeY = defaultWindowSizeY;
    _COORD coord;
    coord.X = defaultWindowSizeX;
    coord.Y = defaultWindowSizeY + 1;
    Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(Handle, coord);

    //Displaying the options
    gotoxy(1, 1);
    cputs("Welcome to the game of GO!");
    gotoxy(1, 2);
    cputs("You have the following options for the board size");
    gotoxy(1, 3);
    cputs("1. A board of size 9x9");
    gotoxy(1, 4);
    cputs("2. A board of size 13x13");
    gotoxy(1, 5);
    cputs("3. A board of size 19x19");
    gotoxy(1, 6);
    cputs("4. A board of a custom size");
    gotoxy(1, 7);
    cputs("Enter your choice (a number 1-4 inclusive)");

    //choosing board size
    int input;
    do {
        input = getche();
        if (input == '1') {
            *sizeX = 9;
            *sizeY = 9;
            break;
        } else if (input == '2') {
            *sizeX = 13;
            *sizeY = 13;
            break;
        } else if (input == '3') {
            *sizeX = 19;
            *sizeY = 19;
            break;
        } else if (input == '4') {
            clrscr();
            gotoxy(1, 1);
            cputs("Please provide the horizontal size of the board: ");
            long value = 0;
            input = getche();
            while (isdigit(input)) {
                value = value * 10 + input - '0';
                input = getche();
            }
            *sizeX = value;
            gotoxy(1, 2);
            cputs("Please provide the vertical size of the board: ");
            value = 0;
            input = getche();
            while (isdigit(input)) {
                value = value * 10 + input - '0';
                input = getche();
            }
            ungetch(input);
            *sizeY = value;
            if (*sizeY >= windowSizeY || *sizeX >= windowSizeX) { //changing terminal buffer size so scrolling becomes
                _wscroll = 1;                                     //apparent
                if (*sizeX >= windowSizeX)
                    coord.X = windowSizeX + *sizeX;
                if (*sizeY >= windowSizeY)
                    coord.Y = windowSizeY + *sizeY;
                SetConsoleScreenBufferSize(Handle, coord);
            }
            break;
        } else {
            delline();
            gotoxy(1, 7);
            cputs("Please enter a number from 1-4 inclusive");
        }
    } while (true);
    clrscr();
}


/*Function puts the legend of the game, as well as the board's representation to the console*/
void printGame(Board *board, int legendCoord, int boardCoord, int boardCursorX, int boardCursorY,
               int blackScore, int whiteScore) {
    char *boardCursorTextX, *boardCursorTextY, *blackScoreText, *whiteScoreText;
    convertToText(boardCursorX, &boardCursorTextX);
    convertToText(boardCursorY, &boardCursorTextY);
    convertToText(blackScore, &blackScoreText);
    convertToText(whiteScore, &whiteScoreText);

    //print legend
    gotoxy(legendCoord, 1);
    cputs("arrows = moving cursor over board");
    gotoxy(legendCoord, 2);
    cputs("q      = quit the program");
    gotoxy(legendCoord, 3);
    cputs("n      = start a new game");
    gotoxy(legendCoord, 4);
    cputs("enter  = confirm choice and end player's turn");
    gotoxy(legendCoord, 5);
    cputs("esc    = cancel current action");
    gotoxy(legendCoord, 6);
    cputs("i      = place a stone on board");
    gotoxy(legendCoord, 7);
    cputs("s      = save the game state");
    gotoxy(legendCoord, 8);
    cputs("l      = load the game state");
    gotoxy(legendCoord, 9);
    cputs("f      = finish the game");
    gotoxy(legendCoord, 10);
    cputs("Functionalities: 2.2abcde, 2.3fghijkl, 2.4mn");

    //print score:
    gotoxy(legendCoord, 11);
    const char *text = "Score: Black's = ";
    cputs(text);
    int posOfText = legendCoord + strlen(text);
    gotoxy(posOfText, 11);
    cputs(blackScoreText);
    posOfText += strlen(blackScoreText);
    text = " | White's = ";
    gotoxy(posOfText, 11);
    cputs(text);
    posOfText += strlen(text);
    gotoxy(posOfText, 11);
    cputs(whiteScoreText);
    posOfText += strlen(whiteScoreText);
    gotoxy(posOfText, 11);
    cputs(".5");

    //cursor key
    gotoxy(legendCoord, 12);
    text = "Cursor at: (";
    cputs(text);
    posOfText = legendCoord + strlen(text);
    gotoxy(posOfText, 12);
    cputs(boardCursorTextX);
    posOfText += strlen(boardCursorTextX);
    gotoxy(posOfText, 12);
    text = ":";
    cputs(text);
    posOfText += strlen(text);
    gotoxy(posOfText, 12);
    cputs(boardCursorTextY);
    posOfText += strlen(boardCursorTextY);
    gotoxy(posOfText, 12);
    cputs(")");

    //board representation
    for (int i = 0; i < board->getSizeY(); i++) {
        char* temp = new char[(board->getSizeX() + 1)];
        temp[board->getSizeX()] = '\0';
        for (int j = 0; j < board->getSizeX(); j++)
            if (board->getArray()[j][i]->getAtariMark() == 0)
                temp[j] = board->getArray()[j][i]->getState();
            else {
                if (board->getArray()[j][i]->getState() == Black)
                    temp[j] = AtariBlack;
                else
                    temp[j] = AtariWhite;
            }
        gotoxy(boardCoord, (i + 1));
        cputs(temp);
        delete[] temp;
    }
    delete[] boardCursorTextX;
    delete[] boardCursorTextY;
    delete[] whiteScoreText;
    delete[] blackScoreText;
}


/*handles the start of a Game*/
void startGame(Board **board, const int defaultWindowSizeX, const int defaultWindowSizeY, int *startOfGame,
               int *legendCoord, int *boardCoord, int *cursorCoordX, int *cursorCoordY, int *boardCursorX,
               int *boardCursorY) {
    _setcursortype(_NOCURSOR);
    _wscroll = 0;
    int sizeX, sizeY;
    chooseBoardSize(&sizeX, &sizeY, defaultWindowSizeX, defaultWindowSizeY);
    *startOfGame = 0;
    *board = new Board(sizeX + 2, sizeY + 2); //For the sake of borders board's sizes are incremented by 2
    *legendCoord = DISPLAY == 1 ? 1 : sizeX + 4;
    *boardCoord = DISPLAY == 1 ? POSITION : 1;
    *cursorCoordX = *boardCoord + 1;
    *cursorCoordY = 2;
    *boardCursorX = *cursorCoordX - POSITION;
    *boardCursorY = *cursorCoordY - 1;
    _setcursortype(_NORMALCURSOR);
}

/*Handles the cursor movement when arrow keys are pressed*/
void handleKeyEvents(Board *board, int *cursorCoordX, int *cursorCoordY, int *boardCursorX, int *boardCursorY,
                     int boardCoord) {
    char pressedKey = getch();
    if (pressedKey == UP_KEY) {
        if (*cursorCoordY > 2) {
            *cursorCoordY = *cursorCoordY - 1;
            *boardCursorY = *boardCursorY - 1;
        }
    } else if (pressedKey == DOWN_KEY) {
        if (*cursorCoordY < board->getSizeY() - 1) {
            *cursorCoordY = *cursorCoordY + 1;
            *boardCursorY = *boardCursorY + 1;
        }
    } else if (pressedKey == LEFT_KEY) {
        if (*cursorCoordX > (boardCoord + 1)) {
            *cursorCoordX = *cursorCoordX - 1;
            *boardCursorX = *boardCursorX - 1;
        }
    } else if (pressedKey == RIGHT_KEY) {
        if (*cursorCoordX < (boardCoord + board->getSizeX() - 2)) {
            *cursorCoordX = *cursorCoordX + 1;
            *boardCursorX = *boardCursorX + 1;
        }
    }
}

//function for events that occur when a player chooses to put a stone in a board cell
void handlePlacement(Board **board, Board **boardCopy, int *boardCursorX, int *boardCursorY, int *forbiddenMoveX,
                     int *forbiddenMoveY, int *handicappedCount, int *stoneCount, char playersMark,
                     int handicap) {
    Board *tempCopy = new Board((*board)->getSizeX(), (*board)->getSizeY());
    (*boardCopy)->copyBoard(&tempCopy);    //copy of the board prior to opponents last move
    if (*handicappedCount < 1 || handicap == 0) {
        (*board)->copyBoard(&(*boardCopy));   //actual board after opponents last move
    }

    (*boardCopy)->placeStone(*boardCursorX, *boardCursorY, playersMark); //move of the current player
    char oppositePlayersMark = playersMark == White ? Black : White;
    (*boardCopy)->detectCapture(oppositePlayersMark);  //detection of capture for the purpose of
    // checking KO fight
    int notKO = tempCopy->checkIfNotKO(&(*boardCopy)); //checking if the board reverted to a board prior to
    //opponents last move
    (*board)->copyBoard(&(*boardCopy));   //making the copy of the actual board
    delete tempCopy;

    int temp = 0;
    if (*stoneCount == 0) {
        temp = (*board)->placeStone(*boardCursorX, *boardCursorY, playersMark);
    }
    if (notKO) {
        if (handicap == 0)
            *stoneCount = *stoneCount + temp;
        else
            *handicappedCount = *handicappedCount + temp;
    } else {
        *forbiddenMoveX = *boardCursorX;  //assigning these coordinates as forbidden for this round if KO perceived
        *forbiddenMoveY = *boardCursorY;
        (*boardCopy)->copyBoard(&(*board));   //reverting the changes if KO happened
    }
}


//function for getting sizes of a console window
void getDefaultWindowSizes(int *defaultWindowSizeX, int *defaultWindowSizeY) {
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;    //getting and calculating console buffer resolution for same
                                            // sizes of the console on different computers
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(Handle, &csbiInfo);
    *defaultWindowSizeX = csbiInfo.dwSize.X;
    *defaultWindowSizeY = csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top;
}

//gets the name of the file from console and puts it in a specified format for file handling
char *getFileName() {
    StringVector *fileName;
    fileName = new StringVector();
    char input;
    clrscr();
    gotoxy(1, 1);
    cputs("Please write the name of the file: ");
    input = getche();
    while (input != ENTER) {
        fileName->push(input);
        input = getche();
    }
    char *text = fileName->transform();
    delete fileName;
    text = strcat(text, ".txt");
    return text;
}


void saveGame(Board* board, Board* boardCopy, float blackScore, float whiteScore, int stoneCount, int handicappedCount,
              int playerTurn, int handicap) {
    char *text = getFileName();
    FILE *fptr;
    fptr = fopen(text, "w");
    if (fptr == NULL) {
        clrscr();
        gotoxy(1, 1);
        cputs("Sorry, file couldn't be opened");
        delete fptr;
        delete text;
        return;
    }
    fprintf(fptr, "%f\n", blackScore);
    fprintf(fptr, "%f\n", whiteScore);
    fprintf(fptr, "%d\n", stoneCount);
    fprintf(fptr, "%d\n", handicappedCount);
    fprintf(fptr, "%d\n", playerTurn);
    fprintf(fptr, "%d\n", handicap);
    fprintf(fptr, "%d\n", board->getSizeX());
    fprintf(fptr, "%d\n", board->getSizeY());
    for (int i = 0; i < board->getSizeX(); i++)
        for (int j = 0; j < board->getSizeY(); j++)
            fprintf(fptr, "%c\n", board->getArray()[i][j]->getState());
    for (int i = 0; i < board->getSizeX(); i++)
        for (int j = 0; j < board->getSizeY(); j++)
            fprintf(fptr, "%c\n", boardCopy->getArray()[i][j]->getState());
    fclose(fptr);
    delete text;
} //saves the state of the game into a txt file of choosing


void loadGame(Board** board, Board** boardCopy, float* blackScore, float* whiteScore, int *stoneCount, int *handicappedCount,
              int *playerTurn, int *handicap) {
    char *text = getFileName();
    FILE *fptr;
    fptr = fopen(text, "r");
    if (fptr == NULL) {
        clrscr();
        gotoxy(1, 1);
        cputs("Sorry, file couldn't be opened");
        delete text;
        delete fptr;
        return;
    }
    int tempX, tempY;
    fscanf(fptr, "%f", &blackScore);
    fscanf(fptr, "\n%f", &whiteScore);
    fscanf(fptr, "\n%d", &stoneCount);
    fscanf(fptr, "\n%d", &handicappedCount);
    fscanf(fptr, "\n%d", &playerTurn);
    fscanf(fptr, "\n%d", &handicap);
    fscanf(fptr, "\n%d", &tempX);
    fscanf(fptr, "\n%d", &tempY);
    (*board)->deallocateArray();
    (*boardCopy)->deallocateArray();
    (*board) = new Board(tempX, tempY);
    (*boardCopy) = new Board(tempX, tempY);
    char temp;
    for (int i = 0; i < (*board)->getSizeX(); i++)
        for (int j = 0; j < (*board)->getSizeY(); j++) {
            fscanf(fptr, "\n%c", &temp);
            (*board)->getArray()[i][j]->setState(temp);
        }
    for (int i = 0; i < (*board)->getSizeX(); i++)
        for (int j = 0; j < (*board)->getSizeY(); j++) {
            fscanf(fptr, "\n%c", &temp);
            (*boardCopy)->getArray()[i][j]->setState(temp);
        }
    fclose(fptr);
    delete text;
} //loads the state of game from a txt file to the program


void printFinalScore(float whiteScore, float blackScore) {
    clrscr();
    char* blackScoreText, *whiteScoreText;
    convertToText(blackScore, &blackScoreText);
    convertToText((int)(whiteScore - 0.5), &whiteScoreText);
    gotoxy(1, 1);
    cputs("Black's Score:");
    gotoxy(1, 2);
    cputs(blackScoreText);
    gotoxy(1, 3);
    cputs("White's Score:");
    gotoxy(1, 4);
    cputs(whiteScoreText);
    gotoxy((1 + strlen(whiteScoreText)), 4);
    cputs(".5");
    system("pause");

    delete[] blackScoreText;
    delete[] whiteScoreText;
}


int main() {
#ifndef __cplusplus
    Conio2_Init();
#endif
    settitle("Adam Sobczuk, 188656");
    int pressedKey;
    char playersMark;
    float blackScore, whiteScore;
    int startOfGame, playerTurn, handicap, stoneCount, handicappedCount;
    int legendCoord, boardCoord; //X coordinates for the position of game
    int cursorCoordX, cursorCoordY; //cursor coordinates in console
    int boardCursorX, boardCursorY; //cursor coordinates on the board
    int defaultWindowSizeX, defaultWindowSizeY; //console window size
    int forbiddenMoveX = 0, forbiddenMoveY = 0; //KO coordinates
    Board *board, *boardCopy;
    blackScore = 0, whiteScore = 6.5;
    startOfGame = 1, handicap = 1, playerTurn = 1; //flags: handicap - initial black stone setting,
    // playerTurn - 1 is black's turn 0 is white's turn
    stoneCount = 0, handicappedCount = 0; //stoneplacement
    getDefaultWindowSizes(&defaultWindowSizeX, &defaultWindowSizeY);

    do {
        playersMark = playerTurn ? Black : White;
        textbackground(LIGHTGRAY);
        clrscr();
        textcolor(BLACK);
        if (startOfGame) {
            startGame(&board, defaultWindowSizeX, defaultWindowSizeY, &startOfGame,
                      &legendCoord, &boardCoord, &cursorCoordX,
                      &cursorCoordY, &boardCursorX, &boardCursorY);
            boardCopy = new Board(board->getSizeX(), board->getSizeY());
        }

        printGame(board, legendCoord, boardCoord, boardCursorX, boardCursorY, blackScore, (whiteScore - 0.5));
        gotoxy(cursorCoordX, cursorCoordY);
        pressedKey = getch();

        if (pressedKey == SPECIAL_CHAR)
            handleKeyEvents(board, &cursorCoordX, &cursorCoordY,
                            &boardCursorX, &boardCursorY, boardCoord);

        else if (pressedKey == NEWGAME) {
            startOfGame = 1;
            clrscr();
            board->deallocateArray();
            boardCopy->deallocateArray();
            boardCopy = nullptr;
            board = nullptr;
        }

        else if (pressedKey == PLACESTONE && (boardCursorX != forbiddenMoveX || boardCursorY != forbiddenMoveY)) {
            handlePlacement(&board, &boardCopy, &boardCursorX, &boardCursorY,
                            &forbiddenMoveX, &forbiddenMoveY, &handicappedCount,
                            &stoneCount, playersMark, handicap);
        }

        else if (pressedKey == ENTER) {
            if (stoneCount > 0 || handicappedCount > 0) {
                if (playerTurn == 0) {
                    playerTurn = 1;
                    whiteScore += board->detectCapture(Black);
                } else {
                    playerTurn = 0;
                    blackScore += board->detectCapture(White);
                }
                if (handicap) {
                    if (handicappedCount > 1)
                        whiteScore = 0.5;
                    handicap = 0;
                }
                stoneCount = 0;
                forbiddenMoveX = 0;
                forbiddenMoveY = 0;
                board->unmarkAtari();
                board->markAtari(Black);
                board->markAtari(White);
            }

        }

        else if (pressedKey == ESC) {
            board->revertBack(&boardCopy);
            stoneCount = 0;
            handicappedCount = 0;
        }

        else if (pressedKey == SAVE)
            saveGame(board, boardCopy, blackScore, whiteScore, stoneCount, handicappedCount, playerTurn, handicap);

        else if (pressedKey == LOAD)
            loadGame(&board, &boardCopy, &blackScore, &whiteScore,
                     &stoneCount, &handicappedCount, &playerTurn, &handicap);

        else if (pressedKey == FINISH) {
            board->finishGame(&whiteScore, &blackScore, playersMark);
            break;
        }

    } while (pressedKey != EXIT);
    printFinalScore(whiteScore, blackScore);

    delete boardCopy;
    delete board;
    return 0;
}
