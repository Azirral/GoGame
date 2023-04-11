//
// Created by adams on 07.11.2022.
//

#ifndef GOGAME_BOARD_H
#define GOGAME_BOARD_H

#include "Cell.h"
#include "SingleLinkedList.h"

class Board {
    int sizeX, sizeY;
    Cell ***array;
public:
    Board(int sizeX, int sizeY); //constructor of class Board
    void initializeArray(); //initializes an array of cells which is a representation of a board
    Cell ***getArray() const;
    int getSizeX();
    int getSizeY();
    int placeStone(int x, int y, char playersMark); //places the stone on the board returns 1 when success, 0 otherwise
    int checkIfNotKO(Board **copy); //compares boards, returns 1 if the boards are different
    void revertBack(Board** copy); //changes the array to the one from the parameter
    void copyBoard(Board **copy); //copys the board to the one in the parameter
    void removeStone(int x, int y); //empties the board's cell
    void removeGroup(int *count); //removes a marked group of stones
    int countLiberties(int x, int y, char playersMark); //returns the number of liberties of a group
    void unmark(); //sets the mark of cells to UNCHECKED
    int detectCapture(char playersMark); //handles the detection and removal of a group of stones with no liberties
    void markAtari(char color); //marks the stones in Atari
    void markChains(int x, int y, char color); //marks the stones in a group to CHECKED
    void finishGame(float* whiteScore, float* blackScore, char playersMark); //handles the scoring at the end of game
    void unmarkAtari(); //unmarks the stones in Atari
    /*marks the liberties of a group and puts those coords into a list*/
    void markLiberties(SingleLinkedList **liberties, int x, int y, char playersMark);
    /*evaluates if a group of stones is alive or dead (dead - certified capture within couple of moves)*/
    int evaluate(Board *board, SingleLinkedList *liberties, int x, int y, char playersMark);
    void setDead(int x, int y, char color); //sets a groups representation to being dead
    /*searches for borders of an empty territory and marks if black or white stones were found at those borders*/
    void search(int x, int y, int *blackStoneFound, int *whiteStoneFound);
    void determineStatus(char status); //sets the marked stones status to that of parameter
    void deallocateArray();
    ~Board();
};


#endif //GOGAME_BOARD_H
