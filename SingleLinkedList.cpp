#include "SingleLinkedList.h"

SingleLinkedList::SingleLinkedList() {
    length = 0;
    head = nullptr;
    tail = nullptr;
}

void SingleLinkedList::insertNodeAfter(Node *node) {
    length++;
    if (head == nullptr && tail == nullptr) //appending as head
    {
        head = node;
        tail = node;
        return;
    }
    tail->setNext(node);
    tail = node;
}

void SingleLinkedList::deleteNode(Node *node) {
    length--;
    if (head == tail) //deleting single node from list
    {
        head = nullptr;
        tail = nullptr;
        delete node;
        return;
    }
    head =node->getNext();
    delete node;
}

SingleLinkedList::~SingleLinkedList() {
    while (head != nullptr)
        deleteNode(head);
}