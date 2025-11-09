#include "queue.h"

Queue::Queue() : name("") {}

Queue::Queue(const string& queueName) : name(queueName) {}

bool Queue::QPUSH(const string& value) {
    if (value.empty()) return false;
    data.push_back(value);
    return true;
}

string Queue::QPOP() {
    if (data.empty()) return "ОШИБКА: Очередь пуста";
    string value = data.front();
    data.pop_front();
    return value;
}

bool Queue::isEmpty() const {
    return data.empty();
}

const deque<string>& Queue::getData() const {
    return data;
}

void Queue::setData(const deque<string>& newData) {
    data = newData;
}

const string& Queue::getName() const {
    return name;
}
