//
// Created by adams on 07.11.2022.
//
#define DEPTH 5
#include "Board.h"

Board::Board(int sizeX, int sizeY) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    array = new Cell **[sizeX];
    for (int i = 0; i < sizeX; ++i)
        array[i] = new Cell *[sizeY];
    initializeArray();
}


void Board::initializeArray() {
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++) {
            if (i == 0 || j == 0 || i == sizeX - 1 || j == sizeY - 1) {
                Cell *c = new Cell(Border);
                array[i][j] = c;
            } else {
                Cell *c = new Cell(Empty);
                array[i][j] = c;
            }
        }
}


Cell ***Board::getArray() const {
    return array;
}


int Board::getSizeX() {
    return sizeX;
}


int Board::getSizeY() {
    return sizeY;
}


int Board::placeStone(int x, int y, char playersMark) {
    char oppositeMark = playersMark == Black ? White : Black;
    if (array[x][y]->getState() == Empty) {
        array[x][y]->setState(playersMark);
        int count = countLiberties(x, y, playersMark);
        unmark();
        if (count != 0)
            return 1;
        Board* temp = new Board(getSizeX(), getSizeY());
        copyBoard(&temp);
        temp->detectCapture(oppositeMark);
        count = temp->countLiberties(x, y, playersMark);
        if (count != 0) {
            delete temp;
            return 1;
        }
        array[x][y]->setState(Empty);
        delete temp;
    }
    return 0;
}


int Board::checkIfNotKO(Board **copy) {
    int flag = 0;
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++)
            if((*copy)->getArray()[i][j]->getState() != array[i][j]->getState()) {
                flag = 1;
                return flag;
            }
    return flag;
}


void Board::revertBack(Board** copy) {
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++) {
            array[i][j]->setState((*copy)->getArray()[i][j]->getState());
            array[i][j]->setMark((*copy)->getArray()[i][j]->getMark());
            array[i][j]->setStatus((*copy)->getArray()[i][j]->getStatus());
            array[i][j]->setAtariMark((*copy)->getArray()[i][j]->getAtariMark());
            array[i][j]->setDead((*copy)->getArray()[i][j]->getDead());
        }
}


void Board::copyBoard(Board** copy) {
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++) {
            (*copy)->getArray()[i][j]->setState(array[i][j]->getState());
            (*copy)->getArray()[i][j]->setMark(array[i][j]->getMark());
            (*copy)->getArray()[i][j]->setStatus(array[i][j]->getStatus());
            (*copy)->getArray()[i][j]->setAtariMark(array[i][j]->getAtariMark());
            (*copy)->getArray()[i][j]->setDead(array[i][j]->getDead());
        }
}


void Board::removeStone(int x, int y) {
    array[x][y]->setState(Empty);
}


void Board::removeGroup(int *count) {
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++)
            if (array[i][j]->getMark() == CHECKED) {
                *count = *count + 1;
                removeStone(i, j);
            }
}


int Board::countLiberties(int x, int y, char playersMark) {
    int count = 0;
    if (array[x][y]->getState() != Border) {
        if (array[x][y]->getState() == playersMark && array[x][y]->getMark() == UNCHECKED) {
            array[x][y]->setMark(CHECKED);
            count += countLiberties(x - 1, y, playersMark); //check West
            count += countLiberties(x + 1, y, playersMark); //check East
            count += countLiberties(x, y - 1, playersMark); //check North
            count += countLiberties(x, y + 1, playersMark); //check South
        } else if (array[x][y]->getState() == Empty && array[x][y]->getMark() == UNCHECKED) {
            array[x][y]->setMark(CHECKED);
            count++;
        }
    }
    return count;
}


void Board::unmark() {
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++)
            array[i][j]->setMark(UNCHECKED);
}


int Board::detectCapture(char playersMark) {
    int count = 0;
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++)
            if (array[i][j]->getState() == playersMark) {
                int tempCount = countLiberties(i, j, playersMark);
                if (tempCount == 0)
                    removeGroup(&count);
                unmark();
            }
    return count;
}


void Board::markAtari(char color) {
    for(int i = 0 ; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++) {
            if (array[i][j]->getState() == color) {
                int count = countLiberties(i, j, color);
                if (count == 1) {
                    markChains(i, j, color);
                }
            }
        }
    unmark();
}


void Board::markChains(int x, int y, char color) {
    if (array[x][y]->getState() == color && array[x][y]->getAtariMark() == UNCHECKED) {
        array[x][y]->setAtariMark(CHECKED);
        markChains(x - 1, y, color);
        markChains(x + 1, y, color);
        markChains(x, y - 1, color);
        markChains(x, y + 1, color);
    }
}


void Board::finishGame(float *whiteScore, float *blackScore, char playersMark) {
    for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            if(array[i][j]->getState() != Empty && array[i][j]->getState() != Border
            && array[i][j]->getDead() == ALIVE) {
                SingleLinkedList* liberties = new SingleLinkedList();
                markLiberties(&liberties, i, j, array[i][j]->getState());
                unmark();
                Board* tempBoard = new Board(sizeX, sizeY);
                copyBoard(&tempBoard);
                int isDead = evaluate(tempBoard, liberties, i, j, playersMark);
                if (isDead == 0)
                    setDead(i, j, array[i][j]->getState());
                delete liberties;
                delete tempBoard;
            }
    for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            if(array[i][j]->getState() != Border && array[i][j]->getState() != Empty) {
                if(array[i][j]->getState() == Black && array[i][j]->getDead() == ALIVE)
                    array[i][j]->setStatus(BLACKS);
                if(array[i][j]->getState() == White && array[i][j]->getDead() == ALIVE)
                    array[i][j]->setStatus(WHITES);
            }
    for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            if(array[i][j]->getStatus() == UNDETERMINED && array[i][j]->getState() != Border) {
                int blackStoneFound = 0, whiteStoneFound = 0;
                search(i, j, &blackStoneFound, &whiteStoneFound);
                if(blackStoneFound == 1 && whiteStoneFound == 0)
                    determineStatus(BLACKSTERRITORY);
                if(whiteStoneFound == 1 && blackStoneFound == 0)
                    determineStatus(WHITESTERRITORY);
                if ((blackStoneFound == 1 && whiteStoneFound == 1) || (blackStoneFound == 0 && whiteStoneFound == 0))
                    determineStatus(NONES);
                unmark();
            }
    int blacksTerritory = 0, whitesTerritory = 0;
    for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            if(array[i][j]->getState() != Border && array[i][j]->getStatus() != NONES) {
                if(array[i][j]->getStatus() == BLACKSTERRITORY
                   || (array[i][j]->getState() == White && array[i][j]->getDead() == DEAD))
                    blacksTerritory += 1;
                if(array[i][j]->getStatus() == WHITESTERRITORY
                   ||(array[i][j]->getState() == Black && array[i][j]->getDead() == DEAD))
                    whitesTerritory += 1;
            }
    *blackScore = *blackScore + blacksTerritory;
    *whiteScore = *whiteScore + whitesTerritory;
}


void Board::unmarkAtari() {
    for (int i = 0; i < sizeX; i++)
        for (int j = 0; j < sizeY; j++)
            array[i][j]->setAtariMark(UNCHECKED);
}


void Board::markLiberties(SingleLinkedList** liberties, int x, int y, char playersMark) {
    if (array[x][y]->getState() != Border) {
        if (array[x][y]->getState() == playersMark && array[x][y]->getMark() == UNCHECKED) {
            array[x][y]->setMark(CHECKED);
            markLiberties(liberties, x - 1, y, playersMark); //check West
            markLiberties(liberties, x + 1, y, playersMark); //check East
            markLiberties(liberties, x, y - 1, playersMark); //check North
            markLiberties(liberties, x, y + 1, playersMark); //check South
        }
        else if (array[x][y]->getState() == Empty && array[x][y]->getMark() == UNCHECKED) {
            array[x][y]->setMark(CHECKED);
            Node* node = new Node(x, y, Empty);
            (*liberties)->insertNodeAfter(node);
        }
    }
}


int Board::evaluate(Board *board, SingleLinkedList* liberties, int x, int y, char playersMark) {
    char oppositePlayersMark = playersMark == Black ? White : Black;
    int count = countLiberties(x, y, array[x][y]->getState()), tempCount;
    unmark();
    if (count == 0)
        return 0;
    if (count > DEPTH)
        return -1;
    Node* temp = liberties->head;
    int flag = 0;
    while(temp != nullptr) {
        if (temp->getStatus() == Empty)
            flag = 1;
        temp = temp->getNext();
    }
    temp = liberties->head;
    if (flag == 0)
        return -1;
    while(temp != nullptr) {
        if(temp->getStatus() == Empty) {
            temp->setStatus(playersMark);
            Board* tempBoard = new Board(sizeX, sizeY);
            board->copyBoard(&tempBoard);
            board->getArray()[temp->getX()][temp->getY()]->setState(playersMark);
            board->detectCapture(oppositePlayersMark);
            if(board->getArray()[x][y]->getState() == Empty) {
                delete tempBoard;
                return 0;
            }
            board->revertBack(&tempBoard);
            board->getArray()[temp->getX()][temp->getY()]->setState(playersMark);
            tempCount = board->countLiberties(temp->getX(), temp->getY(), playersMark);
            unmark();
            if(tempCount == 0) {
                board->detectCapture(oppositePlayersMark);
                if(board->getArray()[x][y]->getState() == Empty) {
                    delete tempBoard;
                    return 0;
                }
                board->revertBack(&tempBoard);
            }
            if(tempCount != 0)
                count = evaluate(board, liberties, x, y, oppositePlayersMark);
            temp->setStatus(Empty);
            delete tempBoard;
        }
        temp = temp->getNext();
    }
    return count;
}


void Board::setDead(int x, int y, char color) {
    if (array[x][y]->getState() != Border) {
        if (array[x][y]->getState() == color && array[x][y]->getDead() == ALIVE) {
            array[x][y]->setDead(DEAD);
            setDead(x - 1, y, color);
            setDead(x + 1, y, color);
            setDead(x, y - 1, color);
            setDead(x, y + 1, color);
        }
    }
}


void Board::search(int x, int y, int* blackStoneFound, int* whiteStoneFound) {
    if(array[x][y]->getState() != Border && array[x][y]->getMark() == UNCHECKED) {
        if (array[x][y]->getStatus() == BLACKS) {
            *blackStoneFound = 1;
            return;
        }
        if (array[x][y]->getStatus() == WHITES) {
            *whiteStoneFound = 1;
            return;
        }
        array[x][y]->setMark(CHECKED);
        search(x - 1, y, blackStoneFound, whiteStoneFound); //check West
        search(x + 1, y, blackStoneFound, whiteStoneFound); //check East
        search(x, y - 1, blackStoneFound, whiteStoneFound); //check North
        search(x, y + 1, blackStoneFound, whiteStoneFound); //check South
    }
}


void Board::determineStatus(char status) {
    for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            if(array[i][j]->getMark() == CHECKED) {
                array[i][j]->setStatus(status);
            }
}


void Board::deallocateArray() {
    for (int i = 0; i < sizeX; ++i)
        delete[] array[i];
    delete[] array;
}


Board::~Board() {
    deallocateArray();
}