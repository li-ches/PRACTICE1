#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>

using namespace std;

class Stack {
private:
    string name;
    string* data;
    int capacity;
    int size;

    void resize();

public:
    Stack();
    Stack(const string& stackName);
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    ~Stack();

    // Основные операции
    bool SPUSH(const string& value);
    string SPOP();
    bool isEmpty() const;
    const string& getName() const;

    // Методы для поддержки Range-based for loop
    string* begin() const;
    string* end() const;
};

#endif
