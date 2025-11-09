#include "stack.h"

Stack::Stack() : name("") {}

Stack::Stack(const string& stackName) : name(stackName) {}

bool Stack::SPUSH(const string& value) {
    if (value.empty()) return false;
    data.push_back(value);
    return true;
}

string Stack::SPOP() {
    if (data.empty()) return "ОШИБКА: Стек пуст";
    string value = data.back();
    data.pop_back();
    return value;
}

bool Stack::isEmpty() const {
    return data.empty();
}

const vector<string>& Stack::getData() const {
    return data;
}

void Stack::setData(const vector<string>& newData) {
    data = newData;
}

const string& Stack::getName() const {
    return name;
}
