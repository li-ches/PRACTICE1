#ifndef SET_H
#define SET_H

#include <string>
#include <iostream>

using namespace std;

// Структура узла для хеш-таблицы
class SetNode {
public:
    string key;
    int denseIndex;
    SetNode* next;
};


class Set {
private:
    string name;
    string* denseList;
    SetNode** buckets;

    int itemCount;
    int capacityVal;
    int bucketCount;

    // Хеш-функция для строк
    size_t calc_hash(const string& val) const;

    // Метод расширения памяти
    void resize_storage();

    // Метод очистки памяти
    void clear_memory();

public:
    Set();

    Set(const string& setName);

    Set(const Set& other);

    Set& operator=(const Set& other);

    ~Set();

    bool SADD(const string& value);

    bool SREM(const string& value);

    bool SISMEMBER(const string& value) const;

    const string& getName() const;

    string* begin() const;
    string* end() const;
};

#endif
