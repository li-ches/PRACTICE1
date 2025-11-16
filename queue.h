#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>

using namespace std;

class Queue {
private:
    string name;
    string* data;
    int capacity;
    int head;
    int tail;

    void reorganize_memory();

public:
    Queue();

    //Конструктор с именем очереди
    Queue(const string& queueName);

    // Конструктор копирования
    Queue(const Queue& other);

    // Оператор присваивания
    Queue& operator=(const Queue& other);

    // Деструктор
    ~Queue();

    bool QPUSH(const string& value);

    string QPOP();

    bool isEmpty() const;

    const string& getName() const;

    string* begin() const;
    string* end() const;
};

#endif
