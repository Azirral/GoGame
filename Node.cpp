#include "Node.h"

Node::Node(int x, int y, char status) {
    this->x = x;
    this->y = y;
    this->status = status;
    next = nullptr;
}


int Node::getX() const {
    return this->x;
}


int Node::getY() const {
    return this->y;
}


char Node::getStatus() const {
    return this->status;
}


Node *Node::getNext() const {
    return this->next;
}


void Node::setStatus(char newStatus) {
    status = newStatus;
}


void Node::setNext(Node *newNext) {
    next = newNext;
}
