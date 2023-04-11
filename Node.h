#include <iostream>

using namespace std;

class Node {
    int x, y;
    char status;
    Node *next;
public:
    explicit Node(int x, int y, char status);
    int getX() const;
    int getY() const;
    char getStatus() const;
    Node *getNext() const;
    void setStatus(char newStatus);
    void setNext(Node *newNext);
};