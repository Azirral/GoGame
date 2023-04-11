//
// Created by adams on 20.11.2022.
//

#ifndef GOGAME_CELL_H
#define GOGAME_CELL_H
//defines for cell representation on the board
#define Black 'b'
#define White 'w'
#define AtariBlack 'B'
#define AtariWhite 'W'
#define Empty '.'
#define Border '#'
//defines for calculating the territory at the end of game
#define UNDETERMINED '?'
#define NONES '0'
#define BLACKS 'b'
#define WHITES 'w'
#define BLACKSTERRITORY 'B'
#define WHITESTERRITORY 'W'

#define ALIVE 0 //alive stone
#define DEAD 1 //dead stone

#define CHECKED 1 //if a stone was visited
#define UNCHECKED 0 //if a stone wasn't visited
class Cell {
    char state; //cell representation on the board
    int mark = UNCHECKED;
    int atariMark = UNCHECKED;
    int dead = ALIVE;
    char status = '?';
public:
    Cell(char state);
    int getDead() const;
    int getAtariMark() const;
    char getStatus() const;
    int getMark() const;
    char getState() const;
    void setDead(int newDead);
    void setAtariMark(int newAtariMark);
    void setStatus(char newStatus);
    void setState(char newState);
    void setMark(int newMark);
};


#endif //GOGAME_CELL_H
