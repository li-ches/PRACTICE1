#include "set.h"

Set::Set() : name("") {}

Set::Set(const string& setName) : name(setName) {}

bool Set::SADD(const string& value) {
    if (value.empty()) return false;
    auto result = data.insert(value);
    return result.second;
}

bool Set::SREM(const string& value) {
    if (value.empty()) return false;
    return data.erase(value) > 0;
}

bool Set::SISMEMBER(const string& value) const {
    if (value.empty()) return false;
    return data.find(value) != data.end();
}

const unordered_set<string>& Set::getData() const {
    return data;
}

void Set::setData(const unordered_set<string>& newData) {
    data = newData;
}

const string& Set::getName() const {
    return name;
}
