#include "Node.h"
class SingleLinkedList {
public:
    int length;
    Node* head;
    Node* tail;
    SingleLinkedList();
    void insertNodeAfter(Node* node); //inserts the node at the end of the list
    void deleteNode(Node* node);
    ~SingleLinkedList();
};

