//
// Created by adams on 20.11.2022.
//

#include "Cell.h"

Cell::Cell(char state) {
    this->state = state;
}


char Cell::getState() const {
    return state;
}


int Cell::getMark() const {
    return mark;
}


char Cell::getStatus() const {
    return status;
}


int Cell::getAtariMark() const {
    return atariMark;
}


int Cell::getDead() const {
    return dead;
}


void Cell::setState(char newState) {
    state = newState;
}


void Cell::setMark(int newMark) {
    mark = newMark;
}


void Cell::setStatus(char newStatus) {
    status = newStatus;
}


void Cell::setAtariMark(int newAtariMark) {
    atariMark = newAtariMark;
}


void Cell::setDead(int newDead) {
    dead = newDead;
}
