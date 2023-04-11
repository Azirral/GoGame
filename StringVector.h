class StringVector {
public:
    char* arr;
    int capacity, current;
    StringVector();
    char* transform();
    void push(char data);
    void pop();
};
