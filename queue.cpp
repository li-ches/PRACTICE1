#include "queue.h"

Queue::Queue() : name("Default"), data(nullptr), capacity(0), head(0), tail(0) {}

// Конструктор с именем
Queue::Queue(const string& queueName) : name(queueName), capacity(10), head(0), tail(0) {
    data = new string[capacity];
}

// Конструктор копирования
Queue::Queue(const Queue& other)
    : name(other.name), head(0), tail(0) {

    int count = other.tail - other.head;

    if (count <= 0) {
        capacity = 10;
        data = new string[capacity];
    } else {
        capacity = count + 10;
        data = new string[capacity];

        for (int i = 0; i < count; ++i) {
            data[i] = other.data[other.head + i];
        }
        tail = count;
    }
}

// Оператор присваивания
Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        if (data != nullptr) {
            delete[] data;
        }

        name = other.name;

        int count = other.tail - other.head;
        if (count <= 0) {
            capacity = 10;
            data = new string[capacity];
            head = 0;
            tail = 0;
        } else {
            capacity = count + 10;
            data = new string[capacity];
            for (int i = 0; i < count; ++i) {
                data[i] = other.data[other.head + i];
            }
            head = 0;
            tail = count;
        }
    }
    return *this;
}

// Деструктор
Queue::~Queue() {
    if (data != nullptr) {
        delete[] data;
    }
}

// Метод расширения/оптимизации памяти
void Queue::reorganize_memory() {
    if (capacity == 0) {
        capacity = 10;
        data = new string[capacity];
        return;
    }

    int count = tail - head;

    int newCapacity = capacity * 2;
    string* newData = new string[newCapacity];

    for (int i = 0; i < count; ++i) {
        newData[i] = data[head + i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;

    head = 0;
    tail = count;
}

//Добавление элемента
bool Queue::QPUSH(const string& value) {
    if (value.empty()) return false;

    if (data == nullptr) {
        reorganize_memory();
    }

    if (tail == capacity) {
        reorganize_memory();
    }

    data[tail++] = value;
    return true;
}

string Queue::QPOP() {
    if (head == tail) {
        return "ОШИБКА: Очередь пуста";
    }

    return data[head++];
}

// Проверка на пустоту
bool Queue::isEmpty() const {
    return head == tail;
}

// Получение имени
const string& Queue::getName() const {
    return name;
}

// Итератор на начало
string* Queue::begin() const {
    return data + head;
}

// Итератор на конец
string* Queue::end() const {
    return data + tail;
}
