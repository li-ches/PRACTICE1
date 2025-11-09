#include "hash_table.h"

HashTable::HashTable() : name("") {}

HashTable::HashTable(const string& hashName) : name(hashName) {}

bool HashTable::HSET(const string& key, const string& value) {
    if (key.empty() || value.empty()) return false;
    data[key] = value;
    return true;
}

bool HashTable::HDEL(const string& key) {
    if (key.empty()) return false;
    return data.erase(key) > 0;
}

string HashTable::HGET(const string& key) const {
    if (key.empty()) return "ОШИБКА: Пустой ключ";
    auto it = data.find(key);
    if (it != data.end()) return it->second;
    return "ОШИБКА: Ключ не найден";
}

bool HashTable::contains(const string& key) const {
    return data.find(key) != data.end();
}

const map<string, string>& HashTable::getData() const {
    return data;
}

void HashTable::setData(const map<string, string>& newData) {
    data = newData;
}

const string& HashTable::getName() const {
    return name;
}
