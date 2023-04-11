#include "StringVector.h"

StringVector::StringVector() {
    arr = new char[1];
    capacity = 1;
    current = 0;
}

void StringVector::push(char data) {
    if (this->current == this->capacity) {
        char *temp = new char[2 * capacity];
        for(int i = 0; i < capacity; i++) {
            temp[i] = arr[i];
        }
        delete[] arr;
        capacity *= 2;
        arr = temp;
    }
    arr[current] = data;
    current++;
}
void StringVector::pop() { current--; }

char* StringVector::transform() {
    char* temp;
    temp = new char[this->current + 1];
    for(int i = 0; i < current; i++)
        temp[i] = arr[i];
    temp[this->current] = '\0';
    return temp;
}
